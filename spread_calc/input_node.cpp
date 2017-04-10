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
#include <stack>

#include "input_node.h"

using namespace std;

void InputNode::NotifyCalculated() {
    for(const auto & k : (*waiting_list)) {
        if(spreadsheet->hasValue(k.first)) {  // trying to help threads that missed a value
            Message v{k.first,"",spreadsheet->getValue(k.first),MessageType::VALUE};
            input_queue->send(v);
            trial_state_counter = 0;
        }
        else { // not yet calculated or loop values
            if(++trial_state_counter>max(int(size*0.1),100)) { // this is hack to identify loop case
                terminate();
                Message terminate{ make_pair(0,0),"",float(Error::LOOP),MessageType::TERMINATE };
                input_queue->send(terminate);
                break;
            }
        }
    }

}
void InputNode::sendToWorker(int workerId, Message & m)
{
    Queue<Message>& q = reinterpret_cast<vector<Queue<Message>>&>(*message_queue)[workerId];
    q.send(m);
}
void InputNode::run(int row,int col)
{
    size = row*col;
    Message m;
    while(continue_execution) {
        chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
        while(input_queue->empty() && continue_execution) {
            chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
            chrono::duration<double> time_span = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
            if(time_span.count()>2.1) { // if we stucked with no progression either it is loop or some threads missed a value
                NotifyCalculated();
            }
        }
        while(!input_queue->empty()) {  
            trial_state_counter = 0;
            input_queue->recieve(m);

            if(m.type == MessageType::TERMINATE) {
                continue_execution =false;
                for(auto & q : reinterpret_cast<vector<Queue<Message>>&>(*message_queue)) 
                    q.send(m); 
                continue;
            }

            if(waiting_list->count(m.key)>0) { // pass messages to all workers that subscribed and wait for it 
                vector<int> ids;
                waiting_list->get(m.key, ids);
                for(const auto& id : ids) {
                   // reinterpret_cast<vector<Queue<Message>>&>(*message_queue)[t].send(m);
                    sendToWorker(id, m);
                }
                if(m.type == MessageType::VALUE)
                    waiting_list->erase(m.key);
            }
            else { //if no one subscribed for the value, send to worker with assosiated id
                int id = ((m.key.first-'A')+m.key.second-1) % n_threads; //assosiation rule
                //reinterpret_cast<vector<Queue<Message>>&>(*message_queue)[id].send(m); 
                sendToWorker(id, m);
            }
        }         
    }
}