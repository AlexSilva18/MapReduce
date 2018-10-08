#include "mapreduce.h"
#include <algorithm>
#include <sys/ipc.h>
#include <sys/shm.h>


// Maps words to their frequency
// Returns a vector pair <string, int>
vector<pair <string, int> > mapWords(vector<string> vStrings, int start, int end){
	vector<pair <string, int> > mappedWords;
	vector<string> vPartition;
	
	for(int i = start; i <= end; i++){
	  vPartition.push_back(vStrings[i]);
	  //cout << i << ") " << vPartition[i] << endl;
	}
	
	// int start, end;
	// start = vIndex[0].first;
	// end = vIndex[0].second;
	cout << "start: " << start << " end: " << end << endl;
	//cout << "vSize: " << vPartition.size() << endl;
	string currStr;
	
	vector<string> stringsProcessed;

	sort(vPartition.begin(), vPartition.end());
	
	for(unsigned int i = 0; i < vPartition.size(); i++){
		currStr = vPartition[i];
		if(find(stringsProcessed.begin(), stringsProcessed.end(), currStr) == stringsProcessed.end()){
			stringsProcessed.push_back(currStr);
			mappedWords.push_back(make_pair(currStr, count(vPartition.begin(), vPartition.end(), currStr)));
		}
	}
	//cout << "mappedWords size: " << mappedWords.size() << endl;
	for(unsigned int i = 0; i < mappedWords.size(); i++){
		cout << mappedWords[i].first << " " << mappedWords[i].second << endl;
	}
		
  return mappedWords;
}


int reduceWords(){
  return 0;
}

// Sorts ints given in parameter vStrings
vector<int> mapInts(vector<string> vStrings, int start, int end){

  vector<int> vPartition;

  string currStr;
  
  for (int i = start; i <= end; i++){
          currStr = vStrings[i];
	  vPartition.push_back(atoi(vStrings[i].c_str()));
  }

  sort(vPartition.begin(), vPartition.end());

  for(unsigned int i = 0; i < vPartition.size(); i++){
          cout << vPartition[i]<< endl;
  }

  return vPartition;
}

int reduceInts(){
  return 0;
}

