#include <fstream>

#include <ctime>

#include "spreadc.h"


using namespace std;

int main(int argc, char * argv[]) {
    clock_t begin = clock();
    string cell;
    unsigned int col, row;
    std::map<Key,Cell> spreadsheet;

    cin >> col >> row;
    getline(cin,cell); 

    Graph g(col,row);
    pushDataToGraph(g,col,row,spreadsheet);

    g.topologicalSort();
    g.topologicalCalc(spreadsheet);

    printSpreadsheet(spreadsheet,col,row);
    clock_t end = clock();
    cout << "Time spent: " << double(end-begin)/CLOCKS_PER_SEC << endl;
    return 0;
}