#include <string>
#include "connection.h"
#include "protocol.h"
#include "messagehandler.h"
#include <iostream>

MessageHandler::MessageHandler(std::shared_ptr<Connection> conn_ptr) : conn(conn_ptr) {
	
}

void MessageHandler::sendCode(int code) {
	sendByte(code);
}

void MessageHandler::sendIntParameter(int parameter) {
	sendCode(Protocol::PAR_NUM);
	sendInt(parameter);
}

void MessageHandler::sendStringParameter(std::string parameter){
	sendCode(Protocol::PAR_STRING);
	sendInt(parameter.length());
	for(auto it = parameter.begin(); it != parameter.end(); ++it) {
		sendByte(*it);
	}
}

int MessageHandler::recvCode() {
	return recvByte();
}

int MessageHandler::recvInt() {
	int b1 = recvByte();
	int b2 = recvByte();
	int b3 = recvByte();
	int b4 = recvByte();

	return b1 << 24 | b2 << 16 | b3 << 8 | b4;
}
int MessageHandler::recvIntParameter() {
	int code = recvCode();
	return recvInt();	
}

std::string MessageHandler::recvStringParameter() {
	int code = recvCode();
	int n = recvInt();
	std::string parameter = "";
	for (int i = 0; i < n; i++) {
		parameter += conn->read();
	}

	return parameter;
}


void MessageHandler::sendInt(int value) {
		sendByte((value >> 24) & 0xFF);
		sendByte((value >> 16) & 0xFF);
		sendByte((value >> 8) & 0xFF);
		sendByte(value & 0xFF);	
}

int MessageHandler::recvByte() {
	/*read returnerar unsigned char*/
	return (int) conn->read();
}
void MessageHandler::sendByte(char code) {
	conn->write((char) code);
}
