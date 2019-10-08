#ifndef ALLOC_H
#define ALLOC_H

#include <vector>

using AllocFunc = int (*)(std::vector<int>::const_iterator, std::vector<int>::const_iterator, int);

int backtrackShortest(std::vector<int>::const_iterator begin, std::vector<int>::const_iterator end, int numWorkers);
int bruteForceShortest(std::vector<int>::const_iterator begin, std::vector<int>::const_iterator end, int numWorkers);

#endif // ALLOC_H
