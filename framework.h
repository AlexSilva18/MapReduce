#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

struct executionStream {
  string app;
  string impl;
  int num_maps;
  int num_reduces;
  string inputFile;
  string outputFile;
};

// iterates through all the flags to verify their validity
void getFlag(char**, executionStream*);

void readInputWords(executionStream*);

void readInputInts(executionStream*);

template <class inputType>
void printVector(vector<inputType>);

// split
template <class inputType>
void split(vector<inputType>, int);



// shuffle

// combine




#endif
