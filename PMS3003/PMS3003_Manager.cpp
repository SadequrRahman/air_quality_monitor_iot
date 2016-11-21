/*
 * PMS3003_Manager.cpp
 *
 *  Created on: Nov 20, 2016
 *      Author: APL-SadequrRahman
 */


#include "PMS3003_Manager.h"

extern "C"{
#include "osapi.h"
#include "os_type.h"
#include "driver/uart.h"
#include "espmissingincludes.h"
#include "gpio.h"
#include "user_interface.h"
#define  word(h,l) (((uint16_t)h<<8)| (l & 0xFF))
}

#define SET_PIN	4


PMS3003_Manager pms3003Manager;


LOCAL os_timer_t readBufTimer;
LOCAL uint8_t state;

LOCAL void readBufCallback(void *arg)
{
	state ^=1;
	GPIO_OUTPUT_SET(SET_PIN, state);

}

ICACHE_FLASH_ATTR
PMS3003_Manager::PMS3003_Manager(){

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
	state = 0;
	GPIO_OUTPUT_SET(SET_PIN, state);
	UART_SetBaudrate(UART0,BIT_RATE_9600);
	os_printf("Creating pms3003 manager\r\n");
	os_timer_disarm(&readBufTimer);
	os_timer_setfn(&readBufTimer,(os_timer_func_t *)readBufCallback,(void*)0);
	os_timer_arm(&readBufTimer,5000,1);

}

ICACHE_FLASH_ATTR PMS3003_Manager::~PMS3003_Manager()
{
}


void ICACHE_FLASH_ATTR
PMS3003_Manager::parseAndUpdate(uint8_t *buf)
{

	pms3003Manager.pms3003Data.setPm010Tsi(word(buf[0],buf[1]));
	pms3003Manager.pms3003Data.setPm025Tsi(word(buf[2],buf[3]));
	pms3003Manager.pms3003Data.setPm100Tsi(word(buf[4],buf[5]));
	pms3003Manager.pms3003Data.setPm010Atm(word(buf[6],buf[7]));
	pms3003Manager.pms3003Data.setPm025Atm(word(buf[8],buf[9]));
	pms3003Manager.pms3003Data.setPm100Atm(word(buf[10],buf[11]));
	pms3003Manager.notify(&pms3003Manager.pms3003Data);
}


void ICACHE_FLASH_ATTR PMS3003_Manager::registerLisenter(PmsSensorInterface *obj)
{
	os_printf("Adding to Event List\r\n");
	LisenterList.push_back(obj);
}

void ICACHE_FLASH_ATTR PMS3003_Manager::notify(PMS3003Data *dataObj)
{
	for (int i = 0; i < LisenterList.size(); i++)
		{
			os_printf("Sending Notification\r\n");
			LisenterList[i]->update(dataObj);
		}
}
