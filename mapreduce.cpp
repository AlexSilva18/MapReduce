#include "mapreduce.h"
#include <algorithm>
#include <sys/ipc.h>
#include <sys/shm.h>


vector<pair <string, int> > mapWords(vector<string> vStrings, vector<pair <int, int> > vIndex){
	vector<pair <string, int> > mappedWords;
	
	int start, end;
	start = vIndex[0].first;
	end = vIndex[0].second;
	
	
	string currStr;
	
	vector<string> stringsProcessed;
	
	sort(vStrings.begin(), vStrings.end());
	
	for(int i = start; i < end; i++){
		currStr = vStrings[i];
		if(find(stringsProcessed.begin(), stringsProcessed.end(), currStr) == stringsProcessed.end()){
			stringsProcessed.push_back(currStr);
			mappedWords.push_back(make_pair(currStr, count(vStrings.begin(), vStrings.end(), currStr)));
		}
	}
	
	for(unsigned int i = 0; i < mappedWords.size(); i++){
		cout << mappedWords[i].first << " " << mappedWords[i].second << endl;
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

