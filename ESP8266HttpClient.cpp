/*
 * ESP8266HttpClient.cpp
 *
 *  Created on: 12 jul. 2016
 *      Author: Manuel Rueda
 */

#include <ESP8266HttpClient.h>

ESP8266HttpClient::ESP8266HttpClient(uint8_t rx, uint8_t tx, long bauds, bool _debug, Stream* output) {
	serial = new SoftwareSerial(rx, tx);
	rate = bauds;
	logging = output;
	debug = _debug;
	response = new SerialResponse(serial);
}

ESP8266HttpClient::~ESP8266HttpClient() {
	// TODO Auto-generated destructor stub
}

void ESP8266HttpClient::println(String command)
{
  serial->println(command);
}

void ESP8266HttpClient::print(String command)
{
  serial->print(command);
}

void ESP8266HttpClient::init()
{
  serial->begin(rate);
  delay(250);
}

void ESP8266HttpClient::log(String msg) {
  if (debug) {
    logging->println("  ESP8266 -> " + msg);
  }
}

void ESP8266HttpClient::log(char msg) {
  log("" + msg);
}

void ESP8266HttpClient::log(int msg) {
  log(String(msg));
}

void ESP8266HttpClient::reset() {
  println("AT+RST");
  log("Reset: " + response->GetResponse(4000, "GOT IP"));	// has no real ending but normally end with WIFI GOT IP
}

void ESP8266HttpClient::changeRate(long bauds)
{
  rate = bauds;
  println("AT+CIOBAUD=" + String(rate));
  serial->begin(rate);

  log("Change rate: " + response->GetResponse(2000, "\nOK"));
}

void ESP8266HttpClient::setMode(int mode) {
	println("AT+CWMODE=" + String(mode));
	log("Change mode: " + response->GetResponse(2000, "\nOK"));
}

void ESP8266HttpClient::connectWiFi(String SSID, String password) {
	println("AT+CWJAP=\"" + SSID +"\",\"" + password +"\"");
	log("Connect Wi-Fi: " + response->GetResponse(6000, "\nOK"));
}

String ESP8266HttpClient::getIP() {
	String ip = "";

	println("AT+CIFSR");
	ip = response->GetResponse(1000, "\nOK");
	ip = ip.substring(ip.indexOf(":STAIP") + 8 );
	ip = ip.substring(0, ip.indexOf("\n") - 1 );
	log("Get IP: " + ip);
	return ip;
}

void ESP8266HttpClient::startRequest(String host, String path, int port) {
	println("AT+CIPSTART=\"TCP\",\"" + host +"\"," + String(port));
	response->GetResponse(1000, "\nOK");
	log("Start request: " + host + ':' + String(port));
}

String ESP8266HttpClient::methodGET(String host, String path, int port) {
	startRequest(host, path, port);
	String req = "GET / HTTP/1.0\r\nHost: google.com:80\r\nConnection: keep-alive\r\n";
	println("AT+CIPSEND=" + String(req.length() + 2));
	response->GetResponse(1000, "\nOK");

	delay(50);

	if (serial->find('>')) {
		println(req);
		log(response->GetResponse(1000, "SEND OK"));
		log(response->GetResponse(10000, "IPD"));
		log(response->GetResponse(2000, "CLOSED"));
		if (serial->find("+IPD,")) {	//TODO: find a way to remove this warning
			String dataLS = "";
			log("1");
			while (serial->available()) {
				log("1-");
				char c = serial->read();
				if (c != ':'){
					log("1--");
					dataLS += c;
				}else{
					log("1---");
					break;
				}
			}
			dumpSerial();
			//log(response->ReadN(dataLS.toInt()));
		}else{
			log("no +IPD,");
		}
	}else {
		println("AT+CIPCLOSE");
		response->GetResponse(500, "\nOK");
	}
	return "";
}

void ESP8266HttpClient::dumpSerial() {
	String rsp = "";
	char c;
	log("Start dumping -------------");
	while (serial->available()) {
		c = serial->read();
		rsp += c;
	}
	log(rsp);
	log("End dumping -------------");
}
