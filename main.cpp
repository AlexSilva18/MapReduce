#include "framework.h"
#include "mapreduce.h"

int main(int argc, char* argv[]){

        if (argc != 13){
	        perror ("ERROR, incorrect input");
		exit(0);
        }     
	
	executionStream *stream = new executionStream;
	getFlag(argv, stream);
	/*cout << "app " << stream->app << endl;
	cout << "impl " << stream->impl << endl;
	cout << "num_maps " << stream->num_maps << endl;
	cout << "num_reduces " << stream->num_reduces << endl;
	cout << "input file " << stream->inputFile << endl;
	cout << "output file " << stream->outputFile << endl;*/
	
        if (stream->app == "wordcount"){
	        //run wordcount
	        readInputWords(stream);
	}
	else if (stream->app == "sort"){
	        // run sort
	        cout << "run sort" << endl;
	}
}
