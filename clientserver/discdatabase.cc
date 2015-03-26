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
using namespace std;
	
	DiscDatabase::DiscDatabase(){
		//skapa fil med nextIDgrejs
		//skapa rootgrejs
		DIR* pdir;		
		pdir=opendir("./srvr");
		if(!pdir){
			mkdir("srvr", S_IRWXU | S_IRWXG | S_IRWXO);
			pdir=opendir("./srvr");
		}
				
		fstream fs;
		fs.open("nextID_file");
		if(fs.is_open()){
			string tmp;
			fs >> tmp;
			setID(stoi(tmp));
		}else{
			fs << 0;		
		}
		fs.close();
	}


	bool DiscDatabase::addNewsgroup(std::string title){
// kolla unikt namn
		string temptitle;

		temptitle = getID() + "_" + title;
		const char* chtitle = temptitle.c_str();
		mkdir(chtitle, S_IRWXU | S_IRWXG | S_IRWXO);
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
	Article art;
return art;}		
	std::map<uint, Newsgroup> DiscDatabase::getNewsgroups(){
	map<uint, Newsgroup> gg;
	return gg;}
	uint DiscDatabase::getNewsgroupCount(){return 0;}



	bool fileExists (const std::string& name) {
    	if (FILE *file = fopen(name.c_str(), "r")) {
        	fclose(file);
       		return true;
    	}
        return false;
	}
