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
#include <unistd.h>

using namespace std;
	

//GÅR NOG IN I "." och ".." FIXA
	DiscDatabase::DiscDatabase(){
		uint counter = 0;
		
		//skapa fil med nextIDgrejs
		//skapa rootgrejs
		DIR* pdir;		
		pdir=opendir("./root");
		if(!pdir){
			mkdir("root", S_IRWXU | S_IRWXG | S_IRWXO);
			pdir=opendir("./root");
		}
		DIR* cdir;
		struct dirent *art;
		fstream fs;
		struct dirent *pent;
		unsigned char isFolder =0x4;	
		int a = 0;
		readdir(pdir); // Läs katalogen två gånger för att undvika . och ..
		readdir(pdir);  
		while ((pent=readdir(pdir))){

			if(pent->d_type == isFolder) {
				string fullName(pent->d_name);				
				size_t index = fullName.find("_");
				uint id = stoi(fullName.substr(0, index));
				string name = fullName.substr(index+1);
				Newsgroup gr;
				gr.title = name;
				//newsgroups.insert(make_pair(id, gr));
				char dirpath[256];
				strcpy (dirpath, "./root/");
				strcat (dirpath, pent->d_name);
				cdir = opendir(dirpath);

				readdir(cdir); // Läs katalogen två gånger för att undvika . och ..
				readdir(cdir); 
				while((art = readdir(cdir))){
					string artName(art->d_name);
					if(artName == "nextID"){
						fs.open("./root/" + fullName + "/nextID");
						if(fs.is_open()){
							string tempnext;
							fs >> tempnext;
							gr.nextArtID = stoi(tempnext);
						}
						fs.close();
					}else{
						fs.open("./root/" + fullName + "/" + artName);
cout << "artname " << artName << endl;
				//		index = artName.find("_");
	//					uint id = stoi(artName);
//						string name = artName.substr(index);
						Article article;
						getline(fs, article.title);
						getline(fs, article.author);
//						article.title = name;
						string text;
						while(getline(fs, text)){
							article.text += text;					
						}
						cout << article.title << endl;
					gr.articles.insert(make_pair(gr.nextArtID, article));
					fs.close();
					}
				}
				newsgroups.insert(make_pair(id, gr));
	 	}
	}
					
		fstream fs2;		
		fs.open("./root/nextID");
		if(fs.is_open()){
			string tmp;
			fs >> tmp;
			setID(stoi(tmp));
		}else{
//			fs << 0;		
			ofstream ofs;
			ofs.open("./root/nextID");
			ofs << 0;
			ofs.close();
		}
		fs.close();
cout << "slutt" << endl;
	}


	bool DiscDatabase::addNewsgroup(std::string title){
		string temptitle;
		temptitle = (to_string(getID()) + "_" + title);
cout << title << endl;		
		for(auto it = newsgroups.begin(); it != newsgroups.end(); ++ it) {
			Newsgroup gr = it->second;
			string str = gr.title;
			if (str==title) return false; 			
		}
cout << temptitle << endl;

		const char* chtitle = ("./root/" + temptitle).c_str();
cout << chtitle << endl;
		mkdir(chtitle, S_IRWXU | S_IRWXG | S_IRWXO);

		fstream fs;
		fs.open("./root/nextID");
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

				
		ofstream ofs;
		ofs.open("./root/" + temptitle + "/nextID");
		ofs << 0;
		ofs.close();

		return true;
	}	

	string getNewsgroupPath(uint id){
		DIR* pdir;
		struct dirent *pent;
		pdir=opendir("./root");
		while ((pent=readdir(pdir))){
			string ngName = pent->d_name;
			size_t index = ngName.find("_");
			if((index != string::npos) && (stoi(ngName.substr(0, index)) == id)){
				return ("./root/" + ngName);
			}
		}
	} 



	bool DiscDatabase::addArticle(uint newsgroupID, std::string title, std::string author, std::string text){
			
			Article ar;	
			ar.title = title;
			ar.author = author;
			ar.text = text;
			
//			if(newsgroups[newsgroupID].articles.insert(make_pair(newsgroups[newsgroupID].nextArtID, ar)).second){
			if(getNewsgroup(newsgroupID) != nullptr){
				cout << "legz den in?" << endl;
				newsgroups[newsgroupID].articles.insert(make_pair(newsgroups[newsgroupID].nextArtID, ar));
				++newsgroups[newsgroupID].nextArtID;

//				ofstream(getNewsgroupPath(newsgroupID)
				string ngPath = "./root/" + to_string(newsgroupID) + "_" + newsgroups[newsgroupID].title + "/";
				
			fstream fs;
			fs.open(ngPath + "nextID");
			
			string id;
			if(fs.is_open()){	
				fs >> id;
				setID(stoi(id));
				incID();
				fs << to_string(getID());
			}else{
				cout << "fel :(" << endl;
				return false;
			}
			fs.close();
			
			ofstream ofs;
			ofs.open(ngPath + id);
			ofs << title << endl;
			ofs << author << endl;
			ofs << text << endl;
			ofs.close();
				return true;
			}


		return false;
	}

	bool DiscDatabase::removeNewsgroup(uint newsgroupID){
		string ngPath = getNewsgroupPath(newsgroupID);
		if(ngPath == "./root/"){
			return false;
		}else if(newsgroups.erase(newsgroupID)){
			const char* cPath = ngPath.c_str();
			rmdir(cPath);
			return true;
		}
		cout << "fel" << endl;
		return false;
	}

	bool DiscDatabase::removeArticle(uint newsgroupID, uint articleID){	
		string artPath = getNewsgroupPath(newsgroupID);
		if(artPath == "./root/"){
			return false;
		} else if (newsgroups[newsgroupID].articles.erase(articleID)) {
			artPath += to_string(articleID);
			const char* cPath = artPath.c_str();
			remove(cPath);
			return true;
		}
			return false;
	}


	bool DiscDatabase::articleExists(uint newsgroupID, uint articleID){		
		return newsgroups[newsgroupID].articles.count(articleID) != 0;
	}		
	


	Article DiscDatabase::getArticle(uint newsgroupID, uint articleID){
		return newsgroups[newsgroupID].articles[articleID];
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
