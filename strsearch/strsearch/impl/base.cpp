#include "base.h"

::BaseSearch::~BaseSearch() {}

SearchSingleton* getInstance() {
   static SearchSingleton instance;
   return &instance;
}

void SearchSingleton::addInstance(const std::string& s, BaseSearch* inst) {
   instances.insert({s, inst});
}
