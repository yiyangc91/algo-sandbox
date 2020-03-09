#include "solver.h"

#include <iostream>

using namespace std;

int main() {
   TreeSolver ts {10, {1,2,3,4}};
   ts.solve([](const Node* res) {
      cout << res->string() << endl;
   });

}
