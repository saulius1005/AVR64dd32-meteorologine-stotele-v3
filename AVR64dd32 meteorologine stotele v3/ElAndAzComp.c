/*
 * ElAndAzComp.c
 *
 * Created: 2024-12-10 18:43:10
 * Author: Saulius
 *
 * This file contains functions for calculating the refraction angle for solar elevation,
 * correcting solar angles, and reading the sun level from an ADC.
 */

#include "Settings.h"
#include "ElAndAzCompVar.h"

/**
 * @brief Calculates the refraction correction based on solar elevation and environmental factors.
 *
 * This function computes the refraction angle that must be applied to the solar elevation angle
 * based on the current atmospheric pressure and temperature, which are adjusted for the altitude
 * of the observer. The refraction correction is calculated using a formula dependent on the solar
 * elevation, pressure, and temperature.
 *
 * @return double The refraction correction in minutes. Returns 0 if the solar elevation is below 
 *                the horizon (i.e., less than or equal to 0 degrees).
 */
double calculate_refraction() {
    if (SUN.elevation <= 0)
        return 0.0;  // No refraction needed if the sun is below the horizon

    double elevation_rad = SUN.elevation * M_PI / 180.0;  // Convert elevation to radians
    // Adjust pressure for altitude (Date_Clock.altitude is in meters)
    if (Date_Clock.altitude > 0) 
        BMP280.Pressure *= pow(1 - (0.0065 * Date_Clock.altitude / 288.15), 5.255); // Temperature lapse rate with altitude

    // Calculate refraction based on solar elevation and environmental factors
    double refraction = 0.0167 / tan(elevation_rad + (10.3 / (SUN.elevation + 5.11)));
    refraction *= (BMP280.Pressure / 1010.0) * (283.0 / (273.0 + SHT21.T));  // Adjust for pressure and temperature
    return refraction;  // Return refraction in minutes
}

/**
 * @brief Corrects the solar angles based on refraction and current solar position.
 *
 * This function adjusts the solar elevation angle by applying the calculated refraction correction
 * if the sun is above the horizon. The azimuth is not corrected in this function.
 * 
 * If the solar elevation is below the horizon, the last known azimuth and elevation values are retained.
 */
void correct_solar_angles() {
    if(SUN.elevation > 0){
        // Adjust the elevation by the refraction and keep the azimuth unchanged
        SUN.adjelevation = SUN.elevation + calculate_refraction() / 60.0;  
        SUN.adjazimuth = SUN.azimuth;
    }
    // If the elevation is below the horizon, retain the last azimuth and elevation values
}

/**
 * @brief Reads and calculates the sun level from an ADC sensor.
 *
 * This function configures the ADC and reads the raw value, which is then scaled down by a factor
 * of 4 and rounded to obtain the sun level. This is typically used to assess the intensity of sunlight.
 */
void SunLevel(){
    ADC0_SetupSLS();  // Set up the ADC for reading the sun level
    SUN.sunlevel = round(ADC0_read()/4);  // Read ADC value, scale it, and round the result
}
