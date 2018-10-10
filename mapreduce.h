#ifndef MAPREDUCE_H
#define MAPREDUCE_H

#include "framework.h"
#include <algorithm>


vector<pair <string, int> > mapWords(vector<string>, int, int);

vector<string> mapInts(vector<string>, int, int);

vector<pair <string, int> > reduceWords(vector<vector <pair <string, int > > >);

vector<int> reduceInts(vector<string>, int, int);

#endif
