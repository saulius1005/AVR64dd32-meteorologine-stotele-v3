/**
 * @file i2c.c
 * @brief I2C Communication Functions
 * 
 * This file contains functions for initializing and communicating over the I2C bus. 
 * It supports both reading and writing single and multi-byte data to/from I2C devices.
 * 
 * Created: 2024-12-04 18:18:11
 * Author: Saulius
 */

#include "Settings.h"
#include "i2cVar.h"

/**
 * @brief Initializes the I2C bus (TWI0).
 * 
 * This function configures the TWI0 interface for communication by setting up 
 * the SDA hold time, SDA setup time, and enabling the I2C master mode.
 * It also sets the baud rate for communication.
 */
void I2C_init(void) {
    // Configure SDA hold, SDA setup, and enable fast mode plus (FMP)
    TWI0.CTRLA = TWI_SDAHOLD_OFF_gc | TWI_SDASETUP_4CYC_gc | TWI_FMPEN_ON_gc;

    // Set baud rate for the I2C bus
    TWI0.MBAUD = (uint8_t)TWI_BAUD;

    // Enable TWI (I2C) module
    TWI0.MCTRLA = TWI_ENABLE_bm;

    // Set bus state to idle
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

/**
 * @brief Transmits the I2C address with read/write flag.
 * 
 * @param addr I2C address of the device.
 * @param read Read flag (0 for write, 1 for read).
 * @return uint8_t Error code (0 for success, Error_Timout, Error_NACK, or Error_Bus).
 * 
 * This function transmits the 7-bit address of the slave device along with the 
 * read/write flag. It checks for errors such as timeout, NACK, or bus errors.
 */
uint8_t TransmitAdd(uint8_t addr, uint8_t read) {
    uint8_t error = 0;

    // Set the address and read/write flag
    TWI0.MADDR = (addr << 1) | read;
    uint32_t timeout_counter = TIMEOUT_COUNTER;

    // Wait for the write interrupt flag or read interrupt flag
    while (!(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm))) {
        if (--timeout_counter == 0) { // Timeout condition
            error = Error_Timout;
            TWI0.MCTRLB = TWI_MCMD_STOP_gc; // Send STOP signal
            break;
        }
    }

    // Check for other errors (NACK or bus error)
    if (TWI0.MSTATUS & TWI_RXACK_bm) error = Error_NACK;
    if (TWI0.MSTATUS & (TWI_ARBLOST_bm | TWI_BUSERR_bm)) error = Error_Bus;

    if (error != 0) TWI0.MCTRLB = TWI_MCMD_STOP_gc; // Send STOP signal on error

    I2C.error = error;
    return error;
}

/**
 * @brief Transmits a single byte over I2C.
 * 
 * @param data The data byte to be transmitted.
 * @return uint8_t Error code (0 for success, Error_NACK, or Error_Bus).
 * 
 * This function transmits a single byte of data and waits for the I2C bus to complete the transmission.
 * It checks for errors such as NACK or bus errors during transmission.
 */
uint8_t TransmitByte(uint8_t data) {
    uint8_t error = 0;

    // Check for NACK or bus error before transmitting
    if (TWI0.MSTATUS & TWI_RXACK_bm) error = Error_NACK;
    if (TWI0.MSTATUS & (TWI_ARBLOST_bm | TWI_BUSERR_bm)) error = Error_Bus;

    if (error) {
        TWI0.MCTRLB = TWI_MCMD_STOP_gc;
    }

    // Transmit data if no errors
    if (error == 0) {
        TWI0.MDATA = data;
        uint32_t timeout_counter = TIMEOUT_COUNTER;

        // Wait for the write interrupt flag to signal transmission completion
        while (!(TWI0.MSTATUS & TWI_WIF_bm)) {
            if (--timeout_counter == 0) { // Timeout condition
                error = Error_Timout;
                TWI0.MCTRLB = TWI_MCMD_STOP_gc;
                break;
            }
        }
    }

    I2C.error = error;
    return error;
}

/**
 * @brief Reads a byte of data from the I2C bus.
 * 
 * @param ack ACK flag (1 for ACK, 0 for NACK).
 * @param data Pointer to store the received byte of data.
 * 
 * This function reads a byte of data from the I2C bus and stores it in the provided data pointer.
 * It sets the ACK or NACK based on the provided flag and handles timeout errors.
 */
void ReadByteInf(uint8_t ack, uint8_t* data) {
    uint32_t timeout_counter = TIMEOUT_COUNTER;

    // Wait for the read interrupt flag or clock hold flag
    while (!(TWI0.MSTATUS & (TWI_CLKHOLD_bm | TWI_RIF_bm))) {
        if (--timeout_counter == 0) { // Timeout condition
            I2C.error = Error_Timout;
            break;
        }
    }

    // If no error, read the data byte
    if (!I2C.error) {
        TWI0.MCTRLB = ack ? TWI_MCMD_RECVTRANS_gc : TWI_ACKACT_NACK_gc;
        *data = TWI0.MDATA;
    }
}

/**
 * @brief Reads a single byte from a register of an I2C device.
 * 
 * @param addr I2C address of the device.
 * @param reg Register address to read from.
 * @return uint8_t The data read from the register.
 * 
 * This function writes the register address to the I2C device and reads the data from it.
 */
uint8_t ReadReg(uint8_t addr, uint8_t reg) {
    uint8_t data = 0;

    if (!TransmitAdd(addr, WRITE)) { // Transmit address for write
        if (!TransmitByte(reg)) { // Write register address
            if (!TransmitAdd(addr, READ)) { // Transmit address for read
                ReadByteInf(0, &data); // Read the data with NACK
            }
        }
    }

    TWI0.MCTRLB |= TWI_MCMD_STOP_gc; // Send STOP signal to release the bus
    return data;
}

/**
 * @brief Writes a byte to a register of an I2C device.
 * 
 * @param addr I2C address of the device.
 * @param reg Register address to write to.
 * @param data Data to write to the register.
 * 
 * This function writes the data to a specific register of an I2C device.
 */
void WriteToReg(uint8_t addr, uint8_t reg, uint8_t data) {
    if (!TransmitAdd(addr, WRITE)) { // Transmit address for write
        if (!TransmitByte(reg)) { // Write register address
            TransmitByte(data); // Write the data with STOP
        }
    }

    TWI0.MCTRLB |= TWI_MCMD_STOP_gc; // Send STOP signal
}

/**
 * @brief Reads multiple bytes from an I2C device.
 * 
 * @param addr I2C address of the device.
 * @param reg Register address to start reading from.
 * @param bytes Number of bytes to read (up to 8).
 * @return uint64_t The data read from the device.
 * 
 * This function reads multiple bytes of data from an I2C device and combines them into a 64-bit value.
 * It handles multiple bytes with proper ACK/NACK handling.
 */
uint64_t ReadMulti(uint8_t addr, uint8_t reg, uint8_t bytes) {
    uint64_t data = 0;
    if (bytes == 0 || bytes > 8) return 0; // Validate byte count

    if (!TransmitAdd(addr, WRITE)) { // Transmit address for write
        if (!TransmitByte(reg)) { // Write register address
            if (!TransmitAdd(addr, READ)) { // Transmit address for read
                for (int i = 0; i < bytes; i++) {
                    uint8_t byte = 0;
                    ReadByteInf(i < bytes - 1 ? 1 : 0, &byte); // Use ACK for all but the last byte
                    if (I2C.error)
                        break;
                    else
                        data |= ((uint64_t)byte << (8 * (bytes - 1 - i))); // Combine bytes into 64-bit value
                }
            }
        }
    }

    TWI0.MCTRLB |= TWI_MCMD_STOP_gc; // Send STOP signal
    return data;
}

/**
 * @brief Writes multiple bytes to an I2C device.
 * 
 * @param addr I2C address of the device.
 * @param reg Register address to write to.
 * @param data Data to write (64-bit value).
 * @param bytes Number of bytes to write (up to 8).
 * 
 * This function writes multiple bytes of data to an I2C device from a 64-bit value.
 */
void WriteMulti(uint8_t addr, uint8_t reg, uint64_t data, uint8_t bytes) {
    if (!TransmitAdd(addr, WRITE)) { // Transmit address for write
        if (!TransmitByte(reg)) { // Write register address
            for (uint8_t i = 0; i < bytes; i++) {
                uint8_t byteToSend = (data >> (8 * (bytes - 1 - i))) & 0xFF;
                if (!TransmitByte(byteToSend)) // Transmit each byte
                    continue;
                else
                    break; // If an error occurs, stop the transmission
            }
        }
    }

    TWI0.MCTRLB |= TWI_MCMD_STOP_gc; // Send STOP signal
}

/**
 * @brief Quickly writes a block of data to the I2C bus.
 * 
 * @param data Pointer to the data to be transmitted.
 * @param size Number of bytes to transmit.
 * 
 * This function sends a block of data to the I2C bus without addressing or registering.
 * It writes each byte individually until the block is complete.
 */
void FastWriteBlock(uint8_t *data, uint8_t size) {
    while (size--) {
        TWI0.MDATA = *data++;
        while (!(TWI0.MSTATUS & TWI_WIF_bm));
    }
    TWI0.MCTRLB = TWI_MCMD_STOP_gc; // Send STOP signal
}
