/*
 * ElAndAzCompVar.h
 *
 * Created: 2024-12-10 18:45:05
 * Author: Saulius
 *
 * This header file contains the definition and initialization of the external `SUN`
 * variable, which is an instance of the `SunAngles` structure.
 * The `SUN` variable holds the solar elevation, azimuth, adjusted angles, and sun level.
 */

#ifndef ELANDAZCOMPVAR_H_
#define ELANDAZCOMPVAR_H_

/**
 * @brief External variable to store solar angle data.
 *
 * This variable is an instance of the `SunAngles` structure, initialized with default values
 * for the solar elevation, azimuth, and sun level. It is used to store and manipulate the
 * solar angle data within the system.
 */
SunAngles SUN = {
    .azimuth = 0,        ///< The solar azimuth angle, initialized to 0 degrees.
    .elevation = 0,      ///< The solar elevation angle, initialized to 0 degrees.
    .sunlevel = 0        ///< The sun level, initialized to 0 (measured via ADC).
};

#endif /* ELANDAZCOMPVAR_H_ */
