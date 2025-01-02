/**
 * @file Communications.c
 * @brief Communication and data processing functions for date, time, and sensor data.
 *
 * This file contains functions to read, validate, and process data received via communication channels.
 * It includes parsing date, time, solar angles, and geographical coordinates.
 * 
 * @author Saulius
 * @date 2024-12-10
 */

#include "Settings.h"
#include "CommunicationsVar.h"

/**
 * @brief Parses and executes a received command string.
 * 
 * The command string is expected to contain data separated by '|', such as date, time,
 * solar angles, latitude, longitude, and timezone. The function extracts and assigns
 * these values to the respective variables.
 * 
 * @param[in] command Pointer to the command string to be processed.
 */
void executeCommand(char *command) {
    // Extract tokens from the command string
    char *token = strtok(command, "|");

    if (token != NULL) {
        // Parse the first token for date and time
        sscanf(token, "%4u%2u%2u%2u%2u%2u%1u", 
            &Date_Clock.year, &Date_Clock.month, &Date_Clock.day,
            &Date_Clock.hour, &Date_Clock.minute, &Date_Clock.second, 
            &Date_Clock.hunderts);
    }

    // Parse additional tokens for solar angles, latitude, longitude, and timezone
    if ((token = strtok(NULL, "|")) != NULL) {
        SUN.azimuth = atof(token);
    }
    if ((token = strtok(NULL, "|")) != NULL) {
        SUN.elevation = atof(token);
    }
    if ((token = strtok(NULL, "|")) != NULL) {
        Date_Clock.latitude = atof(token);
    }
    if ((token = strtok(NULL, "|")) != NULL) {
        Date_Clock.longitude = atof(token);
    }
    if ((token = strtok(NULL, "|")) != NULL) {
        Date_Clock.timezone = atoi(token);
    }
}

/**
 * @brief Reads and processes clock and data commands.
 * 
 * The function listens for data input, processes commands between `<` and `>` delimiters,
 * and handles error conditions related to data validity.
 */
void ClockAndDataReader() {
    uint8_t index = 0;
    char command[60] = {0};
    uint8_t start = 0;

    while (1) {
        char c = USART1_readChar();

        if (!Date_Clock.warning) {
            if (start == 1) {
                if (c == '>') {
                    start = 0;
                    executeCommand(command);
                    index = 0;
                    break;
                } else {
                    command[index++] = c;
                }
            }
            if (c == '<') {
                start = 1;
            }
            if (Date_Clock.error == 1) {
                screen_clear();
                Date_Clock.errorCounter = 0;
                Date_Clock.error = 0;
            }
        } else {
            Date_Clock.warning = 0;
            if (Date_Clock.errorCounter < CountForError) {
                Date_Clock.errorCounter += 1;
            } else {
                if (!Date_Clock.error) {
                    screen_clear();
                }
                Date_Clock.error = 1;
            }
            break;
        }
    }
}

/**
 * @brief Checks if a year is a leap year.
 * 
 * @param[in] year Year to check.
 * @return 1 if the year is a leap year, otherwise 0.
 */
uint8_t isLeapYear(uint16_t year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/**
 * @brief Validates the given date and time.
 * 
 * @param[in] year Year value.
 * @param[in] month Month value (1-12).
 * @param[in] day Day value.
 * @param[in] hour Hour value (0-23).
 * @param[in] minute Minute value (0-59).
 * @param[in] second Second value (0-59).
 * @return 0 if valid, otherwise 1.
 */
uint8_t isValidDateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (year < 1 || month < 1 || month > 12) {
        return 1;
    }
    if (isLeapYear(year)) {
        daysInMonth[1] = 29;
    }
    if (day < 1 || day > daysInMonth[month - 1] || hour > 23 || minute > 59 || second > 59) {
        return 1;
    }
    return 0;
}

/**
 * @brief Validates the given timezone.
 * 
 * @param[in] timeZone Timezone value (-12 to +12).
 * @return 0 if valid, otherwise 1.
 */
uint8_t isValidTimeZone(int8_t timeZone) {
    return (timeZone < -12 || timeZone > 12) ? 1 : 0;
}

/**
 * @brief Validates the given altitude.
 * 
 * @param[in] altitude Altitude value in meters (-500 to 9000).
 * @return 0 if valid, otherwise 1.
 */
uint8_t isValidAltitude(int16_t altitude) {
    return (altitude < -500 || altitude > 9000) ? 1 : 0;
}

/**
 * @brief Validates the given latitude.
 * 
 * @param[in] latitude Latitude value in degrees (-180 to +180).
 * @return 0 if valid, otherwise 1.
 */
uint8_t isValidLatitude(double latitude) {
    return (latitude < -90.0 || latitude > 90.0) ? 1 : 0;
}

/**
 * @brief Validates the given longitude.
 * 
 * @param[in] longitude Longitude value in degrees (-90 to +90).
 * @return 0 if valid, otherwise 1.
 */
uint8_t isValidLongitude(double longitude) {
    return (longitude < -180.0 || longitude > 180.0) ? 1 : 0;
}

/**
 * @brief Reads clock data, processes solar angles, and retransmits formatted output.
 */
void Retransmitt() {
    ClockAndDataReader();
    correct_solar_angles();
    printf("%4d-%02d-%02d %02d:%02d:%02d: Az.: % 3.2f El.: % 3.2f T: %2.2fC P: %4.2fhPa RH: %2.2f%%\r\n",
        Date_Clock.year,
        Date_Clock.month,
        Date_Clock.day,
        Date_Clock.hour,
        Date_Clock.minute,
        Date_Clock.second,
        SUN.azimuth,
        SUN.adjelevation,
        SHT21.T,
        BMP280.Pressure,
        SHT21.RH
    );
}
