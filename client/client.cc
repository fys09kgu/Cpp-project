#include "connection.h"
#include "messagehandler.h"
#include "protocol.h"
#include <iostream>
#include <sstream>
#include <string>

#define START_SCREEN -1

using namespace std;

void printHelp(int);

void listArticles(MessageHandler msgHandler, uint nbr){
	msgHandler.sendCode(Protocol::COM_LIST_ART);
	msgHandler.sendIntParameter(nbr);
	msgHandler.sendCode(Protocol::COM_END);
	msgHandler.recvCode();
	if (msgHandler.recvCode() == Protocol::ANS_ACK) {
		int nbrOfArticles = msgHandler.recvIntParameter();
		cout << "Articles: \n" << endl;
		for (int i = 0; i < nbrOfArticles; i++) {
			cout << "Id: " << msgHandler.recvIntParameter();
			cout << ", Name: " << msgHandler.recvStringParameter() << endl;
		}

	} else {
 		msgHandler.recvCode();
 		cout << "No such Newsgroup" << endl;
 	}
	msgHandler.recvCode();
}


int main(int argc, char* argv[]) {
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
		int counter = 0;
		cout << ++counter << endl;
		
 		istringstream iss(line);

 		string input;
 		iss >> input;

 		if (input == "help") {	
 			printHelp(state);
 		} else if (input == "list") {
			cout << ++counter << endl;
 			if (state == START_SCREEN) {
				cout << ++counter << endl; 			
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
 				listArticles(msgHandler, state);
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
 				

 			} else {

				cout << " apa " << endl;
 				msgHandler.sendCode(Protocol::COM_CREATE_ART);
 				msgHandler.sendIntParameter(state);
					
 				iss >> input;
 				msgHandler.sendStringParameter(input);
				cout << input << endl;
 				iss >> input;
 				msgHandler.sendStringParameter(input);
 				cout << input << endl;
 				iss >> input;
 				msgHandler.sendStringParameter(input);
				cout << input << endl;
 				msgHandler.sendCode(Protocol::COM_END);

 				msgHandler.recvCode();
 				if (msgHandler.recvCode() != Protocol::ANS_ACK) {
 					cout << "Newsgroup doesnt exist" << endl;
 				}else{
					cout << "Article created" << endl;				
				}
			
 			}
		msgHandler.recvCode();
 		} else if (input ==  "delete") {
 			if (state == START_SCREEN) {
 				msgHandler.sendCode(Protocol::COM_DELETE_NG);

 				iss >> input;
 				msgHandler.sendIntParameter(stoi(input));

 				msgHandler.sendCode(Protocol::COM_END);

 				msgHandler.recvCode();
 				if (msgHandler.recvCode() != Protocol::ANS_ACK) {
 					cout << "Newsgroup doesnt exist" << endl;
 				}
 				msgHandler.recvCode();
 			} else {
 				msgHandler.sendCode(Protocol::COM_DELETE_ART);
 				msgHandler.sendIntParameter(state);

 				iss >> input;
 				msgHandler.sendIntParameter(stoi(input));

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
 			msgHandler.sendIntParameter(state);
 			iss >> input;
 			msgHandler.sendIntParameter(stoi(input));
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
			try {
	 			state = stoi(input);	
			} catch(std::invalid_argument&) {
				cout << "Not a number, try again, syntax: open <ID>" << endl;
			}

		listArticles(msgHandler, state);

		} else {
 			cout << "No such command, write help for available commands" << endl;
 		} 
 	} 

	return 0;
}

void printHelp(int state) {
	if (state == START_SCREEN) { 
		cout << "-- Usenet client -- \n \n \
		Commands: \n list - Lists all newsgroups \n create [newsgroup name] - Create new newsgroup \n \
		delete <newsgroup ID> - Delete newsgroup \n open <newsgroup ID> - View content of newsgroup" << endl;
	} else { 		
		cout << "-- Usenet client -- \n \n \
		Commands: \n list - List all articles \n create [article] - Create new article \n \
		delete [article] - Delete articles \n read [article] - View content of newsgroup \n back - Back" << endl;	
	}
}



