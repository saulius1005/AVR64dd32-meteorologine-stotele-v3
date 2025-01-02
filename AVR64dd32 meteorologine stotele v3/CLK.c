/**
 * @file CLK.c
 * @brief Clock configuration functions for the system.
 *
 * This file contains functions to initialize and configure the external high-frequency
 * crystal oscillator and external clock input for the system.
 * 
 * @author Saulius
 * @date 2024-12-05
 */

#include "Settings.h"

/**
 * @brief Initializes the external high-frequency crystal oscillator (XOSCHF).
 * 
 * This function configures the crystal oscillator with a frequency range of 16 MHz
 * and a start-up time of 4K cycles. It also sets the main clock to use XOSCHF as the
 * source and clears the main clock prescaler.
 */
void CLOCK_XOSCHF_crystal_init()
{
    /* Enable crystal oscillator with frequency range 16 MHz and 4K cycles start-up time */
    ccp_write_io((uint8_t *) &CLKCTRL.XOSCHFCTRLA, CLKCTRL_RUNSTDBY_bm
        | CLKCTRL_CSUTHF_4K_gc
        | CLKCTRL_FRQRANGE_24M_gc
        | CLKCTRL_SELHF_XTAL_gc
        | CLKCTRL_ENABLE_bm);

    /* Confirm crystal oscillator start-up */
    while (!(CLKCTRL.MCLKSTATUS & CLKCTRL_EXTS_bm))
    {
        /* Wait for the external clock to stabilize */
    }

    /* Clear Main Clock Prescaler */
    ccp_write_io((uint8_t *) &CLKCTRL.MCLKCTRLB, 0x00);

    /* Set the main clock to use XOSCHF as source */
    ccp_write_io((uint8_t *) &CLKCTRL.MCLKCTRLA, CLKCTRL_CLKSEL_EXTCLK_gc /*| CLKCTRL_CLKOUT_bm*/);

    /* Wait for system oscillator change to complete */
    while (CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm)
    {
        /* Wait for the system clock to stabilize */
    }

    /* Disable RUNSTDBY for power saving during sleep */
    ccp_write_io((uint8_t *) &CLKCTRL.XOSCHFCTRLA, CLKCTRL.XOSCHFCTRLA & ~CLKCTRL_RUNSTDBY_bm);

    /* Configuration complete; main clock is now running at 16 MHz */
}

/**
 * @brief Configures the system to use an external clock input (24 MHz).
 * 
 * This function sets the external clock input as the source for the main clock
 * and optionally adjusts the prescaler. The main clock is then configured to
 * run at the desired frequency.
 */
void CLOCK_XOSCHF_clock_init()
{
    /* Enable external (24 MHz) clock input */
    ccp_write_io((uint8_t *) &CLKCTRL.XOSCHFCTRLA, CLKCTRL_SELHF_EXTCLOCK_gc | CLKCTRL_ENABLE_bm);

    /* Set Main Clock Prescaler (optional) */
    // ccp_write_io((uint8_t *) &CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_2X_gc | CLKCTRL_PEN_bm);

    /* Set the main clock to use external clock as source */
    ccp_write_io((uint8_t *) &CLKCTRL.MCLKCTRLA, CLKCTRL_CLKSEL_EXTCLK_gc /*| CLKCTRL_CLKOUT_bm*/);

    /* Wait for system oscillator change to complete */
    while (CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm)
    {
        /* Wait for the system clock to stabilize */
    }

    /* Configuration complete; main clock is now running at 32 MHz / 2 = 16 MHz */
}
