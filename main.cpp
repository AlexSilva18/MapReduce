#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>


using namespace std;

void printVector(vector<string>);

int main(int argc, char* argv[]){

	// Open file to read
	ifstream readFile;
	readFile.open("word_input.txt");
	
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
				if(currWord[cLen-2] == '.' || currWord[cLen-1] == '?' || currWord[cLen-1] == '!')
					currWord[cLen-2] = '\0';
			}
			
			// Additional check for non-alphabetic words
			if(currWord == "—"){
				readFile >> currWord;
				continue;
			}
			
			// If a word contains a hyphen, split them
			int hPos;
			hPos = currWord.find('-');
			if(hPos != string::npos){
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
	
	printVector(vWords);
}

// Print all words in vector vWords
void printVector(vector<string> vector){
	for(unsigned int i = 0; i < vector.size(); i++){
		cout << vector[i] << "\n";
	}
}

int mapWords(){

}

int reduceWords(){

}

int mapInts(){

}

int reduceInts(){

}
