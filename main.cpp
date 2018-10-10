#include "framework.h"
#include "mapreduce.h"

int main(int argc, char* argv[]){

        if (argc != 13){
	        cout << "ERROR, incorrect input" << endl;
		exit(0);
        }     
	
	executionStream *stream = new executionStream;
	
	if(getFlag(argv, stream) == -1){
	        cout << "ERROR, Invalid Flags" << endl;
		exit(0);
	}
        if (stream->app == "wordcount"){
	  
	        // read input file, retrieve all words and store them into a vector
	        vector<string> vStrings = readInputWords(stream);  
		
		// split input and retrieve indexes of the beginning and end of each partition
		vector<pair <int, int> > vIndexes = split(stream, vStrings);
		
		vector<pair <string, int> > shuffledDataWords;
		vector<string> shuffledDataInts;
		if (stream->impl == "procs"){
		  createProcesses(stream, vStrings, vIndexes, &shuffledDataWords, &shuffledDataInts);
		}
		  		
		else if (stream->impl == "threads"){
		  createThreads(stream, vStrings, vIndexes, &shuffledDataWords, &shuffledDataInts);
		}
		// for (unsigned int i = 0; i < shuffledDataWords.size(); i++){
		//   cout << shuffledDataWords[i].first << " " << shuffledDataWords[i].second   << endl;
		// }

	}
        else if (stream->app == "sort"){
	        vector<string> vStrings = readInputInts(stream);
		vector<pair <int, int> > vIndexes = split(stream, vStrings);

		vector<pair <string, int> > shuffledDataWords;
		vector<string> shuffledDataInts;
		
		if (stream->impl == "procs"){
		createProcesses(stream, vStrings, vIndexes, &shuffledDataWords, &shuffledDataInts);
		}
		
		else if(stream->impl == "threads"){
		  createThreads(stream, vStrings, vIndexes, &shuffledDataWords, &shuffledDataInts);
		  vector<pair <int,int> > vPair = splitReduce(stream, shuffledDataInts);
		  
		  for (int i=0; i < stream->num_reduces; i++){
		  	reduceInts(shuffledDataInts, vPair[i].first, vPair[i].second);
		  }

		}

		for (unsigned int i = 0; i < shuffledDataInts.size(); i++){
		   cout << shuffledDataInts[i] << endl;
		 }
	}
	return 0;
}


