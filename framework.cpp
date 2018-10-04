#include "framework.h"
#include <stdio.h>

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

void readInputWords(const char* file){

	// Open file to read
        ifstream readFile;
	//readFile.open((stream->app).c_str());
	readFile.open(file);
	//readFile.open("word_input.txt");
	// Initialize vector object for storing words
	vector<string> vWords;
	string currWord;
	
	if(readFile.is_open()){
	  
	        // Take first word to be read in from file
	        readFile >> currWord;
	
		// Read file word by word
		while(!readFile.eof()){
	
		        // Change current word to all lower case letters
		        transform(currWord.begin(), currWord.end(), currWord.begin(), ::tolower);
		
			// Checks for commas, periods etc and removes them
			int cLen = currWord.length();
			if(currWord[cLen-1] == '.' || currWord[cLen-1] == ',' || currWord[cLen-1] == '!'
			   || currWord[cLen-1] == ';' || currWord[cLen-1] == '?' || currWord[cLen-1] == ':'
			   || currWord[cLen-1] == ')' || currWord[cLen-1] == '\''){
				  
			        // NEED TO FIX "control.'"
				        
			        currWord[cLen-1] = '\0';
			        
				if(currWord[cLen-2] == '.' || currWord[cLen-1] == '?' || currWord[cLen-1] == '!'
					|| currWord[cLen-1] == '\'')
				  currWord[cLen-2] = '\0';
			}
			
			// Additional check for non-alphabetic words
			if(currWord == "—"){
			        readFile >> currWord;
				continue;
			}
		
			// If a word contains a hyphen, split them
			unsigned int hPos;
			hPos = currWord.find('-');
			if(currWord.find('-') != string::npos){
				string tempStr1, tempStr2;
				tempStr1 = currWord.substr(0, hPos);
				tempStr2 = currWord.substr(hPos+1, currWord.length());
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
	
	}
	
	// If file is not open, print error
	else{
	        cout << "COULD NOT OPEN FILE";
	}
	
	/*
	int key = 99999;
	int seg_id;
	seg_id = shmget(key, sizeof(vWords), IPC_CREAT | 0666);
	
	if(seg_id == -1)
		cout << "ERROR, COULD NOT SAVE VECTOR ON SHARED MEMORY";
	*/
	
	printVector(vWords);

}

// Print all words in vector vWords
void printVector(vector<string> vector){
	for(unsigned int i = 0; i < vector.size(); i++){
	  cout << vector[i] << "\n";
	}
}

