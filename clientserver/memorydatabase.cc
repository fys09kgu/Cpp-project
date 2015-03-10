#include <iostream>
#include <string>
#include "memorydatabase.h"
#include <vector>
#include <algorithm>
using namespace std;


	MemoryDatabase::MemoryDatabase(){
	}

	bool MemoryDatabase::addNewsgroup(string title){
		Newsgroup gr;
		gr.title = title;
		auto iter = find(newsgroups.begin(), newsgroups.end(), gr);
		if (iter == newsgroups.end()) {
			newsgroups.push_back(gr);
			return true;
		} 			
		return false;
	}


	bool MemoryDatabase::addArticle(uint newsgroupID, string title, string author, string text){
		if (newsgroupID < newsgroups.size()) { 		
			Article ar;
			ar.title = title;
			ar.author = author;
			ar.text = text;
			newsgroups[newsgroupID].articles.push_back(ar);
			return true;		
		}
		return false; 
	}

	bool MemoryDatabase::removeNewsgroup(uint newsgroupID){
		if (newsgroupID < newsgroups.size()) { 	
			newsgroups.erase(newsgroups.begin() + newsgroupID);
			return true;
		}
		return false;
	}

	bool MemoryDatabase::removeArticle(uint newsgroupID, uint articleID){
		if(newsgroupID < newsgroups.size() && articleID < newsgroups[newsgroupID].articles.size()){
			newsgroups[newsgroupID].articles.erase(newsgroups[newsgroupID].articles.begin() + articleID);
			return true;
		}
		return false;		
	}

	std::vector<Newsgroup> MemoryDatabase::getNewsgroups(){
		return newsgroups;
	}

	uint MemoryDatabase::getNewsgroupCount() {
		return newsgroups.size();
	}

	bool MemoryDatabase::articleExists(uint newsgroupID, uint articleID) {
		return (newsgroupID < getNewsgroupCount() && articleID < newsgroups[newsgroupID].articles.size());
	}

	Article MemoryDatabase::getArticle(uint newsgroupID, uint articleID) {
			return newsgroups[newsgroupID].articles[articleID];
	}
	








	

