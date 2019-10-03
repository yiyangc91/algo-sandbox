#ifndef TESTGEN_H
#define TESTGEN_H

#include <string>
#include <utility>
#include <random>

template <typename E = std::minstd_rand0>
class TestGen
{
   std::string::size_type needleLen;
   std::string::size_type haystackLen;
   E engine;

public:
   TestGen(long seed, std::string::size_type needleLen, std::string::size_type haystackLen);

   std::pair<std::string, std::string> generateStringSequences();
};


template <typename E> TestGen<E>::TestGen(long seed, std::string::size_type needleLen, std::string::size_type haystackLen)
   : engine{E(seed)}, needleLen{needleLen}, haystackLen{haystackLen}
{
}


template <typename E> std::pair<std::string, std::string> TestGen<E>::generateStringSequences() {
   std::uniform_int_distribution<> dis(0, 25);
   std::pair<std::string, std::string> result;
   result.first.reserve(needleLen);
   result.second.reserve(haystackLen);

   for (int i = 0; i < needleLen; ++i) {
      char c { static_cast<char>(dis(this->engine)) };
      result.first.push_back(c+'a');
   }
   for (int i = 0; i < haystackLen; ++i) {
      char c { static_cast<char>(dis(this->engine)) };
      result.second.push_back(c+'a');
   }

   return result;
}

#endif // TESTGEN_H
