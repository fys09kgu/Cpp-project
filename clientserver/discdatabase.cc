#include <iostream>
#include <string>
#include "discdatabase.h"
#include <vector>
#include <algorithm>
using namespace std;

	
	DiscDatabase(){}
	bool addNewsgroup(std::string title){}	
	bool addArticle(uint newsGroupID, std::string title, std::string author, std::string text){}
	bool removeNewsgroup(uint newsgroupID){}
	bool removeArticle(uint newsgroupID, uint articleID){}
	bool articleExists(uint newsgroupID, uint articleID){}		
	Article getArticle(uint newsgroupID, uint articleID){}		
	std::vector<Newsgroup> getNewsgroups(){}
	uint getNewsgroupCount(){}