/*
 * PMS303.cpp
 *
 *  Created on: Nov 17, 2016
 *      Author: APL-SadequrRahman
 */

#include "c_types.h"
#include "PMS3003.h"

uint16_t ICACHE_FLASH_ATTR PMS3003::getPm010Atm() {
		return pm010_ATM;
	}

	void ICACHE_FLASH_ATTR PMS3003::setPm010Atm(uint16_t pm010Atm) {
		pm010_ATM = pm010Atm;
	}

	uint16_t ICACHE_FLASH_ATTR PMS3003::getPm010Tsi() {
		return pm010_TSI;
	}

	void ICACHE_FLASH_ATTR PMS3003::setPm010Tsi(uint16_t pm010Tsi) {
		pm010_TSI = pm010Tsi;
	}

	uint16_t ICACHE_FLASH_ATTR PMS3003::getPm025Atm() {
		return pm025_ATM;
	}

	void ICACHE_FLASH_ATTR PMS3003::setPm025Atm(uint16_t pm025Atm) {
		pm025_ATM = pm025Atm;
	}

	uint16_t ICACHE_FLASH_ATTR PMS3003::getPm025Tsi() {
		return pm025_TSI;
	}

	void ICACHE_FLASH_ATTR PMS3003::setPm025Tsi(uint16_t pm025Tsi) {
		pm025_TSI = pm025Tsi;
	}

	uint16_t ICACHE_FLASH_ATTR PMS3003::getPm100Atm() {
		return pm100_ATM;
	}

	void ICACHE_FLASH_ATTR PMS3003::setPm100Atm(uint16_t pm100Atm) {
		pm100_ATM = pm100Atm;
	}

	uint16_t ICACHE_FLASH_ATTR PMS3003::getPm100Tsi() {
		return pm100_TSI;
	}

	void ICACHE_FLASH_ATTR PMS3003::setPm100Tsi(uint16_t pm100Tsi) {
		pm100_TSI = pm100Tsi;
	}

	bool ICACHE_FLASH_ATTR PMS3003::isNewData(){
		bool st = isNewDataFlag;
		isNewDataFlag = false;
		return st;
	}
