/**
 * @file BMP390Var.h
 * @brief Variable definitions for the BMP280 sensor.
 *
 * This file defines the initialization of the BMP280Result structure
 * with default or placeholder values.
 * 
 * @author Saulius
 * @date 2024-12-09
 */

#ifndef BMP390VAR_H_
#define BMP390VAR_H_

/**
 * @brief Global instance of BMP280Result for holding sensor data.
 * 
 * This instance initializes the BMP280 structure with default values:
 * - `ID`: The default sensor ID (0x58).
 * - `CalibrationValues.UP`: Placeholder for uncompensated pressure (default: 0x800000).
 * - `CalibrationValues.UT`: Placeholder for uncompensated temperature (default: 0x800000).
 */
BMP280Result BMP280 = {
    .ID = 0x58, /**< Sensor ID, expected default for BMP280. */
    .CalibrationValues.UP = 0x800000, /**< Default uncompensated pressure value. */
    .CalibrationValues.UT = 0x800000 /**< Default uncompensated temperature value. */
};

#endif /* BMP390VAR_H_ */
