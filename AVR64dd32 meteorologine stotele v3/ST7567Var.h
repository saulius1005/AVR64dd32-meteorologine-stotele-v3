/*
 * ST7567Var.h
 *
 * Created: 2024-12-10 23:10:51
 * Author: Saulius
 *
 * @brief This header file contains the initialization command sequence for the 
 *        ST7567S LCD display. These commands are used to configure the display's
 *        operation such as power control, bias voltage, contrast, and display orientation.
 */

#ifndef ST7567VAR_H_
#define ST7567VAR_H_

/** 
 * @brief Array of initialization commands for the ST7567S LCD display.
 * 
 * This sequence of commands is used to initialize the ST7567S LCD display, including
 * commands for software reset, power control, bias voltage, scan direction, contrast
 * adjustment, and display on/off settings.
 */
uint8_t st7567sCommands[11] = {
    0x00,      /**< Command mode (default) */
    0xe2,      /**< Software reset command */
    0x2f,      /**< Power control: Booster, Regulator, and Follower all enabled */
    0xa2,      /**< Bias voltage: 1/9 bias */
    0xc8,      /**< Scan direction: C8 (bottom-to-top scanning) */
    0xa0,      /**< Column address direction: A0 (normal) */
    0x40,      /**< Start line address: 40 (line 0 at the beginning) */
    0x81,      /**< Command for setting contrast */
    0x05,      /**< Contrast value (can be adjusted from 0x00 to 0x3F) */
    0xa6,      /**< Display mode: normal (A6), inverse (A7) */
    0xaf       /**< Display ON: Turns the display on */
};

#endif /* ST7567VAR_H_ */
