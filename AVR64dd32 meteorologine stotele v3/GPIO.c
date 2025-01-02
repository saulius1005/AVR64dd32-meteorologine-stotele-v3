/**
 * @file GPIO.c
 * @brief GPIO Initialization for various peripherals and sensors
 * 
 * This file contains the initialization of GPIO pins for communication peripherals such as USART, TWI,
 * and for managing input/output for keypad and sensor interfaces. It configures the appropriate pins for
 * USART, I2C, and other functionalities like keypad scanning and sensor input.
 * 
 * Created: 2024-12-04 16:54:46
 * Author: Saulius
 */

#include "Settings.h"

/**
 * @brief Initializes GPIO pins for USART, TWI, keypad, and sensor interfaces.
 * 
 * This function configures the GPIO pins for various communication protocols and sensors. 
 * - Sets USART0 and USART1 pins for transmitting/receiving data.
 * - Configures I2C pins (SDA and SCL) for communication.
 * - Initializes keypad pins (rows and columns) for input.
 * - Configures ADC pins for wind speed, wind direction, and sun light sensor readings.
 * 
 * It also configures pull-up resistors for specific pins and sets the appropriate output logic levels.
 */
void GPIO_init(){
    // Configure USART0 and USART1 pin routing
    PORTMUX.USARTROUTEA = PORTMUX_USART0_ALT1_gc | PORTMUX_USART1_DEFAULT_gc; // Set USART0 to alternative pins set 1, USART1 to default pins
    PORTMUX.TWIROUTEA = PORTMUX_TWI0_DEFAULT_gc; // Set TWI0 to default pins

    // Configure Port A (PA) for I2C SDA, SCL, USART0 TX and XDIR
    PORTA.DIRSET = PIN2_bm | PIN3_bm | PIN4_bm | PIN7_bm; // Set PA2, PA3, PA4, PA7 as output (I2C SDA, SCL, USART0 TX, XDIR)
    PORTA.DIRCLR = PIN5_bm; // Set PA5 as input (USART0 RX)
    PORTA.PIN4CTRL = PORT_PULLUPEN_bm; // Enable pull-up for PA4 (I2C SCL)
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm; // Enable pull-up for PA5 (USART0 RX)

    // Configure Port C (PC) for USART1 TX and RX
    PORTC.DIRSET = PIN0_bm; // Set PC0 as output (USART1 TX)
    PORTC.OUTCLR = PIN1_bm; // Set PC1 as input (USART1 RX)
    PORTC.PIN0CTRL = PORT_PULLUPEN_bm; // Enable pull-up for PC0 (USART1 TX)
    PORTC.PIN1CTRL = PORT_PULLUPEN_bm; // Enable pull-up for PC1 (USART1 RX)

    // Configure Port F (PF) for Clock change output command
    PORTF.DIRSET = PIN2_bm; // Set PF2 as output (Clock change command)
    PORTF.OUTSET = PIN2_bm; // Set PF2 high (No action, to be controlled by software)

    // Configure Port D (PD) for Keypad row and column control
    PORTD.DIRSET = PIN2_bm | PIN3_bm | PIN6_bm | PIN7_bm; // Set PD2, PD3, PD6, PD7 as output (Keypad rows)
    PORTD.OUTSET = PIN2_bm | PIN3_bm | PIN6_bm | PIN7_bm; // Set Keypad rows to high
    PORTD.OUTCLR = PIN1_bm; // Set PD1 as low (Keypad column 1)
    PORTF.OUTCLR = PIN4_bm | PIN3_bm; // Set PF4 and PF3 as low (Keypad columns 2-3)
    PORTD.PIN1CTRL = PORT_PULLUPEN_bm; // Enable pull-up for PD1 (Keypad column 1)
    PORTF.PIN4CTRL = PORT_PULLUPEN_bm; // Enable pull-up for PF4 (Keypad column 2)
    PORTF.PIN3CTRL = PORT_PULLUPEN_bm; // Enable pull-up for PF3 (Keypad column 3)

    // Configure ADC pins for wind speed, wind direction, and sun light level sensors
    PORTC.PIN2CTRL &= ~PORT_ISC_gm; // Disable interrupt sense for PC2 (Wind speed sensor)
    PORTC.PIN2CTRL |= PORT_ISC_INPUT_DISABLE_gc; // Disable input sense for PC2
    PORTC.PIN2CTRL &= ~PORT_PULLUPEN_bm; // Disable pull-up for PC2
    PORTC.PIN3CTRL &= ~PORT_ISC_gm; // Disable interrupt sense for PC3 (Wind direction sensor)
    PORTC.PIN3CTRL |= PORT_ISC_INPUT_DISABLE_gc; // Disable input sense for PC3
    PORTC.PIN3CTRL &= ~PORT_PULLUPEN_bm; // Disable pull-up for PC3
    PORTA.PIN6CTRL &= ~PORT_ISC_gm; // Disable interrupt sense for PA6 (Sun light sensor)
    PORTA.PIN6CTRL |= PORT_ISC_INPUT_DISABLE_gc; // Disable input sense for PA6
    PORTA.PIN6CTRL &= ~PORT_PULLUPEN_bm; // Disable pull-up for PA6
}
