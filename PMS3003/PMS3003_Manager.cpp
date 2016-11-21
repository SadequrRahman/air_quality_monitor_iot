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

}

#define SET_PIN	4


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
	os_timer_disarm(&readBufTimer);
	os_timer_setfn(&readBufTimer,(os_timer_func_t *)readBufCallback,(void*)0);
	os_timer_arm(&readBufTimer,1000,1);

}


void ICACHE_FLASH_ATTR
PMS3003_Manager::parseAndUpdate(uint8_t *buf)
{

	setPm010Tsi(word(buf[0],buf[1]));
	setPm025Tsi(word(buf[2],buf[3]));
	setPm100Tsi(word(buf[4],buf[5]));
	setPm010Atm(word(buf[6],buf[7]));
	setPm025Atm(word(buf[8],buf[9]));
	setPm100Atm(word(buf[10],buf[11]));
	os_printf("update database\r\n");
}
