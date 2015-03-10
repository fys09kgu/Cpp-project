#include "connection.h"
#include "messagehandler.h"
#include "protocol.h"
#include <iostream>
#include <sstream>
#include <string>

#define START_SCREEN -1


void printHelp(int);

int main(int argc, char* argv[]) {
//	MessageHandler msgHandler(nullptr);
Connection conn;
	if (argc == 3) {
		conn(argv[1], std::stoi(argv[2]));
		if (!conn.isConnected()) {
			std::cerr << "Connection failed" << std::endl;
			exit(1);
		}
		//std::shared_ptr<Connection> conn_ptr(&conn);
		//MessageHandler msgHandler(conn_ptr);
	} else {

		std::cerr << "Wrong input" << std::endl;
		exit(1);
 	}
	// Connections destruktor anropas?
		std::shared_ptr<Connection> conn_ptr(&conn);
		MessageHandler msgHandler(conn_ptr);
			
	int state = START_SCREEN;
 	std::string line;
 	while (std::cin >> line) {
 		std::istringstream iss(line);

 		std::string input;
 		iss >> input;

 		if (input == "help") {	
 			printHelp(state);
 		} else if (input == "list") {

 			if (state == START_SCREEN) {
 				msgHandler.sendCode(Protocol::COM_LIST_NG);
 				msgHandler.sendCode(Protocol::COM_END);

 				msgHandler.recvCode(); 
 				int nbrOfGroups = msgHandler.recvInt();

 				std::cout << "Newsgroups: \n" << std::endl;
 				for (int i = 0; i < nbrOfGroups; i++) {
 					std::cout << "Id: " << msgHandler.recvIntParameter() << ", Name: \
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
 						std::cout << "Id: " << msgHandler.recvIntParameter() << ", Name: \
 						" << msgHandler.recvStringParameter() << std::endl;
 					}

 					msgHandler.recvCode();
 				} else {
 					msgHandler.recvCode();
 					std::cout << "No such Newsgroup" << std::endl;
 				}

 				msgHandler.recvCode();
 			}

 		} else if (input == "create") {

 			if (state == START_SCREEN) {
 				msgHandler.sendCode(Protocol::COM_CREATE_NG);

 				iss >> input;
 				msgHandler.sendStringParameter(input);
 				msgHandler.sendCode(Protocol::COM_END);

 				msgHandler.recvCode();
 				if (msgHandler.recvCode() != Protocol::ANS_ACK) {
 					std::cout << "Newsgroup already exist" << std::endl;
 				}
 				msgHandler.recvCode();

 			} else {
 				msgHandler.sendCode(Protocol::COM_CREATE_ART);
 				msgHandler.sendInt(state);

 				iss >> input;
 				msgHandler.sendStringParameter(input);

 				iss >> input;
 				msgHandler.sendStringParameter(input);
 				
 				iss >> input;
 				msgHandler.sendStringParameter(input);

 				msgHandler.sendCode(Protocol::COM_END);

 				msgHandler.recvCode();
 				if (msgHandler.recvCode() != Protocol::ANS_ACK) {
 					std::cout << "Newsgroup doesnt exist" << std::endl;
 				}
 				msgHandler.recvCode();
 			}
 		} else if (input ==  "delete") {
 			if (state == START_SCREEN) {
 				msgHandler.sendCode(Protocol::COM_DELETE_NG);

 				iss >> input;
 				msgHandler.sendInt(stoi(input));

 				msgHandler.sendCode(Protocol::COM_END);

 				msgHandler.recvCode();
 				if (msgHandler.recvCode() != Protocol::ANS_ACK) {
 					std::cout << "Newsgroup doesnt exist" << std::endl;
 				}
 				msgHandler.recvCode();
 			} else {
 				msgHandler.sendCode(Protocol::COM_DELETE_ART);
 				msgHandler.sendInt(state);

 				iss >> input;
 				msgHandler.sendInt(stoi(input));

 				msgHandler.sendCode(Protocol::COM_END);

 				msgHandler.recvCode();
 				if (msgHandler.recvCode() != Protocol::ANS_ACK) {
 					if (msgHandler.recvCode() == Protocol::ERR_NG_DOES_NOT_EXIST) {
 						std::cout << "Newsgroup doesnt exist" << std::endl;
 					} else {
 						std::cout << "Article doesnt exist" << std::endl;
 					}
 				} 
 				msgHandler.recvCode();
 			}


 		} else if (input == "read") {
 			msgHandler.sendCode(Protocol::COM_GET_ART);
 			msgHandler.sendInt(state);
 			iss >> input;
 			msgHandler.sendInt(stoi(input));
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
 					std::cout << "Newsgroup doesnt exist" << std::endl;
 				} else {
 					std::cout << "Article doesnt exist" << std::endl;
 				}
 			}

 			msgHandler.recvCode();


 		}else if (input == "back") {
 			state = START_SCREEN;
 		}else if (input == "open") {
 			iss >> input;
 			state = stoi(input);
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



