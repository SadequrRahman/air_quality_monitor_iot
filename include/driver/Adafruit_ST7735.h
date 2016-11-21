#ifndef _ADAFRUIT_ST7735H_
#define _ADAFRUIT_ST7735H_

#include "Adafruit_GFX_AS.h"

extern "C"
{
#include <osapi.h>
#include <gpio.h>
#include "hspi.h"
}

//#define ST7735_TFTWIDTH  240
//#define ST7735_TFTHEIGHT 320

#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 160

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_RDMODE  0x0A
#define ST7735_RDMADCTL  0x0B
#define ST7735_RDPIXFMT  0x0C
#define ST7735_RDIMGFMT  0x0A
#define ST7735_RDSELFDIAG  0x0F

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_GAMMASET 0x26
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29

#define ST7735_CASET   0x2A
#define ST7735_PASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_MADCTL  0x36
#define ST7735_PIXFMT  0x3A

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DFUNCTR 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5
#define ST7735_VMCTR2  0xC7

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1
/*
#define ST7735_PWCTR6  0xFC

*/

// Color definitions
#define	ST7735_BLACK   0x0000
#define	ST7735_BLUE    0x001F
#define	ST7735_RED     0xF800
#define	ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF

#define TFT_DC_DATA		GPIO_OUTPUT_SET(5, 1)
#define TFT_DC_COMMAND	GPIO_OUTPUT_SET(5, 0)
#define TFT_DC_INIT 	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5); TFT_DC_DATA

#define TFT_RST_ACTIVE		GPIO_OUTPUT_SET(4, 0)
#define TFT_RST_DEACTIVE 	GPIO_OUTPUT_SET(4, 1)
#define TFT_RST_INIT		PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4); TFT_RST_DEACTIVE

#define MAKEWORD(b1, b2, b3, b4) (uint32_t(b1) | ((b2) << 8) | ((b3) << 16) | ((b4) << 24))

class Adafruit_ST7735 : public Adafruit_GFX_AS {

private:
 uint8_t  tabcolor;
 void transmitCmdData(uint8_t cmd, const uint8_t *data, uint8_t numDataByte);
 inline void transmitData(uint16_t data) {hspi_wait_ready(); hspi_send_uint16(data);}
 inline void transmitCmdData(uint8_t cmd, uint32_t data) {hspi_wait_ready(); TFT_DC_COMMAND; hspi_send_uint8(cmd); hspi_wait_ready(); TFT_DC_DATA; hspi_send_uint32(data);}
 inline void transmitData(uint16_t data, int32_t repeats){hspi_wait_ready(); hspi_send_uint16_r(data, repeats);}
 inline void transmitCmd(uint8_t cmd){hspi_wait_ready(); TFT_DC_COMMAND; hspi_send_uint8(cmd);hspi_wait_ready(); TFT_DC_DATA;}

public:
  Adafruit_ST7735();

  void     begin(void),
           fillScreen(uint16_t color),
           drawPixel(int16_t x, int16_t y, uint16_t color),
           drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
           drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
           fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
             uint16_t color),
           setRotation(uint8_t r),
           invertDisplay(bool i);
  inline void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
  {	  transmitCmdData(ST7735_CASET, MAKEWORD(x0 >> 8, x0 & 0xFF, x1 >> 8, x1 & 0xFF));
  	  transmitCmdData(ST7735_PASET, MAKEWORD(y0 >> 8, y0 & 0xFF, y1 >> 8, y1 & 0xFF));
	  transmitCmd(ST7735_RAMWR); // write to RAM
  }
  uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
};

#endif
