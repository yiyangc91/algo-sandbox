#include <iostream>
#include <utility>
#include <gtest/gtest.h>
#include <string>

#include "impl/base.h"
#include "generate/testgen.h"


class StringSearchTest : public ::testing::TestWithParam<std::pair<const std::string, BaseSearch*>> {
protected:
};

TEST_P(StringSearchTest, SimpleSearch) {
   std::string haystack { "hello world" };
   std::string needle {"lo wo"};

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 3);
}

TEST_P(StringSearchTest, EndOfString) {
   std::string haystack { "hello world" };
   std::string needle {"world"};

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 6);
}

TEST_P(StringSearchTest, StartOfString) {
   std::string haystack { "hello world" };
   std::string needle {"hello"};

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 0);
}

TEST_P(StringSearchTest, NotFound) {
   std::string haystack { "hello world" };
   std::string needle {"hllo"};

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, std::string::npos);
}

TEST_P(StringSearchTest, EmptyString) {
   std::string haystack { "hello world" };
   std::string needle {""};

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 0);
}

TEST_P(StringSearchTest, SingleNeedle) {
   std::string haystack { "hello world" };
   std::string needle {"l"};

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 2);
}

TEST_P(StringSearchTest, EmptyHaystack) {
   std::string haystack { "" };
   std::string needle {"notfound"};

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, std::string::npos);
}

TEST_P(StringSearchTest, EmptyAll) {
   std::string haystack { "" };
   std::string needle {""};

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 0);
}

TEST_P(StringSearchTest, RepeatedStrings) {
   std::string haystack { "aaaaaaaaaabbbbbbbbbb" };
   std::string needle {"bbb"};

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 10);
}

TEST_P(StringSearchTest, RepeatedStrings2) {
   std::string haystack { "aabbaabbaaccaaabbaabbaacccaa" };
   std::string needle {"aabbaabbaaccc"};

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 13);
}

TEST_P(StringSearchTest, RepeatedStrings3) {
   std::string haystack { "catcatcatcatcatcatcatcatdocatcatcatcatcatcatcatcatcatcatcatcatcatcatcatcatcatdocatcatcatcatcatcatdogcatcatcatcatcatcatcatcatdocatcatcatcatcatdogcatcat" };
   std::string needle {"catcatcatcatcatcatdocatcatcatcatcatdog"};

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 106);
}

TEST_P(StringSearchTest, RepeatedStrings4) {
   std::string haystack { "catcatcatcatcatcatcatcatdocatcatcatcatcatcatcatcatcatcatcatcatcatcatcatcatcatdocatcatcatcatcatcatdogcatcatcatcatcatcatcatcatdocatcatcatcatcatdog" };
   std::string needle {"catcatcatcatcatcatdocatcatcatcatcatdog"};

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 106);
}

TEST_P(StringSearchTest, RepeatedStrings5) {
   std::string haystack { "catcatcatcatcatcatdocatcatcatcatcatdogcatcatcatcatcatcatcatcatdocatcatcatcatcatcatcatcatcatcatcatcatcatcatcatcatcatdocatcatcatcatcatcatdogcatcatcatcatcatcatcatcatdocatcatcatcatcatdog" };
   std::string needle {"catcatcatcatcatcatdocatcatcatcatcatdog"};

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 0);
}

TEST_P(StringSearchTest, KMP1) {
   std::string haystack { "ABACABABACABABC" };
   std::string needle {"ABACABABC"};

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 6);
}

TEST_P(StringSearchTest, KMP2) {
   std::string haystack { "ABACABABACABABCABACABABCABACABABCABACABABC" };
   std::string needle {"ABACABABCABACABABCABACABABC"};

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 6);
}

TEST_P(StringSearchTest, KMP3) {
   std::string haystack { "BLAHBLA BLAHBLA BLAHBLA BLAHBLA BLAHBLAHBLA BLA" };
   std::string needle {"BLAHBLA BLAHBLA BLAHBLAHBLA "};

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 16);
}

TEST_P(StringSearchTest, KMP4) {
   // Actual Found:           ABRABCABRABCABRABRAX
   std::string haystack { "ABRABRABCABRABCABRABRAX" };
   std::string needle   {"ABRABCABRABCABRABRAX"};
   //                        AB_     -- THIS TEST
   //                           ABRABCABRAB_
   //                              AB_
   //                                 ABRAB_
   //                                    ABRA_
   // Slightly longer test case of KMP1 which loops around at least once

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 3);
}

TEST_P(StringSearchTest, KMP5) {
   std::string haystack { "ABRABCABRABRABCABRABCABRABRAX" };
   std::string needle   { "ABRABCABRABCABRABRAX"};
   //                         AB_     |
   //                            ABRABCABRAB_
   //                               AB_   -- this test (match ABR) should be unaffected by the previous longer string
   //                                  ABRAB_
   //                                     ABRA_

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 9);
}

TEST_P(StringSearchTest, KMP6) {
   // Actual Found:       ABRABCABRABCABRABRAX
   std::string haystack { "ABRABCABRABCABRABCABRABRAX" };
   std::string needle   { "ABRABCABRABCABRABRAX"};
   //                         AB_           |
   //                            ABRABCABRAB_  -- this test - simple - should match the longest string because it ends with R
   //                               AB_
   //                                  ABRAB_
   //                                     ABRA_

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 6);
}

TEST_P(StringSearchTest, KMP7) {
   //    Actual Found:             ABRABCABRABCABRABRAX
   std::string haystack { "ABRABCABRABCABRABRABCABRABCABRABRAX" };
   std::string needle   { "ABRABCABRABCABRABRAX"};
   //                         AB_             |
   //                            ABRABCABRAB_ |
   //                               AB_       |
   //                                  ABRAB_ |
   //                                     ABRA_  -- test the final loop

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 15);
}

TEST_P(StringSearchTest, KMP8) {
   std::string haystack { "ABCXABCYABCXABCXABCYABCXABCW" };
   std::string needle   { "ABCXABCYABCXABCW"};
   //                          ABC_
   //                              ABCXABC_
   // There are two cases
   // - last character is X -> we start matching on ABCW
   // - last character is Y -> we start matching on ABCX
   // This tests the first case

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 12);
}

TEST_P(StringSearchTest, KMP9) {
   std::string haystack { "ABCXABCYABCXABCYABCXABCW" };
   std::string needle   { "ABCXABCYABCXABCW"};
   //                          ABC_
   //                              ABCXABC_
   // There are two cases
   // - last character is X -> we start matching on ABCW
   // - last character is Y -> we start matching on ABCX
   // This tests the second case

   auto params = GetParam();
   auto result = (*params.second)(needle, haystack);
   EXPECT_EQ(result, 8);
}

//TEST_P(StringSearchTest, LargeString) {
//   constexpr long seed { 1458491 };
//   TestGen<> tg {seed, 65536, 1048576};

//   auto stringSequences = tg.generateStringSequences();
//   auto params = GetParam();
//   auto result = (*params.second)(stringSequences.first, stringSequences.second);
//   EXPECT_EQ(result, std::string::npos);
//}

TEST_P(StringSearchTest, LargeString2) {
   constexpr long seed { 3145198 };
   TestGen<> tg {seed, 256, 1048576 };

   auto stringSequences = tg.generateStringSequences();
   auto params = GetParam();
   auto result = (*params.second)(stringSequences.first, stringSequences.second);
   EXPECT_EQ(result, std::string::npos);
}

INSTANTIATE_TEST_SUITE_P(RegisteredSearches,
                         StringSearchTest,
                         testing::ValuesIn(getInstance()->getInstances().begin(),
                                           getInstance()->getInstances().end()),
                         [](auto arg) -> std::string {return arg.param.first; });


int main(int argc, char **argv) {
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
