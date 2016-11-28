#include <Adafruit_GFX_AS.h>
#include "driver/Adafruit_ST7735.h"

#include <time.h>
#include <c_types.h>

extern "C" {
#include "mini-printf.h"
}

#define TARGETTEMPSCREEN

#define VGA_BLACK		0x0000
#define VGA_WHITE		0xFFFF
#define VGA_RED			0xF800
#define VGA_GREEN		0x0400
#define VGA_BLUE		0x001F
#define VGA_SILVER		0xC618
#define VGA_GRAY		0x8410
#define VGA_MAROON		0x8000
#define VGA_YELLOW		0xFFE0
#define VGA_OLIVE		0x8400
#define VGA_LIME		0x07E0
#define VGA_AQUA		0x07FF
#define VGA_TEAL		0x0410
#define VGA_NAVY		0x0010
#define VGA_FUCHSIA		0xF81F
#define VGA_PURPLE		0x8010



extern Adafruit_ST7735 tft;


#define HEADERTEXT 2
#define LINETEXT 2

int color(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r&248)|g>>5) << 8 | ((g&28)<<3|b>>3);
}

ICACHE_FLASH_ATTR int drawPlaceholder(int x, int y, int width, int height, int bordercolor, const char* headertext)
{
	int headersize = 18;
	tft.drawRoundRect(x, y, width, height, 3, bordercolor);
	tft.drawCentreString(headertext, x + width / 2, y + 1, HEADERTEXT);
	tft.drawFastHLine(x, y + headersize, width, bordercolor);
	
	return y + headersize;
}

ICACHE_FLASH_ATTR void setupUI()
{
	tft.setRotation(3);
	tft.setTextColor(VGA_GREEN, VGA_BLACK);
	/*
	tft.fillScreen(ST7735_BLACK);


	//tft.drawRect(0,0,160,32,ST7735_GREEN);
	//tft.drawRect(5,2,50,28,ST7735_RED);
	tft.drawCentreString("BAT", 5+25, 2+1,2);
	//tft.drawRect(57,2,50,28,ST7735_RED);
	tft.drawCentreString("WiFi", 50+28, 2+1,2);
	//tft.drawRect(109,2,50,28,ST7735_RED);
	tft.drawCentreString("unit", 109+28, 2+1,2);

	tft.fillRect(30,45,110,60,VGA_BLUE);
	tft.fillRect(35,50,100,50,VGA_WHITE);
	tft.setTextColor(VGA_BLACK,VGA_WHITE);
	tft.drawCentreString("AQI",35+50,50+1,10);

	*/
	drawPlaceholder(10,10,120,100,VGA_RED,"AQI");

}
