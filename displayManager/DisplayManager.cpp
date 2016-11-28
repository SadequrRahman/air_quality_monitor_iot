/*
 * DisplayManager.cpp
 *
 *  Created on: Nov 27, 2016
 *      Author: APL-SadequrRahman
 */

#include "DisplayManager.h"


ICACHE_FLASH_ATTR DisplayManager::DisplayManager(Adafruit_ST7735 *tft) : screen(tft)
{
	// TODO Auto-generated constructor stub
	// Initialize TFT
	screen->begin();
	screen->fillScreen(VGA_WHITE);
	screen->setRotation(3);
	screen->setTextColor(VGA_BLACK,VGA_WHITE);
	//screen->drawCentreString("2.5µm",screen->width()/2,screen->height()/2,4);
	this->drawPlaceholder(5,0,100,40,VGA_BLACK,"Pm010 ug/m3");
	this->drawPlaceholder(5,42,100,40,VGA_BLACK,"Pm025 ug/m3");
	this->drawPlaceholder(5,84,100,40,VGA_BLACK,"Pm100 ug/m3");
	//screen->drawBitmap(0,0,bitmapI,screen->width(),screen->height());
}

ICACHE_FLASH_ATTR DisplayManager::~DisplayManager() {
	// TODO Auto-generated destructor stub
}

void ICACHE_FLASH_ATTR DisplayManager::update(PMS3003Data *obj)
{
	screen->drawNumber(obj->getPm010Tsi(),25,20,2);
	screen->drawNumber(obj->getPm025Tsi(),25,62,2);
	screen->drawNumber(obj->getPm100Tsi(),25,106,2);
}

int ICACHE_FLASH_ATTR DisplayManager::drawPlaceholder(int x, int y, int width, int height,
		int bordercolor, const char* headertext)
{
	int headersize = 18;
	screen->drawRoundRect(x, y, width, height, 3, bordercolor);
	screen->drawCentreString(headertext, x + width / 2, y + 1, 2);
	screen->drawFastHLine(x, y + headersize, width, bordercolor);

	return y + headersize;
}
