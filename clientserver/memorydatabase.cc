#include <iostream>
#include <string>
#include "memorydatabase.h"
#include <vector>
#include <algorithm>
using namespace std;


	MemoryDatabase::MemoryDatabase(){
	}

	bool MemoryDatabase::addNewsGroup(string title){
		NewsGroup gr;
		gr.title = title;
		auto iter = find(newsGroups.begin(), newsGroups.end(), gr);
		if (iter == newsGroups.end()) {
			return false;
		} 	
		newsGroups.push_back(gr);
		return true;
	}


	bool MemoryDatabase::addArticle(uint newsGroupID, string title, string author, string text){
		if (newsGroupID < newsGroups.size()) { 		
			Article ar;
			ar.title = title;
			ar.author = author;
			ar.text = text;
			newsGroups[newsGroupID].articles.push_back(ar);
			return true;		
		}
		return false; 
	}

	bool MemoryDatabase::removeNewsGroup(uint newsGroupID){
		if (newsGroupID < newsGroups.size()) { 	
			newsGroups.erase(newsGroups.begin() + newsGroupID);
			return true;
		}
		return false;
	}

	bool MemoryDatabase::removeArticle(uint newsGroupID, uint articleID){
		if(newsGroupID < newsGroups.size() && articleID < newsGroups[newsGroupID].articles.size()){
			newsGroups[newsGroupID].articles.erase(newsGroups[newsGroupID].articles.begin() + articleID);
			return true;
		}
		return false;		
	}

	int main(){
	}
