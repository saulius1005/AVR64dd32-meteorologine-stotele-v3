/*
 * Settings.h
 *
 * Created: 2024-12-04 17:00:49
 * Author: Saulius
 * 
 * @brief This header file contains function prototypes, macros, and includes necessary for configuring hardware,
 *        managing communication protocols (I2C, USART), and interacting with various sensors and peripherals.
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

/**
 * @brief CPU clock frequency (F_CPU).
 * 
 * This macro defines the system clock frequency used for various calculations, including baud rate configuration.
 */
#define F_CPU 24000000

/**
 * @brief Macro for calculating the USART0 baud rate.
 * 
 * This macro calculates the baud rate for USART0 based on the desired baud rate and system clock (F_CPU).
 * 
 * @param BAUD_RATE Desired baud rate.
 * @return Calculated baud rate setting for USART0.
 */
#define USART0_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (8 *(float)BAUD_RATE)) + 0.5)

/**
 * @brief Macro for calculating the USART1 baud rate.
 * 
 * This macro calculates the baud rate for USART1 based on the desired baud rate and system clock (F_CPU).
 * 
 * @param BAUD_RATE Desired baud rate.
 * @return Calculated baud rate setting for USART1.
 */
#define USART1_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (8 *(float)BAUD_RATE)) + 0.5)

#include <avr/io.h>      /**< Include AVR I/O library for register definitions and hardware control */
#include <avr/cpufunc.h>
#include <util/delay.h>
#include <avr/interrupt.h> /**< Include AVR interrupt library for ISR (Interrupt Service Routine) support */
#include <stdio.h>       /**< Include standard I/O library for functions like printf */
#include <string.h>      /**< Include string library for handling string functions like strlen */
#include <stdlib.h>
#include <math.h>        /**< Include math library for mathematical functions (e.g., sin, cos) */
#include <float.h>       /**< Include float.h for floating point constants like FLT_MAX */
#include <stdbool.h>     /**< Include stdbool.h for boolean type support (true/false) */
#include <avr/pgmspace.h>
#include "i2c.h"
#include "SHT45.h"
#include "BMP390.h"
#include "Altitude.h"
#include "ElAndAzComp.h"
#include "Communications.h"
#include "St7567S.h"
#include "Keypad3x4.h"
#include "Wind.h"

////////////////////////////////////////////////////////////////////////////////
// Function Prototypes
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Initializes the General Purpose I/O (GPIO) pins for necessary tasks.
 * 
 * This function configures the necessary GPIO pins for communication, control, 
 * or other I/O purposes. The exact configuration will depend on the application.
 */
void GPIO_init();

/**
 * @brief Initializes the external oscillator clock.
 * 
 * Configures the external oscillator for system clock usage.
 */
void CLOCK_XOSCHF_clock_init();

/**
 * @brief Initializes the external crystal oscillator.
 * 
 * Configures the external crystal for system clock usage.
 */
void CLOCK_XOSCHF_crystal_init();

/**
 * @brief CRC decoder for CRC8 (Dallas/MAXIM).
 * 
 * Decodes a CRC-8 checksum for a given command.
 *
 * @param command 32-bit command value to decode the CRC.
 * @return CRC-8 checksum value.
 */
uint8_t CRC(uint32_t command);

/**
 * @brief Initializes the I2C bus for communication.
 * 
 * Configures the I2C peripheral to communicate with external devices.
 */
void I2C_init();

/**
 * @brief Transmits an address with read/write bit over I2C.
 * 
 * @param Add Address of the device.
 * @param Read Read or Write flag (1 for Read, 0 for Write).
 * @return 1 if successful, 0 if error.
 */
uint8_t TransmitAdd(uint8_t Add, uint8_t Read);

/**
 * @brief Reads one byte of data from the I2C bus.
 * 
 * @param ack Acknowledge flag.
 * @param data Pointer to store the read data.
 */
void ReadByteInf(uint8_t ack, uint8_t* data);

/**
 * @brief Transmits a byte of data over I2C.
 * 
 * @param Data Data byte to transmit.
 * @return 1 if successful, 0 if error.
 */
uint8_t TransmitByte(uint8_t Data);

/**
 * @brief Writes a byte of data to a register over I2C.
 * 
 * @param Add Address of the device.
 * @param Reg Register to write to.
 * @param Data Data to write.
 */
void WriteToReg(uint8_t Add, uint8_t Reg, uint8_t Data);

/**
 * @brief Reads a byte of data from a register over I2C.
 * 
 * @param Add Address of the device.
 * @param Reg Register to read from.
 * @return Data read from the register.
 */
uint8_t ReadReg(uint8_t Add, uint8_t Reg);

/**
 * @brief Writes multiple bytes of data to a register over I2C.
 * 
 * @param addr Address of the device.
 * @param reg Register to start writing.
 * @param data Data to write.
 * @param bytes Number of bytes to write.
 */
void WriteMulti(uint8_t addr, uint8_t reg, uint64_t data, uint8_t bytes);

/**
 * @brief Reads multiple bytes of data from a register over I2C.
 * 
 * @param addr Address of the device.
 * @param reg Register to start reading.
 * @param bytes Number of bytes to read.
 * @return Data read from the register.
 */
uint64_t ReadMulti(uint8_t addr, uint8_t reg, uint8_t bytes);

/**
 * @brief Initializes USART0 for serial communication.
 * 
 * Configures the USART0 for transmitting and receiving data.
 */
void USART0_init();

/**
 * @brief Sends a character over USART0.
 * 
 * @param c Character to send.
 */
void USART0_sendChar(char c);

/**
 * @brief Sends a string over USART0.
 * 
 * @param str String to send.
 */
void USART0_sendString(char *str);

/**
 * @brief USART0 character output function for printf.
 * 
 * @param c Character to send.
 * @param stream Stream for the output.
 * @return Character sent.
 */
int USART0_printChar(char c, FILE *stream);

/**
 * @brief Reads a character from USART0.
 * 
 * @return Character received.
 */
char USART0_readChar();

/**
 * @brief Initializes USART1 for serial communication.
 * 
 * Configures USART1 for transmitting and receiving data.
 */
void USART1_init();

/**
 * @brief Sends a character over USART1.
 * 
 * @param c Character to send.
 */
void USART1_sendChar(char c);

/**
 * @brief Sends a string over USART1.
 * 
 * @param str String to send.
 */
void USART1_sendString(char *str);

/**
 * @brief USART1 character output function for printf.
 * 
 * @param c Character to send.
 * @param stream Stream for the output.
 * @return Character sent.
 */
int USART1_printChar(char c, FILE *stream);

/**
 * @brief Reads a character from USART1.
 * 
 * @return Character received.
 */
char USART1_readChar();

/**
 * @brief Sends formatted data over USART.
 * 
 * @param usart_number USART port (0 or 1).
 * @param format Format string for the data.
 */
void USART_printf(uint8_t usart_number, const char *format, ...);

/**
 * @brief Initializes the ADC0 (Analog-to-Digital Converter).
 * 
 * Configures the ADC for reading analog signals.
 */
void ADC0_init();

/**
 * @brief Configures ADC0 for the wind sensor.
 */
void ADC0_SetupWS();

/**
 * @brief Configures ADC0 for watchdog functionality.
 */
void ADC0_SetupWD();

/**
 * @brief Configures ADC0 for sensor signal setup.
 */
void ADC0_SetupSLS();

/**
 * @brief Reads data from ADC0.
 * 
 * @return ADC conversion result.
 */
uint16_t ADC0_read();

/**
 * @brief Computes CRC-8 checksum for MAXIM/Dallas devices.
 * 
 * This function computes the 8-bit CRC checksum for the provided data, typically used for validating communication
 * with MAXIM or Dallas sensors.
 * 
 * @param data The 32-bit data for which CRC-8 is calculated.
 * @return The 8-bit CRC value.
 */
uint16_t CRC8MAXIM(uint32_t data);

/**
 * @brief Separates a 32-bit data value into individual components.
 * 
 * This function processes a 32-bit value, possibly splitting it into different parts or performing some form of separation.
 * 
 * @param data The 32-bit data to separate.
 * @return A processed 8-bit result based on the separation logic.
 */
uint8_t Separator(uint32_t data);

/**
 * @brief Reads the settings from the SHT21 sensor.
 * 
 * This function retrieves the configuration or settings from the SHT21 temperature and humidity sensor.
 */
void SHT21_Settings_Read();

/**
 * @brief Writes settings to the SHT21 sensor.
 * 
 * This function writes the configuration or settings to the SHT21 sensor.
 */
void SHT21_Settings_Write();

/**
 * @brief Resets the SHT21 sensor to factory default settings.
 * 
 * This function resets the SHT21 sensor. If the save flag is set to 1, the current settings are saved for future use.
 * If the save flag is 0, the default settings are loaded but not saved.
 * 
 * @param save Flag indicating whether to save the settings (1 = save, 0 = load default only).
 */
void SHT21_Reset(uint8_t save);

/**
 * @brief Reads raw data (temperature or humidity) from the SHT21 sensor.
 * 
 * This function reads the raw data from the SHT21 sensor, including CRC checking.
 * 
 * @param mode The mode specifying whether to read temperature or humidity.
 * @return The 32-bit raw data read from the sensor, including CRC.
 */
uint32_t SHT21_Read(uint8_t mode);

/**
 * @brief Reads the unique ID of the BMP280 sensor.
 * 
 * This function reads and returns the unique ID of the BMP280 sensor, used for identification.
 */
void ReadBMP280ID();

/**
 * @brief Reads the calibration data from the BMP280 sensor.
 * 
 * This function reads and returns the calibration data from the BMP280 sensor, used for accurate pressure and temperature
 * measurements.
 */
void ReadBMP280Calibration();

/**
 * @brief Reads the configuration data from the BMP280 sensor.
 * 
 * This function reads and returns the current configuration of the BMP280 sensor.
 */
void ReadBMP280Config();

/**
 * @brief Writes the configuration data to the BMP280 sensor.
 * 
 * This function writes the configuration settings to the BMP280 sensor to adjust its behavior.
 */
void WriteBMP280Config();

/**
 * @brief Reads the status data from the BMP280 sensor.
 * 
 * This function reads the status register of the BMP280 sensor to check its current operational status.
 */
void ReadBMP280Status();

/**
 * @brief Reads the temperature and pressure data from the BMP280 sensor.
 * 
 * This function reads the temperature and pressure data from the BMP280 sensor.
 */
void ReadBMP280TP();

/**
 * @brief Resets the BMP280 sensor.
 * 
 * This function performs a reset on the BMP280 sensor, restoring it to its default settings.
 */
void ResetBMP280();

/**
 * @brief Calculates the true temperature from the raw BMP280 data.
 * 
 * This function uses the calibration data and raw temperature data from the BMP280 sensor to calculate the true temperature.
 * 
 * @return The calculated true temperature in degrees Celsius.
 */
int16_t CalcTrueTemp();

/**
 * @brief Calculates the true pressure from the raw BMP280 data.
 * 
 * This function uses the calibration data and raw pressure data from the BMP280 sensor to calculate the true pressure.
 * 
 * @return The calculated true pressure in Pascals.
 */
uint32_t CalcTruePres();

/**
 * @brief Computes the average altitude.
 * 
 * This function calculates the average altitude from the available sensor data or computations.
 */
void AltitudeAverage();

/**
 * @brief Computes the wind speed.
 * 
 * This function calculates the wind speed based on sensor readings or other environmental factors.
 */
void WindSpeed();

/**
 * @brief Computes the wind direction.
 * 
 * This function calculates the wind direction based on sensor readings or other environmental factors.
 */
void WindDirection();

/**
 * @brief Computes the sun level.
 * 
 * This function calculates the sun level (possibly related to solar intensity or angle) based on available data.
 */
void SunLevel();

/**
 * @brief Returns the name of the wind direction.
 * 
 * This function returns a human-readable name for the current wind direction, based on the computed or detected value.
 * 
 * @return The wind direction as a string.
 */
const char * WindDirNames();

/**
 * @brief Reads and processes clock and data from the system.
 * 
 * This function reads and processes the system time and other data, possibly for logging or synchronization purposes.
 */
void ClockAndDataReader();

/**
 * @brief Validates the provided date and time.
 * 
 * This function checks if the provided year, month, day, hour, minute, and second form a valid date and time.
 * 
 * @param year The year to validate.
 * @param month The month to validate.
 * @param day The day to validate.
 * @param hour The hour to validate.
 * @param minute The minute to validate.
 * @param second The second to validate.
 * @return 1 if valid, 0 if invalid.
 */
uint8_t isValidDateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);

/**
 * @brief Validates the provided time zone value.
 * 
 * This function checks if the provided time zone is valid.
 * 
 * @param timeZone The time zone value to validate.
 * @return 1 if valid, 0 if invalid.
 */
uint8_t isValidTimeZone(int8_t timeZone);

/**
 * @brief Validates the provided altitude value.
 * 
 * This function checks if the provided altitude is within a valid range.
 * 
 * @param altitude The altitude value to validate.
 * @return 1 if valid, 0 if invalid.
 */
uint8_t isValidAltitude(int16_t altitude);

/**
 * @brief Validates the provided latitude value.
 * 
 * This function checks if the provided latitude is within a valid range (-90 to 90 degrees).
 * 
 * @param latitude The latitude value to validate.
 * @return 1 if valid, 0 if invalid.
 */
uint8_t isValidLatitude(double latitude);

/**
 * @brief Validates the provided longitude value.
 * 
 * This function checks if the provided longitude is within a valid range (-180 to 180 degrees).
 * 
 * @param longitude The longitude value to validate.
 * @return 1 if valid, 0 if invalid.
 */
uint8_t isValidLongitude(double longitude);

/**
 * @brief Corrects the solar angles.
 * 
 * This function computes or adjusts solar angles (such as the azimuth and elevation angles) based on the
 * current location and time, ensuring that the solar data is accurate.
 */
void correct_solar_angles();

/**
 * @brief Initializes the screen.
 * 
 * This function initializes the screen, configuring the necessary hardware interfaces and settings
 * to prepare it for display operations.
 */
void screen_init();

/**
 * @brief Sends a command to the screen.
 * 
 * This function sends a specific command byte to the screen to control its behavior (e.g., to reset, 
 * adjust settings, or switch modes).
 * 
 * @param cmd The command byte to send to the screen.
 */
void screen_command(uint8_t cmd);

/**
 * @brief Sends data to the screen.
 * 
 * This function sends a data byte to the screen, typically to be displayed or used for rendering.
 * 
 * @param cmd The data byte to send to the screen.
 */
void screen_data(uint8_t cmd);

/**
 * @brief Draws an image on the screen.
 * 
 * This function draws an image on the screen based on the provided image data. The image is displayed
 * according to the specified mode.
 * 
 * @param mode The mode in which the image will be drawn (e.g., monochrome, color).
 * @param image_data Pointer to the image data to be displayed on the screen.
 */
void screen_draw_image(uint8_t mode, const uint8_t *image_data);

/**
 * @brief Draws a single character on the screen.
 * 
 * This function draws a character on the screen at the current position.
 * 
 * @param c The character to display.
 */
void screen_draw_char(char c);

/**
 * @brief Draws a string of text on the screen.
 * 
 * This function draws a string of text on the screen, ensuring that it fits within the specified maximum
 * length.
 * 
 * @param text Pointer to the null-terminated string to display.
 * @param max_length The maximum length of text to display.
 */
void screen_draw_text(char *text, uint8_t max_length);

/**
 * @brief Writes text to the screen at a specific line and pixel position.
 * 
 * This function writes text to the screen, starting from the specified line and pixel position.
 * 
 * @param text Pointer to the null-terminated string to display.
 * @param line The line number where the text should be written.
 * @param start_pixel The starting pixel position on the screen line.
 */
void screen_write_text(char *text, uint8_t line, uint8_t start_pixel);

/**
 * @brief Clears the screen.
 * 
 * This function clears the entire screen, typically by resetting all pixels to the background color.
 */
void screen_clear();

/**
 * @brief Adjusts the screen contrast.
 * 
 * This function adjusts the contrast of the screen to make the display more readable.
 * 
 * @param contrast The contrast level to set (typically a value between 0 and 255).
 */
void screen_contrast(uint8_t contrast);

/**
 * @brief Writes text to the screen with alignment on a specific line.
 * 
 * This function writes text to the screen, aligning it according to the specified alignment mode
 * (e.g., left, center, right).
 * 
 * @param text Pointer to the null-terminated string to display.
 * @param line The line number where the text should be written.
 * @param alignment The alignment mode to use (e.g., left, center, right).
 */
void screen_write_text_aligned(char *text, uint8_t line, alignment_t alignment);

/**
 * @brief Writes formatted text to the screen with alignment on a specific line.
 * 
 * This function writes formatted text to the screen, aligning it according to the specified alignment mode
 * (e.g., left, center, right). It supports variable arguments, allowing formatted text similar to printf.
 * 
 * @param format The format string for the text to display.
 * @param line The line number where the text should be written.
 * @param alignment The alignment mode to use (e.g., left, center, right).
 * @param ... The variable arguments for the format string.
 */
void screen_write_formatted_text(const char *format, uint8_t line, alignment_t alignment, ...);

/**
 * @brief Retransmits data.
 * 
 * This function retransmits data that may have been lost or needs to be resent, possibly in the context
 * of communication protocols.
 */
void Retransmitt();

/**
 * @brief Handles window-related operations.
 * 
 * This function manages windowing tasks, such as opening, closing, or resizing windows, depending on
 * the application context.
 */
void windows();

/**
 * @brief Scans the keypad for a pressed key.
 * 
 * This function scans the keypad and returns the key that has been pressed. It typically checks for
 * key states in a 3x4 matrix layout.
 * 
 * @return The key code of the pressed key.
 */
uint8_t scan_keypad();

/**
 * @brief Retrieves the current state of the keypad.
 * 
 * This function checks and returns the current state of all keys on the keypad, indicating which keys
 * are pressed or released.
 */
void get_key_state();

/**
 * @brief Manages keypad input.
 * 
 * This function handles the logic for keypad input, detecting which key is pressed and processing
 * the input accordingly.
 */
void keypad();


#endif /* SETTINGS_H_ */