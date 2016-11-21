/*
 * PMS3003_Manager.h
 *
 *  Created on: Nov 20, 2016
 *      Author: APL-SadequrRahman
 */

#ifndef INCLUDE_PMS3003_MANAGER_H_
#define INCLUDE_PMS3003_MANAGER_H_

#include "PMS3003Data.h"
#include "../PMS3003/PmsSensorInterface.h"
#include <vector>
#include <list>

using namespace std;
typedef void (*FuncPtr)(uint8_t *val);



extern "C" {
#include "c_types.h"
}

class PMS3003_Manager {
public:
	PMS3003_Manager();
	~PMS3003_Manager();

	PMS3003Data pms3003Data;
	static void parseAndUpdate(uint8_t *buf);

	void setFrequency(uint16_t _freq);
	void registerLisenter(PmsSensorInterface *Obj);
private:
	void notify(PMS3003Data *dataObj);
	std::vector <PmsSensorInterface*>LisenterList;
};

#endif /* INCLUDE_PMS3003_MANAGER_H_ */
