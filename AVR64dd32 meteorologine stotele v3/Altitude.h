/**
 * @file Altitude.h
 * @brief Header file defining constants, structures, and variables for altitude calculations.
 * @details This file contains physical and empirical constants required for altitude calculations,
 * as well as the declaration of the `Alt` structure and the global `Altitude` variable.
 * It is intended for use in conjunction with `Altitude.c`.
 * @author Saulius
 * @date 2024-12-09
 */

#ifndef ALTITUDE_H_
#define ALTITUDE_H_

/** 
 * @brief Absolute zero temperature in Kelvin (K).
 */
#define T0 273.15 

/** 
 * @brief Empirical constant for the saturation vapor pressure formula.
 */
#define A 6.112 

/** 
 * @brief Empirical constant for the saturation vapor pressure formula.
 */
#define B 17.67 

/** 
 * @brief Empirical constant for the saturation vapor pressure formula.
 */
#define C 243.5 

/** 
 * @brief Standard gravity acceleration (m/s^2).
 */
#define GRAVITY 9.80665 

/** 
 * @brief Molar mass of dry air (kg/mol).
 */
#define MOLAR_MASS_AIR 0.0289644 

/** 
 * @brief Universal gas constant (J/(mol*K)).
 */
#define UNIVERSAL_GAS_CONSTANT 8.31432 

/** 
 * @brief Average sea-level pressure (hPa).
 */
#define SEA_LEVEL_PRESSURE 1013.25 

/**
 * @struct Alt
 * @brief Structure for storing altitude-related data.
 * @details Contains the following fields:
 * - `UNCOMP`: Uncompensated altitude (meters).
 * - `COMP`: Compensated altitude (meters).
 * - `AVRG`: Average altitude (meters).
 */
typedef struct { 
    double UNCOMP; /**< Uncompensated altitude. */
    double COMP;   /**< Compensated altitude. */
    float AVRG;    /**< Average altitude. */
} Alt;

/**
 * @brief Global variable for storing altitude data.
 */
extern Alt Altitude;

#endif /* ALTITUDE_H_ */
