/*
 * St7567S.h
 *
 * Created: 2024-12-10 23:11:16
 * Author: Saulius
 *
 * @brief This header file defines constants, macros, data structures, and function
 *        prototypes for interfacing with the ST7567S LCD display. It includes
 *        configuration parameters such as screen dimensions, contrast settings,
 *        and alignment options.
 */

#ifndef ST7567S_H_
#define ST7567S_H_

/** @brief The I2C address of the ST7567S display. */
#define ST7567S_ADD 0x3f

/** @brief The number of commands in the display's initialization sequence. */
#define ST7567S_CMD_COUNT 11

/** @brief The width of the ST7567S display in pixels. */
#define ST7567S_SCREEN_WIDTH 128

/** @brief The height of the ST7567S display in pixels (64 pixels = 8 pages). */
#define ST7567S_SCREEN_HEIGHT 64

/** @brief The number of pages in the ST7567S display (height / 8). */
#define ST7567S_PAGE_COUNT (ST7567S_SCREEN_HEIGHT / 8)

/** @brief Default contrast level for the ST7567S display (0x00-0x3F). */
#define ST7567S_CONTRAST 0x20  ///< Contrast level (0x00 to 0x3F)

/** @brief Maximum text length that can be displayed on the screen. */
#define MAX_TEXT_LENGTH 50

/** 
 * @brief Enumeration for text alignment options. 
 * Defines how text should be aligned on the screen: left, center, or right.
 */
typedef enum {
    ALIGN_LEFT,   /**< Left alignment. */
    ALIGN_CENTER, /**< Center alignment. */
    ALIGN_RIGHT  /**< Right alignment. */
} alignment_t;

#endif /* ST7567S_H_ */
