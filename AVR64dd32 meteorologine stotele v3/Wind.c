/*
 * Wind.c
 *
 * Created: 2024-12-19 19:27:04
 *  Author: Saulius
 * 
 * This file contains functions for reading and processing wind speed and direction.
 * It uses the ADC to measure the wind speed and direction, and calculates the corresponding values.
 * The wind direction is converted to a short name based on the measured ADC value.
 */

#include "Settings.h"
#include "WindVar.h"

/**
 * @brief Reads the wind speed from the ADC and calculates the speed in m/s.
 * 
 * The wind speed is calculated based on the ADC value and the scaling factor. The scaling factor is 
 * determined by the sensor's characteristics (in this case, 0.00732421875). This function updates the 
 * global variable `Wind.speed` with the calculated value.
 */
void WindSpeed(){
	ADC0_SetupWS();
	Wind.speed = (ADC0_read()* 0.00732421875); // same as *30m/s /4096 = 0.00732421875 //and rounding to lower side
}

/**
 * @brief Reads the wind direction from the ADC and determines the wind direction.
 * 
 * The wind direction is determined by reading the ADC value and comparing it to predefined thresholds. 
 * Based on the comparison, the corresponding direction is stored in the global variable `Wind.direction`.
 */
void WindDirection(){
	ADC0_SetupWD();
	uint16_t rawDirect = 0;
	rawDirect = ADC0_read();

    if (rawDirect < WINDADCHALFSTEP)
	    Wind.direction = 0; // Position 0
	else if (rawDirect > WINDADCRES - WINDADCHALFSTEP)
	    Wind.direction = 7; // Position 7
	else { //Positions 1-6
	    uint8_t position = (rawDirect + WINDADCHALFSTEP) / WINDADCSTEP;
	    uint16_t center = position * WINDADCSTEP;
	    if (rawDirect >= center - WINDDIRTOLERANCE && rawDirect <= center + WINDDIRTOLERANCE)
		    Wind.direction = position;
    }
}

/**
 * @brief Returns the short name of the wind direction.
 * 
 * This function returns a short name for the wind direction based on the value of the global variable 
 * `Wind.direction`. The possible directions are NE (Northeast), E (East), SE (Southeast), S (South), SW 
 * (Southwest), W (West), NW (Northwest), and N (North). The names are returned as a string.
 * 
 * @return A string representing the short name of the wind direction.
 */
const char * WindDirNames(){ // return wind direction short name
	switch(Wind.direction) {
		/*case 1: return "ÐR"; // Lithuanian
		case 2: return "R ";
		case 3: return "PR";
		case 4: return "P ";
		case 5: return "PV";
		case 6: return "V ";
		case 7: return "ÐV";*/
		case 1: return "NE";  // Northeast
		case 2: return "E ";  // East
		case 3: return "SE"; // Southeast
		case 4: return "S ";  // South
		case 5: return "SW"; // Southwest
		case 6: return "W ";  // West
		case 7: return "NW"; // Northwest
	}
	//return "Ð "; // Lithuanian: North
	return "N ";  // English: North
}
