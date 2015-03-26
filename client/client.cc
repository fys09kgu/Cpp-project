#include "connection.h"
#include "messagehandler.h"
#include "protocol.h"
#include <iostream>
#include <sstream>
#include <string>

#define START_SCREEN -1

using namespace std;

void printHelp(int);

int main(int argc, char* argv[]) {
//	MessageHandler msgHandler(nullptr);
	if (argc != 3) {
		cerr << "Wrong input" << endl;
		exit(1);
 	}
	Connection conn(argv[1], stoi(argv[2]));		

	if (!conn.isConnected()) {
			cerr << "Connection failed" << endl;
			exit(1);
	
		//shared_ptr<Connection> conn_ptr(&conn);
		//MessageHandler msgHandler(conn_ptr);
	} 
	// Connections destruktor anropas?
		shared_ptr<Connection> conn_ptr(&conn);
		MessageHandler msgHandler(conn_ptr);
			
	int state = START_SCREEN;
 	string line;
	printHelp(state);
 	while (getline(cin, line)) {
 		istringstream iss(line);

 		string input;
 		iss >> input;

 		if (input == "help") {	
 			printHelp(state);
 		} else if (input == "list") {

 			if (state == START_SCREEN) {
 				msgHandler.sendCode(Protocol::COM_LIST_NG);
 				msgHandler.sendCode(Protocol::COM_END);
 				msgHandler.recvCode();
 				int nbrOfGroups = msgHandler.recvIntParameter();
				cout << nbrOfGroups << endl;
 				cout << "Newsgroups: \n" << endl;
 				for (int i = 0; i < nbrOfGroups; i++) {
 					cout << "Id: " << msgHandler.recvIntParameter();
					cout << ", Name: " << msgHandler.recvStringParameter() << endl;
 				}
 				msgHandler.recvCode();
 			} else {
 				msgHandler.sendCode(Protocol::COM_LIST_ART);
 				msgHandler.sendInt(state);
 				msgHandler.sendCode(Protocol::COM_END);

 				msgHandler.recvCode();
 				if (msgHandler.recvCode() == Protocol::ANS_ACK) {

 					int nbrOfArticles = msgHandler.recvInt();

 					cout << "Articles: \n" << endl;
 					for (int i = 0; i < nbrOfArticles; i++) {
 						cout << "Id: " << msgHandler.recvIntParameter() << ", Name: \
 						" << msgHandler.recvStringParameter() << endl;
 					}

 					msgHandler.recvCode();
 				} else {
 					msgHandler.recvCode();
 					cout << "No such Newsgroup" << endl;
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
 					cout << "Newsgroup already exist" << endl;
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
 					cout << "Newsgroup doesnt exist" << endl;
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
 					cout << "Newsgroup doesnt exist" << endl;
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
 						cout << "Newsgroup doesnt exist" << endl;
 					} else {
 						cout << "Article doesnt exist" << endl;
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
 				string title = msgHandler.recvStringParameter();
 				string author = msgHandler.recvStringParameter();
 				string text = msgHandler.recvStringParameter();
 				cout << "Title: " << title << endl;
 				cout << "Author: " << author << endl;
 				cout << text << endl;
 			} else {
 				if (msgHandler.recvCode() == Protocol::ERR_NG_DOES_NOT_EXIST) {
 					cout << "Newsgroup doesnt exist" << endl;
 				} else {
 					cout << "Article doesnt exist" << endl;
 				}
 			}

 			msgHandler.recvCode();


 		}else if (input == "back") {
 			state = START_SCREEN;
 		}else if (input == "open") {
 			iss >> input;
 			state = stoi(input);
 		} else {
 			cout << "No such command, write help for available commands" << endl;

 		} 

 	} 

	return 0;
}

void printHelp(int state) {
	if (state == START_SCREEN) { 
		cout << "-- Usenet client -- \n \n \
		Commands: \n list - Lists all newsgroups \n create [newsgroup] - Create new newsgroup \n \
		delete [newsgroup] - Delete newsgroup \n open [newsgroup] - View content of newsgroup" << endl;
	} else { 		
		cout << "-- Usenet client -- \n \n \
		Commands: \n list - List all articles \n create [article] - Create new article \n \
		delete [article] - Delete articles \n read [article] - View content of newsgroup \n back - Back" << endl;	
	}
}



