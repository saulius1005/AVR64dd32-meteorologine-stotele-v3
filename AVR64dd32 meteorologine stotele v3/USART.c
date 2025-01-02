/*
 * USART.c
 *
 * Created: 2024-12-10 16:21:15
 * Author: Saulius
 *
 * @brief This file contains functions for initializing and using USART (Universal Synchronous Asynchronous Receiver Transmitter)
 *        for communication on two USART channels (USART0 and USART1). Functions for sending and receiving characters and strings
 *        as well as printing formatted messages are provided.
 */

#include "Settings.h"

/**
 * @brief Initializes USART0 with a baud rate of 2500000.
 * 
 * This function configures USART0 for asynchronous communication, enabling both
 * transmission and reception at a baud rate of 2.5 Mbps with double-speed operation.
 */
void USART0_init() {
	USART0.BAUD = (uint16_t)USART0_BAUD_RATE(2500000); // Set baud rate to 2.5 Mbps
	USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_RXMODE_CLK2X_gc; // Enable RX, TX, double speed mode
	USART0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc; // Configure for 8-bit, no parity, 1 stop bit, asynchronous mode
}

/**
 * @brief Sends a single character via USART0.
 * 
 * This function waits until the USART0 data register is empty and then transmits a character.
 * 
 * @param c The character to send.
 */
void USART0_sendChar(char c) {
	while (!(USART0.STATUS & USART_DREIF_bm)); // Wait for data register to be empty
	USART0.TXDATAL = c; // Send character
}

/**
 * @brief Sends a string via USART0.
 * 
 * This function sends each character of the string one by one using the USART0_sendChar function.
 * 
 * @param str The string to send.
 */
void USART0_sendString(char *str) {
	for (size_t i = 0; i < strlen(str); i++) {
		USART0_sendChar(str[i]); // Send each character
	}
}

/**
 * @brief Prints a single character to a stream using USART0.
 * 
 * This function is used with the standard output stream (stdout) to send characters to USART0.
 * 
 * @param c The character to print.
 * @param stream The stream to print the character to.
 * @return 0 on success.
 */
int USART0_printChar(char c, FILE *stream) {
	USART0_sendChar(c); // Send character
	return 0;
}

/**
 * @brief Reads a single character from USART0.
 * 
 * This function waits until a character is received on USART0, clears the receive interrupt flag, and then returns the received character.
 * 
 * @return The received character.
 */
char USART0_readChar() {
	USART0.STATUS = USART_RXCIF_bm; // Clear buffer before reading
	while (!(USART0.STATUS & USART_RXCIF_bm)); // Wait for data to be received
	return USART0.RXDATAL; // Return received character
}

/**
 * @brief Initializes USART1 with a baud rate of 2500000.
 * 
 * This function configures USART1 for asynchronous communication, enabling both
 * transmission and reception at a baud rate of 2.5 Mbps with double-speed operation.
 */
void USART1_init() {
	USART1.BAUD = (uint16_t)USART1_BAUD_RATE(2500000); // Set baud rate to 2.5 Mbps
	USART1.CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_RXMODE_CLK2X_gc; // Enable RX, TX, double speed mode
	USART1.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc; // Configure for 8-bit, no parity, 1 stop bit, asynchronous mode
}

/**
 * @brief Sends a single character via USART1.
 * 
 * This function waits until the USART1 data register is empty and then transmits a character.
 * 
 * @param c The character to send.
 */
void USART1_sendChar(char c) {
	while (!(USART1.STATUS & USART_DREIF_bm)); // Wait for data register to be empty
	USART1.TXDATAL = c; // Send character
}

/**
 * @brief Sends a string via USART1.
 * 
 * This function sends each character of the string one by one using the USART1_sendChar function.
 * 
 * @param str The string to send.
 */
void USART1_sendString(char *str) {
	for (size_t i = 0; i < strlen(str); i++) {
		USART1_sendChar(str[i]); // Send each character
	}
}

/**
 * @brief Prints a single character to a stream using USART1.
 * 
 * This function is used with the standard output stream (stdout) to send characters to USART1.
 * 
 * @param c The character to print.
 * @param stream The stream to print the character to.
 * @return 0 on success.
 */
int USART1_printChar(char c, FILE *stream) {
	USART1_sendChar(c); // Send character
	return 0;
}

/**
 * @brief Reads a single character from USART1.
 * 
 * This function waits until a character is received on USART1, clears the receive interrupt flag, and then returns the received character.
 * If a timeout occurs, it returns a predefined warning.
 * 
 * @return The received character.
 */
char USART1_readChar() {
	USART1.STATUS = USART_RXCIF_bm; // Clear buffer before reading
	uint32_t timeout_counter = TIMEOUT_COUNTER; // Set a timeout counter
	while (!(USART1.STATUS & USART_RXCIF_bm)) { // Wait for data to be received
		if (--timeout_counter == 0) { // Timeout condition
			Date_Clock.warning = 3; // Set warning if timeout occurs
			break;
		}
	}
	return USART1.RXDATAL; // Return received character
}

/**
 * @brief Sends a formatted string via the selected USART.
 * 
 * This function formats the input string with the provided arguments and sends it via the specified USART (either USART0 or USART1).
 * 
 * @param usart_number The USART number (0 or 1).
 * @param format The format string.
 * @param ... The arguments to be formatted into the string.
 */
void USART_printf(uint8_t usart_number, const char *format, ...) {
	char buffer[128]; // Temporary buffer for formatted message
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args); // Format the message into the buffer
	va_end(args);

	// Select the USART channel for sending the formatted string
	if (usart_number == 0) {
		USART0_sendString(buffer); // Use USART0 for sending
	} else if (usart_number == 1) {
		USART1_sendString(buffer); // Use USART1 for sending
	} else {
		// Handle invalid USART number – add error management if needed
	}
}
