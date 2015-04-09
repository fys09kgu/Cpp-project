#include <iostream>
#include <string>
#include "memorydatabase.h"
#include <vector>
#include <algorithm>
using namespace std;
	
	// Constructor of memorydatabase
	MemoryDatabase::MemoryDatabase() {}

	// Add new newsgroup with title
	bool MemoryDatabase::addNewsgroup(string title){
		Newsgroup gr;
		gr.title = title;

		for(auto grp : newsgroups){
			if(gr.title == grp.second.title){
				return false;
			}
		}
		newsgroups.insert(make_pair(getID(), gr));
		incID();
		return true;
	}

	// Returns a pointer to newsgroup with ID
	Newsgroup* MemoryDatabase::getNewsgroup(uint ID){
		Newsgroup* ptr = nullptr;
		if(newsgroups.count(ID) != 0){
			ptr = &newsgroups[ID];			
		}
		return ptr;
	}

	//Add article with title, author and text in a newsgroup with ID newsgroupID,
	// returns true if article successfully was added. 
	bool MemoryDatabase::addArticle(uint newsgroupID, string title, string author, string text){
			Article ar;	
			ar.title = title;
			ar.author = author;
			ar.text = text;
			
			if(getNewsgroup(newsgroupID) != nullptr){
				newsgroups[newsgroupID].articles.insert(make_pair(newsgroups[newsgroupID].nextArtID, ar));
				++newsgroups[newsgroupID].nextArtID;
				return true;
			}
			return false;
	}
	
	// Remove newsgroup, returns true if it was successfully removed.
	bool MemoryDatabase::removeNewsgroup(uint newsgroupID){
		return	newsgroups.erase(newsgroupID);
	}

	// Remove article, returns true if it was successfully removed.
	bool MemoryDatabase::removeArticle(uint newsgroupID, uint articleID){
		return newsgroups[newsgroupID].articles.erase(articleID);
	}
	
	// Returns newsgroups
	std::map<uint, Newsgroup> MemoryDatabase::getNewsgroups(){
		return newsgroups;
	}
	
	// Returns the number of newsgroups.
	uint MemoryDatabase::getNewsgroupCount() {
		return newsgroups.size();
	}

	// Check if an article exists.
	bool MemoryDatabase::articleExists(uint newsgroupID, uint articleID) {
		return newsgroups[newsgroupID].articles.count(articleID) != 0;
	}

	Article MemoryDatabase::getArticle(uint newsgroupID, uint articleID) {
		return newsgroups[newsgroupID].articles[articleID];
	}

