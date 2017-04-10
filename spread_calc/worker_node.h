#include <iostream>
#include <sstream>
#include <utility>
#include <map>
#include <thread>
#include <mutex>
#include <regex>
#include <condition_variable>
#include <algorithm>
#include <atomic>

#include "spreadsheet.h"
#include "basic_node.h"


using namespace std;

class WorkerNode :  public BasicWorker
{
public:
    WorkerNode(WorkerNode&&) = default;
    WorkerNode(int id) : BasicWorker(id) { }
    void run(int thread_id);

private:
    void execute(Message& m);
    void parseFormulaForDeps(string formula, vector<Key>& deps) const;
    bool isOperation(const char b) const;
    float evaluate_posfix(const std::string& expression ) const;
    bool preprocessFormula(string & formula) const;
    bool evaluateFormula(string formula, float& result) const;
    void parseFormula(Message& m);

private:
    map<Key,vector<Key>> deps_cache;
    map<Key,string> formula_cache;
    static const regex pattern;
};