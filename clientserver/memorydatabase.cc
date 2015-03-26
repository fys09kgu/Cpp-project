#include <iostream>
#include <string>
#include "memorydatabase.h"
#include <vector>
#include <algorithm>
using namespace std;


	MemoryDatabase::MemoryDatabase() {
	
	}
/*
	void MemoryDatabase::incID(){
		++nextID;	
	}
*/
/*	Newsgroup MemoryDatabase::getNewsgroup(uint id){
		return *newsgroups.find(id);
	}
*/
	bool MemoryDatabase::addNewsgroup(string title){
		Newsgroup gr;
		gr.title = title;

		for(auto grp : newsgroups){
			if(gr.title == grp.second.title){
				return false;
			}
		}
/*
		for_each(newsgroups.begin(), newsgroups.end(), [&](pair<const uint, Newsgroup>& grp){
			if(gr.title == grp.second.title){
				return false;
			}	
		}); */
		newsgroups.insert(make_pair(getID(), gr));
		incID();
		return true;
		
	}

	Newsgroup* MemoryDatabase::getNewsgroup(uint ID){
		Newsgroup* ptr = nullptr;
		if(newsgroups.count(ID) != 0){
			ptr = &newsgroups[ID];			
		}
		return ptr;
	}


	bool MemoryDatabase::addArticle(uint newsgroupID, string title, string author, string text){
			Article ar;	
			ar.title = title;
			ar.author = author;
			ar.text = text;
			
//			if(newsgroups[newsgroupID].articles.insert(make_pair(newsgroups[newsgroupID].nextArtID, ar)).second){
			if(getNewsgroup(newsgroupID) != nullptr){
				newsgroups[newsgroupID].articles.insert(make_pair(newsgroups[newsgroupID].nextArtID, ar));
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
	








	

