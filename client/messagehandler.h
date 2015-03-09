#ifdef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <string>
#include "connection.h"
#include "protocol.h"


class MessageHandler {
public:
	MessageHandler(Connection) {};
	void sendCode(int) {};
	void sendIntParameter(int) {};
	void sendStringParameter(std::string)) {};
	int recvCode() {};
	int recvInt() {};
	int recvIntParameters() {};
	std::string recvStringParameter() {};
	void sendInt(int) {};
private:
	int recvByte() {};
	void sendByte(char) {};
	Connection conn;
};

#endif