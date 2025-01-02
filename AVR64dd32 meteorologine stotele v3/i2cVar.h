/**
 * @file i2cVar.h
 * @brief I2C Variable Definitions
 * 
 * This header file defines the global instance of the `I2C_Status` structure, which is used 
 * to store the error state of I2C operations.
 * 
 * Created: 2024-12-05 20:26:32
 * Author: Saulius
 */

#ifndef I2CVAR_H_
#define I2CVAR_H_

/**
 * @brief Global I2C Status Instance
 * 
 * An instance of the `I2C_Status` structure that holds the current error state of I2C communication.
 * The `error` field is initialized to 0, indicating no error at startup.
 */
I2C_Status I2C = {
    .error = 0 ///< Initialize the error state to 0 (no error)
};

#endif /* I2CVAR_H_ */
