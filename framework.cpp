#include "framework.h"
#include "mapreduce.h"

// Global variables
vector<pthread_t> exIDs;
//vector<string> vStrings;

int getFlag(char* inputFlags[], executionStream *stream){
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
    return -1;
  return 0;

}

vector<string> readInputInts(executionStream *stream){
        
	const char* file = stream->inputFile.c_str();
	ifstream readFile;
	readFile.open(file);
	vector<string> vStrings;
	string currStr;

	if(readFile.is_open()){
		
		while(!readFile.eof()){
			
			readFile >> currStr;
			vStrings.push_back(currStr);
		}

		//printVector(vInts);
		// CALL SPLIT METHOD HERE
	}
	else{
	  cout << "COULD NOT OPEN FILE";
	  exit(0);
	}
	return vStrings;
	//split(stream, vStrings);
}


vector<string> readInputWords(executionStream *stream){

	// Open file to read
        const char* file = stream->inputFile.c_str();
        ifstream readFile;
	readFile.open(file);
	vector<string> vStrings;
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
					vStrings.push_back(tempStr1);
				}
				if(tempStr2[0] >= 'a' && tempStr2[0] <= 'z'){
					vStrings.push_back(tempStr2);
				}
			}
			// If a word doesn't contain special characters, push them onto vector
			else{
				vStrings.push_back(currWord);
			}
			
			// Advance to next word to be read in
			readFile >> currWord;
		}
		readFile >> currWord;
	}
	
	// If file is not open, print error
	else{
	        cout << "COULD NOT OPEN FILE";
		exit(0);
	}
	
	//printVector(vStrings);
	return vStrings;
	//split(stream, vStrings);
	

}


// Print all words in vector vStrings
template <class inputType>
void printVector(vector<inputType> vector){
	for(unsigned int i = 0; i < vector.size(); i++){
	  cout << i << ") " << vector[i] << "\n";
	}
}

vector<pair <int, int> > split(executionStream *stream, vector<string> vInput){
        vector<pair <int, int> > indexes;
	int splitFactor;
	int vSize;
	
	vSize = vInput.size();
	cout << "vSize: " << vSize << endl;

	splitFactor = vSize/(stream->num_maps);
	int remainder = vSize % stream->num_maps;
	
	vector<int> vInputSizes;
	int startInd, endInd;
	startInd = 0;
	endInd = splitFactor;
	
	for(int i = 0; i < stream->num_maps; i++){
		if(i == (stream->num_maps-1)){
		  vInputSizes.push_back(splitFactor + remainder);			
		}
		else{
		  vInputSizes.push_back(splitFactor);
	        
		}
	}
	// Fills in input structs and starts threads
	
	for(int i = 0; i < stream->num_maps; i++){
	      vector<int> vInputIndexes;
	      vInputIndexes.push_back(startInd);
	      endInd = startInd + (vInputSizes[i] - 1);
	      vInputIndexes.push_back(endInd);
	      startInd = endInd+1;

	      indexes.push_back(make_pair(vInputIndexes[0],vInputIndexes[1]));	      
	}
	return indexes;
	
	//   if (stream->impl == "procs"){
		
// 		vector<string>* sMemoryPtr;
		
// 		if (i == 0){
// 		  int shmid;
// 		  key_t key;
// 		  key = 9876;
		  
// 		  shmid = shmget(key, sizeof(vInput), IPC_CREAT | 0666);
// 		  if (shmid < 0){
// 		    cout << ("ERROR, Unable to create shared memory") << endl;;
// 		    exit(0);
// 		  }
// 		  cout << "shared memory key is: " << shmid << endl;
		  
// 		  sMemoryPtr = (vector<string>*)shmat (shmid, NULL, 0);
// 		  sMemoryPtr = &vInput;
// 		}
	      

// 		  sMemoryPtr = new(shm) vector<string>;
// 		  *sMemoryPtr = vInput;
// 		printVector(*shm);

// 		pid_t pid = fork();
// 	        int status = 0;

// 		// child process
// 		if (pid == 0) {
		  
// 		  // map
// 		  if (stream->app == "wordcount"){
// 		    vector<pair <string, int> > mappedWords;
// 		    mappedWords = mapWords(vInput, vInputIndexes);
// 		  }
// 		  else if (stream->app == "sort"){
// 		     vector<int> mappedInts;
// 		     mappedInts = mapInts(vInput, vInputIndexes);
// 		  }

		  
// 		  break;
		  
		  

// cout << "child #" << i << " pid: " << pid << endl;
// 		  cout << "\t>>>>>>>>> \t child Start: " << start << " child End: " << end << endl;
// 		  unsigned int j = start;
// 		  unsigned int k = end;
// 		  while (j < k+1){
// 		    cout << j << ") " << (*sMemoryPtr)[j] << endl;
// 		    j++;
// 		   }
		  

// 		}
// 		// parent process
// 		if (pid > 0){
// 		  wait(&status);
// 		  cout << "parent pid: " << pid << endl;
// 		}
// 		else if (pid < 0){
// 		  cout << ("ERROR, fork failed") << endl;;
// 		  exit(0);			      
// 		}
// 	}
	      
// 	      else if (stream->impl == "threads"){	
// 	            // Start thread
// 	            pthread_t newThread;
// 		    int retval = pthread_create(&newThread, NULL, runMapWords, (void*)inStruct);    
// 		    if(!retval)
// 	                  exIDs.push_back(newThread);
// 		    else
// 	                  cout << "ERROR, PTHREAD " << stream->num_maps << " NOT CREATED" << endl;
// 	      }
// 	  }
// 	  if (stream->impl == "threads"){	
// 	        for(int i = 0; i < stream->num_maps; i++){
// 		      pthread_join(exIDs[i], NULL);
// 		}
// 	  }
}



vector<string>* createSharedMemory(vector<string> vStrings){
        int shmid;
	key_t key;
	key = 9876;
	
	shmid = shmget(key, sizeof(vStrings), IPC_CREAT | 0666);
	if (shmid < 0){
	        cout << ("ERROR, Unable to create shared memory") << endl;;
		exit(0);
	}
	
	//  vector pointing to the shared memory
	vector<string>* sMemoryPtr;		
	sMemoryPtr = (vector<string>*)shmat (shmid, NULL, 0);
	
	// ways to add to shared memory: 
	// sMemoryPtr = &vInput;
	// sMemoryPtr = new(shm) vector<string>;
	// *sMemoryPtr = vInput;
	
	// print out shared memory
	// 		  unsigned int j = start;
	// 		  unsigned int k = end;
	// 		  while (j < k+1){
	// 		    cout << j << ") " << (*sMemoryPtr)[j] << endl;
	// 		    j++;
	// 		   }

	cout << "Memory created Successfull" << endl;
	// return pointer to shared memory
	return sMemoryPtr;
}

void createProcesses(executionStream *stream, vector<string> vStrings, vector<pair <int, int> > vIndexes, vector<string>* sMemoryPtr){
        // paralelised MapReduce, Shuffle and Combine
        for(int i = 0; i < stream->num_maps; i++){

	  // process creation
	        pid_t pid = fork();
		int status = 0;
		
			// child process
		if (pid == 0) {
		  // call Map
		  if (stream->app == "wordcount")      
		  vector<pair <string, int> > mappedWords = mapWords(vStrings, vIndexes[i].first, vIndexes[i].second);
		  else if (stream->app == "sort")
		    vector<int> mappedInts = mapInts(vStrings, vIndexes[i].first, vIndexes[i].second);
			// after map complete: add mappedWords to shared memory
			// When everything in shared memory: call Shuffle
			// after shuffle complete: call Reduce
			// after reduce complete: call Combine
		}
		// parent process
		else if (pid > 0){
		        // wait for all processes to finish combining and then return to main
		        wait(&status);
			cout << "parent pid: " << pid << endl;
			break;
		}
		else if (pid < 0){
		        cout << ("ERROR, fork failed") << endl;;
			exit(0);			      
		}
	}
}

void createThreads(executionStream *stream, vector<string> vStrings, vector<pair <int, int> > vIndexes, vector<string>* sMemoryPtr){
	for(int i = 0; i < stream->num_maps; i++){
	        InputStructData inStruct;
		inStruct.vPartition = vStrings;
		inStruct.vIndexes.push_back(vIndexes[i].first);
		inStruct.vIndexes.push_back(vIndexes[i].second);
		  
		pthread_t newThread;
		int retval = pthread_create(&newThread, NULL, runMapWords, (void*)&inStruct);    
	if(!retval)
		exIDs.push_back(newThread);
	else
	        cout << "ERROR, PTHREAD " << stream->num_maps << " NOT CREATED" << endl;

	}
	for(int i = 0; i < stream->num_maps; i++){
	        pthread_join(exIDs[i], NULL);
	}
}

void *runMapWords(void* input){
	
	//InputStructData *inStruct = ((InputStructData*)input);
	//mapWords(vStrings, inStruct->vInputIndexes);
        		        // call Map
  //vector<pair <string, int> > mappedWords = mapWords(vStrings, vIndexes[i].first, vIndexes[i].second);
			// after map complete: add mappedWords to shared memory
			// When everything in shared memory: call Shuffle
			// after shuffle complete: call Reduce
			// after reduce complete: call Combine

	return NULL;
}
