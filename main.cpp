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

		if (stream->impl == "procs")
		        createProcesses(stream, vStrings, vIndexes);
		
		else if (stream->impl == "threads")
		        createThreads(stream, vStrings, vIndexes);
				
	}
        else if (stream->app == "sort"){
	        vector<string> vStrings = readInputInts(stream);
		vector<pair <int, int> > vIndexes = split(stream, vStrings);

		if (stream->impl == "procs")
		        createProcesses(stream, vStrings, vIndexes);
		
		else if (stream->impl == "threads")
		        createThreads(stream, vStrings, vIndexes);
		//vector<int> mappedInts = mapInts(vStrings, indexes[0].first, indexes[0].second);

	}
	return 0;
}
