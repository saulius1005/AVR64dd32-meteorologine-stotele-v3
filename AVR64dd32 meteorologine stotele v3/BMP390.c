/**
 * @file BMP390.c
 * @brief Functions for interfacing with the BMP280 sensor, including calibration, configuration, and data retrieval.
 * @details This file contains implementations for reading and writing configuration, calibration data,
 * and performing calculations for temperature and pressure using the BMP280 sensor.
 * @author Saulius
 * @date 2024-12-04
 */

#include "Settings.h"
#include "BMP390Var.h"

/**
 * @brief Reads the BMP280 sensor ID from its EEPROM.
 */
void ReadBMP280ID() {
    BMP280.ID = ReadReg(BMP280Add, id);
}

/**
 * @brief Reads calibration data from the BMP280 EEPROM.
 * @details The calibration data is read in three parts and stored in the `BMP280.CalibrationValues` structure.
 */
void ReadBMP280Calibration() {
    for (uint8_t i = 0; i < 3; i++) {
        uint8_t startAdd = calib00, part = 0;
        if (i == 1) {
            startAdd = calib08;
            part = 1;
        } else if (i == 2) {
            startAdd = calib16;
            part = 2;        
        }    
        uint64_t data = ReadMulti(BMP280Add, startAdd, 8);

        if (part == 0) {
            BMP280.CalibrationValues.dig_T1 = ((data >> 48) & 0xFF) << 8 | ((data >> 48) & 0xFF00) >> 8;
            BMP280.CalibrationValues.dig_T2 = ((data >> 32) & 0xFF) << 8 | ((data >> 32) & 0xFF00) >> 8;
            BMP280.CalibrationValues.dig_T3 = ((data >> 16) & 0xFF) << 8 | ((data >> 16) & 0xFF00) >> 8;
            BMP280.CalibrationValues.dig_P1 = ((data & 0xFF) << 8) | ((data & 0xFF00) >> 8);
        } else if (part == 1) {
            BMP280.CalibrationValues.dig_P2 = ((data >> 48) & 0xFF) << 8 | ((data >> 48) & 0xFF00) >> 8;
            BMP280.CalibrationValues.dig_P3 = ((data >> 32) & 0xFF) << 8 | ((data >> 32) & 0xFF00) >> 8;
            BMP280.CalibrationValues.dig_P4 = ((data >> 16) & 0xFF) << 8 | ((data >> 16) & 0xFF00) >> 8;
            BMP280.CalibrationValues.dig_P5 = ((data & 0xFF) << 8) | ((data & 0xFF00) >> 8);
        } else { // part = 2
            BMP280.CalibrationValues.dig_P6 = ((data >> 48) & 0xFF) << 8 | ((data >> 48) & 0xFF00) >> 8;
            BMP280.CalibrationValues.dig_P7 = ((data >> 32) & 0xFF) << 8 | ((data >> 32) & 0xFF00) >> 8;
            BMP280.CalibrationValues.dig_P8 = ((data >> 16) & 0xFF) << 8 | ((data >> 16) & 0xFF00) >> 8;
            BMP280.CalibrationValues.dig_P9 = ((data & 0xFF) << 8) | ((data & 0xFF00) >> 8);
        }
    }
}

/**
 * @brief Reads the configuration data from the BMP280 sensor.
 */
void ReadBMP280Config() {
    uint64_t data = ReadMulti(BMP280Add, ctrl_meas, 2);
    BMP280.Config.osrs_t = (data >> 13) & 0xfff;
    BMP280.Config.osrs_p = (data >> 10) & 0xfff;
    BMP280.Config.Mode = (data >> 8) & 0xff;
    BMP280.Config.t_sb = (data >> 5) & 0xfff;
    BMP280.Config.filter = (data >> 2) & 0xfff;
    BMP280.Config.spi3w_en = data & 0xf;
}

/**
 * @brief Writes the configuration data to the BMP280 sensor.
 */
void WriteBMP280Config() {
    WriteToReg(BMP280Add, ctrl_meas, (BMP280.Config.osrs_p << 5) + (BMP280.Config.osrs_t << 2) + BMP280.Config.Mode);
    WriteToReg(BMP280Add, config, (BMP280.Config.t_sb << 5) + (BMP280.Config.filter << 2) + BMP280.Config.spi3w_en);
}

/**
 * @brief Reads the status register of the BMP280 sensor.
 */
void ReadBMP280Status() {
    uint8_t data = ReadReg(BMP280Add, status);
    BMP280.Status.measuring = (data >> 3) & 0xf;
    BMP280.Status.im_update = data & 0xf;
}

/**
 * @brief Reads both temperature and pressure from the BMP280 sensor.
 */
void ReadBMP280TP() {
    while (BMP280.Status.measuring && BMP280.Status.im_update) {
        ReadBMP280Status();
    }
    BMP280.CalibrationValues.UP = (ReadMulti(BMP280Add, press_msb, 3)) >> 4;
    BMP280.CalibrationValues.UT = (ReadMulti(BMP280Add, temp_msb, 3)) >> 4;
}

/**
 * @brief Resets the BMP280 sensor.
 */
void ResetBMP280() {
    WriteToReg(BMP280Add, reset, BMP280_Reset);
}

/**
 * @brief Calculates the true temperature in degrees Celsius.
 * @return The true temperature as a signed 16-bit integer.
 */
int16_t CalcTrueTemp() {
    int32_t var1 = 0, var2 = 0, T = 0;
    var1 = ((((BMP280.CalibrationValues.UT >> 3) - ((int32_t)BMP280.CalibrationValues.dig_T1 << 1))) * 
             ((int32_t)BMP280.CalibrationValues.dig_T2)) >> 11;
    var2 = (((((BMP280.CalibrationValues.UT >> 4) - ((int32_t)BMP280.CalibrationValues.dig_T1)) *
              ((BMP280.CalibrationValues.UT >> 4) - ((int32_t)BMP280.CalibrationValues.dig_T1))) >> 12) * 
              ((int32_t)BMP280.CalibrationValues.dig_T3)) >> 14;
    BMP280.CalibrationValues.t_fine = var1 + var2;
    T = (BMP280.CalibrationValues.t_fine * 5 + 128) >> 8;
    BMP280.CalibrationValues.T = T;
    BMP280.Temperature = (float)T / 100;
    return T;
}

/**
 * @brief Calculates the true pressure in Pascals.
 * @return The true pressure as an unsigned 32-bit integer.
 */
uint32_t CalcTruePres() {
    int64_t var1 = 0, var2 = 0, p = 0;
    var1 = ((int64_t)BMP280.CalibrationValues.t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)BMP280.CalibrationValues.dig_P6;
    var2 = var2 + ((var1 * (int64_t)BMP280.CalibrationValues.dig_P5) << 17);
    var2 = var2 + (((int64_t)BMP280.CalibrationValues.dig_P4) << 35);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)BMP280.CalibrationValues.dig_P1) >> 33;

    if (var1 == 0)
        return 0;

    p = 1048576 - BMP280.CalibrationValues.UP;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)BMP280.CalibrationValues.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)BMP280.CalibrationValues.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)BMP280.CalibrationValues.dig_P7) << 4);

    BMP280.Pressure = (float)p / 25600;
    BMP280.CalibrationValues.p = p;
    return (uint32_t)p;
}
