#ifndef MAPREDUCE_H
#define MAPREDUCE_H

#include "framework.h"
#include <algorithm>


vector<pair <string, int> > mapWords(vector<string>, int, int);

int reduceWords();

vector<int> mapInts(vector<string>, int, int);

int reduceInts();

#endif
