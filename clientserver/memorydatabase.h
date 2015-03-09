#ifndef MEMORYDATABASE_H
#define MEMORYDATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include "database.h"

class MemoryDatabase : public Database{

public:
	MemoryDatabase();
	bool addNewsgroup(std::string title);	
	bool addArticle(uint newsGroupID, std::string title, std::string author, std::string text);
	bool removeNewsgroup(uint newsgroupID);
	bool removeArticle(uint newsgroupID, uint articleID);
	std::vector<Newsgroup> getNewsgroups();


private: 
	std::vector<Newsgroup> newsgroups;
};
#endif
