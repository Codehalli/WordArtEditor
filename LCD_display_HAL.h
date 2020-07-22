/*
 * LCD_display_HAL.h
 *
 *  Created on: Jun 3, 2019
 *      Author: Prao
 */

#ifndef LCD_DISPLAY_HAL_H_
#define LCD_DISPLAY_HAL_H_

#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "edit.h"



// This function initializes the graphics part

void init_graphics(edit_t *edit);

// This functions draws the string at x and y location
// TODO: implement this function to the LCD_display_HAL.c if you like to use it.

void lcd_draw_string(edit_t * edit);

// This function clears the display.
// TODO: implement this function to the LCD_display_HAL.c if you like to use it.

void lcd_clear_display(edit_t * edit);

// Suggested TODO: add other functions here that help you work with the display easier.
// For example, you can write functions that change the background color, foreground color, etc.

#endif /* LCD_DISPLAY_HAL_H_ */
