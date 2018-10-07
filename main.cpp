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
	/*cout << "app " << stream->app << endl;
	cout << "impl " << stream->impl << endl;
	cout << "num_maps " << stream->num_maps << endl;
	cout << "num_reduces " << stream->num_reduces << endl;
	cout << "input file " << stream->inputFile << endl;
	cout << "output file " << stream->outputFile << endl;*/
	
        if (stream->app == "wordcount"){
	        //run wordcount
	        vector<string> vStrings = readInputWords(stream);
	        //cout << vStrings[2] << endl;
		//split(stream, vStrings);
		vector<pair <int, int> > indexes;
		indexes = split(stream, vStrings);
		// cout << indexes[0].first << endl;
		// cout << indexes[0].second << endl;
		// cout << indexes[1].first << endl;
		// cout << indexes[1].second << endl;
	}
        else if (stream->app == "sort"){
	        // run sort
	        vector<string> vStrings = readInputInts(stream);
		//split(stream, vStrings);
	}
	return 0;
}
