#ifndef DISCDATABASE_H
#define DISCDATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include "database.h"

class DiscDatabase : public Database{

public:
	DiscDatabase();
	bool addNewsgroup(std::string title);	
	bool addArticle(uint newsGroupID, std::string title, std::string author, std::string text);
	bool removeNewsgroup(uint newsgroupID);
	bool removeArticle(uint newsgroupID, uint articleID);
	bool articleExists(uint newsgroupID, uint articleID);		
	Article getArticle(uint newsgroupID, uint articleID);		
	std::vector<Newsgroup> getNewsgroups();
	uint getNewsgroupCount();

private: 
	std::vector<Newsgroup> newsgroups;
};
#endif
