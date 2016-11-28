// =============================================================================================
// C includes and declarations
// =============================================================================================
#include "../PMS3003/PMS3003_Manager.h"
#include "../mqtt/MqttManager.h"
#include "driver/Adafruit_ST7735.h"
#include "routines.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include "user_config.h"
#include "espmissingincludes.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_interface.h"
#include "ets_sys.h"
#include "driver/uart.h"
#include "../mqtt/mqtt_c/include/mqtt.h"
#include "../displayManager/DisplayManager.h"
#include "wifi.h"
#include "config.h"

// declare library methods
extern int os_printf(const char *fmt, ...);
extern void uart_setHook(void(*funcPtr)(uint8_t *buf));
void ets_timer_disarm(ETSTimer *ptimer);
void ets_timer_setfn(ETSTimer *ptimer, ETSTimerFunc *pfunction, void *parg);
#ifdef __cplusplus
}
#endif




LOCAL os_timer_t timerHandler;

PMS3003_Manager pms3003Manager;
MqttManager myMqttManager;
Adafruit_ST7735 tft;
DisplayManager displayManager(&tft);



extern "C" void ICACHE_FLASH_ATTR
wifiConnectCb(uint8_t status)
{
	if(status == STATION_GOT_IP){
		myMqttManager.connect();
	} else {
		myMqttManager.disconnect();
	}
}

ICACHE_FLASH_ATTR void sendMsgToHandler(void *arg)
{
	uart_setHook(static_cast<FuncPtr>(pms3003Manager.parseAndUpdate));
	os_delay_us(1000);

	WIFI_Connect(sysCfg.sta_ssid, sysCfg.sta_pwd, wifiConnectCb);
	pms3003Manager.registerLisenter(&myMqttManager);
	pms3003Manager.registerLisenter(&displayManager);
	os_printf("\r\nSystem started ...\r\n");

}


extern "C" void user_rf_pre_init(void)
{
}

extern "C" void user_rf_cal_sector_set(void)
{
}


extern "C" ICACHE_FLASH_ATTR void user_init(void)
{
	// Configure the UART
	uart_init(BIT_RATE_9600,BIT_RATE_9600);
	os_delay_us(10000);
	CFG_Load();
	os_printf("\r\nSystem init...\r\n");
	do_global_ctors();
	/*
	// Initialize TFT
	tft.begin();

	tft.fillScreen(0);

	setupUI();
	*/

	// Set up a timer to send the message to handler
	os_timer_disarm(&timerHandler);
	os_timer_setfn(&timerHandler, (os_timer_func_t *)sendMsgToHandler, (void *)0);
	os_timer_arm(&timerHandler,10000, 0);
}
