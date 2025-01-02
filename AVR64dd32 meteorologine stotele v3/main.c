/**
 * @file AVR64dd32_meteorologine_stotele_v3.c
 * @brief Main program for a meteorological station on AVR64dd32 microcontroller.
 * 
 * This program reads data from various sensors such as temperature, pressure, and humidity 
 * using I2C and other protocols. It calculates environmental parameters such as wind speed, 
 * wind direction, and solar level. It also sends data to external devices via USART and displays 
 * information on a screen.
 * 
 * The program initializes various hardware components, reads sensor data, performs calculations, 
 * and outputs the results in a structured format. The key functionality includes reading data 
 * from the SHT21 and BMP280 sensors, calculating the true temperature and pressure, and displaying 
 * the results.
 * 
 * Created: 2024-12-04 16:52:56
 * Author: Saulius
 */

#include "Settings.h"

int main(void)
{
    // Initialize system clock, GPIO, I2C, ADC, USART, and screen
    CLOCK_XOSCHF_crystal_init();
    GPIO_init();
    I2C_init();
    ADC0_init();
    USART0_init();
    USART1_init();
    screen_init();

    // Configure SHT21 sensor settings
    SHT21.Resolution = RH_12b_T_14b; // Set resolution for RH and temperature measurements
    SHT21.Heater = OFF; // Disable heater (adds ~0.5-1.5°C)
    SHT21.OTP_DISABLE = ON; // Keep changed settings
    SHT21.Battery = OFF; // Disable battery detection
    SHT21_Settings_Write(); // Apply the settings

    // Configure BMP280 sensor settings
    BMP280.Config.osrs_p = BMP280_Pressure_UHR; // Set oversampling for pressure
    BMP280.Config.osrs_t = BMP280_Temperature_Os_x16; // Set oversampling for temperature
    BMP280.Config.Mode = BMP280_Mode_Normal; // Set normal mode
    BMP280.Config.t_sb = BMP280_StanBy_0m5; // Set standby time
    BMP280.Config.filter = BMP280_Filter_16; // Set filter
    BMP280.Config.spi3w_en = BMP280_SPI_Mode_3w; // Set SPI mode
    WriteBMP280Config(); // Write configuration to BMP280
    ReadBMP280Calibration(); // Read BMP280 calibration values

    screen_clear(); // Clear the screen

    while (1) 
    {
        // Read and process sensor data
        ReadBMP280TP(); // Read temperature and pressure from BMP280
        CalcTrueTemp(); // Calculate true temperature from BMP280
        // BMP280.CalibrationValues.t_fine = SHT21.T * 5120.0; // Uncomment if you want to use SHT21 temperature instead of BMP280 for pressure calculations
        CalcTruePres(); // Calculate true pressure

        // Read humidity and temperature from SHT21
        Separator(SHT21_Read(HOLD_MASTER_RH_MES)); // Read humidity from SHT21
        Separator(SHT21_Read(HOLD_MASTER_T_MES)); // Read temperature from SHT21

        // Retransmit data from the clock device via USART1
        Retransmitt();

        // Read and process additional environmental parameters
        WindSpeed(); // Calculate wind speed
        WindDirection(); // Calculate wind direction
        SunLevel(); // Calculate sun level
        AltitudeAverage(); // Calculate altitude based on pressure

        // Handle keypad input
        keypad();

        // Display data on screen based on selected window
        windows();

        // Send data over USART (e.g., sun azimuth, wind speed, etc.)
        USART_printf(0, "{%.2f|%.2f|%d|%d|%d}\r\n", 
                     SUN.adjazimuth, SUN.adjelevation, 
                     Wind.speed, Wind.direction, 
                     SUN.sunlevel); // Send formatted data
    }
}
