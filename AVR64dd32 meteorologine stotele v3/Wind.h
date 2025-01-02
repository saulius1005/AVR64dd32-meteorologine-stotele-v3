/*
 * Wind.h
 *
 * Created: 2024-12-19 19:28:39
 *  Author: Saulius
 * 
 * This file contains the definition of the `WindParam` structure and the declaration of the 
 * global `Wind` variable, which holds the wind speed and direction.
 */

#ifndef WIND_H_
#define WIND_H_

#define WINDADCRES 4095 //ADC resolution for WIND direction. This time it is 12bit
#define WINDADCSTEP (WINDADCRES / 7) // 8 values for Wind directions: 585
#define WINDADCHALFSTEP (WINDADCSTEP / 2) //Half step is required for tolerance calculation for 0 and 7 directions: 292
#define WINDDIRTOLERANCE (WINDADCSTEP / 4) // Toleration ± 146 steps

/**
 * @brief Structure for storing wind parameters.
 * 
 * This structure holds two values:
 * - `speed`: The wind speed, in m/s.
 * - `direction`: The wind direction, represented as an integer code (e.g., 0 for North, 1 for Northeast, etc.).
 */
typedef struct {
	uint8_t speed;      ///< Wind speed in m/s
	uint8_t direction;  ///< Wind direction (0 = North, 1 = Northeast, 2 = East, etc.)
} WindParam;

/**
 * @brief External variable representing the current wind parameters.
 * 
 * The `Wind` variable holds the current wind speed and direction. This variable is updated 
 * by the `WindSpeed` and `WindDirection` functions.
 */
extern WindParam Wind;

#endif /* WIND_H_ */
