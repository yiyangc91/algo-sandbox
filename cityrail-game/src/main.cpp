#include "solver.h"

#include <iostream>

using namespace std;

int main() {
   TreeSolver ts {24, {1, 2, 3, 4}};
   auto results = ts.solve();

   for (const auto& res : results) {
      cout << res->string() << endl;
   }
}
