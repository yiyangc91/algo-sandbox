#include "solver.h"

#include <iostream>

using namespace std;

int main() {
   TreeSolver ts {24, {4, 6, 2, 1}};
   ts.solve([](const Node* res) {
      cout << res->string() << endl;
   });

}
