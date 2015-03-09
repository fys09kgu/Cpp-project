#include <iostream>
#include <string>
#include "database.h"

	bool Database::addNewsGroup(std::string title){
		return false;	
	}
	bool Database::addArticle(int newsGroupID, std::string title, std::string author, std::string text){
		return false;
	}
	bool Database::removeNewsGroup(int newsGroupID){
		return false;
	}
	bool Database::removeArticle(int newsGroupID, int articleID){
		return false;
	}
	
	bool NewsGroup::operator==(const NewsGroup& n){
		return (title == n.title);
	}
