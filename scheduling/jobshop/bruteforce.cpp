#include <algorithm>
#include <limits>
#include <vector>
#include "alloc.h"

using namespace std;

namespace {
   int allocateTask(int taskNum, vector<int>& tasks, vector<int>& workers) {
      if (taskNum == tasks.size()) {
         return *max_element(workers.begin(), workers.end());
      }

      int min = numeric_limits<int>::max();
      for (int i = 0; i < workers.size(); ++i) {
         workers[i] += tasks[taskNum];
         int result = allocateTask(taskNum+1, tasks, workers);
         if (result < min) {
            min = result;
         }
         workers[i] -= tasks[taskNum];
      }
      return min;
   }
}

int bruteForceShortest(vector<int> tasks, int numWorkers) {
   vector<int> workers (numWorkers);
   return allocateTask(0, tasks, workers);
}
