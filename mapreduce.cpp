#include "mapreduce.h"
#include <algorithm>
#include <sys/ipc.h>
#include <sys/shm.h>

vector<pair <string, int> > mapWords(vector<string> vStrings, vector<int> vIndex){
        vector<pair <string, int> > mappedWords;
	// int start, end;
	// start = vIndex[0];
	// end = vIndex[1];
	vector<string> vMapped;
	vector<int> vCount;
	
	sort(vStrings.begin(), vStrings.end());
	
	//cout << "Count: " << count(vStrings.begin(), vStrings.end(), vStrings[103]);
	
	
	
	for(unsigned int i = 0; i < vStrings.size(); i++){
		cout << vStrings[i] << endl;
	}
	
	
	
  return mappedWords;
}

int reduceWords(){
  return 0;
}

vector<int> mapInts(){
  vector<int> mappedInts;
  return mappedInts;
}

int reduceInts(){
  return 0;
}

