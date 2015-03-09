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
	for(auto it = parameter.begin(); it != parameter.end(); ++it){
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
}
	int recvInt() {};
	int recvIntParameters() {};
	std::string recvStringParameter() {};
	void sendInt(int) {};
private:
	int recvByte() {};
	void sendByte(char) {};
	Connection conn;
