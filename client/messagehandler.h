#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <string>
#include "connection.h"
#include "protocol.h"
#include <memory>


class MessageHandler {
public:
	MessageHandler(std::shared_ptr<Connection>);
	void sendCode(int);
	void sendIntParameter(int);
	void sendStringParameter(std::string);
	int recvCode();
	int recvInt();
	int recvIntParameter();
	std::string recvStringParameter();
	void sendInt(int);

private:
	int recvByte();
	void sendByte(char);
	std::shared_ptr<Connection> conn;
	
};

#endif