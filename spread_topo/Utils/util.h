#ifndef _UTIL_
#define _UTIL_
#include <cmath>
#include <utility>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>

using namespace std;

class Graph;

typedef std::pair<char,int> Key;

struct Cell {
    float value = 0.0;
    std::string formula = "";
    bool has_value = false;
};

typedef map<Key,Cell> Table;

enum class Error {
    SUCCESS = 0,
    LOOP = 100, // there are loops in spreadsheet cells formulas 
    SIZE = 101, // the size of the data not equal to format definition
    FORMAT = 102, // cell formula evaluation impossible due to incorrect format
};

std::string to_string (Error code);
float stringToFloat(const std::string& in_str);
bool replaceAll(string& str, const string& from, const string& to) ;
int mapCell(const Key& key,int row, int col);
Key mapCell(int v,int row,int col);
void parseFormulaForDeps(const std::string& formula, std::vector<Key>& deps);

bool isOperation(char b);
float evaluate_posfix ( const std::string& expression );
bool preprocessFormula(string & formula);
bool replaceWithValues(string& formula, Table& table);
bool evaluateFormula(string& formula, Table& table,  float& result);
int pushDataToGraph(Graph& graph,unsigned int col,unsigned int row,map<Key,Cell>& spreadsheet);
int printSpreadsheet(std::map<Key,Cell> spreadsheet,unsigned int col,unsigned int row);
#endif