/**
 * @file BMP390.h
 * @brief Header file for BMP280/BMP390 sensor configuration, calibration, and communication.
 *
 * This header file defines the register settings, sensor configuration options, 
 * and structures for handling the calibration values, sensor data, and control of the BMP280/BMP390 sensor.
 *
 * @note The file is also applicable to the BMP280 sensor as both share a similar architecture and register setup.
 *
 * @author Saulius
 * @date 2024-12-04
 */

#ifndef BMP390_H_
#define BMP390_H_

/** @name BMP280 I2C Address */
///@{
#define BMP280Add 0x76 /**< I2C address for BMP280 sensor with SDO connected to GND */
///@}

/** @name BMP280 Mode Options */
///@{
#define BMP280_Mode_Sleep      0b00 /**< Sleep mode */
#define BMP280_Mode_Forced     0b01 /**< Forced mode */
#define BMP280_Mode_Normal     0b11 /**< Normal mode */
///@}

/** @name BMP280 Filter Settings */
///@{
#define BMP280_Filter_Off      0b000 /**< No filtering */
#define BMP280_Filter_2        0b001 /**< Filter coefficient 2 */
#define BMP280_Filter_4        0b010 /**< Filter coefficient 4 */
#define BMP280_Filter_8        0b011 /**< Filter coefficient 8 */
#define BMP280_Filter_16       0b100 /**< Filter coefficient 16 */
///@}

/** @name BMP280 Temperature Oversampling */
///@{
#define BMP280_Temperature_Off     0b000 /**< Temperature measurement off */
#define BMP280_Temperature_Os_x1   0b001 /**< Oversampling x1 */
#define BMP280_Temperature_Os_x2   0b010 /**< Oversampling x2 */
#define BMP280_Temperature_Os_x4   0b011 /**< Oversampling x4 */
#define BMP280_Temperature_Os_x8   0b100 /**< Oversampling x8 */
#define BMP280_Temperature_Os_x16  0b101 /**< Oversampling x16 */
///@}

/** @name BMP280 Pressure Oversampling */
///@{
#define BMP280_Pressure_Off        0b000 /**< Pressure measurement off */
#define BMP280_Pressure_ULP        0b001 /**< Ultra-low power (oversampling x1) */
#define BMP280_Pressure_LP         0b010 /**< Low power (oversampling x2) */
#define BMP280_Pressure_SR         0b011 /**< Standard resolution (oversampling x4) */
#define BMP280_Pressure_HR         0b100 /**< High resolution (oversampling x8) */
#define BMP280_Pressure_UHR        0b101 /**< Ultra-high resolution (oversampling x16) */
///@}

/** @name BMP280 Standby Time */
///@{
#define BMP280_StanBy_0m5          0b000 /**< Standby time: 0.5ms */
#define BMP280_StanBy_62m5         0b001 /**< Standby time: 62.5ms */
#define BMP280_StanBy_125m         0b010 /**< Standby time: 125ms */
#define BMP280_StanBy_250m         0b011 /**< Standby time: 250ms */
#define BMP280_StanBy_500m         0b100 /**< Standby time: 500ms */
#define BMP280_StanBy_1000m        0b101 /**< Standby time: 1000ms */
#define BMP280_StanBy_2000m        0b110 /**< Standby time: 2000ms */
#define BMP280_StanBy_4000m        0b111 /**< Standby time: 4000ms */
///@}

/** @name BMP280 SPI Modes */
///@{
#define BMP280_SPI_Mode_4w         0 /**< SPI 4-wire mode */
#define BMP280_SPI_Mode_3w         1 /**< SPI 3-wire mode */
///@}

/** @name BMP280 Reset */
///@{
#define BMP280_Reset               0xb6 /**< Reset register value */
///@}

/** @name BMP280 Register Addresses */
///@{
#define temp_xlsb 0xfc /**< Temperature data (least significant byte) */
#define temp_lsb   0xfb /**< Temperature data (second least significant byte) */
#define temp_msb   0xfa /**< Temperature data (most significant byte) */
#define press_xlsb 0xf9 /**< Pressure data (least significant byte) */
#define press_lsb  0xf8 /**< Pressure data (second least significant byte) */
#define press_msb  0xf7 /**< Pressure data (most significant byte) */
#define config     0xf5 /**< Configuration register */
#define ctrl_meas  0xf4 /**< Control and measurement register */
#define status     0xf3 /**< Status register */
#define reset      0xe0 /**< Reset register */
#define id         0xd0 /**< ID register */
#define calib00    0x88 /**< Calibration register 00 */
#define calib01    0x89 /**< Calibration register 01 */
#define calib02    0x8a /**< Calibration register 02 */
#define calib03    0x8b /**< Calibration register 03 */
#define calib04    0x8c /**< Calibration register 04 */
#define calib05    0x8d /**< Calibration register 05 */
#define calib06    0x8e /**< Calibration register 06 */
#define calib07    0x8f /**< Calibration register 07 */
#define calib08    0x90 /**< Calibration register 08 */
#define calib09    0x91 /**< Calibration register 09 */
#define calib10    0x92 /**< Calibration register 10 */
#define calib11    0x93 /**< Calibration register 11 */
#define calib12    0x94 /**< Calibration register 12 */
#define calib13    0x95 /**< Calibration register 13 */
#define calib14    0x96 /**< Calibration register 14 */
#define calib15    0x97 /**< Calibration register 15 */
#define calib16    0x98 /**< Calibration register 16 */
#define calib17    0x99 /**< Calibration register 17 */
#define calib18    0x9a /**< Calibration register 18 */
#define calib19    0x9b /**< Calibration register 19 */
#define calib20    0x9c /**< Calibration register 20 */
#define calib21    0x9d /**< Calibration register 21 */
#define calib22    0x9e /**< Calibration register 22 */
#define calib23    0x9f /**< Calibration register 23 */
#define calib24    0xa0 /**< Calibration register 24 (not used) */
#define calib25    0xa1 /**< Calibration register 25 (not used) */
///@}

/** 
 * @brief BMP280 sensor calibration values and measurement data.
 * 
 * This structure holds the calibration values, temperature, pressure data, 
 * and fine temperature correction data for the BMP280 sensor.
 */
typedef struct {
    uint16_t dig_T1; /**< Calibration coefficient T1 */
    int16_t dig_T2; /**< Calibration coefficient T2 */
    int16_t dig_T3; /**< Calibration coefficient T3 */
    
    uint16_t dig_P1; /**< Calibration coefficient P1 */
    int16_t dig_P2; /**< Calibration coefficient P2 */
    int16_t dig_P3; /**< Calibration coefficient P3 */
    int16_t dig_P4; /**< Calibration coefficient P4 */
    int16_t dig_P5; /**< Calibration coefficient P5 */
    int16_t dig_P6; /**< Calibration coefficient P6 */
    int16_t dig_P7; /**< Calibration coefficient P7 */
    int16_t dig_P8; /**< Calibration coefficient P8 */
    int16_t dig_P9; /**< Calibration coefficient P9 */
    
    int32_t UT; /**< Raw temperature measurement */
    int32_t UP; /**< Raw pressure measurement */
    int32_t t_fine; /**< Fine temperature correction value */
    int16_t T; /**< Temperature result */
    uint32_t p; /**< Pressure result */
} BMP280Values;

/** 
 * @brief BMP280 sensor configuration settings.
 * 
 * This structure holds the configuration options for filter, oversampling, 
 * mode, and other sensor parameters.
 */
typedef struct {
    uint8_t t_sb; /**< Standby time setting */
    uint8_t filter; /**< Filter setting */
    uint8_t spi3w_en; /**< 3-wire SPI mode enable */
    uint8_t osrs_t; /**< Oversampling setting for temperature */
    uint8_t osrs_p; /**< Oversampling setting for pressure */
    uint8_t Mode; /**< Operating mode */
} BMP280Config;

/** 
 * @brief BMP280 sensor status flags.
 * 
 * This structure holds the measurement and update status of the BMP280 sensor.
 */
typedef struct {
    uint8_t measuring; /**< Measurement ongoing flag */
    uint8_t im_update; /**< Information memory update flag */
} BMP280Status;

/** 
 * @brief BMP280 sensor result.
 * 
 * This structure holds the final result of sensor measurements, including
 * temperature, pressure, calibration values, and status.
 */
typedef struct {
    uint8_t ID; /**< Sensor ID */
    float Temperature; /**< Temperature reading in Celsius */
    double Pressure; /**< Pressure reading in hPa */
    BMP280Values CalibrationValues; /**< Calibration coefficients */
    BMP280Config Config; /**< Configuration settings */
    BMP280Status Status; /**< Sensor status */
} BMP280Result;

/** @brief Global variable for the BMP280 sensor result */
extern BMP280Result BMP280;

#endif /* BMP390_H_ */

//Example for calculation tests answer should be T=25,08C P= 1006,399hPa
/*BMP280.CalibrationValues.dig_T1= 27504;
		BMP280.CalibrationValues.dig_T2= 26435;
		BMP280.CalibrationValues.dig_T3= -1000;

		BMP280.CalibrationValues.dig_P1= 36477;
		BMP280.CalibrationValues.dig_P2= -10685;
		BMP280.CalibrationValues.dig_P3= 3024;
		BMP280.CalibrationValues.dig_P4= 2855;
		BMP280.CalibrationValues.dig_P5= 140;
		BMP280.CalibrationValues.dig_P6= -7;
		BMP280.CalibrationValues.dig_P7= 15500;
		BMP280.CalibrationValues.dig_P8= -14600;
		BMP280.CalibrationValues.dig_P9= 6000;

		BMP280.CalibrationValues.UT= 519888;
		BMP280.CalibrationValues.UP= 415148;*/