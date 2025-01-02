/**
 * @file i2c.h
 * @brief Header file for I2C Communication Functions
 * 
 * This header file defines constants, macros, and function prototypes for I2C communication.
 * It also defines a structure for storing the I2C status, including error codes.
 * 
 * Created: 2024-12-05 14:29:10
 * Author: Saulius
 */

#ifndef I2C_H_
#define I2C_H_

/**
 * @brief I2C Clock Frequency
 * 
 * Defines the I2C clock frequency (1000 kHz) used for communication.
 */
#define F_SCL 1200000UL   ///< I2C clock frequency (1000 kHz)

/**
 * @brief Timeout Counter
 * 
 * Defines the timeout value for I2C operations.
 */
#define TIMEOUT_COUNTER 500000 ///< Timeout counter value for operations

/**
 * @brief Baud Rate Calculation
 * 
 * Calculates the I2C baud rate based on the system clock frequency and the I2C clock frequency.
 */
#define TWI_BAUD ((F_CPU - 10 * F_SCL) / (2 * F_SCL))

/**
 * @brief Write Operation Flag
 * 
 * Defines the value to be used for indicating a write operation in I2C.
 */
#define WRITE 0 ///< Write operation flag

/**
 * @brief Read Operation Flag
 * 
 * Defines the value to be used for indicating a read operation in I2C.
 */
#define READ 1 ///< Read operation flag

/**
 * @brief Stop Condition Flag
 * 
 * Defines the flag value for sending a STOP condition after I2C communication.
 */
#define STOP 1 ///< Stop condition flag

/**
 * @brief No Stop Condition Flag
 * 
 * Defines the flag value for not sending a STOP condition after I2C communication.
 */
#define NO_STOP 0 ///< No stop condition flag

/**
 * @brief Error Code for NACK
 * 
 * Defines the error code to indicate a NACK (No Acknowledge) response from the slave device.
 */
#define Error_NACK 1 ///< NACK error code

/**
 * @brief Error Code for Timeout
 * 
 * Defines the error code to indicate a timeout error during I2C communication.
 */
#define Error_Timout 3 ///< Timeout error code

/**
 * @brief Error Code for Bus Error
 * 
 * Defines the error code for a bus error during I2C communication.
 */
#define Error_Bus 2 ///< Bus error code

/**
 * @brief I2C Status Structure
 * 
 * Structure to hold the status of I2C operations, including the error code.
 */
typedef struct {
    volatile uint8_t error; ///< Error code for I2C operations
} I2C_Status;

/**
 * @brief Global I2C Status Instance
 * 
 * An instance of the I2C_Status structure to store the current I2C status.
 */
extern I2C_Status I2C; ///< Global I2C status instance

#endif /* I2C_H_ */
