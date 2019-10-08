#include <algorithm>
#include <limits>
#include <vector>
#include "alloc.h"

using namespace std;

namespace {
   int allocateTask(vector<int>::const_iterator begin, vector<int>::const_iterator end, vector<int>& workers) {
      if (begin == end) {
         return *max_element(workers.begin(), workers.end());
      }

      int min = numeric_limits<int>::max();
      for (int i = 0; i < workers.size(); ++i) {
         workers[i] += *begin;
         int result = allocateTask(begin+1, end, workers);
         if (result < min) {
            min = result;
         }
         workers[i] -= *begin;
      }
      return min;
   }
}

int bruteForceShortest(vector<int>::const_iterator begin, vector<int>::const_iterator end, int numWorkers) {
   if (!numWorkers) return numeric_limits<int>::max();
   vector<int> workers (numWorkers);
   return allocateTask(begin, end, workers);
}
