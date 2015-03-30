#include "discdatabase.h"
#include <algorithm>
#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/stat.h>
#include <vector>
#include <fstream> 	
#include <string>
#include <string.h>
using namespace std;
	

//GÅR NOG IN I "." och ".." FIXA
	DiscDatabase::DiscDatabase(){
		uint counter = 0;
cout << ++counter << endl;
		
		//skapa fil med nextIDgrejs
		//skapa rootgrejs
		DIR* pdir;		
		pdir=opendir("./root");
		if(!pdir){
			mkdir("root", S_IRWXU | S_IRWXG | S_IRWXO);
			pdir=opendir("./root");
		}
cout << ++counter << endl;
		DIR* cdir;
		struct dirent *art;
		fstream fs;
		struct dirent *pent;
		unsigned char isFolder =0x4;	
		while ((pent=readdir(pdir))){
cout << ++counter << endl;
			if(pent->d_type == isFolder) {
				string fullName(pent->d_name);				
				size_t index = fullName.find("_");
cout << ++counter << endl;
				uint id = stoi(fullName.substr(0, index-1));
cout << ++counter << endl;
				string name = fullName.substr(index);
				Newsgroup gr;
				gr.title = name;
				newsgroups.insert(make_pair(id, gr));
				char dirpath[256];
				strcpy (dirpath, "./root/");
				strcat (dirpath, pent->d_name);
				cdir = opendir(dirpath);
				while((art = readdir(cdir))){
					if(art->d_name == "nextID"){
						fs.open("./root/" + fullName + "/nextID");
						if(fs.is_open()){
							string tempnext;
							fs >> tempnext;
							gr.nextArtID = stoi(tempnext);
						}
						fs.close();
					}
					string artName(art->d_name);					
					index = artName.find("_");
cout << ++counter << endl;
					uint id = stoi(artName.substr(0, index-1));
cout << ++counter << endl;
					string name = artName.substr(index);
					Article article;
					getline(fs, article.author);
					article.title = name;
					string text;
					while(getline(fs, text)){
						article.text += text;					
					}
				}
			}
	 	}
cout << ++counter << endl;				
		fs.open("nextID");
		if(fs.is_open()){
			string tmp;
			fs >> tmp;
			setID(stoi(tmp));
		}else{
			fs << 0;		
		}
		fs.close();
cout << ++counter << endl;
	}


	bool DiscDatabase::addNewsgroup(std::string title){
		string temptitle;
		temptitle = getID() + "_" + title;
		
		for(auto it = newsgroups.begin(); it != newsgroups.end(); ++ it) {
			Newsgroup gr = it->second;
			string str = gr.title;
			if (str==title) return false; 			
		}

		const char* chtitle = temptitle.c_str();
		mkdir(chtitle, S_IRWXU | S_IRWXG | S_IRWXO);

		fstream fs;
		fs.open("nextID");
		if(fs.is_open()){
			incID();			
			fs << getID();
		} else {
			
			return false;
		}
		fs.close();
		Newsgroup ng;
		ng.title = title;
		newsgroups.insert(make_pair(getID()-1,ng));

		
	
		fs.open(temptitle + "/nextID");
				





		//skapa fil i katalogen med nextID
		return true;
	}	

	bool DiscDatabase::addArticle(uint newsGroupID, std::string title, std::string author, std::string text){
			







		return false;
	}

	bool DiscDatabase::removeNewsgroup(uint newsgroupID){
		return false;
	}

	bool DiscDatabase::removeArticle(uint newsgroupID, uint articleID){		return false;}
	bool DiscDatabase::articleExists(uint newsgroupID, uint articleID){		return false;}		
	Article DiscDatabase::getArticle(uint newsgroupID, uint articleID){
		Article* art = new Article();
		return *art;
	}		
	
	std::map<uint, Newsgroup> DiscDatabase::getNewsgroups(){
		return newsgroups; 	
}


	uint DiscDatabase::getNewsgroupCount(){
		return newsgroups.size();

}

	Newsgroup* DiscDatabase::getNewsgroup(uint ID){
		Newsgroup* ptr = nullptr;
		if(newsgroups.count(ID) != 0){
			ptr = &newsgroups[ID];			
		}
		return ptr;		
	}



	bool fileExists (const std::string& name) {
    	if (FILE *file = fopen(name.c_str(), "r")) {
        	fclose(file);
       		return true;
    	}
        return false;
	}
