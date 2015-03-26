#include <iostream>
#include <string>
#include "memorydatabase.h"
#include <vector>
#include <algorithm>
using namespace std;


	MemoryDatabase::MemoryDatabase() : nextID(0){
	}

	void MemoryDatabase::incID(){
		++nextID;	
	}

/*	Newsgroup MemoryDatabase::getNewsgroup(uint id){
		return *newsgroups.find(id);
	}
*/
	bool MemoryDatabase::addNewsgroup(string title){
		Newsgroup gr;
		gr.title = title;
		if(newsgroups.insert(make_pair(nextID, gr)).second){
			incID();
			return true;
		}
		return false;
	}


	bool MemoryDatabase::addArticle(uint newsgroupID, string title, string author, string text){
			Article ar;	
			ar.title = title;
			ar.author = author;
			ar.text = text;
			
			if(newsgroups[newsgroupID].articles.insert(make_pair(newsgroups[newsgroupID].nextArtID, ar)).second){
				++newsgroups[newsgroupID].nextArtID;
				return true;
			}
			return false;
	}

	bool MemoryDatabase::removeNewsgroup(uint newsgroupID){
		return	newsgroups.erase(newsgroupID);
	}

	bool MemoryDatabase::removeArticle(uint newsgroupID, uint articleID){
		return newsgroups[newsgroupID].articles.erase(articleID);
	}

	std::map<uint, Newsgroup> MemoryDatabase::getNewsgroups(){
		return newsgroups;
	}

	uint MemoryDatabase::getNewsgroupCount() {
		return newsgroups.size();
	}

	bool MemoryDatabase::articleExists(uint newsgroupID, uint articleID) {
		return newsgroups[newsgroupID].articles.count(articleID) != 0;
	}

	Article MemoryDatabase::getArticle(uint newsgroupID, uint articleID) {
		return newsgroups[newsgroupID].articles[articleID];
	}
	








	

