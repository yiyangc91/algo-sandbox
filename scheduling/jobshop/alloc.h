#ifndef ALLOC_H
#define ALLOC_H

#include <vector>

using AllocFunc = int (*)(std::vector<int> , int);

int backtrackShortest(std::vector<int> tasks, int numWorkers);
int bruteForceShortest(std::vector<int> tasks, int numWorkers);

#endif // ALLOC_H
