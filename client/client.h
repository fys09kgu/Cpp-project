#include "connection.h"
#include "messagehandler.h"
#include <iostream>
#include <sstream>

#define START_SCREEN 1
#define INSIDE_NEWSGROUP 2

int main(int argc, char* argv[]) {
	int state = START_SCREEN;
	if (argc == 3) {
		Connection conn(argv[1], stoi(argv[2]));
		if (!conn.isConnected()) {
			std::cerr << "Connection failed" << std::endl;
			exit(1);
		}
		MessageHandler msgHandler(conn);
	} else {

		std::cerr << "Wrong input" << std::endl;
		exit(1);
 	}

		
		
 	std::string input;
 	while (std::cin >> msg) {
 		std::istringstream iss(msg);

 		std::string command;
 		iss >> command;

 		switch (command) {
 			case "help" :
 				printInfo(state);
 				break;
 			case "list" :
 				list(state);
 				breat;
 			case
 		} 
 		
 	} 

	return 0;
}

int sendMessage(std::string msg, MessageHandler msgHandler) {

}

void printInfo(int state) {
	switch(state){
		case START_SCREEN : 
			std::cout << "-- Usenet client -- \n \n \
			Commands: \n list - Lists all newsgroups \n create [newsgroup] - Create new newsgroup \n \
			delete [newsgroup] - Delete newsgroup \n open [newsgroup] - View content of newsgroup" << std::endl;
			break;
		case INSIDE_NEWSGROUP :
			std::cout << "-- Usenet client -- \n \n \
			Commands: \n list - List all articles \n write [article] - Create new article \n \
			delete [article] - Delete articles \n read [article] - View content of newsgroup \n back - Back" << std::endl;	
		}
}

