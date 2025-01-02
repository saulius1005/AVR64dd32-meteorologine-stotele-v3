/**
 * @file AltitudeVar.h
 * @brief Header file defining the initialization of the `Altitude` variable.
 * @details This file initializes the `Altitude` structure with default values.
 * It is designed to be included in modules that require access to the altitude-related data.
 * @author Saulius
 * @date 2024-12-09
 */

#ifndef ALTITUDEVAR_H_
#define ALTITUDEVAR_H_

/**
 * @brief Global variable for storing altitude data.
 * @details The `Altitude` variable is initialized with default values:
 * - `AVRG`: Set to 88 as a placeholder value.
 */
Alt Altitude = {
    .AVRG = 88, /**< Default average altitude value. */
};

#endif /* ALTITUDEVAR_H_ */
