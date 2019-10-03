#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "base.h"

using namespace std;

class Hash : public BaseSearch {
public:
   static constexpr auto polynomial = numeric_limits<unsigned char>::max();
protected:
   static unsigned int appendHash(unsigned int seed, char next) {
      return (seed * polynomial) + next;
   }
};

ALGO_F(Hash, Hash, needle, haystack) {
   if (needle.empty()) return 0;
   if (needle.size() > haystack.size()) return string::npos;

   // hash the needle
   unsigned int needleHash {};
   for (char c : needle) {
      needleHash = appendHash(needleHash, c);
   }

   unsigned int runningHash {};
   for (string::size_type i = 0; i < needle.size()-1; ++i) {
      runningHash = appendHash(runningHash, haystack[i]);
   }

   // fastpow to length-1
   string::size_type st = needle.size() - 1;
   unsigned int power { Hash::polynomial };
   unsigned int mult { 1 };
   for (string::size_type mask = 1; mask <= st && mask != 0; mask <<= 1) {
      if (st & mask) {
         mult *= power;
      }
      power *= power;
   }

   for (string::size_type i = needle.size()-1; i < haystack.size(); ++i) {
      runningHash = appendHash(runningHash, haystack[i]);

      if (runningHash == needleHash) {
         bool allMatch { true };
         auto needlePtr = needle.begin();
         auto haystackPtr = haystack.begin()+i-needle.size()+1;
         while (needlePtr != needle.end()) {
            if (*needlePtr != *haystackPtr) {
               allMatch = false;
               break;
            }
            ++needlePtr;
            ++haystackPtr;
         }
         if (allMatch) {
            return i-needle.size()+1;
         }
      }

      runningHash = runningHash - haystack[i-needle.size()+1] * mult;
   }

   if (!runningHash) {
      return 0;
   }

   return string::npos;
}
