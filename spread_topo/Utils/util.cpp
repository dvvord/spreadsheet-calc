#include <sstream>
#include <utility>
#include <cstring>
#include <regex>

#include "../graph.h"
#include "util.h"

using namespace std;

bool replaceAll(string& str, const string& from, const string& to) {
    size_t start_pos = 0;
    bool result = false;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
        result = true;
    }
    return result;
}

string to_string (Error code)
{
    string result="";
    switch (code)
    {
        case Error::SUCCESS: result = "Success."; break;
        case Error::LOOP: result = "There is a loop in an input data."; break;
        case Error::SIZE: result = "Incorrect size of input data or input file."; break;
        case Error::FORMAT: result = "One of formulas format is incorrect."; break;
    }
    return result;
}

float stringToFloat(const string& in_str) {
    char * end;
    float value = strtof (in_str.c_str(), &end);

    if(!isnormal(value) || strlen(end)!=0)
        throw exception();

    return value;
}

int mapCell(const Key& key,int col, int row)
{
    return (key.first-'A')*col+key.second-1;
}

Key mapCell(int v,int col,int row)
{
    return make_pair('A'+v/col,v%col+1);
}

void parseFormulaForDeps(const string& formula, std::vector<Key>& deps) {  
    regex pattern("[A-Z][1-9][0-9]*");
    smatch results;
    regex_match (formula,results,pattern);
    string::const_iterator searchStart( formula.cbegin() );
    while ( regex_search( searchStart, formula.cend(), results, pattern ) ) {
        string res(results[0]);
        char c = res[0];
        int n = atoi(res.substr(1,res.size()).c_str());
        auto p = make_pair(c,n);
        deps.emplace_back(p);
        searchStart += results.position() + results.length();
    }
}

bool isOperation(char b) {
    return b=='+' || b=='-' || b=='*' || b=='/' ; 
}

float evaluate_posfix ( const std::string& expression ) {
    float l=0.0,r=0.0,ans=0.0;
    float result = 0;
    stringstream postfix(expression);
    vector<float> stack;
    string s;
    while ( postfix >> s ) {
        if( isOperation(s[0]) && stack.size()>=2 && (s[1]==' ' || s[1]=='\0')) {
            r = stack.back();
            stack.pop_back();
            l = stack.back();
            stack.pop_back();
            switch( s[0]) {
                case '+': ans =  l + r ; break;
                case '-': ans =  l - r ; break;
                case '*': ans =  l * r ; break;
                case '/':
                    if(r==0) throw exception();
                    ans =  l / r ; 
                    break;
                default:
                    throw invalid_argument( "incorrect format of the value" );
            }
            stack.push_back( ans );
        }
        else {
            stack.push_back(stof(s));
        }
    }
    if(stack.size()>0)
        result = stack.back();
    else {
        throw invalid_argument( "incorrect format of the value" );
    }

    return result;
} 

bool preprocessFormula(string & formula)
{
    bool result = false;
    formula+=" ";

    while((result = replaceAll(formula," -- "," 1 - ")));
    while((result = replaceAll(formula," ++ "," 1 + ")));
    while((result = replaceAll(formula,"++","")));
    while((result = replaceAll(formula,"--","")));
    return ~result;
}

bool replaceWithValues(string& formula, Table& table)
{
    bool res = true;
    vector<Key> deps;
    formula+=" ";
    parseFormulaForDeps(formula, deps);
    for(const Key& k : deps) {
        if(table[k].has_value)
        {
            stringstream ss_val;
            stringstream ss_key;
            ss_val << table[k].value << " ";
            ss_key << k.first << k.second << " ";
            replaceAll(formula,ss_key.str(),ss_val.str());
        }
        else {
            res=false;
            break;
        }
    }
    return res;
}

bool evaluateFormula(string& formula, Table& table, float& result) {
    bool res = true;
    try {
        replaceWithValues(formula,table);
        preprocessFormula(formula);
        result = evaluate_posfix(formula);
    }
    catch(const exception&) {
        res = false; 
    }
    return res;
}

int pushDataToGraph(Graph& graph,unsigned int col,unsigned int row,map<Key,Cell>& spreadsheet)
{
    string cell;

    int total =0;
    unsigned int n=1;
    char c='A';

    auto increment = [&] () {
        total++;
        if(n>=col) { // iterate cell key where "c" is letter and "n" is number
            n=1;c++; 
        }
        else { 
            n++;
        } 
    };

    while(getline(cin,cell)) { 
        if(cell.size()<=0) {
            increment();
            continue;
        }
        replaceAll(cell,"\r","");
        Key key = make_pair(c,n);
      //  cout << key.first << key.second << ": " << cell << "\n";

        try {
            auto val = stringToFloat(cell);
            spreadsheet[key].value=val;
            spreadsheet[key].has_value=true;
        }
        catch(const std::exception&) {
           spreadsheet[key].formula=cell;
           int v = mapCell(key,col,row);
           vector<Key> deps;
           parseFormulaForDeps(cell, deps);
           for(const Key& k : deps) {
              int u = mapCell(k,col,row);
             // cout << "Link: " << v << "-" << u << "\n";
              graph.addEdge(v,u);
           }
        }

        increment();
    }

    return total;
}

int printSpreadsheet(std::map<Key,Cell> spreadsheet,unsigned int col,unsigned int row)
{
    unsigned int total = 0;
    unsigned int n=1;
    char c='A';
    
    auto increment = [&] () {
        total++;
        if(n>=col) { // iterate cell key where "c" is letter and "n" is number
            n=1;c++; 
        }
        else { 
            n++;
        } 
    };

    while(total<row*col) { 
        Key key = make_pair(c,n);
       if(spreadsheet[key].has_value)
            cout << spreadsheet[key].value << "\n";
        else
            cout << spreadsheet[key].formula << "\n";

        increment();
    }

    return total;
}

