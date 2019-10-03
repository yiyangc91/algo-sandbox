#include <string>
#include <limits>
#include "base.h"

ALGO(Reference, needle, haystack) {
  return haystack.find(needle, 0);
}
