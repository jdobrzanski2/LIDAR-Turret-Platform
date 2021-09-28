/*
ST7735R.h - Library for ST7735R 1.8'' TFT display
Created by Matevž Marš, november 2016
Released into the public domain
https://github.com/matevzmars/ST7735R
*/

// Modified by Joseph Dobrzanski to work with this project (November 14, 2019)
//      Search 'jd' for changes
//       - Removed unneeded functions and constants
//       - Modified all commands to be useable in CSS
//       - Rewrote drawCircle entirely to create "donut"
//       - Added SPIA function to allow for communication to screen using SPIA interface
#ifndef SPI_SCREEN_H
#define SPI_SCREEN_H

#include "Peripheral_Headers/F2802x_Device.h"

// constants needed to control screen
// jd: removed unneeded constants
#define MADCTL      0x36
#define SLPOUT      0x11
#define FLIP_0      0x00
#define DISPON      0x29
#define COLMOD      0x3a
#define CASET       0x2a
#define RASET       0x2b
#define RAMWR       0x2c
#define _width      130
#define _height     130

#define STD_DEL     1000000

// jd: removed unneeded functions
void delay_loop(long ticks);
void screen_begin(void);
void fillScreen(int color);
void drawPixel(int x, int y, int color);
void fillRect(int x, int y, int w, int h, int color);
void drawCircle(int x, int y, int r_in, int r_out, int color_bg, int color_rim); // jd: rewrote this function
void _writeCommand(int c);
void _writeData(int c);
void _setAddressWindow(int x0, int y0, int x1, int y1);
int _writeCharacter(char c, int x, int y, int b, int col, int size);
uint8_t spi_send(const uint8_t data); // jd: added this function

#endif
