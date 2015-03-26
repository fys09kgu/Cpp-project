#ifndef MEMORYDATABASE_H
#define MEMORYDATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include "database.h"
#include <map>

class MemoryDatabase : public Database{

public:
	MemoryDatabase();
	bool addNewsgroup(std::string title);	
	bool addArticle(uint newsGroupID, std::string title, std::string author, std::string text);
	bool removeNewsgroup(uint newsgroupID);
//	Newsgroup getNewsgroup(uint ID);
	bool removeArticle(uint newsgroupID, uint articleID);
	bool articleExists(uint newsgroupID, uint articleID);
	void incID();		
	Article getArticle(uint newsgroupID, uint articleID);		
	std::map<uint, Newsgroup> getNewsgroups();
	uint getNewsgroupCount();

private: 
	std::map<uint, Newsgroup> newsgroups;
	uint nextID;
};
#endif
