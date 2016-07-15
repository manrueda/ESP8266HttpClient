/*
 * ESP8266HttpClient.h
 *
 *  Created on: 12 jul. 2016
 *      Author: Manuel Rueda
 */

#ifndef LIBRARIES_ESP8266HTTPCLIENT_ESP8266HTTPCLIENT_H_
#define LIBRARIES_ESP8266HTTPCLIENT_ESP8266HTTPCLIENT_H_
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "inttypes.h"
#include "string.h"
#include <SerialResponse.h>

class ESP8266HttpClient {
public:
	ESP8266HttpClient(uint8_t rx, uint8_t tx, long bauds, bool _debug, Stream* output);
	virtual ~ESP8266HttpClient();
	void reset();
	void init();
	void changeRate(long bauds);
	void println(String command);
	void print(String command);
	void setMode(int mode);
	void connectWiFi(String SSID, String password);
	String methodGET(String host, String path, int port);
	String getIP();
	SoftwareSerial* serial;
	SerialResponse* response;
private:
	Stream* logging;
	long rate;
	bool debug;
	void log(String msg);
	void log(char msg);
	void log(int msg);
	void startRequest(String host, String path, int port);
	void dumpSerial();
};

#endif /* LIBRARIES_ESP8266HTTPCLIENT_ESP8266HTTPCLIENT_H_ */
