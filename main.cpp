#include "framework.h"
#include "mapreduce.h"

int main(int argc, char* argv[]){

	// Argument checks
        if (argc != 13){
	        cout << "ERROR, incorrect input" << endl;
		exit(0);
        }     
	
	executionStream *stream = new executionStream;
	
	if(getFlag(argv, stream) == -1){
	        cout << "ERROR, Invalid Flags" << endl;
		exit(0);
	}

	// When running wordcount, this code is executed
        if (stream->app == "wordcount"){
	  
	        // read input file, retrieve all words and store them into a vector
	        vector<string> vStrings = readInputWords(stream);  
		
		// split input and retrieve indexes of the beginning and end of each partition
		vector<pair <int, int> > vIndexes = split(stream, vStrings);
		
		vector<string>* sMemoryPtr = createSharedMemory(vStrings);

		if (stream->impl == "procs")
		        createProcesses(stream, vStrings, vIndexes, sMemoryPtr);
		
		else if (stream->impl == "threads")
		        createThreads(stream, vStrings, vIndexes, sMemoryPtr);
				
	}
	
	// When running sort, this code is executed
        else if (stream->app == "sort"){
	        vector<string> vStrings = readInputInts(stream);
		vector<pair <int, int> > vIndexes = split(stream, vStrings);
		
		vector<string>* sMemoryPtr = createSharedMemory(vStrings);

		if (stream->impl == "procs")
		        createProcesses(stream, vStrings, vIndexes, sMemoryPtr);
		
		else if (stream->impl == "threads")
		        createThreads(stream, vStrings, vIndexes, sMemoryPtr);
		//vector<int> mappedInts = mapInts(vStrings, indexes[0].first, indexes[0].second);

	}
	return 0;
}
