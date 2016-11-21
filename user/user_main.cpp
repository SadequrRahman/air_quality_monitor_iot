//#define UIDEMO
#include "cube.h"
// =============================================================================================
// C includes and declarations
// =============================================================================================
#include "cpp_routines/routines.h"
#include "user_config.h"
#include <time.h>

#include "../PMS3003/PMS3003_Manager.h"
#include "Adafruit_ST7735.h"

extern "C"
{
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


MQTT_Client mqttClient;
// declare library methods
extern int os_printf(const char *fmt, ...);
extern void uart_setHook(void(*funcPtr)(uint8_t *buf));
extern bool MQTT_Subscribe(MQTT_Client *client, char* topic, uint8_t qos);
extern bool MQTT_Publish(MQTT_Client *client, const char* topic, const char* data, int data_length, int qos, int retain);
extern void MQTT_InitLWT(MQTT_Client *mqttClient, uint8_t* will_topic, uint8_t* will_msg, uint8_t will_qos, uint8_t will_retain);
void ets_timer_disarm(ETSTimer *ptimer);
void ets_timer_setfn(ETSTimer *ptimer, ETSTimerFunc *pfunction, void *parg);
#define os_malloc   pvPortMalloc
#define os_free     vPortFree
#define os_zalloc   pvPortZalloc
}
LOCAL os_timer_t timerHandler;

Adafruit_ST7735 tft;
extern PMS3003_Manager pms3003Manager;



#ifdef UIDEMO
extern void updateScreen(bool mode);
extern void setupUI();

float target_room_temperature = 23.5;
float RW_temperature = 65;
float target_RW_temperature = 70;
float room1_temperature = 23.4;
float room2_temperature = 23.3;
float outside_temperature = 2.4;
float min_target_temp = 18;
float max_target_temp = 26;
bool heater_enabled = false;
unsigned long room1_updated = -1;
unsigned long room2_updated = -1;
unsigned long outside_updated = -1;
unsigned long heater_state_changed_time = 0;
time_t total_on_time = 1;
time_t total_off_time = 1;
time_t last24h_on_time = 1;
time_t last24h_off_time = 1;

ICACHE_FLASH_ATTR static void updateScreen(void)
{
	REG_SET_BIT(0x3ff00014, BIT(0));
	os_update_cpu_frequency(160);

	target_room_temperature += 0.1;
	if (target_room_temperature > max_target_temp)
		target_room_temperature = min_target_temp;
	updateScreen((system_get_rtc_time() / 1000000) % 2);
}
#else
LOCAL double degree = -180.0;
LOCAL double scale = 1.5;
int16_t current[VERTEX_COUNT][3];
int16_t previous[VERTEX_COUNT][3];



extern "C" void ICACHE_FLASH_ATTR
wifiConnectCb(uint8_t status)
{
	if(status == STATION_GOT_IP){
		MQTT_Connect(&mqttClient);
	} else {
		MQTT_Disconnect(&mqttClient);
	}
}
extern "C" void ICACHE_FLASH_ATTR
mqttConnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	os_printf("MQTT: Connected\r\n");
	char ptr[] = "/mqtt/topic";
	MQTT_Subscribe(client,ptr, 0);


	MQTT_Publish(client, "/mqtt/topic/0", "hello0", 6, 0, 0);

}

extern "C" void ICACHE_FLASH_ATTR
mqttDisconnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
}

extern "C" void ICACHE_FLASH_ATTR
mqttPublishedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
}

extern "C" void ICACHE_FLASH_ATTR
mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{
	char *topicBuf = (char*)os_zalloc(topic_len+1),
		 *dataBuf = (char*)os_zalloc(data_len+1);

	MQTT_Client* client = (MQTT_Client*)args;

	os_memcpy(topicBuf, topic, topic_len);
	topicBuf[topic_len] = 0;

	os_memcpy(dataBuf, data, data_len);
	dataBuf[data_len] = 0;
	os_printf("Receive topic: %s, data: %s \r\n", topicBuf, dataBuf);
	pms3003Manager.turnOn();
	os_free(topicBuf);
	os_free(dataBuf);
}


ICACHE_FLASH_ATTR static void updateScreen(void)
{
	REG_SET_BIT(0x3ff00014, BIT(0));
	os_update_cpu_frequency(160);
	cube_calculate(current, degree, 0, 0, scale, 0, 0, 0);
	degree += 4;
	if (degree > 180.0) degree -= 360.0;
	cube_draw(previous, 0);
	cube_draw(current, 0XFFFF);
    memcpy(previous, current, sizeof (previous));
}
#endif

ICACHE_FLASH_ATTR void sendMsgToHandler(void *arg)
{
	//system_os_post(USER_TASK_PRIO_0, UPDATE_SCREEN, 'a');

	uart_setHook(static_cast<FuncPtr>(pms3003Manager.parseAndUpdate));
	os_delay_us(1000);

	MQTT_InitConnection(&mqttClient, sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.security);
	//MQTT_InitConnection(&mqttClient, "192.168.11.128", 1883, 0);

	MQTT_InitClient(&mqttClient, sysCfg.device_id, sysCfg.mqtt_user, sysCfg.mqtt_pass, sysCfg.mqtt_keepalive, 1);
	//MQTT_InitClient(&mqttClient, "client_id", "user", "pass", 120, 1);
	uint8_t test[] = "/test";
	uint8_t test1[] = "offline";
	MQTT_InitLWT(&mqttClient,test ,test1 , 0, 0);
	MQTT_OnConnected(&mqttClient, mqttConnectedCb);
	MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
	MQTT_OnPublished(&mqttClient, mqttPublishedCb);
	MQTT_OnData(&mqttClient, mqttDataCb);
	WIFI_Connect(sysCfg.sta_ssid, sysCfg.sta_pwd, wifiConnectCb);
	os_printf("\r\nSystem started ...\r\n");

}

extern "C" ICACHE_FLASH_ATTR void handler_task (os_event_t *e)
{
	switch (e->sig)
	{
		case UPDATE_SCREEN: updateScreen(); break;
		default: break;
	}
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
	os_event_t *handlerQueue;
	// Initialize TFT
	tft.begin();

	tft.fillScreen(0);
#ifdef UIDEMO
	setupUI();
	target_room_temperature = min_target_temp;
#endif

	// Set up a timer to send the message to handler
	os_timer_disarm(&timerHandler);
	os_timer_setfn(&timerHandler, (os_timer_func_t *)sendMsgToHandler, (void *)0);
	os_timer_arm(&timerHandler,10000, 0);

	// Set up a timerHandler to send the message to handler
	//handlerQueue = (os_event_t *)os_malloc(sizeof(os_event_t)*TEST_QUEUE_LEN);
	//system_os_task(handler_task, USER_TASK_PRIO_0, handlerQueue, TEST_QUEUE_LEN);


}
