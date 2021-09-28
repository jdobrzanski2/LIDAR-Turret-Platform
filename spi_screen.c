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

#include <spi_screen.h>


// initialize screen
// jd: has lots of debug code commented out, removed some unneeded commands
void screen_begin(void)
{
    //GpioDataRegs.GPASET.bit.GPIO1 = 1; // disable screen

    _writeCommand(SLPOUT); //don't sleep
    _writeCommand(DISPON);  // turn display on
    _writeCommand(COLMOD);  // request bit per pixel change
    _writeData(0x05);  // 16 bit per pixel
}

// send a command to the screen
// jd: made CCS compatible
void _writeCommand(int c){
    GpioDataRegs.GPACLEAR.bit.GPIO2 = 1; // tell screen to accept a command
    GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;// (select screen)
    spi_send(c);delay_loop(75);
    //GpioDataRegs.GPASET.bit.GPIO7 = 1; // (de-select screen)
}

// send data to display on the screen
// jd: made CCS compatible
void _writeData(int c){
    GpioDataRegs.GPASET.bit.GPIO2 = 1; // tell screen to accept data
    GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;// (select screen)
    spi_send(c);delay_loop(75);
    //GpioDataRegs.GPASET.bit.GPIO7 = 1; // (de-select screen)
}

// jd: made CCS compatible
void _setAddressWindow(int x0, int y0, int x1, int y1){
    _writeCommand(CASET); //column addr set
    _writeData(0x00);
    _writeData(x0); //xstart
    _writeData(0x00);
    _writeData(x1); //xend

    _writeCommand(RASET); //row addr set
    _writeData(0x00);
    _writeData(y0); //ystart
    _writeData(0x00);
    _writeData(y1); //yend
}

// jd: made CCS compatible
void fillRect(int x, int y, int w, int h, int color){
    if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;
    if((x + w - 1) >= _width)  w = _width  - x;
    if((y + h - 1) >= _height) h = _height - y;

   _setAddressWindow(x, y, x+w-1, y+h-1);

    int hi = color >> 8;
    int lo = color & 0x0F;

    _writeCommand(RAMWR);

    GpioDataRegs.GPASET.bit.GPIO2 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;

    for(y=h;y>0;y--){
        for(x=w;x>0;x--){
            spi_send(hi);
            spi_send(lo);
        }
    }

    //GpioDataRegs.GPASET.bit.GPIO7 = 1;
}

// jd: made CCS compatible
void fillScreen(int color){
    fillRect(0, 0, _width, _height, color);
}

// jd: made CCS compatible
void drawPixel(int x, int y, int color){
    if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;

    _setAddressWindow(x,y,x+1,y+1);
    _writeCommand(RAMWR);

    int hi = color >> 8;
    int lo = color & 0x0F;

    GpioDataRegs.GPASET.bit.GPIO2 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;
    spi_send(hi);
    spi_send(lo);
    //GpioDataRegs.GPASET.bit.GPIO7 = 1;
}

// jd: rewrote this function to make "donut"
void drawCircle(int x, int y, int r_in, int r_out, int color_bg, int color_rim){
  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;
  if((x + r_out - 1) >= _width)  r_out = _width  - x;
  if((y + r_out - 1) >= _height) r_out = _height - y;

  _setAddressWindow(x-r_out, y-r_out, x+r_out, y+r_out);

  int r_out_square = r_out*r_out;
  int r_in_square = r_in*r_in;

  int hi_bg = color_bg >> 8;
  int lo_bg = color_bg & 0x0F;

  int hi_rim = color_rim >> 8;
  int lo_rim = color_rim & 0x0F;

  _writeCommand(RAMWR);

  GpioDataRegs.GPASET.bit.GPIO2 = 1;
  GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;

  int row = 0;
  int col = 0;
  int dist = 0;
  for(row = y-r_out; row <= y+r_out; row++){
    for(col= x-r_out; col <= x+r_out; col++){
      dist = ((row-x)*(row-x)+(col-y)*(col-y));
      if (dist > r_out_square) {
        // outside outer radius
        spi_send(hi_bg);
        spi_send(lo_bg);
      }
      else if (dist < r_in_square)
      {
        // inside inner radius
        spi_send(hi_bg);
        spi_send(lo_bg);
      }
      else
      {
       // boundary
       spi_send(hi_rim);
       spi_send(lo_rim);
       //test_var++;
      }
    }
  }
  //GpioDataRegs.GPASET.bit.GPIO7 = 1;
}

// jd: added this function since screen intended to be connected to SPIA only
uint8_t spi_send(const uint8_t data)
{
    // put information into SPI buffer
   while(SpiaRegs.SPISTS.bit.BUFFULL_FLAG == 1)
   {
   }
   SpiaRegs.SPITXBUF = data << 8;
   while(SpiaRegs.SPISTS.bit.INT_FLAG != 1)
   {
   }
  return(SpiaRegs.SPIRXBUF & 0xFF);

}

//
// delay_loop -
// jd: made so different delays values can be inputted
void
delay_loop(long ticks)
{
    long i;
    for (i = 0; i < ticks; i++)
    {
    }
}















