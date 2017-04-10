#include "gtest/gtest.h"

#include "../graph.h"
#include "../Utils/util.h"


using namespace ::testing;

TEST(UtilsTests, MapCell_FromIndex) { 
    ASSERT_EQ(make_pair('B',1), mapCell(50,50,4));
}
TEST(UtilsTests, MapCell_ToIndex) { 
    ASSERT_EQ(50, mapCell(make_pair('B',1),50,4));
}
TEST(UtilsTests, TestReplaceAllUnaryMinus) { 
    string test_string = "A1 -- B2 -- C3 + ";
    bool success = replaceAll(test_string, " -- ", " 1 - ");
    EXPECT_TRUE(success);
    ASSERT_EQ("A1 1 - B2 1 - C3 + ", test_string);

    success = replaceAll(test_string,"--","");
    ASSERT_EQ("A1 1 - B2 1 - C3 + ", test_string); 
}

TEST(UtilsTests, TestReplaceAllUnaryPlus) { 
    string test_string = "A1 ++ B2 ++ C3 + ";
    bool success = replaceAll(test_string," ++ "," 1 + ");
    EXPECT_TRUE(success);
    ASSERT_EQ("A1 1 + B2 1 + C3 + ", test_string);

    success = replaceAll(test_string,"++","");
    ASSERT_EQ("A1 1 + B2 1 + C3 + ", test_string);
}

TEST(UtilsTests, TestReplaceAllByValues) { 
    string test_string = "A1 -- B2 C3";
    bool success = replaceAll(test_string, "A1", "1.045343");
    EXPECT_TRUE(success);
    ASSERT_EQ("1.045343 -- B2 C3", test_string);
}

TEST(UtilsTests, TestStringToFloat) { 
    string test_string = "1.54859";
    float results = stringToFloat(test_string);
    EXPECT_TRUE((results-1.54859)<numeric_limits<float>::epsilon());
}

TEST(UtilsTests, TestParseFormulaForDeps) { 
    string test_string = "A1 1 + B2 1 + C3 +";
    Key k[] = {make_pair('A',1),make_pair('B',2),make_pair('C',3)};
    vector<Key> deps;
    parseFormulaForDeps(test_string, deps);
    for(int i=0;i<3;++i) {
        ASSERT_EQ(k[i],deps[i]);
    }
}

TEST(UtilsTests, TestIsOperation) { 
    char ops[] = {'+','-','/','*'};
    for(const auto o : ops)
        EXPECT_TRUE(isOperation(o));
}

TEST(UtilsTests, TestEvaluatePostfix) { 
    string test_string = "1.5 2.1 + 2 / 3 * 1 -";
    float results = evaluate_posfix ( test_string);
    EXPECT_TRUE((results-4.4)<numeric_limits<float>::epsilon());
}

TEST(UtilsTests, TestPreprocessFormula) { 
    string test_string = "A2 B1 + C3 -- +";
    bool result = preprocessFormula(test_string);
    EXPECT_TRUE(result);
    ASSERT_EQ("A2 B1 + C3 1 - + ",test_string);
}

TEST(UtilsTests, TestReplaceWithValues) {
    std::map<Key,Cell> spreadsheet;
    string formula = "A1 B1 + A2 B2 + +";
    string value_formula = "1.44535 3.04335 + 2.42354 1.55463 + + ";

    Key k1 = make_pair('A',1);
    Key k2 = make_pair('A',2);
    Key k3 = make_pair('B',1);
    Key k4 = make_pair('B',2);

    spreadsheet[k1].value = 1.44535;
    spreadsheet[k1].has_value=true;

    spreadsheet[k2].value = 2.42354;
    spreadsheet[k2].has_value=true;

    spreadsheet[k3].value = 3.04335;
    spreadsheet[k3].has_value=true;

    spreadsheet[k4].value = 1.55463;
    spreadsheet[k4].has_value=true;            

    replaceWithValues(formula,spreadsheet);

    ASSERT_EQ(formula,value_formula);
}

int main(int argc,char * argv[])
{
    int returnValue =0;
    InitGoogleTest(&argc, argv);
    returnValue = RUN_ALL_TESTS();

    return returnValue;
}