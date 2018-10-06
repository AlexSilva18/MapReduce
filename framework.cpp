#include "framework.h"


// Global variables
vector<pthread_t> exIDs;
vector<int> vInts;
vector<string> vWords;

void getFlag(char* inputFlags[], executionStream *stream){
  int i = 0;
  
  // check each flag and make copy it to execution stream struct
  if (strcmp(inputFlags[1], "--app") == 0){
    ++i;
    if (strcmp(inputFlags[2], "wordcount") == 0 || strcmp(inputFlags[2], "sort") == 0){
      string s(inputFlags[2]);
      stream->app = s;
      ++i;
      
      if (strcmp(inputFlags[3], "--impl") == 0){
	++i;
  	if (strcmp(inputFlags[4], "procs") == 0 || strcmp(inputFlags[4], "threads") == 0){
	  string s(inputFlags[4]);
	  stream->impl = s;
	  ++i;

  	  if (strcmp(inputFlags[5], "--maps") == 0){
  	    sscanf(inputFlags[6], "%d", &stream->num_maps);
	    ++i;

  	    if (strcmp(inputFlags[7], "--reduces") == 0){
  	      sscanf(inputFlags[8], "%d", &stream->num_reduces);
	      ++i;
	      
  	      if (strcmp(inputFlags[9], "--input") == 0){
		string s(inputFlags[10]);
		stream->inputFile = s;
		++i;

  		if (strcmp(inputFlags[11], "--output") == 0){
		  string s(inputFlags[12]);
		  stream->outputFile = s;
		  ++i;
  		}
  	      }
  	    }
  	  }
  	}
      }
    }
  }    
  if (i != 8)
    perror ("ERROR, Invalid flags");

}


void readInputInts(executionStream *stream){
	const char* file = stream->inputFile.c_str();
	ifstream readFile;
	readFile.open(file);
	//vector<int> vInts;
	string currStr;
	
	if(readFile.is_open()){
		
		while(!readFile.eof()){
			
			readFile >> currStr;
			vInts.push_back(atoi(currStr.c_str()));
		}

		printVector(vInts);
		
		// CALL SPLIT METHOD HERE

	}
}


void readInputWords(executionStream *stream){

	// Open file to read
        const char* file = stream->inputFile.c_str();
        ifstream readFile;
	readFile.open(file);
	//vector<string> vWords;
	string currWord;
	
	if(readFile.is_open()){
	  
	        // Take first word to be read in from file
	        readFile >> currWord;
	
		// Read file word by word
		while(!readFile.eof()){
		        
		        // Check for non-alphabetic words
			if(currWord == "—"){
			        readFile >> currWord;
				continue;
			}
			
			// Change current word to all lower case letters
		        transform(currWord.begin(), currWord.end(), currWord.begin(), ::tolower);
		        
			// Checks for commas, periods etc and removes them
			int cLen = currWord.length();
			if(currWord[cLen-1] == '.' || currWord[cLen-1] == ',' || currWord[cLen-1] == '!'
			   || currWord[cLen-1] == ';' || currWord[cLen-1] == '?' || currWord[cLen-1] == ':'
			   || currWord[cLen-1] == ')' || currWord[cLen-1] == '\''){
				  			        
			        // NEED TO FIX "control.'"
			        currWord[cLen-1] = '\0';

				if(cLen > 1 && (currWord[cLen-2] == '.' || currWord[cLen-2] == '?' 
				|| currWord[cLen-2] == '!' || currWord[cLen-2] == '\''))
				  currWord[cLen-2] = '\0';
			}
		
			// If a word contains a hyphen, split them
			if(currWord.find('-') != string::npos){
				unsigned int hPos;
				hPos = currWord.find('-');
				string tempStr1, tempStr2;
				tempStr1 = currWord.substr(0, (int)hPos);
				tempStr2 = currWord.substr((int)hPos+1, currWord.length());
				if(tempStr1[0] >= 'a' && tempStr1[0] <= 'z'){
					vWords.push_back(tempStr1);
				}
				if(tempStr2[0] >= 'a' && tempStr2[0] <= 'z'){
					vWords.push_back(tempStr2);
				}
			}
			// If a word doesn't contain special characters, push them onto vector
			else{
				vWords.push_back(currWord);
			}
			
			// Advance to next word to be read in
			readFile >> currWord;
		}
		readFile >> currWord;
	}
	
	// If file is not open, print error
	else{
	        cout << "COULD NOT OPEN FILE";
	}
	
	//printVector(vWords);

	// place vWords in shared memory and then call split

	if (stream->impl == "procs"){
	  split(stream->num_maps);
	}
	else if (stream->impl == "threads"){
	  split(stream->num_maps);
	}
	

}

// Print all words in vector vWords
template <class inputType>
void printVector(vector<inputType> vector){
	for(unsigned int i = 0; i < vector.size(); i++){
	  cout << i << ") " << vector[i] << "\n";
	}
}

void split(int num_maps){
	
	int splitFactor;
	int vSize = vWords.size();
	cout << "vSize: " << vSize << endl;
	splitFactor = vSize/num_maps;
	int remainder = vSize % num_maps;
	
	vector<int> vInputSizes;
	int startInd, endInd;
	startInd = 0;
	
	for(int i = 0; i < num_maps; i++){
		if(i == (num_maps-1)){
			vInputSizes.push_back(splitFactor + remainder);
			cout << "SplitFactor: " << splitFactor+remainder << endl;
			
		}
		else{
			vInputSizes.push_back(splitFactor);
			cout << "SplitFactor: " << splitFactor << endl;
		}
		
		
	}
	
	// Fills in input structs and starts threads
	
	for(int i = 0; i < num_maps; i++){
		vector<int> vInputIndexes;
		vInputIndexes.push_back(startInd);
		endInd = startInd + (vInputSizes[i] - 1);
		vInputIndexes.push_back(endInd);
		startInd = endInd+1;

		InputStructWords *inStruct = new InputStructWords;
		inStruct->vInputIndexes = vInputIndexes;
		
		
		// Start thread
		pthread_t newThread;
		int retval = pthread_create(&newThread, NULL, runMapWords, (void*)inStruct);
		
		if(!retval)
			exIDs.push_back(newThread);
		else
			cout << "ERROR, PTHREAD " << num_maps << " NOT CREATED" << endl;
			
	}
	
	for(int i = 0; i < num_maps; i++){
		
		pthread_join(exIDs[i], NULL);

		cout << "Joining thread: " << i << endl;
	}
	
}

// Run thread/words version of map
void *runMapWords(void* input){
	
	// DON'T NEED vWords in struct
	
	InputStructWords *inStruct = ((InputStructWords*)input);
	vector<int> vInputIndexes = inStruct->vInputIndexes;
	int start, end;
	start = vInputIndexes[0];
	end = vInputIndexes[1];
	
	cout << "Start: " << start << " End: " << end << endl;
	return NULL;
}

