#include <algorithm>
#include <limits>
#include <vector>

#include "alloc.h"

/*
 * So dumb brute force is dumb. There are several optimizations to be made:
 *
 * 1. When trying each worker, we can skip workers with the same current value (as adding
 *    the same value to each worker will result in well... the same result.
 * 2. Track the current minimum, and prune branches with workers greater than the minimum.
 * 3. Track the worker maximum so we don't need to calculate it.
 * 4. When trying the same character as the previous, we don't need to try previous positions
 * 5. Each worker idealling handles an equal share of load. If the current worker is already
 *    handling more than its fair share, then it means that at least another worker can
 *    be allocated more work (so we stop allocating work to the current worker).
 */

using namespace std;

class Solution {
   int div;
   const vector<int>::const_iterator tasksBegin;
   const vector<int>::const_iterator tasksEnd;

public:
   Solution(int div,
            const vector<int>::const_iterator tasksBegin,
            const vector<int>::const_iterator tasksEnd
            ): div{div}, tasksBegin{tasksBegin}, tasksEnd{tasksEnd} {}
   int allocateTask(vector<int>::const_iterator begin, vector<int>& workers, int bestMinimum, int curMax, int prevIdx) {
      if (begin == tasksEnd) {
         // OPTMIZATION 3: maximum is tracked so we can avoid calculating it
         return curMax;
      }

      // OPTIMIZATION 4: - for groups of numbers that are the same, smartly form combinations
      // (i.e. this is like writing "for i=0, j=i")
      int i = (begin != tasksBegin && *begin == *(begin-1)) ? prevIdx : 0;

      // OPTIMIZATION 1: avoid attempting the same worker values
      int prev = workers[i]-1; // something different
      for (; i < workers.size(); ++i) {
         auto& w = workers[i];

         if (w == prev) {
            // this only partially catches duplicates, but its more efficient than allocating a structure/sorting
            continue;
         }
         prev = w;

         // OPTIMIZATION 2/5:
         // - if this worker has already got its share of work
         // - or if adding work to this worker takes it over what we've seen as the minimum already
         // -> don't bother
         if (w < div && w + *begin < bestMinimum) {
            w += *begin;
            int curMin = allocateTask(begin+1, workers, bestMinimum, max(w, curMax), i);
            if (curMin < bestMinimum) {
               bestMinimum = curMin;
            }
            w -= *begin;
         }
      }

      return bestMinimum;
   }
};

int backtrackShortest(vector<int>::const_iterator begin, vector<int>::const_iterator end, int numWorkers) {
   if (!numWorkers) return numeric_limits<int>::max();

   vector<int> copy {begin, end};
   sort(copy.begin(), copy.end());

   int sum {};
   for (auto t : copy) {
      sum += t;
   }

   Solution s { (sum+numWorkers-1)/numWorkers, copy.begin(), copy.end() };
   vector<int> workers(numWorkers);

   return s.allocateTask(copy.begin(), workers, numeric_limits<int>::max(), 0, 0);
}
