/*
 * PmsSensorInterface.h
 *
 *  Created on: Nov 21, 2016
 *      Author: APL-SadequrRahman
 */

#ifndef PMS3003_PMSSENSORINTERFACE_H_
#define PMS3003_PMSSENSORINTERFACE_H_

#include "../PMS3003/PMS3003Data.h"

class PmsSensorInterface {
public:
	PmsSensorInterface();
	virtual ~PmsSensorInterface();
	virtual void update(PMS3003Data *dataObj) = 0;
};

#endif /* PMS3003_PMSSENSORINTERFACE_H_ */
