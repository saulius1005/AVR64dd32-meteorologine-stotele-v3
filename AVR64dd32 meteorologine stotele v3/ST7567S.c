/*
 * ST7567S.c
 *
 * Created: 2024-12-10 23:10:03
 * Author: Saulius
 *
 * @brief This file contains the implementation of functions for controlling
 *        the ST7567S LCD display. These functions handle sending commands,
 *        drawing images, text, and other display-related tasks such as 
 *        setting contrast, clearing the screen, and text alignment.
 */

#include "Settings.h"
#include "ST7567Var.h"
#include "font.h"

/**
 * @brief Sends a command byte to the ST7567S display.
 * 
 * This function writes a single command byte to the display, using the appropriate 
 * command register.
 * 
 * @param cmd The command byte to send to the display.
 */
void screen_command(uint8_t cmd) {
    WriteToReg(ST7567S_ADD, 0x00, cmd);  ///< Send command to ST7567S display
}

/**
 * @brief Sends data byte to the ST7567S display.
 * 
 * This function writes a single data byte to the display.
 * 
 * @param data The data byte to send to the display.
 */
void screen_data(uint8_t data) {
    WriteToReg(ST7567S_ADD, 0x40, data);  ///< Send data to ST7567S display
}

/**
 * @brief Initializes the ST7567S LCD display.
 * 
 * This function sends a series of initialization commands to the display to set it 
 * up for use.
 */
void screen_init() { 
    TransmitAdd(ST7567S_ADD, WRITE);
    for (uint8_t i = 0; i < ST7567S_CMD_COUNT; i++) {
        TransmitByte(st7567sCommands[i]);  ///< Send initialization command
    }
}

/**
 * @brief Draws an image on the ST7567S display.
 * 
 * This function draws an image on the LCD screen by sending image data for each 
 * page and column. It supports both PGM (flash memory) and direct SRAM images.
 * 
 * @param mode The mode for reading image data (0 for PGM, 1 for SRAM).
 * @param image_data A pointer to the image data.
 */
void screen_draw_image(uint8_t mode, const uint8_t *image_data) {
    uint8_t cmd[4] = {0x00, 0xb0, 0x10, 0x00};

    for (uint8_t page = 0; page < ST7567S_PAGE_COUNT; page++) {
        cmd[1] = 0xb0 + page;
        TransmitAdd(ST7567S_ADD, WRITE);
        for (uint8_t i = 0; i < 4; i++) {
            TransmitByte(cmd[i]);  ///< Send page and column commands
        }
        uint8_t page_offset = 7 - page;
        for (uint8_t col = 0; col < ST7567S_SCREEN_WIDTH; col++) {
            uint16_t index = col * 8 + page_offset;
            uint8_t dat[2] = {0x40, 0x00};
            if (mode == 0) {
                dat[1] = pgm_read_byte(&image_data[index]);  ///< Read from program memory
            } else {
                dat[1] = image_data[index];  ///< Read directly from SRAM
            }
            TransmitAdd(ST7567S_ADD, WRITE);
            for (uint8_t i = 0; i < 2; i++) {
                TransmitByte(dat[i]);  ///< Send image data byte
            }
        }
    }
}

/**
 * @brief Draws a character on the ST7567S display.
 * 
 * This function draws a single character on the display using the font array. If the 
 * character is out of range, it substitutes a question mark (ASCII 63).
 * 
 * @param c The character to draw.
 */
void screen_draw_char(char c) {
    if ((c < 32 || c > 127) && (c != 176) && (c < 192)) {
        c = 32;  ///< If character is unsupported, use a space
    }

    uint8_t minus = 32;  ///< Default offset for printable characters (ASCII 32-127)
    
    if (c == 176) {
        minus = 80;  ///< Special character 176 uses a different offset
    } else if (c > 191) {
        minus = 95;  ///< Characters from 192-255 start from index 97
    }

    // Draw the character using the font data
    for (uint8_t i = 0; i < 5; i++) {
        uint8_t line = font[c - minus][i];  ///< Get the corresponding font byte
        screen_data(line);  ///< Display the character line by line
    }

    screen_data(0x00);  ///< Add space between characters
}

/**
 * @brief Adjusts the contrast of the ST7567S display.
 * 
 * This function sets the contrast level of the LCD display, where the maximum value 
 * is 63 (0x3F).
 * 
 * @param contrast The contrast level (0 to 63).
 */
void screen_contrast(uint8_t contrast) {
    if (contrast > 0x3f) {
        contrast = 0x3f;  ///< Ensure contrast does not exceed maximum
    }
    screen_command(0x81);  ///< Send command to set contrast
    screen_command(contrast);  ///< Set the contrast value
}

/**
 * @brief Clears the ST7567S display.
 * 
 * This function clears the entire display by setting all pixels to 0 and restoring 
 * the default contrast.
 */
void screen_clear() {
    for (int page = 0; page < 8; page++) {
        screen_command(0xB0 + page);  ///< Select page
        screen_command(0x00);  ///< Set column address
        screen_command(0x10);  ///< Set column address

        for (int column = 0; column < 128; column++) {
            screen_data(0x00);  ///< Clear each column
        }
    }
    screen_contrast(ST7567S_CONTRAST);  ///< Restore contrast
}

/**
 * @brief Draws a string of text on the ST7567S display.
 * 
 * This function draws a string of text on the display, ensuring that no more than 
 * the maximum allowed characters are displayed.
 * 
 * @param text A pointer to the text string to draw.
 * @param max_length The maximum number of characters to display.
 */
void screen_draw_text(char *text, uint8_t max_length) {
    uint8_t length = 0;
    while (*text && length < max_length) {
        screen_draw_char(*text);  ///< Draw each character in the string
        text++;
        length++;
    }
    while (length < max_length) {
        screen_draw_char(' ');  ///< Fill remaining space with spaces
        length++;
    }
}

/**
 * @brief Writes a string of text to a specific line and starting pixel on the ST7567S display.
 * 
 * This function writes the text to a specific line, starting at a specified pixel 
 * position. It calculates the maximum number of characters that can fit in the 
 * remaining space.
 * 
 * @param text A pointer to the text string to write.
 * @param line The line (page) where the text will be written.
 * @param start_pixel The starting pixel column for the text.
 */
void screen_write_text(char *text, uint8_t line, uint8_t start_pixel) {
    uint8_t max_chars = (128 - start_pixel) / 6;  ///< Calculate max characters per line
    screen_command(0xB0 | line);  ///< Set the page (line)
    screen_command(0x10 | (start_pixel >> 4));  ///< Set high byte of column address
    screen_command(0x00 | (start_pixel & 0x0F));  ///< Set low byte of column address
    screen_draw_text(text, max_chars);  ///< Draw the text
}

/**
 * @brief Calculates the starting pixel based on text alignment.
 * 
 * This function calculates the starting pixel for the text based on the chosen 
 * alignment (left, center, right).
 * 
 * @param text A pointer to the text string.
 * @param max_length The maximum number of characters.
 * @param alignment The desired text alignment (left, center, right).
 * 
 * @return The starting pixel for the text.
 */
uint8_t calculate_start_pixel(char *text, alignment_t alignment) {
    uint8_t text_length = 0;
    while (text[text_length] != '\0') {
        text_length++;
    }

    uint8_t text_width = text_length * 6;  ///< Calculate the width of the text in pixels

    switch (alignment) {
        case ALIGN_CENTER:
            return (128 - text_width) / 2;  ///< Center the text
        case ALIGN_RIGHT:
            return (128 - text_width);  ///< Right-align the text
        case ALIGN_LEFT:
        default:
            return 0;  ///< Left-align the text
    }
}

/**
 * @brief Writes a string of text with alignment on the ST7567S display.
 * 
 * This function writes the text to a specific line, adjusting for alignment (left, 
 * center, or right).
 * 
 * @param text A pointer to the text string to write.
 * @param line The line (page) where the text will be written.
 * @param alignment The desired text alignment (left, center, right).
 */
void screen_write_text_aligned(char *text, uint8_t line, alignment_t alignment) {
	uint8_t start_pixel = calculate_start_pixel(text, alignment);  ///< Calculate start pixel
	screen_write_text(text, line, start_pixel);
}

/**
 * @brief Writes formatted text with alignment to the ST7567S display.
 * 
 * This function formats the text using the specified format string and arguments 
 * and then writes the formatted text to the display with the chosen alignment.
 * 
 * @param format The format string for the text.
 * @param line The line (page) where the text will be written.
 * @param alignment The desired text alignment (left, center, right).
 */
void screen_write_formatted_text(const char *format, uint8_t line, alignment_t alignment, ...) {
    char textStorage[MAX_TEXT_LENGTH];  ///< Buffer for storing formatted text
    va_list args;  ///< Variable argument list

    va_start(args, alignment);  ///< Start reading variable arguments
    vsnprintf(textStorage, MAX_TEXT_LENGTH, format, args);  ///< Format the text
    va_end(args);  ///< End reading variable arguments

    screen_write_text_aligned(textStorage, line, alignment);  ///< Write formatted text to display
}
