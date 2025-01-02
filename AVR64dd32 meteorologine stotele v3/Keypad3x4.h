/**
 * @file Keypad3x4.h
 * @brief Header file for managing a 3x4 keypad matrix.
 * 
 * This file defines constants and structures used for handling key press events,
 * including debounce handling, key press detection, and long press detection.
 * It also defines the global structure used to store the current key states.
 * 
 * Created: 2024-12-11 22:19:46
 * Author: Saulius
 */

#ifndef KEYPAD3X4_H_
#define KEYPAD3X4_H_

/** 
 * @brief Debounce delay in milliseconds.
 * 
 * This constant defines the debounce delay, which helps filter out noise from
 * mechanical key presses and ensures stable key state detection.
 */
#define DEBOUNCE_DELAY 10 ///< Debounce delay in milliseconds

/** 
 * @brief Hold threshold in milliseconds.
 * 
 * This constant sets the duration in milliseconds that a key must be held down
 * before being considered a "long press". Short presses will be registered as
 * regular key presses, while long presses are registered separately.
 */
#define HOLD_THRESHOLD 50 ///< Time in milliseconds to register a "hold"

/** 
 * @brief Value added to a key number for long presses.
 * 
 * This constant is used to differentiate between short and long presses. 
 * Short presses are numbered 1-12, while long presses are numbered 21-32.
 */
#define LONG_PRESS_ADD 20 ///< Short press is 1-12, long press is 21-32

/** 
 * @brief Structure to store the current state of a keypress.
 * 
 * This structure holds the current key that is pressed and the key that is held
 * down for a long duration. It helps track both short and long presses separately.
 */
typedef struct {
    uint8_t key; ///< The currently pressed key (1-12 for short presses)
    uint8_t key_held; ///< The key number for long presses (21-32)
} KeypadButtons;

/** 
 * @brief Global structure that holds the current key states.
 * 
 * The global variable `Keypad3x4` is used to store the state of the currently pressed
 * key and the key that is being held down for a long duration. It is updated by 
 * keypad scanning functions.
 */
extern KeypadButtons Keypad3x4;

#endif /* KEYPAD3X4_H_ */
