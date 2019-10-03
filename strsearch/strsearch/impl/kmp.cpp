#include <string>
#include <vector>
#include "base.h"

/*
 * Implementation of Knuth-Morris-Pratt.
 *
 * The original idea and implementation is in presearch.cpp. That one used a array of indices
 * and it was a giant mess - the idea is the same as KMP, but the implementation is dodgy.
 * I came up with the algorithm and I had to make a bunch of modifications to fully cover the
 * edge cases.
 *
 * After writing down my algorithm, studying it and then formally studying KMP, I know now it
 * quite well enough to implement KMP (at least, pretty naively without crazy optimizations).
 */

using namespace std;

class KMP : public BaseSearch {
protected:
   static vector<string::size_type> calculateOffsets(const string& needle) {
      vector<string::size_type> offsets (needle.size());
      string::size_type match {};
      auto offPtr = offsets.begin()+1;
      for (auto ptr = needle.begin()+1; ptr != needle.end(); ++ptr) {
         if (*ptr == needle[match]) {
            *offPtr = offsets[match++];
         } else if (match) {
            *offPtr = match;

            // try find updated position of match
            do {
               match = offsets[match];
            } while (match && *ptr != needle[match]);

            if (match || *ptr == needle[0]) {
               ++match;
            }
         }
         ++offPtr;
      }
      return offsets;
   }

   static string::size_type findString(const vector<string::size_type>& offsets, const string& needle, const string& haystack) {
      // search string
      string::size_type match = 0;
      auto hs = haystack.begin();
      for (string::size_type i = 0; i < haystack.size(); ++i, ++hs) {
         if (needle[match] == *hs) {
            if (++match == needle.size()) {
               return i-match+1;
            }
         } else if (match) {
            // try find updated position of match
            do {
               match = offsets[match];
            } while (match && *hs != needle[match]);

            if (match || *hs == needle[0]) {
               ++match;
            }
         }
      }

      return string::npos;
   }
};

ALGO_F(KMP, KMP, needle, haystack) {
   if (needle.size() == 0) return 0;
   vector<string::size_type> offsets { calculateOffsets(needle) };
   return findString(offsets, needle, haystack);
}
