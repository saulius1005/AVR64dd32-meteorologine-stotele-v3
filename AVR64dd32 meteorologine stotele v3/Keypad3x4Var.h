/**
 * @file Keypad3x4Var.h
 * @brief Variable definitions for the 3x4 keypad matrix handling.
 * 
 * This file defines and initializes the global variable that holds the current
 * state of the keypad buttons, including the key that is currently pressed 
 * and the key that is being held down for a long duration.
 * 
 * Created: 2024-12-13 20:49:45
 * Author: Saulius
 */

#ifndef KEYPAD3X4VAR_H_
#define KEYPAD3X4VAR_H_

/** 
 * @brief Global variable holding the state of the keypad buttons.
 * 
 * The `Keypad3x4` variable is a structure that stores the state of the keypad, including:
 * - `key`: The currently pressed key (for short presses).
 * - `key_held`: The key being held down for a long duration (for long presses).
 * 
 * This variable is used throughout the program to track the current key states.
 */
KeypadButtons Keypad3x4 = {
    .key = 0,        ///< Initial state of the pressed key (no key pressed).
    .key_held = 0    ///< Initial state of the held key (no key held).
};

#endif /* KEYPAD3X4VAR_H_ */
