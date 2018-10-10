#include "mapreduce.h"
#include <algorithm>
#include <sys/ipc.h>
#include <sys/shm.h>


vector<pair <string, int> > mapWords(vector<string> vStrings, int start, int end){
	vector<pair <string, int> > mappedWords;
	vector<string> vPartition;

	//cout << "Here" << endl;
	
	for(int i = start; i <= end; i++){
	  vPartition.push_back(vStrings[i]);
	  //cout << i << ") " << vPartition[i] << endl;
	}
	
	// int start, end;
	// start = vIndex[0].first;
	// end = vIndex[0].second;
	//cout << "start: " << start << " end: " << end << endl;
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
	// for(unsigned int i = 0; i < mappedWords.size(); i++){
	// 	cout << mappedWords[i].first << " " << mappedWords[i].second << endl;
	// }
		
  return mappedWords;
}

vector<pair <string, int > > reduceWords(vector<vector <pair <string, int> > > vWords){
	
	vector<pair <string, int> > newVector;
	vector<string> vAllWords;
	string currWord;
	int currInt = 0;
	if(vWords.size() == 0)
		return newVector;
		
	
	// Find all unique words
	for(unsigned int i = 0; i < vWords.size(); i++){
		for(unsigned int j = 0; j < vWords[i].size(); j++){
			currWord = vWords[i][j].first;
			
			if(find(vAllWords.begin(), vAllWords.end(), currWord) == vAllWords.end()){
				vAllWords.push_back(currWord);
				cout << "currWord: " << currWord << endl;
			}
		}
	}
	
	
	for(unsigned int k = 0; k < vAllWords.size(); k++){
		currWord = vAllWords[k];
		
		for(unsigned int i = 0; i < vWords.size(); i++){
		
			for(unsigned int j = 0; j < vWords[i].size(); j++){
			
				if(currWord.compare(vWords[i][j].first) == 0){
					currInt += vWords[i][j].second;
					cout << "inside " << vWords[i][j].first << endl;
				}
			
			}
		}
		
		newVector.push_back(make_pair(currWord, currInt));
		currInt = 0;
		
	}

	return newVector;
}


vector<string> mapInts(vector<string> vStrings, int start, int end){

  vector<string> vPartition;

  string currStr;
  
  for (int i = start; i <= end; i++){
        currStr = vStrings[i];
	//vPartition.push_back(atoi(vStrings[i].c_str()));
	vPartition.push_back(vStrings[i]);
  }

  sort(vPartition.begin(), vPartition.end());

  // for(unsigned int i = 0; i < vPartition.size(); i++){
  //         cout << vPartition[i]<< endl;
  // }

  return vPartition;;
}

vector<int> reduceInts(vector<string> vInts, int start, int end){

	vector<int> newVector;
	
	if(vInts.size() == 0)
		return newVector;
		
	for(unsigned int i = 0; i < vInts.size(); i++){
		for(unsigned int j = 0; j < vInts[i].size(); j++){
			newVector.push_back(vInts[i][j]);
		}
	}
	
	return newVector;
}

