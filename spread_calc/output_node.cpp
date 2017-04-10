#include "output_node.h"

using namespace std;

void OutputNode::run(int row,int col) {
    int size = row * col;
    int value_counter=0;
    int n=1;
    char c='A';
    Key k;
    cout << col << " " << row << endl;    
    while(value_counter<size && continue_execution) { // execute till we processed all values or been forced to terminate
        k  = make_pair (c,n); 
        target_key = k;
        float value = spreadsheet->getValue(k, continue_execution);
        if(!m_debug && continue_execution)
            cout << k.first << k.second << " "<< setprecision (5) << fixed << value << endl;
        if(n>=col) { // iterate cell key where "c" os letter and "n" is number
            n=1;c++; 
            } 
        else { 
            n++;
        }
        value_counter++;
    } 
    if (continue_execution) {
        Message terminate{ make_pair(0,0),"",0.0,MessageType::TERMINATE };
        input_queue->send(terminate);
    }
}