// =============================================================================================
// C includes and declarations
// =============================================================================================
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
#include "wifi.h"
#include "config.h"
#include "mem.h"

// declare library methods
extern int os_printf(const char *fmt, ...);
extern void uart_setHook(void(*funcPtr)(uint8_t *buf));
void ets_timer_disarm(ETSTimer *ptimer);
void ets_timer_setfn(ETSTimer *ptimer, ETSTimerFunc *pfunction, void *parg);
#ifdef __cplusplus
}
#endif

#include <time.h>
#include "cpp_routines/routines.h"
#include "../PMS3003/PMS3003_Manager.h"
#include "../mqtt/MqttManager.h"


LOCAL os_timer_t timerHandler;
LOCAL MQTT_Client myClient;

extern PMS3003_Manager pms3003Manager;
MqttManager myMqttManager(&myClient);

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
	os_printf("\r\nSystem started ...\r\n");

}


extern "C" void user_rf_pre_init(void)
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
	os_printf("\r\nGlobal constructors invoked\r\n");
	// Set up a timer to send the message to handler
	os_timer_disarm(&timerHandler);
	os_timer_setfn(&timerHandler, (os_timer_func_t *)sendMsgToHandler, (void *)0);
	os_timer_arm(&timerHandler,10000, 0);
}
