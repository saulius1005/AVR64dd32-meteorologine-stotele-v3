/*
 * SHT45.h
 *
 * Created: 2024-12-04 18:21:51
 * Author: Saulius
 *
 * @brief This header file contains definitions and declarations for interacting 
 *        with the SHT21 (and possibly SHT45) sensor, including I2C addresses, 
 *        measurement commands, sensor settings, and the SHT structure used to 
 *        store sensor data.
 */

#ifndef SHT45_H_
#define SHT45_H_

/**
 * @brief Constants for CRC handling in the `Separator` function.
 * 
 * `USE_CRC` enables CRC checking, while `IGNORE_CRC` disables it for faster but 
 * less reliable data separation.
 */
#define USE_CRC 0 // For CRC checking in Separator function
#define IGNORE_CRC 1 // For ignoring CRC in Separator function

/**
 * @brief I2C address for the SHT21 sensor.
 * 
 * This is the I2C address of the SHT21 sensor (0x40), excluding the read/write 
 * bit. The sensor communicates over I2C using this address.
 */
#define SHT21_ADD 0x40 // I2C address for SHT sensor (SHT21), 0x40 (without R/W bit)

/**
 * @brief I2C command codes for SHT21 sensor measurements.
 * 
 * These are the command codes used for requesting temperature and humidity 
 * measurements from the SHT21 sensor. The codes differ based on the measurement 
 * mode (hold master or no hold master).
 */
#define HOLD_MASTER_T_MES 0xe3 // Temperature measurement with hold master mode
#define HOLD_MASTER_RH_MES 0xe5 // Humidity measurement with hold master mode
#define NO_HOLD_MASTER_T_MES 0xf3 // Temperature measurement with no hold master mode
#define NO_HOLD_MASTER_RH_MES 0xf5 // Humidity measurement with no hold master mode

/**
 * @brief Register addresses for reading and writing user settings in the SHT21.
 * 
 * These are the register addresses used to read and write the user settings 
 * of the SHT21 sensor.
 */
#define W_USER_REG 0b11100110 // Write user register
#define R_USER_REG 0b11100111 // Read user register

/**
 * @brief Command for performing a soft reset of the SHT21 sensor.
 * 
 * This command resets the SHT21 sensor and restores it to its default state.
 */
#define SOFT_RESET 0b11111110 // Soft reset command

/**
 * @brief Bit positions for settings in the user register of the SHT21 sensor.
 * 
 * These constants define the bit positions for various settings in the user 
 * register, including OTP disable, heater, battery status, and resolution.
 */
#define OTP_ADD 1 // OTP disable bit
#define HEATER_ADD 2 // Heater bit (2nd bit)
#define BATTERY_ADD 6 // Battery status bit (6th bit)
#define RESOLUTION_ADD 129 // Resolution setting bits (0 and 7)

/**
 * @brief Resolution settings for the SHT21 sensor.
 * 
 * These constants define the resolution options for the temperature and humidity 
 * measurements in the SHT21 sensor.
 */
#define RH_12b_T_14b 0 // 12-bit RH, 14-bit temperature resolution
#define RH_8b_T_12b 1 // 8-bit RH, 12-bit temperature resolution
#define RH_10b_T_13b 128 // 10-bit RH, 13-bit temperature resolution
#define RH_11b_T_11b 129 // 11-bit RH, 11-bit temperature resolution

/**
 * @brief ON/OFF constants for enabling/disabling features in the SHT21 sensor.
 * 
 * These constants are used to enable or disable features such as the OTP disable 
 * and heater in the `SHT21_Settings_Read` and `SHT21_Reset` functions.
 */
#define ON 1 // Enable setting (used for OTP_DISABLE and Heater)
#define OFF 0 // Disable setting (used for OTP_DISABLE and Heater)

/**
 * @brief Structure to store sensor data and settings for the SHT21 sensor.
 * 
 * The `SHT` structure contains all the necessary information for the SHT21 sensor, 
 * including settings (OTP disable, heater, resolution, battery) and measurement data 
 * (temperature, humidity, fault status). It also includes an error flag for CRC 
 * verification and a variable for calculating the vapor pressure (`e`).
 */
typedef struct {
	uint8_t OTP_DISABLE; // Flag to restore factory settings only
	uint8_t Heater; // Flag to turn on or off the integrated heater
	uint8_t Resolution; // Resolution for temperature and humidity measurements
	uint8_t Battery; // Flag to detect battery status (2.25V detection)
	float T; // Calculated temperature in Celsius
	float RH; // Calculated relative humidity in percentage
	uint8_t Fault; // Flag for CRC correctness (0: valid CRC, 1: invalid CRC)
	float e; // Vapor pressure calculation (optional, for advanced applications)
} SHT;

/**
 * @brief External variable to store the current sensor data and settings for the SHT21.
 * 
 * This global variable holds the data and settings for the SHT21 sensor and is 
 * accessible throughout the program.
 */
extern SHT SHT21;

#endif /* SHT45_H_ */
