#ifndef MAPREDUCE_H
#define MAPREDUCE_H

#include "framework.h"
#include <algorithm>


vector<pair <string, int> > mapWords(vector<string>, vector<int>);

int reduceWords();

vector<int> mapInts();

int reduceInts();

#endif
