#include "base.h"

ALGO(Naive, needle, haystack) {
   if (haystack.size() == 0) {
      if (needle.size() == 0) return 0;
      return std::string::npos;
   }

   for (std::string::size_type i = 0; i < haystack.size()-needle.size()+1; ++i) {
      bool match = true;

      for (std::string::size_type j = 0; j < needle.size(); ++j) {
         if (needle[j] != haystack[i+j]) {
            match = false;
            break;
         }
      }

      if (match) {
         return i;
      }
   }

   return std::string::npos;
}
