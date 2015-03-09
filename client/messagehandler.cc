#include <string>
#include "connection.h"
#include "protocol.h"
#include "messagehandler.h"

MessageHandler::MessageHandler(Connection connection) : conn(connection) {

}

void MessageHandler::sendCode(int code) {
	sendByte(code);
}

void sendIntParameter(int parameter) {
	sendCode(Protocol::PAR_NUM);
	sendInt(parameter);
}

void sendStringParameter(std::string parameter){
	sendCode(Protocol::PAR_STRING);
	sendInt(parameter.length());
	for(auto it = parameter.begin(); it != parameter.end(); ++it) {
		sendByte(*it);
	}
}

int recvCode() {
	return recvByte();
}

int recvInt() {
	int b1 = recvByte();
	int b2 = recvByte();
	int b3 = recvByte();
	int b4 = recvByte();

	return b1 << 24 | b2 << 16 | b3 << 8 | b4;
}

//Maybe throw exception
int recvIntParameters() {
		//Check code??
	int code = recvCode();
	return recvInt();	
}

//throw excep?
std::string recvStringParameter() {

	//Check code??
	int code = recvCode();
	int n = recvInt();
	string parameter = "";
	for (int i = 0; i < n; i++) {
		parameter += conn.read();
	}

	return parameter;
}


void sendInt(int value) {
		sendByte((value >> 24) & 0xFF);
		sendByte((value >> 16) & 0xFF);
		sendByte((value >> 8) & 0xFF);
		sendByte(value & 0xFF);	
}
private:
int recvByte() {
	/*read returnerar unsigned char*/
	return (int) conn.read();
}
void sendByte(char code) {
	conn.write((char) code);
}
