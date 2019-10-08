#include <gtest/gtest.h>

#include "employee.h"

TEST(EmployeeTest, EveryoneIsHired1) {
   EXPECT_EQ(1 + 2*3 + 4, minCost(1, 2, 4, {1, 1, 1}));
}

TEST(EmployeeTest, EveryoneIsHired2) {
   EXPECT_EQ(6 * (3 + 2*4 + 4), minCost(3, 2, 4, {6, 6, 6, 6}));
}

TEST(EmployeeTest, BasicHireFire1) {
   // It's cheaper to just keep our employees hired
   // (hire and fire is $15, but hiring for two months is just $14
   EXPECT_EQ(3 + 7*4 + 12, minCost(3, 7, 12, {1, 0, 0, 1}));
}

TEST(EmployeeTest, BasicHireFire2) {
   // Cheaper to fire our employees at this point
   EXPECT_EQ(3 + 7 + 10 + 3 + 7 + 10, minCost(3, 7, 10, {1, 0, 0, 1}));
}

TEST(EmployeeTest, ManyEmployees) {
   // 3 months or more -> choose to fire

   // one employee stays hired:
   // 0 1 1 0 1 0 1 1 1 0
   int employee1 = 3 + 4*8 + 7;

   // second employee
   // 0 1 1 0 0 0 1 0 1 0
   int employee2 = 3 + 4*2 + 7 + 3 + 4*3 + 7;

   // third employee
   // 0 0 1 0 0 0 0 0 1 0
   int employee3 = 2 * (3 + 4 + 7);

   /*
    * 2 employees = hire + work 6   + 8
    * 3 employees = hire one more 3 + 12
    * 0 employees = 0
    * 1 employee  = rehire 4        + 4
    * 0 employee  = 0
    * 2 employee  = rehire 4, fh 10 + 8
    * 1 employee  =                 + 4
    * 3 employee  = rehire 4, fh 10 + 12
    * final fire  = 3*7
    *
    */

   EXPECT_EQ(110, employee1+employee2+employee3);
   EXPECT_EQ(employee1 + employee2 + employee3, minCost(3, 4, 7, {0,2,3,0,1,0,2,1,3,0}));
}

TEST(EmployeeTest, Zero) {
   EXPECT_EQ(0, minCost(3, 4, 7, {}));
}

TEST(EmployeeTest, ZeroSalary) {
   EXPECT_EQ(2, minCost(1, 0, 1, {1, 0, 0, 1}));
}
