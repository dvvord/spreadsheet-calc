#include <list>
#include <queue>

#include "Utils/util.h"

using namespace std;
typedef pair<char,int> Key;

class Graph
{
    int V;
    int row;
    int col;

    list<int> *adj;
    vector<int> top_order;
    bool isSorted = false;
 
public:
    Graph(unsigned int col,unsigned int row); 
 
    void addEdge(unsigned int u, unsigned int v);

    void topologicalSort();
    void topologicalCalc(std::map<Key,Cell>& spreadsheet);
    void printQueue();
};
