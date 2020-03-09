#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <variant>
#include <functional>
#include "node.h"

class Solver {
public:
   virtual ~Solver() = default;

   virtual void solve(
         std::function<void(const Node*)>) = 0;
};

// This lets me test different implementations

class TreeSolver : public Solver {
   std::vector<int> numbers;
   int target;

   // preallocated vectors for our search to avoid repeat
   // memory allocations while permuting
   std::vector<
      std::vector<std::variant<OpRaw, Node>>
   > preallocated;

   void permute(unsigned int length,
         std::function<void(const Node*)>);
   void generateOps(
         std::variant<OpRaw, Node>& left,
         std::variant<OpRaw, Node>& right,
         std::vector<std::variant<OpRaw, Node>>& out,
         unsigned int length,
         std::function<void(void)> fillRemainder,
         std::function<void(const Node*)> f);

public:
   TreeSolver(int target, std::vector<int> nums);
   virtual void solve(
         std::function<void(const Node*)>);
};

