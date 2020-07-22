# Word Art Editor

## Disclaimer
This is not a complete discreption. For full description refer to the pdf posted on canvas.

## Description

This word art editor has two modes: an editing mode and a formatting mode.  In the editing mode, the user can modify a 10 character string, by sending those characters to the MSP432 over UART.  In the formatting mode, the user can modify the appearance of the string on the LCD display by send different sequences of characters to the MSP432 over UART.

## Detailed Guide

### Pushbuttons

The top pushbutton on the Boosterpack is used to switch back and forth between editing mode and formatting mode.

The bottom pushbutton on the Boosterpack is used to change the UART baud rate. This rate is initially set to 9600 and cycles through 19200, 38400, 57600 and back to 9600 with each button press. The current UART baud rate set on the MSP432 is shown on the Boosterpack tri-color LED with the corresponding colors listed below:

* 9600 - red
* 19200 - green
* 38400 - blue
* 57600 - white

### Formatting Features

Send these sequences of characters over UART to the MSP432 to change the appearance of the text on the LCD display.

#### Resetting the formatting to the default values

Type 'r' at the UART terminal to reset the format of the text to the initial (default) format.

#### Font Color

Type 'c' at the UART terminal to set the font color. Then type one of the following keys according to the desired font color:

* 'l' - black
* 'r' - red
* 'g' - green
* 'y' - yellow
* 'b' - blue
* 'm' - magenta
* 'c' - cyan
* 'w' - white

Examples:
* The sequence 'c', 'y' would change the text color to yellow.

#### Background Color

Type 'b' at the UART terminal to set the background color. Then type one of the following keys according to the desired background color:

* 'l' - black
* 'r' - red
* 'g' - green
* 'y' - yellow
* 'b' - blue
* 'm' - magenta
* 'c' - cyan
* 'w' - white

Examples:
* The sequence 'b', 'g' would change the background color to green.

#### Position

Type 'm' at the UART terminal to move the text. Next optionally type an integer number *x* reflecting the number of pixels the position should change. The default number of pixels moved is 1 if no number is provided. Then type one of the following keys according to the desired movement direction:

* 'u' - move text up by *x* pixels
* 'd' - move text down by *x* pixels
* 'l' - move text left by *x* pixels
* 'r' - move text right by *x* pixels

Examples:

* The sequence 'm','1','5','d' would move the text down 15 pixels on the display.
* The sequence 'm','l' would move the text left 1 pixel on the display.

#### Font Size

Type 's' at the UART terminal to change the font size. Then type one of the following keys according to the desired font size change:

* 'b' - increase the font size by 2 points (bigger)
* 's' - decrease the font size by 2 points (smaller)

Examples:
* The sequence 's', 'b' would increase the current font size by 2 points.

#### Font Type

Type 't' at the UART terminal to change the font type. Then type one of the following keys according to the desired font type:

* 'c' - computer modern font
* 's' - computer modern sans-serif font
* 't' - computer modern teletype font

Examples:

* The sequence 't', 's' would set the font type to computer modern sans-serif font.

## Course

ECE 2534

## Author

Sajan Ronvelwala
