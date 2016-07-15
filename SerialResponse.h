/*
 * SerialResponse2.h
 *
 *  Created on: 12 jul. 2016
 *      Author: manue
 */

#ifndef LIBRARIES_ESP8266HTTPCLIENT_SERIALRESPONSE_H_
#define LIBRARIES_ESP8266HTTPCLIENT_SERIALRESPONSE_H_
#include "Arduino.h"
#include "SoftwareSerial.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <Delay.h>
#include <Streaming.h>

class SerialResponse {
public:
	SerialResponse(SoftwareSerial* serial);
	String GetResponse(long wait_time, String end);
	String ReadN(long size);
private:
	SoftwareSerial* serial;

};

#endif /* LIBRARIES_ESP8266HTTPCLIENT_SERIALRESPONSE_H_ */
