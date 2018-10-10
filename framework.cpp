#include "framework.h"
#include "mapreduce.h"

// Global variables
vector<pthread_t> exIDs;
//vector<string> vStrings;
mutex mutx;

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
	}
	else{
	  cout << "COULD NOT OPEN FILE";
	  exit(0);
	}
	return vStrings;
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
				currWord = currWord.substr(0, cLen-1);
				cLen--;

				if(cLen > 1 && (currWord[cLen-1] == '.' || currWord[cLen-1] == '?' 
						|| currWord[cLen-1] == '!' || currWord[cLen-1] == '\'')){
				  currWord[cLen-1] = '\0';
				  currWord = currWord.substr(0, cLen-1);
				}
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
	
	return vStrings;
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
}


template <class vectorType>
vector<pair <string, int> >* createSharedMemoryWords(vector<vectorType> vStrings){

        int shmid;
	key_t key;
	key = 9876;
	shmid = shmget(key, sizeof(vStrings), IPC_CREAT | 0666);
	if (shmid < 0){
	        cout << ("ERROR, Unable to create shared memory") << endl;;
		exit(0);
	}
	
	//  vector pointing to the shared memory
	vector<pair <string, int> >* sMemoryPtr;		

	sMemoryPtr = (vector<pair <string, int> >*)shmat (shmid, NULL, 0);

	//cout << "Memory created Successfull" << endl;
	// return pointer to shared memory
	return sMemoryPtr;
}
//template <class vectorType>
vector<string>* createSharedMemoryInts(vector<string> vStrings){
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
	
	// return pointer to shared memory
	return sMemoryPtr;
}

int turn = 0;

void createProcesses(executionStream *stream, vector<string> vStrings, vector<pair <int, int> > vIndexes, vector<pair <string, int> > *returnVectorWords, vector<string> *returnVectorInts){
  
          vector<pair <string, int> >* sMemoryPtr = createSharedMemoryWords(vStrings);
	  vector<pair <string, int> > finalVectorWords;
	  vector<string>* sIntMemoryPtr = createSharedMemoryInts(vStrings);
	  vector<string> *finalVectorInts = NULL;
	  //int partitionNumber = 0;

	if (stream->app == "wordcount")
	  sIntMemoryPtr = NULL;
	else if (stream->app == "sort")
	    sMemoryPtr = NULL;
	int flag = 1;
	//int status;
	vector<pair <string, int> > tempVector;
	vector<string> tempIntVector;
	// paralelised MapReduce, Shuffle and Combine
        for(int i = 0; i < stream->num_maps; i++){
	       //partitionNumber = i;
	        // process creation
	        pid_t pid = fork();
		int status;
		
		// child process
		if (pid == 0) {
		  // call Map
		  if (stream->app == "wordcount") {

		    // run Map
		    vector<pair <string, int> > mappedWords = 
		      mapWords(vStrings, vIndexes[i].first, vIndexes[i].second);
		    
		    // get from shared memory
		    //while(turn != partitionNumber){
			//}
		    if (flag != 1)
		      tempVector = *sMemoryPtr;
		    
		    // create shared memory pointer
		    vector<pair <string, int> >* shm = new (sMemoryPtr) vector<pair <string, int> >;
		    // push mappedWords into a tempVector to assign to shared memory (avoid writing straight to shared memory)
		    for (unsigned int i = 0; i < mappedWords.size(); i++){
		      tempVector.push_back(make_pair(mappedWords[i].first, mappedWords[i].second));
		    }

		    // write temp Mapped vector back to shared memory vector
		    (*shm) = tempVector;
		    flag = 0;
		    
		    (*finalVectorInts).insert((*finalVectorInts).end(), (*sIntMemoryPtr).begin(), (*sIntMemoryPtr).end());
		    
		    sMemoryPtr = createSharedMemoryWords(tempVector);
			cout << "t1" << endl;
		    // call Shuffle on a partition but with access to full shared memory
		    
		    //shuffleWords(stream, sMemoryPtr, vIndexes[i].first, vIndexes[i].second, partitionNumber);

		    // finalVectorWors here if we wanted to add all shared memory to a vector
		  }
		  else if (stream->app == "sort"){
		          // run Map
		          vector<string> mappedInts = 
			    mapInts(vStrings, vIndexes[i].first, vIndexes[i].second);
			  // get from shared memory
			  //while(turn != partitionNumber){
				//}
			  if (flag != 1){
			          mutx.lock();		
			          tempIntVector = *sIntMemoryPtr;
			  }
			  
			  // create shared memory pointer
			  vector<string>* shm = new (sIntMemoryPtr) vector<string>;
			  
			  // push mappedInts into a tempVector to assign to shared memory (avoid writing straight to shared memory)
			  for (unsigned int i = 0; i < mappedInts.size(); i++){
			    tempIntVector.push_back(mappedInts[i]);
			  }

			  // push mapped data to shared memory
			  (*shm) = tempIntVector;			  
			  mutx.unlock();
			    
			  //sMemoryPtr = createSharedMemoryWords(tempIntVector);
			  flag = 0;  
			  
			  sIntMemoryPtr = createSharedMemoryInts(tempIntVector);
			  cout << "t2" << endl;
			 
			  
			  // call Shuffle on a partition but with access to full shared memory
			  //shuffleInts(stream, sIntMemoryPtr, vIndexes[i].first, vIndexes[i].second, partitionNumber);

			  // finalVectorWors here if we wanted to add all shared memory to a vector
		  }
		  turn++;
		  cout << "turn++" << endl;
		  //break;
		}
		
		// parent process
		else if (pid > 0){
		  
		  // wait for all processes to finish combining and then return to main
		  wait(&status);
		  //shuffleWords(stream, sMemoryPtr, vIndexes[0].first, vIndexes[0].second, partitionNumber);
		  break;
		}
		// process failed
		else if (pid < 0){
		        cout << ("ERROR, fork failed") << endl;
			exit(0);			      
		}	
		
		    
	}
	
	for (unsigned int i = 0; i < (*finalVectorInts).size(); i++){
			   cout << (*finalVectorInts)[i] << endl;
	}
	// print out shared memory
	// cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\t>>>>>>>>>>>>>>" << endl;
	// for (unsigned int i = 0; i < (*sIntMemoryPtr).size(); i++){
	//   cout << (*sIntMemoryPtr)[i] << endl;
	// }
	
	
	// copies vector to main (returnsVectorWords goes to main.cpp) 
	// LEAVE COMMENTED OUT OR IT WILL OVERFLOW
	// if (stream->app == "wordcount"){
	//   // attempt to sort the finalVector with all the data (doesn't work)
	//   sort(finalVectorWords.begin(), finalVectorWords.end());
	//   *returnVectorWords = finalVectorWords;
	// }
	// else if (stream->app == "sort"){
	//   //sort(finalVectorInts.begin(), finalVectorInts.end());
	//   finalVectorInts = *sIntMemoryPtr;
	//   *returnVectorInts = finalVectorInts;
	// }
	
	//shuffleWords(stream, sMemoryPtr, vIndexes[0].first, vIndexes[0].second, partitionNumber);
	//shuffleInts(stream, sIntMemoryPtr, vIndexes[0].first, vIndexes[0].second, partitionNumber);
	
	//for(unsigned int i = 0; i < (*sMemoryPtr).size(); i++){
	//	cout << "first: " << (*sMemoryPtr)[i].first << " second: " << (*sMemoryPtr)[i].second << endl;
	//}
	
	//for(unsigned int i = 0; i < (*sIntMemoryPtr).size(); i++){
	//	cout << "Int: " << (*sIntMemoryPtr)[i] << endl;
	//}
}

void createThreads(executionStream *stream, vector<string> vStrings, vector<pair <int, int> > vIndexes, vector<pair <string, int> > *returnVectorWords, vector<string> *returnVectorInts){

        //vector<pair <string, int> > *finalVectorWords;
	//vector<int> *finalVectorInts;
	vector<pair <string, int> >* sMemoryPtr;
	vector<string>* sIntMemoryPtr;
	InputStructData *inStruct;
	
	
	for(int i = 0; i < stream->num_maps; i++){
		inStruct = new InputStructData;
		inStruct->vPartition = vStrings;
		inStruct->vIndexes.push_back(vIndexes[i].first);
		inStruct->vIndexes.push_back(vIndexes[i].second);
		inStruct->flag = i;
		inStruct->execStream = stream;
		inStruct->partitionNumber = i;
		
		pthread_t newThread;
		int retval;
		if (stream->app == "wordcount"){
		        // create shared memory before creating threads
		        sMemoryPtr = createSharedMemoryWords(vStrings);
			inStruct->sMemoryPtr = sMemoryPtr;
			// create threads
			retval = pthread_create(&newThread, NULL, runMapWords, (void*)inStruct);
			// assign shared memory data to vector to try to call sort on it
			//finalVectorWords = *sMemoryPtr;
		}
		else if (stream->app == "sort"){
		        // create shared memory before creating threads
		        sIntMemoryPtr = createSharedMemoryInts(vStrings);
			inStruct->sIntMemoryPtr = sIntMemoryPtr;
			// create threads
			retval = pthread_create(&newThread, NULL, runMapInts, (void*)inStruct);
			// assign shared memory data to vector to try to call sort on it
			//finalVectorInts = *sIntMemoryPtr;
		}
	if(!retval)
		exIDs.push_back(newThread);
	else
	        cout << "ERROR, PTHREAD " << stream->num_maps << " NOT CREATED" << endl;

	}
	for(int i = 0; i < stream->num_maps; i++){
	        pthread_join(exIDs[i], NULL);
	}
	
	//shuffleWords(inStruct->execStream, sMemoryPtr, inStruct->vIndexes[0], inStruct->vIndexes[1], inStruct->partitionNumber);
	//shuffleInts(inStruct->execStream, sIntMemoryPtr, inStruct->vIndexes[0], inStruct->vIndexes[1], inStruct->partitionNumber);
	
	for(unsigned int i = 0; i < (*sMemoryPtr).size(); i++){
		cout << "first: " << (*sMemoryPtr)[i].first << " second: " << (*sMemoryPtr)[i].second << endl;
	}
	
	for(unsigned int i = 0; i < (*sIntMemoryPtr).size(); i++){
		cout << "Int: " << (*sIntMemoryPtr)[i] << endl;
	}
	
	// attempt to run shuffle after all threads finished
	// SHUFFLE CAUSING OVERFLOW HERE IN THREADS
	// shuffleInts(inStruct->execStream, inStruct->sIntMemoryPtr, inStruct->vIndexes[0], inStruct->vIndexes[1], inStruct->partitionNumber);
	
	// copies vector to main (returnsVectorWords goes to main.cpp)
	// LEAVE COMMENTED OUT OR IT WILL OVERFLOW
	if (stream->app == "wordcount"){
		shuffleWords(inStruct->execStream, sMemoryPtr, inStruct->vIndexes[0], inStruct->vIndexes[1], inStruct->partitionNumber);
	  	*returnVectorWords = *sMemoryPtr;
	}
	else if (stream->app == "sort"){
		shuffleInts(inStruct->execStream, sIntMemoryPtr, inStruct->vIndexes[0], inStruct->vIndexes[1], inStruct->partitionNumber);
		*returnVectorInts = *sIntMemoryPtr;
	}
}

void *runMapWords(void* input){
        vector<pair <string, int> > tempVector;
	InputStructData *inStruct = ((InputStructData*)input);
	
	// run Map
	vector<pair <string, int> > mappedWords = mapWords(inStruct->vPartition, inStruct->vIndexes[0], inStruct->vIndexes[1]);
	
	while(turn != inStruct->partitionNumber){
	}
	if(inStruct->flag != 0){
	        mutx.lock();		
	        tempVector = (*inStruct->sMemoryPtr);
	}
	// create shared memory pointer
	vector<pair <string, int> >* shm = new (inStruct->sMemoryPtr) vector<pair <string, int> >;
		    
	// push mappedWords into a tempVector to assign to shared memory (avoid writing straight to shared memory)
	for (unsigned int i = 0; i < mappedWords.size(); i++){
	         tempVector.push_back(make_pair(mappedWords[i].first, mappedWords[i].second));
	}
	
	// push mapped data to shared memory
	(*shm) = tempVector;

	// attempt to reallocate shared memory size
	inStruct->sMemoryPtr = createSharedMemoryWords(tempVector);

	mutx.unlock();
	turn++;
	
	return NULL;
}

void *runMapInts(void* input){
	vector<string> tempIntVector;
	InputStructData *inStruct = ((InputStructData*)input);

	// run Map
	vector<string> mappedInts = mapInts(inStruct->vPartition, inStruct->vIndexes[0], inStruct->vIndexes[1]);
	
	while(turn != inStruct->partitionNumber){
	}
	if (inStruct->flag != 0){
	        mutx.lock();
	        tempIntVector = (*inStruct->sIntMemoryPtr);
	}
	
	// create shared memory pointer
	vector<string>* shm = new (inStruct->sIntMemoryPtr) vector<string>;
	
	// push mappedWords into a tempVector to assign to shared memory (avoid writing straight to shared memory)
	for (unsigned int i = 0; i < mappedInts.size(); i++){
	        tempIntVector.push_back(mappedInts[i]);
	}

	// push mapped data to shared memory
	(*shm) = tempIntVector;

	inStruct->sIntMemoryPtr = createSharedMemoryInts(tempIntVector);
	mutx.unlock();
	turn++;
	
	// call Shuffle on a partition but with access to full shared memory
	// SHUFFLE CAUSING OVERLOW HERE IN THREADS
	//shuffleInts(inStruct->execStream, inStruct->sIntMemoryPtr, inStruct->vIndexes[0], inStruct->vIndexes[1], inStruct->partitionNumber);
	
	
	return NULL;
}
// Print Shared Memory
//cout << ">>>>>>>>>>>>>>>>\t\tsMemorySize " << (*sMemoryPtr).size() << endl;
// for (unsigned int i = 0; i < (*sMemoryPtr).size(); i++){
//   cout << "sMemPtr is: " << (*sMemoryPtr)[i].first << " " << (*sMemoryPtr)[i].second << endl;
// }
		    
void shuffleWords(executionStream* stream, vector<pair <string, int> >* sMemoryPtr, int memoryStart, int memoryEnd, int partitionNumber){
       // skip this part
        vector<pair <int, int> > memoryIndexes;
	vector<pair <int, int> > alphabetIndexes;
	int sMemorySize = (*sMemoryPtr).size();
  	int splitFactor  = sMemorySize/(stream->num_reduces);;
  	int remainder = sMemorySize % stream->num_reduces;

        string alphabet = "abcdefghijklmnopqrstuvwxyz";
	int alphabetSplitFactor = 26 / stream->num_reduces;
	int alphabetRemainder = 26 % stream->num_reduces;
	
	vector<int> sMemoryPartitions;
	vector<int> alphabetPartitions;
  	int startInd, endInd, startAlph, endAlph;
	
  	startInd = 0;
  	endInd = splitFactor;

	startAlph = 0;
	endAlph = alphabetSplitFactor;

  	for(int i = 0; i < stream->num_reduces; i++){
  		if(i == (stream->num_maps-1)){
  		  sMemoryPartitions.push_back(splitFactor + remainder);
		  alphabetPartitions.push_back(alphabetSplitFactor + alphabetRemainder);			
  		}
  		else{
  		  sMemoryPartitions.push_back(splitFactor);
		  alphabetPartitions.push_back(alphabetSplitFactor);			
  		}
		//cout << "sMemoryPartitions: " << sMemoryPartitions[i] << endl;
  	}
	
  	// Fills in input structs and starts threads
	
  	for(int i = 0; i < stream->num_reduces; i++){
  	      vector<int> sPartitionIndexes;
	      vector<int> alphabetPartitionIndexes;
	      
  	      sPartitionIndexes.push_back(startInd);
	      alphabetPartitionIndexes.push_back(startAlph);

  	      endInd = startInd + (sMemoryPartitions[i] - 1);
	      endAlph = startAlph + (alphabetPartitions[i] - 1);
	      
  	      sPartitionIndexes.push_back(endInd);
	      alphabetPartitionIndexes.push_back(endAlph);

  	      startInd = endInd+1;
	      startAlph = endAlph+1;

	      memoryIndexes.push_back(make_pair(sPartitionIndexes[0],sPartitionIndexes[1]));	  
	      alphabetIndexes.push_back(make_pair(alphabetPartitionIndexes[0],alphabetPartitionIndexes[1]));	  
	}

	// Everything until here was for the partitioning

	mutx.lock();
	vector<pair <string, int> > tempVector;
	
	// grab from shared memory
	tempVector = *sMemoryPtr;
	
	// create shared memory pointer,pointing to beginning of shared memory
	vector<pair <string, int> >* shm = new (sMemoryPtr) vector<pair <string, int> >;
	
	// run sort on tempVector holding all shared memory data
	sort(tempVector.begin(), tempVector.end());
	
	// push sorted data back to shared memory (sorted as in duplicates together)
	(*shm) = tempVector;
	//(*shm).insert((*shm).begin(), tempVector.begin(), tempVector.end());

	mutx.unlock();
	
	// print shared memory data
	// cout << "------------------------\t-------------------" << endl;
	// for( unsigned int i = 0; i < (*sMemoryPtr).size(); i++){
	//   cout << (*sMemoryPtr)[i].first << " " <<  (*sMemoryPtr)[i].second << endl;
	// }
	

	// IGNORE UNTIL END OF FUNCTION
	//int lastString = alphabetIndexes[partitionNumber].second;
	// cout << "alphaet: " << alphabetIndexes[partitionNumber].second << endl;
	//for( unsigned int i = 0; i < (*sMemoryPtr).size(); i++){
	  //if (((*sMemoryPtr)[i].first[0]).compare(alphabet[lastString]) < 0){
	  // char memoryChar = (*sMemoryPtr)[i].first[0];
	  // char alphabetChar = alphabet[lastString];
	//cout << (*sMemoryPtr)[i].first << " " <<  (*sMemoryPtr)[i].second << endl;
	  // if (memoryChar == alphabetChar){
	  //   cout << "no" << endl;
	  // }
	  // else {
	  //   cout << "yes" << endl;
	  // }
	//}
	
	// for( unsigned int i = 0; i < (*sMemoryPtr).size(); i++){
	//   ((*sMemoryPtr)[i].first[0]
	// }
	

	// int lastString = alphabetIndexes[partitionNumber].second;
	// cout << "lastString " << lastString << endl;
	// for( unsigned int i = 0; i < (*sMemoryPtr).size(); i++){
	//   int k = i;
	//   if (k >= memoryStart && k <= memoryEnd)
	//     continue;
	  
	//   //else if (((*sMemoryPtr)[i].first).compare(alphabet[lastString]) < 0)
	//     // insert into my partition
	//     //cout << (*sMemoryPtr)[i].first << " < " << alphabet[lastString] << endl;
	  
	// }
	
	//cout << "memory partitions: " << memoryIndexes[0].first << " " << memoryIndexes[0].second << endl;
	//cout << "alphabet partitions: " << alphabetIndexes[1].first << " " << alphabetIndexes[1].second << endl;
	
	// cout << "lengthL " << alphabet.length() << endl;
	// for (unsigned int i = 0; i < (*sMemoryPtr).size(); i++){
	//   cout << (*sMemoryPtr)[i].first << " " << (*sMemoryPtr)[i].second << endl;
	// }
}
void shuffleInts(executionStream* stream, vector<string>* sIntMemoryPtr, int memoryStart, int memoryEnd, int partitionNumber){
        // skip this part
        vector<pair <int, int> > memoryIndexes;
	int sIntMemorySize = (*sIntMemoryPtr).size();
  	int splitFactor  = sIntMemorySize / (stream->num_reduces);;
  	int remainder = sIntMemorySize % stream->num_reduces;
	
	vector<int> sIntMemoryPartitions;
  	int startInd, endInd;
  	startInd = 0;
  	endInd = splitFactor;
	
  	for(int i = 0; i < stream->num_reduces; i++){
  		if(i == (stream->num_maps-1)){
  		  sIntMemoryPartitions.push_back(splitFactor + remainder);
  		}
  		else{
  		  sIntMemoryPartitions.push_back(splitFactor);
	        
  		}
		//cout << "sIntMemoryPartitions: " << sIntMemoryPartitions[i] << endl;
  	}
	
  	// Fills in input structs and starts threads
	
  	for(int i = 0; i < stream->num_reduces; i++){
  	      vector<int> sPartitionIndexes;
  	      sPartitionIndexes.push_back(startInd);
  	      endInd = startInd + (sIntMemoryPartitions[i] - 1);
  	      sPartitionIndexes.push_back(endInd);
  	      startInd = endInd+1;
	      
	      memoryIndexes.push_back(make_pair(sPartitionIndexes[0],sPartitionIndexes[1]));
	}
	
	// Everything until here was for the partitioning
      	mutx.lock();
	vector<string> tempVector;
	
	// grab from shared memory
	tempVector = *sIntMemoryPtr;
	
	vector<int> temp1Vector;
	for(unsigned int i = 0; i < tempVector.size(); i++){
		temp1Vector.push_back(atoi(tempVector[i].c_str()));
	}
	
	
	sort(temp1Vector.begin(), temp1Vector.end());
	tempVector.clear();
	
	for(unsigned int i = 0; i < temp1Vector.size(); i++){
		tempVector.push_back(to_string(temp1Vector[i]));
	}
	// shared memory pointer, pointing to beginning of shared memory
	vector<string>* shm = new (sIntMemoryPtr) vector<string>;

	// run sort on tempVector holding all shared memory data
	//sort((*sIntMemoryPtr).begin(), (*sIntMemoryPtr).end());

	// push sorted data back to shared memory (sorted as in duplicates together)
	(*shm) = tempVector;
	
	mutx.unlock();
	
	// print shared memory data
	// for (unsigned int i = 0; i < (*sIntMemoryPtr).size(); i++){
	//   cout << (*sIntMemoryPtr)[i] << endl;
	// }
}

vector<int> combineInts(vector<vector <int> > vInts){
	
	vector<int> newVector;
	
	for(unsigned int i = 0; i < vInts.size(); i++){
		
		for(unsigned int j = 0; j < vInts[i].size(); j++){
		
			newVector.push_back(vInts[i][j]);
		
		}
		
	}
	return newVector;
	
}

vector<pair <int, int> > splitReduce(executionStream *stream, vector<string> vInput){
        vector<pair <int, int> > indexes;
	int splitFactor;
	int vSize;
	
	vSize = vInput.size();

	splitFactor = vSize/(stream->num_reduces);
	int remainder = vSize % stream->num_reduces;
	
	vector<int> vInputSizes;
	int startInd, endInd;
	startInd = 0;
	endInd = splitFactor;
	
	for(int i = 0; i < stream->num_reduces; i++){
		if(i == (stream->num_reduces-1)){
		  vInputSizes.push_back(splitFactor + remainder);			
		}
		else{
		  vInputSizes.push_back(splitFactor);
	        
		}
	}
	// Fills in input structs and starts threads
	
	for(int i = 0; i < stream->num_reduces; i++){
	      vector<int> vInputIndexes;
	      vInputIndexes.push_back(startInd);
	      endInd = startInd + (vInputSizes[i] - 1);
	      vInputIndexes.push_back(endInd);
	      startInd = endInd+1;

	      indexes.push_back(make_pair(vInputIndexes[0],vInputIndexes[1]));	      
	}
	return indexes;
}
