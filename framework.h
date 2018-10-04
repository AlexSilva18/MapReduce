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
void getFlag(char* inputFlags[], executionStream *stream);



// split
void split(vector<string> vWords, string app, string impl);


// shuffle

// combine

void readInputWords(const char*);
void printVector(vector<string>);



#endif
