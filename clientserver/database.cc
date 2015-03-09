#include <iostream>
#include <string>
#include <vector>
#include "database.h"

using namespace std;

	bool Newsgroup::operator==(const Newsgroup& n){
		return (title == n.title);
	}


