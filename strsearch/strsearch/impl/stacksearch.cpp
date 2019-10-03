#include <list>
#include "base.h"

/**
 * StackSearch:
 * - Scans through the string, maintaining multiple lists of "matched" strings.
 * - This algorithm is as slow as a dog due to the insane amount of time it spends allocating memory
 */
using namespace std;

ALGO(StackSearch, needle, haystack) {
   list<pair<string::size_type, string::size_type>> searches;

   if (needle.size() == 0) return 0;

   for (string::size_type i = 0; i < haystack.length(); ++i) {
      char c { haystack[i] };

      // Advance the search
      auto searchIterator = searches.begin();
      while (searchIterator != searches.end()) {
         if (needle[searchIterator->second+1] == c) {
            ++(searchIterator->second);
            ++searchIterator;
         } else {
            searchIterator = searches.erase(searchIterator);
         }
      }

      if (c == needle[0]) {
         searches.emplace_back(i, 0);
      }

      for (auto& p : searches) {
         if (p.second == needle.length()-1) {
            return p.first;
         }
      }
   }

   return string::npos;
}
