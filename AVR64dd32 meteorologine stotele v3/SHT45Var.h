/*
 * SHT45Var.h
 *
 * Created: 2024-12-07 18:37:22
 * Author: Saulius
 *
 * @brief This header file contains the initialization of the SHT21 sensor data 
 *        structure. It initializes the global `SHT21` variable with default values.
 *        The `SHT21` structure holds sensor settings such as OTP disable, heater 
 *        status, battery status, and measurement resolution.
 */

#ifndef SHT45VAR_H_
#define SHT45VAR_H_

/**
 * @brief Initialization of the SHT21 sensor data structure.
 * 
 * This structure holds the default settings for the SHT21 sensor, including the 
 * status of OTP disable, heater, battery detection, and resolution. The values are 
 * initialized to their default states upon system startup.
 * 
 * - `OTP_DISABLE`: Indicates if the OTP feature is disabled (set to 1).
 * - `Battery`: Indicates if battery detection is disabled (set to 0).
 * - `Heater`: Indicates if the heater is turned off (set to 0).
 * - `Resolution`: Default measurement resolution for temperature and humidity (set to 0).
 */
SHT SHT21 = {
    .OTP_DISABLE = 1,   ///< OTP disable setting (factory settings restore enabled)
    .Battery = 0,       ///< Battery detection disabled (2.25V detection off)
    .Heater = 0,        ///< Heater turned off
    .Resolution = 0,    ///< Default resolution setting for temperature and humidity
};

#endif /* SHT45VAR_H_ */
