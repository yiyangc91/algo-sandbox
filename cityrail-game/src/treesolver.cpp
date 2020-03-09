#include <functional>

#include "solver.h"

/*
 * Tree Solver
 * ====================
 * This works by maintaining a list of nodes (where
 * in this case node is only ever op or val).
 *
 * O(n^2) memory to store all the combined nodes.
 *
 * Then we "combine" all combinations of two nodes,
 * until we end up with a single node representing
 * our final solution.
 *
 * There are some optimizations that could be made:
 * 1. When combining two nodes, it doesn't make sense
 *    to try the "+" and "*" operands if the RHS is
 *    +- or *\/.
 * 2. Obviously, order doesn't matter when combining
 *    nodes, but at least for - and / we should flip
 *    the arguments around both ways.
 * 3. Nodes sorted. For each particular level, if we
 *    re-encounter a particular sorted set of nodes,
 *    we know we've already tried that combination.
 *    (questionable if this optimization makes sense).
 */

using namespace std;

namespace {
   Node* getAsNodePtr(variant<OpRaw, Node>& var) {
      if (auto ptr = get_if<OpRaw>(&var)) {
         return ptr;
      } else {
         return get_if<Node>(&var);
      }
   }
}

void TreeSolver::generateOps(
      variant<OpRaw, Node>& left,
      variant<OpRaw, Node>& right,
      vector<variant<OpRaw, Node>>& out,
      unsigned int length,
      function<void(void)> fillRemainder,
      function<void(const Node*)> f) {
   // God this whole fucktion is a clusterfuck.
   // I've been coding for too long.
   // This first statement adds the subtract operation
   // and then fills the remainder of the list
   out.emplace_back(OpRaw(getAsNodePtr(left), getAsNodePtr(right), Operation::Subtract));
   auto it = out.end()-1;

   fillRemainder();
   permute(length, f);

   *it = OpRaw(getAsNodePtr(right), getAsNodePtr(left), Operation::Subtract);
   permute(length, f);

   *it = OpRaw(getAsNodePtr(left), getAsNodePtr(right), Operation::Divide);
   permute(length, f);

   *it = OpRaw(getAsNodePtr(right), getAsNodePtr(left), Operation::Divide);
   permute(length, f);

   if (!holds_alternative<OpRaw>(right) || (
            get<OpRaw>(right).getOperation() != Operation::Add &&
            get<OpRaw>(right).getOperation() != Operation::Subtract
            )) {
      *it = OpRaw(getAsNodePtr(left),
            getAsNodePtr(right),
            Operation::Add);
      permute(length, f);
   }

   if (!holds_alternative<OpRaw>(right) || (
            get<OpRaw>(right).getOperation() != Operation::Multiply &&
            get<OpRaw>(right).getOperation() != Operation::Divide
   )) {
      *it = OpRaw(getAsNodePtr(left),
            getAsNodePtr(right),
            Operation::Multiply);
      permute(length, f);
   }
}

void TreeSolver::permute(unsigned int length,
         std::function<void(const Node*)> f) {
   // Qn: Have I encountered this particular combo
   //     of nodes before?
   // TODO
   
   if (!length) {
      return;
   }

   if (length == 1) {
      if (visit([this](const auto& alternative) {
         return alternative.getNumerator() == target &&
            alternative.getDenominator() == 1;
      }, preallocated[0][0])) {
         f(getAsNodePtr(preallocated[0][0]));
      }
      return;
   }

   for (unsigned int i = 0; i < length-1; ++i) {
      for (unsigned int j = i+1; j < length; ++j) {
         generateOps(preallocated[length-1][i],
               preallocated[length-1][j],
               preallocated[length-2],
               length-1,
               // HACK because cbf
               [i,j,length,this]() {
                  for (unsigned int k = i+1; k < length; ++k) {
                     if (k == j) continue;

                     preallocated[length-2].emplace_back(preallocated[length-1][k]);
                  }
               },
               f);
      }

      preallocated[length-2].erase(preallocated[length-2].begin()+i, preallocated[length-2].end());
      preallocated[length-2].emplace_back(preallocated[length-1][i]);
   }

   preallocated[length-2].clear();
}

TreeSolver::TreeSolver(int target, vector<int> nums)
   : target(target), numbers(std::move(nums)) {
   preallocated.reserve(numbers.size());

   for (decltype(numbers.size()) i = 0; i < numbers.size(); ++i) {
      preallocated.emplace_back();
      preallocated[i].reserve(i+1);
   }

}

void TreeSolver::solve(function<void(const Node*)> f) {
   // Dodgy, should use a bool. Oh well.
   for (auto num : numbers) {
      preallocated[numbers.size()-1].emplace_back(
            in_place_type<Node>,
            num, 1
            );
   }
   permute(numbers.size(), f);
}
