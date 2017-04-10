#include <fstream>

#include <ctime>

#include "spreadc.h"
#include "input_node.h"
#include "worker_node.h"
#include "output_node.h"

using namespace std;

int moveInputStreamToQueue(Queue<Message>& input_queue,unsigned int col)
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
        Message m;
        if(cell.size()<=0) {
            increment();
            continue;
        }

        m.key = make_pair(c,n);
        try {
            auto val = stringToFloat(cell); // try parse
            m.value = val;
            m.type = MessageType::VALUE;
        }
        catch(const std::exception&) {
            m.formula = cell;
            m.type = MessageType::FORMULA;
        }

        input_queue.send(m);

        increment();
    }

    return total;
}

int main(int argc, char * argv[]) {
    	clock_t begin = clock();
    string cell;
    unsigned int row, col;
    
    cin >> col >> row;
    getline(cin,cell); // skip end of line symbol

    unsigned int g_threads = 0;
    g_threads = thread::hardware_concurrency() - 2; // 1 thread for input, 1 for output and other for processing

    vector<Queue<Message>> message_queue(g_threads);
    Queue<Message> input_queue;
    Spreadsheet<Key,float> spreadsheet;
    WaitList waiting_list;

    std::vector<std::thread> threads;
    
    cout << "Number of worker nodes:" << g_threads << endl;
    
    vector<WorkerNode *> t_worker(g_threads);

    for(unsigned int i = 0; i < g_threads; ++i) {
        t_worker[i] = new WorkerNode(i);
        t_worker[i]->setInputQueue(&input_queue);
        t_worker[i]->setMessageQueue(&(message_queue[i]));
        t_worker[i]->setWaitingList(&waiting_list);
        t_worker[i]->setSpreadsheet(&spreadsheet,row,col);
        auto t = std::thread(&WorkerNode::run,t_worker[i],i);
        threads.push_back(move(t));
    } 

    InputNode t_input;
    t_input.setWaitingList(&waiting_list);
    t_input.setInputQueue(&input_queue);
    t_input.setSpreadsheet(&spreadsheet,row,col);
    t_input.setWorkerQueue(&message_queue);

    OutputNode t_output;
    t_output.setSpreadsheet(&spreadsheet,row,col);    
    t_output.setInputQueue(&input_queue);

    thread in(&InputNode::run,&t_input,row,col); 
    thread out(&OutputNode::run, &t_output,row,col);

    unsigned int total = moveInputStreamToQueue(input_queue,col);

    if(total<col*row) // if total data size less then expected than stop all threads
    { 
        for (unsigned int i = 0; i < g_threads; ++i)
            t_worker[i]->terminate();
        t_input.terminate();
        t_output.terminate();
    }

    in.join();
    for( auto & t: threads)
        t.join();

    t_output.terminate();
    
    out.join();
    cout.flush();
    clock_t end = clock();
    cout << "Time spent: " << double(end-begin)/CLOCKS_PER_SEC << endl;
    return 0;
}