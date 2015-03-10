#include "connection.h"
#include "messagehandler.h"
#include "protocol.h"
#include <iostream>
#include <sstream>
#include <string>

#define START_SCREEN -1


void printHelp(int);

int main(int argc, char* argv[]) {
	int state = START_SCREEN;
	MessageHandler msgHandler(nullptr);
	if (argc == 3) {
		Connection conn(argv[1], std::stoi(argv[2]));
		if (!conn.isConnected()) {
			std::cerr << "Connection failed" << std::endl;
			exit(1);
		}
		std::shared_ptr<Connection> conn_ptr(&conn);
		MessageHandler msgHandler(conn_ptr);
	} else {

		std::cerr << "Wrong input" << std::endl;
		exit(1);
 	}

		
		
 	std::string msg;
 	while (std::cin >> msg) {
 		std::istringstream iss(msg);

 		std::string command;
 		iss >> command;

 		if (command == "help") {	
 			printHelp(state);
 		} else if (command == "list") {
 			if (state == START_SCREEN) {
 				msgHandler.sendCode(Protocol::COM_LIST_NG);
 				msgHandler.sendCode(Protocol::COM_END);

 				msgHandler.recvCode();
 				int nbrOfGroups = msgHandler.recvInt();
 				std::cout << "Newsgroups: \n" << std::endl;
 				for (int i = 0; i < nbrOfGroups; i++) {
 					std::cout << "Id: " << msgHandler.recvIntParameters() << ", Name: \
 					 " << msgHandler.recvStringParameter() << std::endl;
 				}
 				msgHandler.recvCode();
 			} else {
 				msgHandler.sendCode(Protocol::COM_LIST_ART);
 				msgHandler.sendInt(state);
 				msgHandler.sendCode(Protocol::COM_END);

 				msgHandler.recvCode();
 				if (msgHandler.recvCode() == Protocol::ANS_ACK) {
 					int nbrOfArticles = msgHandler.recvInt();
 					std::cout << "Articles: \n" << std::endl;
 					for (int i = 0; i < nbrOfArticles; i++) {
 						std::cout << "Id: " << msgHandler.recvIntParameters() << ", Name: \
 						" << msgHandler.recvStringParameter() << std::endl;
 					}

 					msgHandler.recvCode();
 				}
 			}

 		} else if (command == "create") {
 			if (state == START_SCREEN) {
 				msgHandler.sendCode(Protocol::COM_CREATE_NG);

 				iss >> command;
 				msgHandler.sendStringParameter(command);

 				msgHandler.sendCode(Protocol::COM_END);

 				msgHandler.recvCode();
 				if (msgHandler.recvCode() != Protocol::ANS_ACK) {
 					//FEL!!
 				}
 				msgHandler.recvCode();

 			} else {
 				msgHandler.sendCode(Protocol::COM_CREATE_ART);
 				msgHandler.sendInt(state);

 				iss >> command;
 				msgHandler.sendStringParameter(command);

 				iss >> command;
 				msgHandler.sendStringParameter(command);
 				
 				iss >> command;
 				msgHandler.sendStringParameter(command);

 				msgHandler.sendCode(Protocol::COM_END);

 				msgHandler.recvCode();
 				if (msgHandler.recvCode() != Protocol::ANS_ACK) {
 					//FEL!!
 				}
 				msgHandler.recvCode();
 			}
 		} else if (command ==  "delete") {
 			if (state == START_SCREEN) {
 				msgHandler.sendCode(Protocol::COM_DELETE_NG);

 				iss >> command;
 				msgHandler.sendInt(stoi(command));

 				msgHandler.sendCode(Protocol::COM_END);

 				msgHandler.recvCode();
 				if (msgHandler.recvCode() != Protocol::ANS_ACK) {
 					//FEL!!
 				}
 				msgHandler.recvCode();
 			} else {
 				msgHandler.sendCode(Protocol::COM_DELETE_ART);
 				msgHandler.sendInt(state);

 				iss >> command;
 				msgHandler.sendInt(stoi(command));

 				msgHandler.sendCode(Protocol::COM_END);

 				msgHandler.recvCode();
 				if (msgHandler.recvCode() != Protocol::ANS_ACK) {
 					if (msgHandler.recvCode() == Protocol::ERR_NG_DOES_NOT_EXIST) {
 						// NO SUCH NG
 					} else {
 						// NU SUCH ARTICLE
 					}
 				} 
 				msgHandler.recvCode();
 			}


 		} else if (command == "read") {
 			msgHandler.sendCode(Protocol::COM_GET_ART);
 			msgHandler.sendInt(state);
 			iss >> command;
 			msgHandler.sendInt(stoi(command));
 			msgHandler.sendCode(Protocol::COM_END);

 			msgHandler.recvCode();

 			if (msgHandler.recvCode() == Protocol::ANS_ACK) {
 				std::string title = msgHandler.recvStringParameter();
 				std::string author = msgHandler.recvStringParameter();
 				std::string text = msgHandler.recvStringParameter();
 				std::cout << "Title: " << title << std::endl;
 				std::cout << "Author: " << author << std::endl;
 				std::cout << text << std::endl;
 			} else {
 				if (msgHandler.recvCode() == Protocol::ERR_NG_DOES_NOT_EXIST) {

 				} else {

 				}
 			}

 			msgHandler.recvCode();


 		}else if (command == "back") {
 			state = START_SCREEN;
 		}else if (command == "open") {
 			iss >> command;
 			state = stoi(command);
 		} else {
 			std::cout << "No such command, write help for available commands" << std::endl;

 		} 

 	} 

	return 0;
}

void printHelp(int state) {
	if (state == START_SCREEN) { 
		std::cout << "-- Usenet client -- \n \n \
		Commands: \n list - Lists all newsgroups \n create [newsgroup] - Create new newsgroup \n \
		delete [newsgroup] - Delete newsgroup \n open [newsgroup] - View content of newsgroup" << std::endl;
	} else { 		
		std::cout << "-- Usenet client -- \n \n \
		Commands: \n list - List all articles \n create [article] - Create new article \n \
		delete [article] - Delete articles \n read [article] - View content of newsgroup \n back - Back" << std::endl;	
	}
}



