#include "connection.h"
#include "messagehandler.h"
#include "protocol.h"
#include <iostream>
#include <sstream>
#include <string>

#define START_SCREEN -1

using namespace std;

void printHelp(int state) {
	if (state == START_SCREEN) { 
		cout << "-- Usenet client -- \n \n \
		Commands: \nlist - Lists all newsgroups \ncreate - Starts new newsgroup creation \n"
		"delete <newsgroup ID> - Delete newsgroup \nopen <newsgroup ID> - View content of newsgroup\n"
		"help - Print available commands" << endl;	
	} else { 		
		cout << "-- Usenet client -- \n \n \
		Commands: \nlist - List all articles \ncreate - Starts new article creation\n"
		"delete <article ID> - Delete articles \nread <article ID> - View content of newsgroup \nback - Back\n"
		"help - Print available commands" << endl;	
	}
}
bool listArticles(MessageHandler msgHandler, int groupId){
	msgHandler.sendCode(Protocol::COM_LIST_ART);
	msgHandler.sendIntParameter(groupId);
	msgHandler.sendCode(Protocol::COM_END);
	msgHandler.recvCode();
	if (msgHandler.recvCode() == Protocol::ANS_ACK) {
		int nbrOfArticles = msgHandler.recvIntParameter();
		
		if (nbrOfArticles > 0) {
			
			cout << "Articles: " << endl;

		} else {
			cout << "No articles in current newsgroup" << endl;
		}
		for (int i = 0; i < nbrOfArticles; i++) {
			cout << "Id: " << msgHandler.recvIntParameter();
			cout << ", Name: " << msgHandler.recvStringParameter() << endl;
		}
		msgHandler.recvCode();
		return true;
	} else {
		msgHandler.recvCode();
		cout << "No such Newsgroup" << endl;
		msgHandler.recvCode();
		return false;
	}
}
void listNewsgroups(MessageHandler msgHandler) {
	msgHandler.sendCode(Protocol::COM_LIST_NG);
	msgHandler.sendCode(Protocol::COM_END);
	msgHandler.recvCode();
	int nbrOfGroups = msgHandler.recvIntParameter();
	
	if (nbrOfGroups > 0) {
		cout << "Newsgroups: " << endl;
	} else {
		cout << "No newsgroups" << endl;	
	}
	for (int i = 0; i < nbrOfGroups; i++) {
		cout << "Id: " << msgHandler.recvIntParameter();
		cout << ", Name: " << msgHandler.recvStringParameter() << endl;
	}
	msgHandler.recvCode();
}
void createNewsgroup(MessageHandler msgHandler, string groupName) {
	msgHandler.sendCode(Protocol::COM_CREATE_NG);
	msgHandler.sendStringParameter(groupName);
	msgHandler.sendCode(Protocol::COM_END);

	msgHandler.recvCode();
	if (msgHandler.recvCode() != Protocol::ANS_ACK) {
		cout << "Newsgroup already exist" << endl;
		msgHandler.recvCode();
	} else {
		cout << "Newsgroup \"" << groupName << "\" created" << endl; 
		
	}

	msgHandler.recvCode();
}
void createArticle(MessageHandler msgHandler,int groupId, string title, string author, string text) {
	msgHandler.sendCode(Protocol::COM_CREATE_ART);
	msgHandler.sendIntParameter(groupId);

	msgHandler.sendStringParameter(title);

	
	msgHandler.sendStringParameter(author);

	msgHandler.sendStringParameter(text);

	msgHandler.sendCode(Protocol::COM_END);

	msgHandler.recvCode();
	if (msgHandler.recvCode() != Protocol::ANS_ACK) {
		cout << "Newsgroup doesnt exist" << endl;
	}else{
		cout << "Article \"" << title << "\" created" << endl;				
	}
	msgHandler.recvCode();
}
void deleteNewsgroup(MessageHandler msgHandler, int groupId) {
	msgHandler.sendCode(Protocol::COM_DELETE_NG);
	msgHandler.sendIntParameter(groupId);
	msgHandler.sendCode(Protocol::COM_END);
	msgHandler.recvCode();
	if (msgHandler.recvCode() != Protocol::ANS_ACK) {
		cout << "Newsgroup doesnt exist" << endl;
	} else {
		cout << "Newsgroup deleted" << endl;
	}
	msgHandler.recvCode();
}
void deleteArticle(MessageHandler msgHandler, int groupId, int articleId) {
	msgHandler.sendCode(Protocol::COM_DELETE_ART);
	msgHandler.sendIntParameter(groupId);
	msgHandler.sendIntParameter(articleId);

	msgHandler.sendCode(Protocol::COM_END);

	msgHandler.recvCode();
	if (msgHandler.recvCode() != Protocol::ANS_ACK) {
		if (msgHandler.recvCode() == Protocol::ERR_NG_DOES_NOT_EXIST) {
			cout << "Newsgroup doesnt exist" << endl;
		} else {
			cout << "Article doesnt exist" << endl;
		}
	} else {
		cout << "Article deleted" << endl;
	}
	msgHandler.recvCode();
}
void readArticle(MessageHandler msgHandler, int groupId, int articleId) {
	msgHandler.sendCode(Protocol::COM_GET_ART);
	msgHandler.sendIntParameter(groupId);
	msgHandler.sendIntParameter(articleId);
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
	} 

	shared_ptr<Connection> conn_ptr(&conn);
	MessageHandler msgHandler(conn_ptr);

	int state = START_SCREEN;
	string line;
	printHelp(state);
	cout << "> ";
	while (getline(cin, line)) {		
		istringstream iss(line);

		string input;
		iss >> input;
		if (input == "help") {	
			printHelp(state);
		} else if (input == "list") {
			if (state == START_SCREEN) { 			
				listNewsgroups(msgHandler);
			} else {
				listArticles(msgHandler, state);
			}
		} else if (input == "create") {
			if (state == START_SCREEN) {
				cout << "Enter newsgroup name: ";
				string groupName;
				getline(cin, groupName);
				if (groupName.size() != 0)  {
					createNewsgroup(msgHandler, groupName);
				} else {
					cout << "Must name new newsgroup" << endl;
				}
			} else {
				
				string title, author, text;
				cout << "Enter title: ";
				getline(cin, title);
				cout << "Enter author: ";
				getline(cin, author);
				cout << "Enter text: ";
				getline(cin, text);
				if (text.size() * title.size() * author.size() != 0) {
					createArticle(msgHandler, state, title, author, text);
				} else {
					cout << "Incorrect input" << endl;
				}

			}
		} else if (input ==  "delete") {
			if (state == START_SCREEN) {

				iss >> input;
				try {
					int groupId = stoi(input);
					deleteNewsgroup(msgHandler, groupId);
				} catch (std::invalid_argument&) {
					cout << "Not a number, try again, syntax: delete <ID>" << endl;
				}
			} else {
				iss >> input;
				try {
					int articleId = stoi(input);
					deleteArticle(msgHandler, state, articleId);
				} catch (std::invalid_argument&) {
					cout << "Not a number, try again, syntax: delete <ID>" << endl;
				}
			}
		} else if (input == "read") {
			if (state != START_SCREEN) {
				iss >> input;
				try {
					int articleId = stoi(input);
					readArticle(msgHandler, state, articleId);
				} catch(std::invalid_argument&) {
					cout << "Not a number, try again, syntax: read <ID>" << endl;
				}
			} else {
				cout << "No such command, write help for available commands" << endl;
			}

		}else if (input == "back") {
			if (state != START_SCREEN) {
				state = START_SCREEN;
				listNewsgroups(msgHandler);
			} else {
				cout << "No such command, write help for available commands" << endl;
			}

		}else if (input == "open") {
			if (state == START_SCREEN) {
				iss >> input;
				
				try {
					int newState = stoi(input);
					if (listArticles(msgHandler, newState)) {
						state = newState;
						
					}
				} catch(std::invalid_argument&) {
					cout << "Not a number, try again, syntax: open <ID>" << endl;
				}
			} else {
				cout << "No such command, write help for available commands" << endl;
			}

			
		} else {
			cout << "No such command, write help for available commands" << endl;

		} 
		cout << "> ";
	} 

	return 0;
}




