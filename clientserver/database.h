#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <vector>
struct Newsgroup;
class Database {

public:
	virtual bool addNewsgroup(std::string title) = 0;	
	virtual bool addArticle(uint newsgroupID, std::string title, std::string author, std::string text) = 0;
	virtual bool removeNewsgroup(uint newsgroupID) = 0;
	virtual bool removeArticle(uint newsgroupID, uint articleID) = 0;
	virtual std::vector<Newsgroup> getNewsgroups() = 0;
};

struct Article {
	std::string title;
	std::string author;
	std::string text;
};

struct Newsgroup {
	std::string title;
	std::vector<Article> articles;
	bool operator==(const Newsgroup& n);
};

#endif
