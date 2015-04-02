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
	
	DiscDatabase::DiscDatabase(){
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
						Article article;
						getline(fs, article.title); // Läs två gånger för att undvika . och ..
						getline(fs, article.author);
						string text;
						while(getline(fs, text)){
							article.text += text;					
						}
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
			ofstream ofs;
			ofs.open("./root/nextID");
			ofs << 0;
			ofs.close();
		}
		fs.close();
	}


	bool DiscDatabase::addNewsgroup(std::string title){
		string temptitle;
		temptitle = (to_string(getID()) + "_" + title);
		for(auto it = newsgroups.begin(); it != newsgroups.end(); ++ it) {
			Newsgroup gr = it->second;
			string str = gr.title;
			if (str==title) return false; 			
		}

		const char* chtitle = ("./root/" + temptitle).c_str();
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
			
			if(getNewsgroup(newsgroupID) != nullptr){
				newsgroups[newsgroupID].articles.insert(make_pair(newsgroups[newsgroupID].nextArtID, ar));
				++newsgroups[newsgroupID].nextArtID;
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
		if (newsgroups.find(newsgroupID) != newsgroups.end()) {
		string ngPath = getNewsgroupPath(newsgroupID);
		
		if(ngPath == "./root/"){

			return false;
		}else {
			DIR* pdir;
			newsgroups.erase(newsgroupID);
			const char* cPath = ngPath.c_str();
			pdir=opendir(cPath);
			struct dirent* file;
			string filePath;
			while((file=readdir(pdir))){
				filePath = ngPath + "/" + file->d_name;
				remove(filePath.c_str());
			}

			cout << cPath << endl;
			rmdir(cPath);
			return true;
		}
	}
		return false;
	}

	bool DiscDatabase::removeArticle(uint newsgroupID, uint articleID){	
		if (newsgroups.find(newsgroupID) != newsgroups.end()) {
			string artPath = getNewsgroupPath(newsgroupID);
			if(artPath == "./root/"){
				return false;
			} else {
				if (newsgroups[newsgroupID].articles.find(articleID) != newsgroups[newsgroupID].articles.end()) {
					newsgroups[newsgroupID].articles.erase(articleID);
					artPath += "/" + to_string(articleID);
					const char* cPath = artPath.c_str();
					remove(cPath);
					return true;
				} else {
					return false;
				}
			}
		} else {
			return false;
		}
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
