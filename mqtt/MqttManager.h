/*
 * MqttManager.h
 *
 *  Created on: Nov 21, 2016
 *      Author: APL-SadequrRahman
 */

#ifndef MQTT_MQTTMANAGER_H_
#define MQTT_MQTTMANAGER_H_



extern "C"{
#include "../mqtt/mqtt_c/include/mqtt.h"
#include "../PMS3003/PmsSensorInterface.h"
#include "../PMS3003/PMS3003Data.h"

}



class MqttManager : public PmsSensorInterface{
public:
	MqttManager(MQTT_Client *client);
	virtual ~MqttManager();
	static void onConnectedcb(uint32_t *arg);
	static void onDisconnectedcd(uint32_t *arg);
	static void publishedCb(uint32_t *args);
	static void dataCb(uint32_t *args, const char* topic,
			uint32_t topic_len, const char *data, uint32_t data_len);

	void publish(const char* topic, const char* data,
				int data_length, int qos, int retain);
	void subscribe(char* topic, uint8_t qos);
	void connect(void);
	void disconnect(void);
	void update(PMS3003Data *dataObj);

private:
	MQTT_Client *client;

};

#endif /* MQTT_MQTTMANAGER_H_ */
