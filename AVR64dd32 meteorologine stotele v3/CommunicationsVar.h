/**
 * @file CommunicationsVar.h
 * @brief Variable definitions for date, time, and geographical information.
 *
 * This file contains the initialization of the `Date_Clock` variable, which 
 * holds the current date, time, geographical information, and configuration 
 * for timezone and altitude.
 *
 * @author Saulius
 * @date 2024-12-10
 */

#ifndef COMMUNICATIONSVAR_H_
#define COMMUNICATIONSVAR_H_

/** 
 * @brief Global variable to hold date, time, and location information.
 *
 * The `Date_Clock` variable is initialized with the location's latitude, 
 * longitude, timezone offset, and altitude. The structure also tracks the 
 * current date, time, and any error or warning states.
 */
Calendar Date_Clock = {
    .latitude = 0,       /**< Latitude of the location (in degrees) */
    .longitude = 0,      /**< Longitude of the location (in degrees) */
    .timezone = 2,               /**< Base timezone offset (adjust as needed for daylight savings or other time zones) */
    .altitude = -50               /**< Altitude of the location (in meters) */
};

#endif /* COMMUNICATIONSVAR_H_ */
