#include <vector>
#include "base.h"

/*
 * This algorithm searches for the needle inside the needle.
 *
 *   Needle:  catacarpcatatonic
 *   Overlap:     xxt xxxxc   x
 *
 * The idea here is that, if we get a character "incorrect", we should have a structure
 * which tells us if our incorrect character actually matches the some prefix of the needle.
 *
 * Apparently I just invented a weird version of Knuth-Morris-Pratt.
 *
 * I also believe that this algorithm is wrong somehow. I feel like I missed something but I haven't
 * caught it in testing yet.
 *
 */

using namespace std;

class PreSearch : public BaseSearch {
protected:
   static vector<string::size_type> searchPrefixes(const string& needle) {
      vector<string::size_type> result (needle.size());
      string::size_type firstMatch {};
      string::size_type matchPos {};
      for (string::size_type i = 1; i < needle.length(); ++i) {
         if (firstMatch) {
            if (needle[i] != needle[matchPos]) {
               // mismatch
               result[i] = firstMatch;

               // use our own information to pick a potentially smaller match
               // we "know" the continuing character at needle[i]
               bool foundSubStr {};
               while (result[matchPos]) {
                  string::size_type offset { matchPos - result[matchPos] };
                  matchPos -= offset;
                  firstMatch = i - offset;

                  if (needle[i] == needle[matchPos]) {
                     foundSubStr = true;
                     break;
                  }
               }

               if (foundSubStr) {
                  ++matchPos;
                  continue;
               }
            } else {
               // match, continue
               // copy any needle information as well for substrings
               if (result[matchPos]) {
                  string::size_type offset { matchPos - result[matchPos] };
                  result[i] = i - offset;
               }

               ++matchPos;
               continue;
            }
         }

         if (needle[i] == needle[0]) {
            // first match, record and do nothing
            firstMatch = i;
            matchPos = 1;
         }
      }
      return result;
   }

   static string::size_type searchWithNeedlePrefixes(const vector<string::size_type>& needlePrefixes, const string& needle, const string& haystack) {
      string::size_type matchPosition {};
      string::size_type initialMatch {};

      for (string::size_type i = 0; i < haystack.size(); ++i) {
         if (!matchPosition) {
            if (haystack[i] != needle[matchPosition]) {
               continue;
            }
            initialMatch = i;
            matchPosition = 1;
         } else {
            auto hs = haystack[i];

            if (hs == needle[matchPosition]) {
               ++matchPosition;
            } else {
               auto ptr = needlePrefixes[matchPosition];

               while (ptr && hs != needle[matchPosition-ptr]) {
                  matchPosition -= ptr;
                  initialMatch += ptr;
                  ptr = needlePrefixes[matchPosition];
               }

               if (ptr && hs == needle[matchPosition-ptr]) {
                  matchPosition -= ptr - 1;
                  initialMatch += ptr;
               } else if (hs == needle[0]) {
                  matchPosition = 1;
                  initialMatch = i;
               } else {
                  // nothing matches
                  matchPosition = 0;
                  continue;
               }
            }
         }

         if (matchPosition == needle.size()) {
            return initialMatch;
         }
      }

      return string::npos;
   }
};

ALGO_F(PreSearch, PreSearch, needle, haystack) {
   if (needle.size() == 0) return 0;

   vector<string::size_type> needlePrefixes { this->searchPrefixes(needle) };

   return searchWithNeedlePrefixes(needlePrefixes, needle, haystack);
}
