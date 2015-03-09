#ifndef MEMORYDATABASE_H
#define MEMORYDATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include "database.h"

class MemoryDatabase : public Database{

public:
	MemoryDatabase();
	bool addNewsGroup(std::string title);	
	bool addArticle(uint newsGroupID, std::string title, std::string author, std::string text);
	bool removeNewsGroup(uint newsGroupID);
	bool removeArticle(uint newsGroupID, uint articleID);


private: 
	std::vector<NewsGroup> newsGroups;
};
#endif
