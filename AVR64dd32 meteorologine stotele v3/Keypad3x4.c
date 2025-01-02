/**
 * @file Keypad3x4.c
 * @brief Functions for handling a 3x4 Keypad matrix
 * 
 * This file contains functions for scanning and detecting key presses 
 * on a 3x4 keypad, as well as handling key states and debouncing.
 * It includes methods for scanning the keypad, detecting key presses, 
 * and updating key states for both short and long key presses.
 * 
 * Created: 2024-12-11 22:04:49
 * Author: Saulius
 */

#include "Settings.h"
#include "Keypad3x4Var.h"

/**
 * @brief Scans the 3x4 keypad for pressed keys.
 * 
 * This function scans the rows and columns of a 3x4 keypad to detect 
 * which key is pressed. It returns the key number based on the row and 
 * column mapping or 0 if no key is pressed.
 * 
 * @return uint8_t The key number (1-12), or 0 if no key is pressed.
 */
uint8_t scan_keypad() {
    uint8_t row_pins[4] = {PIN7_bm, PIN6_bm, PIN3_bm, PIN2_bm}; ///< Row pins
    uint8_t col_pins[3] = {PIN1_bm, PIN4_bm, PIN3_bm}; ///< Column pins

    for (uint8_t row = 0; row < 4; row++) {
        PORTD.OUTCLR = row_pins[row]; // Set the current row pin low
        uint8_t pin_state = 0;
        for (uint8_t col = 0; col < 3; col++) {
            if (col == 0) {
                pin_state = (!(PORTD.IN & col_pins[col])); // Read from PORTD
            } else {
                pin_state = (!(PORTF.IN & col_pins[col])); // Read from PORTF
            }
            if (pin_state) {
                PORTD.OUTSET = row_pins[row]; // Reset the row pin
                return ((row * 3) + col + 1); // Return the key number
            }
        }
        PORTD.OUTSET = row_pins[row]; // Reset the row pin
    }
    return 0; // No key pressed
}

/**
 * @brief Updates the state of the keypad, including debouncing and long press detection.
 * 
 * This function handles key state changes by monitoring the key press state, 
 * managing debouncing, and detecting when a key is held down for a long period.
 * 
 * It updates the global key state (`Keypad3x4.key` and `Keypad3x4.key_held`).
 */
void get_key_state() {
    static uint8_t last_key = 0; ///< Last pressed key (used for debouncing)
    static uint16_t hold_counter = 0; ///< Counter for long press detection
    uint8_t key = scan_keypad(); ///< Scan the keypad for the current key

    // If the same key is still pressed, handle the hold state
    if ((key == last_key) && (key != 0)) {
        hold_counter += DEBOUNCE_DELAY;
        if (hold_counter >= HOLD_THRESHOLD) {
            Keypad3x4.key_held = Keypad3x4.key + 20; // Key is held down
        } else {
            Keypad3x4.key = key; // Key is not yet held
        }
    } 
    // If the key has changed, reset the hold counter
    else if ((key != last_key)) {
        hold_counter = 0;
        Keypad3x4.key = key;
    }

    last_key = Keypad3x4.key; // Update the last key
    _delay_ms(DEBOUNCE_DELAY); // Debounce delay
}

/**
 * @brief Keypad scanning and key state updating with debouncing and long press detection.
 * 
 * This function manages the scanning of the keypad and updates the key state 
 * while handling debouncing and detecting long key presses. The state is updated 
 * and saved in the global `Keypad3x4.key` and `Keypad3x4.key_held` variables.
 */
void keypad() {
    static uint8_t last_key = 0; ///< Last pressed key (used for debouncing)
    static uint16_t hold_counter = 0; ///< Counter for long press detection

    uint8_t key = scan_keypad(); ///< Scan the keypad for the current key

    // Update key state based on current and previous key states
    if ((key == last_key) && (key != 0)) {
        hold_counter += DEBOUNCE_DELAY;
        if (hold_counter == HOLD_THRESHOLD) {
            screen_clear(); // Clear screen on long press detection
            Keypad3x4.key_held = key + LONG_PRESS_ADD; // Key is held down
            Keypad3x4.key = 0; // Reset key state to prevent repeated actions
            hold_counter = HOLD_THRESHOLD + 1;
        } else {
            Keypad3x4.key = key; // Key is pressed for a short time
        }
    } else {
        hold_counter = 0;
        Keypad3x4.key = key;
    }

    last_key = key; // Update the last key state
    _delay_ms(DEBOUNCE_DELAY); // Debounce delay
}
