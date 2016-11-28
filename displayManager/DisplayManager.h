/*
 * DisplayManager.h
 *
 *  Created on: Nov 27, 2016
 *      Author: APL-SadequrRahman
 */

#ifndef DISPLAYMANAGER_DISPLAYMANAGER_H_
#define DISPLAYMANAGER_DISPLAYMANAGER_H_

#include "../PMS3003/PmsSensorInterface.h"
#include "../PMS3003/PMS3003Data.h"
#include "../include/driver/Adafruit_ST7735.h"

extern "C"{
#include "c_types.h"
}

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


class DisplayManager : public PmsSensorInterface{
public:
	DisplayManager(Adafruit_ST7735 *tft);
	virtual ~DisplayManager();
	int drawPlaceholder(int x, int y, int width, int height, int bordercolor, const char* headertext);
	void update(PMS3003Data *obj);
private:
	Adafruit_ST7735 *screen;
};

#endif /* DISPLAYMANAGER_DISPLAYMANAGER_H_ */
