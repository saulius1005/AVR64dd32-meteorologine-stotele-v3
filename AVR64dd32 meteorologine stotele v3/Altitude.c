/**
 * @file Altitude.c
 * @brief Contains functions for altitude calculations based on atmospheric pressure and temperature.
 * @author Saulius
 * @date 2024-12-09
 */

#include "Settings.h"
#include "AltitudeVar.h"

/**
 * @brief Calculates the adjusted elevation based on atmospheric pressure, temperature, and humidity.
 *
 * The function performs the following steps:
 * 1. Converts the temperature to Kelvin.
 * 2. Calculates the saturation vapor pressure and the vapor pressure.
 * 3. Adjusts the atmospheric pressure by subtracting the vapor pressure.
 * 4. Computes the elevation using the barometric formula.
 *
 * @return The calculated elevation in meters.
 */
double calculate_adjusted_elevation() {
    // 1. Adjust relative humidity
    double temp_k = SHT21.T + T0; // Convert temperature to Kelvin
    double es = A * exp((B * SHT21.T) / (SHT21.T + C)); // Saturation vapor pressure (hPa)
    double e = es * (SHT21.RH / 100.0); // Vapor pressure (hPa)
    
    // 2. Adjust atmospheric pressure
    double adjusted_pressure = BMP280.Pressure - e; // Adjusted atmospheric pressure (hPa)

    // 3. Calculate elevation
    double elevation = (temp_k / GRAVITY) * log(SEA_LEVEL_PRESSURE / adjusted_pressure) * 
                       (UNIVERSAL_GAS_CONSTANT / MOLAR_MASS_AIR);

    return elevation;
}

/**
 * @brief Calculates the average altitude using uncompensated and compensated elevation values.
 *
 * The function computes:
 * - Uncompensated altitude based on the barometric formula.
 * - Compensated altitude using `calculate_adjusted_elevation()`.
 * - Average altitude as the mean of uncompensated and compensated values.
 */
void AltitudeAverage() {
    Altitude.UNCOMP = 44330.7692307 * (pow(BMP280.Pressure / SEA_LEVEL_PRESSURE, -0.1902632) - 1); 
    // -0.1902632: Precomputed value of (-R*L/g*M). 44330.7692307 is T0/L.

    Altitude.COMP = calculate_adjusted_elevation();

    Altitude.AVRG = (Altitude.UNCOMP + Altitude.COMP) / 2;
}
