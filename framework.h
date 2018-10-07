#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

struct executionStream {
  string app;
  string impl;
  int num_maps;
  int num_reduces;
  string inputFile;
  string outputFile;
};

struct InputStructData {
	vector<string> vInput;
	vector<int> vInputIndexes;
};

struct partitionIndexes {
  vector<pair <int, int> > indexes;
};

// iterates through all the flags to verify their validity
int getFlag(char**, executionStream*);

vector<string> readInputWords(executionStream*);

vector<string> readInputInts(executionStream*);

template <class inputType>
void printVector(vector<inputType>);

vector<pair <int, int> > split(executionStream*, vector<string> vInput);


void *runMapWords(void*);
// shuffle

// combine




#endif
