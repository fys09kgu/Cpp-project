#include <iostream>
#include <string>
#include <vector>
#include "database.h"

using namespace std;

	bool Newsgroup::operator==(const Newsgroup& n){
		return (title == n.title);
	}
	void Database::incID(){
		++nextID;	
	}
	uint Database::getID(){
		return nextID;
	}
	void Database::setID(uint id){
		nextID = id;
	}


