#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <vector>
class Database {

public:
	virtual bool addNewsGroup(std::string title);	
	virtual bool addArticle(int newsGroupID, std::string title, std::string author, std::string text);
	virtual bool removeNewsGroup(int newsGroupID);
	virtual bool removeArticle(int newsGroupID, int articleID);
};

struct Article {
	std::string title;
	std::string author;
	std::string text;
};

struct NewsGroup {
	std::string title;
	std::vector<Article> articles;
	bool operator==(const NewsGroup& n);
};

#endif
