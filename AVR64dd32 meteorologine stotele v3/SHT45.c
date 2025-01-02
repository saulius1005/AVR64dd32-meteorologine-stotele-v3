/*
 * SHT45.c
 *
 * Created: 2024-12-04 18:21:11
 * Author: Saulius
 *
 * @brief This file contains the implementation of functions to interact with 
 *        the SHT21 (and possibly SHT45) sensor, including reading and writing 
 *        sensor settings, reading temperature and humidity data, and sensor reset.
 */

#include "Settings.h"
#include "SHT45Var.h"

/**
 * @brief Reads the settings from the SHT21 sensor.
 *
 * This function reads the user register from the SHT21 sensor and extracts 
 * individual settings, including the battery status, OTP disable flag, resolution, 
 * and heater status.
 */
void SHT21_Settings_Read(){
	uint8_t reader = ReadReg(SHT21_ADD, R_USER_REG);

	// Extracting settings from the user register
	SHT21.Battery = (reader & (ON << BATTERY_ADD)) >> BATTERY_ADD;
	SHT21.OTP_DISABLE = (reader & (ON << OTP_ADD)) >> OTP_ADD;
	SHT21.Resolution = reader & RESOLUTION_ADD;
	SHT21.Heater = (reader & (ON << HEATER_ADD)) >> HEATER_ADD;
}

/**
 * @brief Writes the settings to the SHT21 sensor.
 *
 * This function writes the current settings stored in the global `SHT21` 
 * structure back to the SHT21 sensor, configuring the resolution, heater, 
 * OTP disable, and battery settings.
 */
void SHT21_Settings_Write(){
	WriteToReg(SHT21_ADD, W_USER_REG, SHT21.Resolution + (SHT21.Heater << HEATER_ADD) + (SHT21.OTP_DISABLE << OTP_ADD) + (SHT21.Battery << BATTERY_ADD));
}

/**
 * @brief Reads data from the SHT21 sensor.
 *
 * This function reads either temperature or humidity data from the SHT21 sensor 
 * depending on the provided mode. It handles both "hold" and "no hold" master 
 * reading modes. In hold mode, the sensor waits for the measurement to complete, 
 * while in no hold mode, the measurement is performed while the master device 
 * communicates with the sensor.
 *
 * @param mode The mode of measurement: temperature or humidity.
 * @return The raw sensor data (32-bit).
 */
uint32_t SHT21_Read(uint8_t mode){
	// Faster read for hold modes (temperature or humidity measurement)
	if((mode == HOLD_MASTER_T_MES) || (mode == HOLD_MASTER_RH_MES)){
		return (uint32_t)ReadMulti(SHT21_ADD, mode, 3);
	}
	// Slower read for no hold modes (temperature or humidity measurement)
	else if((mode == NO_HOLD_MASTER_T_MES) || (mode == NO_HOLD_MASTER_RH_MES)){
		if(!TransmitAdd(SHT21_ADD, WRITE)){
			if(!TransmitByte(mode)){
				_delay_us(20); // Wait for the measurement to complete
				TWI0.MCTRLB = TWI_MCMD_STOP_gc; // Stop condition to finish the transmission
				// Handle different resolutions for different measurement delays
				switch(SHT21.Resolution){
					case RH_11b_T_11b:
						if(mode == NO_HOLD_MASTER_T_MES)
							_delay_ms(11);
						else
							_delay_ms(15);
						break;
					case RH_10b_T_13b:
						if(mode == NO_HOLD_MASTER_T_MES)
							_delay_ms(43);
						else
							_delay_ms(9);
						break;
					case RH_8b_T_12b:
						if(mode == NO_HOLD_MASTER_T_MES)
							_delay_ms(22);
						else
							_delay_ms(4);
						break;
					default:
						if(mode == NO_HOLD_MASTER_T_MES)
							_delay_ms(85);
						else
							_delay_ms(29);
						break;
				}
				uint32_t data = 0;
				if(!TransmitAdd(SHT21_ADD, READ)){
					// Read the data byte by byte and reconstruct the 32-bit result
					for (int i = 0; i < 3; i++) {
						uint8_t byte = 0;
						ReadByteInf(i < 2 ? 1 : 0, &byte); // ACK for first two bytes, NACK for last byte
						if(I2C.error)
							break;
						else
							data |= ((uint32_t)byte << (8 * (2 - i))); // Insert byte into the correct position
					}
					TWI0.MCTRLB |= TWI_MCMD_STOP_gc; // Send stop condition
					return data;
				}
			}
		}
	}
	return 0; // Return 0 if read failed
}

/**
 * @brief Separates the sensor data into temperature or humidity.
 *
 * This function separates the 32-bit sensor data into temperature and 
 * humidity values depending on the CRC validity. It calculates the corresponding 
 * temperature in Centigrade or humidity percentage.
 *
 * @param data The raw sensor data.
 * @return 0 (always returns 0, but could be used for error codes in future).
 */
uint8_t Separator(uint32_t data){
	// Ignoring CRC for speed but sacrificing reliability
	uint32_t reader = CRC8MAXIM(data); // CRC check
	float result = 0.00;
	if(reader & 2){ // After CRC check, calculate humidity percentage
		result = (float)(reader - 2) / 65536;
		SHT21.RH = (result * 125) - 6; // Convert to humidity in percentage
	}
	else{ // Calculate temperature in Centigrade
		result = (float)reader / 65536;
		SHT21.T = (result * 175.72) - 46.85; // Convert to temperature in Centigrade
	}
	return 0; // Return 0, could be used for error handling
}

/**
 * @brief Resets the SHT21 sensor and restores its settings.
 *
 * This function sends a soft reset command to the SHT21 sensor, and after 
 * resetting, it reads the default settings and optionally writes the default 
 * settings back to the sensor.
 *
 * @param save If 1, writes the default settings back to the sensor.
 */
void SHT21_Reset(uint8_t save){
	TransmitAdd(SHT21_ADD, WRITE);
	TransmitByte(SOFT_RESET); // Send soft reset command
	SHT21_Settings_Read(); // Read the default settings after reset
	if(save & 1)
		SHT21_Settings_Write(); // Write the default settings back to the sensor
}
