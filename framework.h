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
#include <mutex>
#include <sstream>

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
	vector<string> vPartition;
        vector<int> vIndexes;
        int flag;
        vector<pair <string, int> >* sMemoryPtr;
        vector<string>* sIntMemoryPtr;
        executionStream* execStream;
        int partitionNumber;
	int turn;
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

vector<pair <int, int> > split(executionStream*, vector<string>);

template <class vectorType>
vector<pair <string, int> >* createSharedMemoryWords(vector<vectorType>);

vector<string>* createSharedMemoryInts(vector<string>);

void createProcesses(executionStream*, vector<string>, vector<pair <int, int> >, vector<pair <string, int> >*, vector<string>*);

void createThreads(executionStream*, vector<string>, vector<pair <int, int> >, vector<pair <string, int> >*, vector<string>*);

void *runMapWords(void*);
void *runMapInts(void*);

void shuffleWords(executionStream*, vector<pair <string, int> >*, int, int, int);
void shuffleInts(executionStream*, vector<string>*, int, int, int);

vector<int> combineInts(vector<vector <int> >);

vector<pair <int, int> > splitReduce(executionStream *, vector<string>);


#endif
