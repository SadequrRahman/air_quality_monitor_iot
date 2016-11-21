/*
 * PMS3003_Manager.h
 *
 *  Created on: Nov 20, 2016
 *      Author: APL-SadequrRahman
 */

#ifndef INCLUDE_PMS3003_MANAGER_H_
#define INCLUDE_PMS3003_MANAGER_H_

#include "PMS3003Data.h"

extern "C" {
#include "c_types.h"
}

class PMS3003_Manager : public PMS3003Data {
public:
	PMS3003_Manager(void);
	void setFrequency(uint16_t _freq);
	void parseAndUpdate(uint8_t *buf);
private:
	ICACHE_FLASH_ATTR
	uint16_t word(uint8_t h, uint8_t l){return (((uint16_t)h<<8)| (l & 0xFF));}
};

#endif /* INCLUDE_PMS3003_MANAGER_H_ */
