#include "discdatabase.h"
#include <algorithm>
using namespace std;
	
	DiscDatabase::DiscDatabase(){
		//skapa fil med nextIDgrejs
		//skapa rootgrejs		
	}


	bool DiscDatabase::addNewsgroup(std::string title){
// kolla unikt namn


		temptitle = nextIDfilengrejen + "_" + title;
		char* chtitle = temptitle;
		mkdir(chtitle, S_IRWXU | S_IRWXG | S_IRWXO);
		//skapa fil i katalogen med nextID
	}	

	bool DiscDatabase::addArticle(uint newsGroupID, std::string title, std::string author, std::string text){
		//skapa fil :D
	}

	bool DiscDatabase::removeNewsgroup(uint newsgroupID){

	}

	bool DiscDatabase::removeArticle(uint newsgroupID, uint articleID){}
	bool DiscDatabase::articleExists(uint newsgroupID, uint articleID){}		
	Article DiscDatabase::getArticle(uint newsgroupID, uint articleID){}		
	std::map<Newsgroup> DiscDatabase::getNewsgroups(){}
	uint DiscDatabase::getNewsgroupCount(){}



	bool fileExists (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}
