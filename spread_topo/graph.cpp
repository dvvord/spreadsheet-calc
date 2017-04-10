#include <sstream>

#include "graph.h"

using namespace std;
 
Graph::Graph(unsigned int col,unsigned int row)
{
    this->V = row*col;
    this->row = row;
    this->col = col;
    adj = new list<int>[V];
    isSorted=false;
}

void Graph::addEdge(unsigned int u, unsigned int v)
{
    adj[u].push_back(v);
    isSorted=false;
}

void Graph::topologicalSort()
{
    queue<int> q;
    vector<int> in_degree(V, 0);

    if(V==1){
        isSorted=true;
        return;
    }

    for (int u=0; u<V; u++)
    {
        for (const auto v : adj[u])
             in_degree[v]++;
    }

    for (int i = 0; i < V; i++)
        if (in_degree[i] == 0)
            q.push(i);

    int cnt = 0;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        top_order.push_back(u);

        for (const auto v : adj[u])
            if (--in_degree[v] == 0)
                q.push(v);

        cnt++;
    }

    if (cnt != V) {
        isSorted=false;
        cout << "LOOP!" << endl;
        return;
    }

    isSorted=true;
}

void Graph::topologicalCalc(Table& spreadsheet)
{
    if(isSorted){
        vector<Key> deps;
        while(!top_order.empty())
        {
            stringstream ss;
            int indx = top_order.back();
            top_order.pop_back();
            Key k = mapCell(indx,col,row);
            auto& cell = spreadsheet[k];
            bool has_value = cell.has_value;
            float result=0;
            string formula = cell.formula;     

            if(has_value) 
            {
                ss << indx << ":" << k.first << k.second <<  ": "<< cell.value << endl;
                continue;
            }
            if(evaluateFormula(formula,spreadsheet,result))
            {
                cell.value = result;
                cell.has_value=true;
                ss << indx << ":" << k.first << k.second <<  ": "<< formula << "=" << result << endl;
            }
            else
            {
                ss << indx << ":" << k.first << k.second << ": "<< formula << endl;
            }
            //cout << ss.str();
        }

    }
}

void Graph::printQueue()
{
    if(isSorted){ 
        for(const auto & c : top_order)
        {
            cout << c << " ";
        }
        cout << endl;
    }
}