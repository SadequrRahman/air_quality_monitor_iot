#include "driver/Adafruit_ST7735.h"
#include "Adafruit_GFX_AS.h"

extern "C"{
#include <ets_sys.h>
#include <os_type.h>
#include <osapi.h>
#include "hspi.h"
#include "espmissingincludes.h"
}

#define SWAPBYTES(i) ((i>>8) | (i<<8))
ICACHE_FLASH_ATTR Adafruit_ST7735::Adafruit_ST7735() : Adafruit_GFX_AS(ST7735_TFTWIDTH, ST7735_TFTHEIGHT) {
	tabcolor = 0;
}

void ICACHE_FLASH_ATTR Adafruit_ST7735::transmitCmdData(uint8_t cmd, const uint8_t *data, uint8_t numDataByte)
{
	hspi_wait_ready();
	TFT_DC_COMMAND;
	hspi_send_uint8(cmd);
	hspi_wait_ready();
	TFT_DC_DATA;
	hspi_send_data(data, numDataByte);
}

ICACHE_FLASH_ATTR void Adafruit_ST7735::begin(void) {
	//Set communication using HW SPI Port
	hspi_init();
	TFT_DC_INIT;
	/* TFT RESET is connected directly VCC*/
	//TFT_RST_INIT;

	TFT_RST_ACTIVE;
	os_delay_us(10000);
	TFT_RST_DEACTIVE;
	os_delay_us(1000);

	uint8_t data[15] = {0};

	data[0] = 0x39;
	data[1] = 0x2C;
	data[2] = 0x00;
	data[3] = 0x34;
	data[4] = 0x02;
	transmitCmdData(0xCB, data, 5);

	data[0] = 0x00;
	data[1] = 0XC1;
	data[2] = 0X30;
	transmitCmdData(0xCF, data, 3);

	data[0] = 0x85;
	data[1] = 0x00;
	data[2] = 0x78;
	transmitCmdData(0xE8, data, 3);

	data[0] = 0x00;
	data[1] = 0x00;
	transmitCmdData(0xEA, data, 2);

	data[0] = 0x64;
	data[1] = 0x03;
	data[2] = 0X12;
	data[3] = 0X81;
	transmitCmdData(0xED, data, 4);

	data[0] = 0x20;
	transmitCmdData(0xF7, data, 1);

	data[0] = 0x23;   	//VRH[5:0]
	transmitCmdData(0xC0, data, 1);    	//Power control

	data[0] = 0x10;   	//SAP[2:0];BT[3:0]
	transmitCmdData(0xC1, data, 1);    	//Power control

	data[0] = 0x3e;   	//Contrast
	data[1] = 0x28;
	transmitCmdData(0xC5, data, 2);    	//VCM control

	data[0] = 0x86;  	 //--
	transmitCmdData(0xC7, data, 1);    	//VCM control2

	data[0] = 0x08;//0x48;  	//C8
	transmitCmdData(0x36, data, 1);    	// Memory Access Control

	data[0] = 0x55;
	transmitCmdData(0x3A, data, 1);

	data[0] = 0x00;
	data[1] = 0x18;
	transmitCmdData(0xB1, data, 2);

	data[0] = 0x08;
	data[1] = 0x82;
	data[2] = 0x27;
	transmitCmdData(0xB6, data, 3);    	// Display Function Control

	data[0] = 0x00;
	transmitCmdData(0xB4, data, 1);    	//Display inversion control

	data[0] = 0x00;
	transmitCmdData(0xF2, data, 1);    	// 3Gamma Function Disable

	data[0] = 0x01;
	transmitCmdData(0x26, data, 1);    	//Gamma curve selected

	data[0] = 0x0F;
	data[1] = 0x31;
	data[2] = 0x2B;
	data[3] = 0x0C;
	data[4] = 0x0E;
	data[5] = 0x08;
	data[6] = 0x4E;
	data[7] = 0xF1;
	data[8] = 0x37;
	data[9] = 0x07;
	data[10] = 0x10;
	data[11] = 0x03;
	data[12] = 0x0E;
	data[13] = 0x09;
	data[14] = 0x00;
	transmitCmdData(0xE0, data, 15);    	//Set Gamma

	data[0] = 0x00;
	data[1] = 0x0E;
	data[2] = 0x14;
	data[3] = 0x03;
	data[4] = 0x11;
	data[5] = 0x07;
	data[6] = 0x31;
	data[7] = 0xC1;
	data[8] = 0x48;
	data[9] = 0x08;
	data[10] = 0x0F;
	data[11] = 0x0C;
	data[12] = 0x31;
	data[13] = 0x36;
	data[14] = 0x0F;
	transmitCmdData(0xE1, data, 15);    	//Set Gamma

	transmitCmd(0x11);    	//Exit Sleep
	os_delay_us(120000);

	transmitCmd(0x29);    //Display on
	transmitCmd(0x2c);
}

void ICACHE_FLASH_ATTR Adafruit_ST7735::drawPixel(int16_t x, int16_t y, uint16_t color) {

	if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;
	setAddrWindow(x,y,x+1,y+1);
	transmitData(SWAPBYTES(color));
}


void ICACHE_FLASH_ATTR Adafruit_ST7735::drawFastVLine(int16_t x, int16_t y, int16_t h,
		uint16_t color) {

	// Rudimentary clipping
	if((x >= _width) || (y >= _height)) return;

	if((y+h-1) >= _height)
		h = _height-y;

	setAddrWindow(x, y, x, y+h-1);
	transmitData(SWAPBYTES(color), h);
}

void ICACHE_FLASH_ATTR Adafruit_ST7735::drawFastHLine(int16_t x, int16_t y, int16_t w,
		uint16_t color) {

	// Rudimentary clipping
	if((x >= _width) || (y >= _height)) return;
	if((x+w-1) >= _width)  w = _width-x;
	setAddrWindow(x, y, x+w-1, y);
	transmitData(SWAPBYTES(color), w);
}

ICACHE_FLASH_ATTR void Adafruit_ST7735::fillScreen(uint16_t color) {
	fillRect(0, 0,  _width, _height, color);
}

// fill a rectangle
void ICACHE_FLASH_ATTR Adafruit_ST7735::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
		uint16_t color) {

	// rudimentary clipping (drawChar w/big text requires this)
	if((x >= _width) || (y >= _height)) return;
	if((x + w - 1) >= _width)  w = _width  - x;
	if((y + h - 1) >= _height) h = _height - y;

	setAddrWindow(x, y, x+w-1, y+h-1);
	transmitData(SWAPBYTES(color), h*w);
}


// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t ICACHE_FLASH_ATTR Adafruit_ST7735::color565(uint8_t r, uint8_t g, uint8_t b) {
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}


#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

void ICACHE_FLASH_ATTR Adafruit_ST7735::setRotation(uint8_t m) {

	uint8_t data;
	rotation = m % 4; // can't be higher than 3
	switch (rotation) {
	case 0:
		data = MADCTL_MX | MADCTL_BGR;
		_width  = ST7735_TFTWIDTH;
		_height = ST7735_TFTHEIGHT;
		break;
	case 1:
		data = MADCTL_MV | MADCTL_BGR;
		_width  = ST7735_TFTHEIGHT;
		_height = ST7735_TFTWIDTH;
		break;
	case 2:
		data = MADCTL_MY | MADCTL_BGR;
		_width  = ST7735_TFTWIDTH;
		_height = ST7735_TFTHEIGHT;
		break;
	case 3:
		data = MADCTL_MY | MADCTL_MV | MADCTL_BGR; //MADCTL_MX |
		_width  = ST7735_TFTHEIGHT;
		_height = ST7735_TFTWIDTH;
		break;
	}
	transmitCmdData(ST7735_MADCTL, &data, 1);
}


void ICACHE_FLASH_ATTR Adafruit_ST7735::invertDisplay(bool i) {
	transmitCmd(i ? ST7735_INVON : ST7735_INVOFF);
}
