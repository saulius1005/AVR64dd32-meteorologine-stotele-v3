/**
 * @file Communications.h
 * @brief Header file for communication and data processing functions.
 *
 * This file defines structures, constants, and declarations for functions 
 * related to date, time, and sensor data handling in the communication system.
 * 
 * @author Saulius
 * @date 2024-12-10
 */

#ifndef COMMUNICATIONS_H_
#define COMMUNICATIONS_H_

/** 
 * @brief Error count threshold for handling errors.
 */
#define CountForError 10

/** 
 * @brief Calendar structure to store date, time, and geographical information.
 *
 * This structure is used to hold the current date, time, latitude, longitude,
 * timezone, altitude, and error/warning states.
 */
typedef struct {
    int year;          /**< Year (e.g., 2024) */
    int month;         /**< Month (1 to 12) */
    int day;           /**< Day of the month (1 to 31) */
    int hour;          /**< Hour (0 to 23) */
    int minute;        /**< Minute (0 to 59) */
    int second;        /**< Second (0 to 59) */
    int hunderts;      /**< Hunderts of a second */
    double latitude;   /**< Latitude in decimal degrees (-90.0000 to 90.0000) */
    double longitude;  /**< Longitude in decimal degrees (-180.0000 to 180.0000) */
    int timezone;      /**< Timezone offset from UTC (-12 to +12) */
    int16_t altitude;  /**< Altitude in meters (-500 to 9000) */
    uint8_t error;     /**< Error flag (1 if an error occurs, 0 otherwise) */
    uint8_t errorCounter; /**< Counter for error occurrences */
    uint8_t warning;   /**< Warning flag (1 if a warning occurs) */
    uint32_t counterImitator; /**< Counter for imitating time-related data */
} Calendar;

/** 
 * @brief Extern reference to the Calendar structure.
 * 
 * The `Date_Clock` variable holds the current date, time, and related information.
 */
extern Calendar Date_Clock;

#endif /* COMMUNICATIONS_H_ */
