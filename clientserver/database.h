#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
struct Newsgroup;
struct Article;
class Database {

public:
	virtual bool addNewsgroup(std::string title) = 0;	
	virtual bool addArticle(uint newsgroupID, std::string title, std::string author, std::string text) = 0;
	virtual bool removeNewsgroup(uint newsgroupID) = 0;
	virtual bool removeArticle(uint newsgroupID, uint articleID) = 0;
	virtual Newsgroup* getNewsgroup(uint ID) = 0;
	virtual std::map<uint, Newsgroup> getNewsgroups() = 0;
	virtual uint getNewsgroupCount() = 0;
	virtual bool articleExists(uint newsgroupID, uint articleID) = 0;
	virtual Article getArticle(uint newsgroupID, uint articleID) = 0;
	void incID();
	uint getID();
	void setID(uint id);


private:
	uint nextID = 0;

};

struct Article {
	std::string title;
	std::string author;
	std::string text;
};

struct Newsgroup {
	std::string title;
	std::map<uint, Article> articles;
	bool operator==(const Newsgroup& n);
	uint nextArtID = 0;
};

#endif
