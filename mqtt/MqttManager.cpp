/*
 * MqttManager.cpp
 *
 *  Created on: Nov 21, 2016
 *      Author: APL-SadequrRahman
 */

#include "MqttManager.h"


#ifdef __cplusplus
extern "C"
{
#endif
#include "config.h"
#include "osapi.h"
#include "mem.h"
#include "espmissingincludes.h"
#include "routines.h"
#ifdef __cplusplus
}
#endif

static MQTT_Client myclient;

LOCAL char pub_topic0[] = "pms3003/tsi/010";
LOCAL char pub_topic1[] = "pms3003/tsi/025";
LOCAL char pub_topic2[] = "pms3003/tsi/100";
LOCAL char pub_topic3[] = "pms3003/Atm/010";
LOCAL char pub_topic4[] = "pms3003/Atm/025";
LOCAL char pub_topic5[] = "pms3003/Atm/100";


ICACHE_FLASH_ATTR char *ultostr(unsigned long value, char *ptr, int base)
{
  unsigned long t = 0, res = 0;
  unsigned long tmp = value;
  int count = 0;

  if (NULL == ptr)
  {
    return NULL;
  }

  if (tmp == 0)
  {
    count++;
  }

  while(tmp > 0)
  {
    tmp = tmp/base;
    count++;
  }

  ptr += count;

  *ptr = '\0';

  do
  {
    res = value - base * (t = value / base);
    if (res < 10)
    {
      * -- ptr = '0' + res;
    }
    else if ((res >= 10) && (res < 16))
    {
        * --ptr = 'A' - 10 + res;
    }
  } while ((value = t) != 0);

  return(ptr);
}


ICACHE_FLASH_ATTR MqttManager::MqttManager()
{
	os_printf("Creating mqtt object\r\n");
	MQTT_InitConnection(&myclient, (uint8_t*)"192.168.11.125", 1883 , 0);
	//MQTT_InitConnection(&mqttClient, "192.168.11.128", 1883, 0);

	MQTT_InitClient(&myclient, (uint8_t*) "lens_tX0vTNPfuNB8q2ztTywn1yRLGtO",(uint8_t*)"rabby25",
			(uint8_t*)"rabby25227", 120, 1);
	//MQTT_InitClient(&mqttClient, "client_id", "user", "pass", 120, 1);
	MQTT_InitLWT(&myclient, (uint8_t*)"/test" ,(uint8_t*)"offline" , 0, 0);
	MQTT_OnConnected(&myclient, this->onConnectedcb);
	MQTT_OnDisconnected(&myclient, this->onDisconnectedcd);
	MQTT_OnPublished(&myclient, this->publishedCb);
	MQTT_OnData(&myclient, this->dataCb);
}

ICACHE_FLASH_ATTR MqttManager::~MqttManager() {
}


void ICACHE_FLASH_ATTR MqttManager::
onConnectedcb(uint32_t *arg)
{
	MQTT_Client* client = (MQTT_Client*)arg;
	//os_printf("MQTT: Connected\r\n");
	MQTT_Subscribe(&myclient,(char*)"test/setting", 0);

	MQTT_Publish(&myclient,"/test/topic/0" ,"hello", 5, 0, 0);
}

void ICACHE_FLASH_ATTR MqttManager::
onDisconnectedcd(uint32_t *arg)
{
	MQTT_Client* client = (MQTT_Client*)arg;
	//os_printf("MQTT: Disconnected\r\n");
}

void ICACHE_FLASH_ATTR MqttManager::
publishedCb(uint32_t *arg)
{
	MQTT_Client* client = (MQTT_Client*)arg;
	//os_printf("MQTT: Published\r\n");
}

void ICACHE_FLASH_ATTR MqttManager::
dataCb(uint32_t *args, const char* topic,
		uint32_t topic_len, const char *data, uint32_t data_len)
{
	char *topicBuf = new char[topic_len+1];
	char *dataBuf = new char[data_len+1];

	os_memcpy(topicBuf, topic, topic_len);
	topicBuf[topic_len] = 0;

	os_memcpy(dataBuf, data, data_len);
	dataBuf[data_len] = 0;
	os_printf("Receive topic: %s, data: %s \r\n", topicBuf, dataBuf);


	delete[] topicBuf;
	delete[] dataBuf;

}


void ICACHE_FLASH_ATTR MqttManager::
connect()
{
	MQTT_Connect(&myclient);
	os_printf("Connecting mqtt\r\n");
}

void ICACHE_FLASH_ATTR MqttManager::
disconnect()
{
	MQTT_Disconnect(&myclient);
}


void ICACHE_FLASH_ATTR MqttManager::
update(PMS3003Data *dataObj)
{
	os_printf("Mqtt manager Update\r\n");
	char bu[9] = "";
	ultostr(dataObj->getPm010Tsi(),bu,10);
	MQTT_Publish(&myclient,pub_topic0, bu, strlen(bu), 0, 0);

	ultostr(dataObj->getPm025Tsi(),bu,10);
	MQTT_Publish(&myclient,pub_topic1, bu, strlen(bu), 0, 0);

	ultostr(dataObj->getPm100Tsi(),bu,10);
	MQTT_Publish(&myclient,pub_topic2, bu, strlen(bu), 0, 0);

	ultostr(dataObj->getPm010Atm(),bu,10);
	MQTT_Publish(&myclient,pub_topic3, bu, strlen(bu), 0, 0);

	ultostr(dataObj->getPm025Atm(),bu,10);
	MQTT_Publish(&myclient,pub_topic4, bu, strlen(bu), 0, 0);

	ultostr(dataObj->getPm100Atm(),bu,10);
	MQTT_Publish(&myclient,pub_topic5, bu, strlen(bu), 0, 0);

	/*
	os_printf("PM010 TSI -> %s\r\n",dataObj->getPm010Tsi());
	os_printf("PM025 TSI -> %d\r\n",dataObj->getPm025Tsi());
	os_printf("PM100 TSI -> %d\r\n",dataObj->getPm100Tsi());
	os_printf("PM010 ATM -> %d\r\n",dataObj->getPm010Atm());
	os_printf("PM025 ATM -> %d\r\n",dataObj->getPm025Atm());
	os_printf("PM100 ATM -> %d\r\n",dataObj->getPm100Atm());
	*/

}



