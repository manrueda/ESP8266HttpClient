/*
 * SerialResponse2.cpp
 *
 *  Created on: 12 jul. 2016
 *      Author: manue
 */

#include <SerialResponse.h>

char buf[256];			// buffer to gather ESP responses

SerialResponse::SerialResponse(SoftwareSerial* serialEsp) {
	serial = serialEsp;
}

// Get a response from ESP
// return when string received or timeout
// returns 1 if expected response received, 0 if not
// Similar to GetResponse, but doesn't send a command

String SerialResponse::GetResponse(long wait_time, String end) {
	char c;
	String response = "";

	NonBlockDelay wt;                 // Allows us to do something while waiting

	memset(buf, 0, sizeof(buf));

	wt.Delay(wait_time);                         // seconds to wait for response

	while (!wt.Timeout()) {

		if (serial->available() > 0)             // if something there
				{

			c = serial->read();                   // read  character

			//if ((c >= 32 && c <= 127) || c == 12 || c == 15 || c == 10) {
				response += c;

				if (response.endsWith(end)){
					break;
				}
			//}
		}
	}
	return response;
}

String SerialResponse::ReadN(long size){
	String rsp = "";
	long iter = 1;
	char c;
	while(serial->available() && iter <= size){
		c = serial->read();
		rsp += c;
		iter++;
	}
	return rsp;
}
