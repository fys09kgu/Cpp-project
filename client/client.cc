#include "connection.h"
#include "messagehandler.h"
#include <iostream>
#include <sstream>
#include <string>

#define START_SCREEN -1


void printInfo(int);

int main(int argc, char* argv[]) {
	int state = START_SCREEN;
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
 			
 			printInfo(state);

 		} else if (command == "list") {
 			
 		} else if (command == "create") {

 		} else if (command ==  "delete") {


 		} else if (command == "read") {


 		}else if (command == "back") {
 			state = START_SCREEN;
 		}else if (command == "open") {

 		} else {
 			std::cout << "No such command, write help for available commands" << std::endl;

 		} 

 	} 

	return 0;
}

int sendMessage(std::string msg, MessageHandler msgHandler) {

}

void printInfo(int state) {
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

