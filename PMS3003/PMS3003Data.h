/*
 * PMS3003.h
 *
 *  Created on: Nov 17, 2016
 *      Author: APL-SadequrRahman
 */

#ifndef PMS3003DATA_H_
#define PMS3003DATA_H_

extern "C"
{
#include "c_types.h"
}


class PMS3003Data{
public:
	uint16_t getPm010Atm(void);
	void setPm010Atm(uint16_t pm010Atm);
	uint16_t getPm010Tsi(void);
	void setPm010Tsi(uint16_t pm010Tsi);
	uint16_t getPm025Atm(void);
	void setPm025Atm(uint16_t pm025Atm);
	uint16_t getPm025Tsi(void);
	void setPm025Tsi(uint16_t pm025Tsi);
	uint16_t getPm100Atm(void);
	void setPm100Atm(uint16_t pm100Atm);
	uint16_t getPm100Tsi(void);
	void setPm100Tsi(uint16_t pm100Tsi);

private:
	uint16_t pm010_TSI;
	uint16_t pm010_ATM;
	uint16_t pm025_TSI;
	uint16_t pm025_ATM;
	uint16_t pm100_TSI;
	uint16_t pm100_ATM;
};

#endif /* INCLUDE_PMS3003DATA_H_ */
