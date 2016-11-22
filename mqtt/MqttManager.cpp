/*
 * MqttManager.cpp
 *
 *  Created on: Nov 21, 2016
 *      Author: APL-SadequrRahman
 */

#include "MqttManager.h"


extern "C"{
#include "config.h"
#include "osapi.h"
#include "mem.h"
#include "espmissingincludes.h"
}


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


ICACHE_FLASH_ATTR MqttManager::MqttManager(MQTT_Client *client):
client(client)
{
	os_printf("Creating mqtt object\r\n");
	MQTT_InitConnection(this->client, sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.security);
	//MQTT_InitConnection(&mqttClient, "192.168.11.128", 1883, 0);

	MQTT_InitClient(this->client, sysCfg.device_id, sysCfg.mqtt_user, sysCfg.mqtt_pass, sysCfg.mqtt_keepalive, 1);
	//MQTT_InitClient(&mqttClient, "client_id", "user", "pass", 120, 1);
	uint8_t test[] = "/test";
	uint8_t test1[] = "offline";
	MQTT_InitLWT(this->client,test ,test1 , 0, 0);
	MQTT_OnConnected(this->client, this->onConnectedcb);
	MQTT_OnDisconnected(this->client, this->onDisconnectedcd);
	MQTT_OnPublished(this->client, this->publishedCb);
	MQTT_OnData(this->client, this->dataCb);
}

ICACHE_FLASH_ATTR MqttManager::~MqttManager() {
}


void ICACHE_FLASH_ATTR MqttManager::
onConnectedcb(uint32_t *arg)
{
	os_printf("MQTT: Connected\r\n");
}

void ICACHE_FLASH_ATTR MqttManager::
onDisconnectedcd(uint32_t *arg)
{
	os_printf("MQTT: Disconnected\r\n");
}

void ICACHE_FLASH_ATTR MqttManager::
publishedCb(uint32_t *arg)
{
	os_printf("MQTT: Published\r\n");
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
	MQTT_Connect(this->client);
	os_printf("Connecting mqtt\r\n");
}

void ICACHE_FLASH_ATTR MqttManager::
disconnect()
{
	MQTT_Disconnect(this->client);
}


void ICACHE_FLASH_ATTR MqttManager::
update(PMS3003Data *dataObj)
{
	os_printf("Mqtt manager Update\r\n");

	os_printf("PM010 TSI -> %s\r\n",dataObj->getPm010Tsi());
	os_printf("PM025 TSI -> %d\r\n",dataObj->getPm025Tsi());
	os_printf("PM100 TSI -> %d\r\n",dataObj->getPm100Tsi());
	os_printf("PM010 ATM -> %d\r\n",dataObj->getPm010Atm());
	os_printf("PM025 ATM -> %d\r\n",dataObj->getPm025Atm());
	os_printf("PM100 ATM -> %d\r\n",dataObj->getPm100Atm());

}



