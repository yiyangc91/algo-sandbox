#include <vector>
#include <deque>
#include <utility>
#include <algorithm>

using namespace std;

int minCost(int hire, int salary, int severance, vector<int> minEmployees) {
   if (!salary) {
      // EMPLOY ALL THE PEOPLE
      return *max_element(minEmployees.begin(), minEmployees.end()) * (hire + severance);
   }
   
   // crux: is it cheaper to fire and rehire an employee vs keep them employed?
   int months = (hire + severance) / salary;
   // months or less, we keep the employee

   // walk through array, hirng employees as needed, and on reduction, "tentatively"
   // firing them (but tracking when they were fired)
   int hired {};
   int cost {};
   deque<pair<int, int>> fired; // (index, amount)
   for (int i = 0; i < minEmployees.size(); ++i) {
      int required = minEmployees[i];

      if (required > hired) {
         int diff = required - hired;
         while (diff) {
            if (fired.empty()) {
               cost += diff * hire;
               diff = 0;
            } else if (i-fired.back().first > months) {
               // fire previously tentatively fired employees
               if (fired.back().second > diff) {
                  cost += diff * (hire + severance);
                  fired.back().second -= diff;
                  diff = 0;
               } else {
                  cost += fired.back().second * (hire + severance);
                  diff -= fired.back().second;
                  fired.pop_back();
               }
            } else {
               if (fired.back().second > diff) {
                  cost += diff*salary*(i-fired.back().first);
                  fired.back().second -= diff;
                  diff = 0;
               } else {
                  cost += fired.back().second*salary*(i-fired.back().first);
                  diff -= fired.back().second;
                  fired.pop_back();
               }
            }
         }
      } else if (required < hired) {
         // downsize the workforce (tentatively)
         fired.emplace_back(i, hired - required);  
      }
      
      cost += required * salary;
      hired = required;
   }

   // finally just fire everyone, once and for all
   cost += hired * severance;
   while (!fired.empty()) {
      cost += fired.back().second * severance;
      fired.pop_back();
   }

   return cost;
}
