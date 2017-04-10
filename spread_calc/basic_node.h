#ifndef _BASIC_NODE_
#define _BASIC_NODE_

#include "util.h"
#include "message_queue.h"
#include "wait_list.h"

class BasicNode {

public:
    void setInputQueue(Queue<Message> * _input_queue) { input_queue = _input_queue; }
    virtual void terminate() {continue_execution = false;  }
    void setWaitingList(WaitList * _waiting_list) { waiting_list = _waiting_list; }
    void setDebug(bool _debug) {m_debug = _debug;};
    void setSpreadsheet(Spreadsheet<Key,float> * _spreadsheet,unsigned int _col, unsigned int _row) { spreadsheet = _spreadsheet; col = _col; row = _row;}
protected:
    bool m_debug = false;
    int col;
    int row;
    bool continue_execution = true;
    Spreadsheet<Key,float> * spreadsheet = nullptr; 
    Queue<Message> * input_queue = nullptr;
    WaitList * waiting_list = nullptr;
};

class BasicInput : public BasicNode {

public:
    virtual void run(int row, int col) {}
    void setWorkerQueue(std::vector<Queue<Message>> *_message_queue) { message_queue = _message_queue; n_threads = message_queue->size();}

protected:
    unsigned int n_threads;
    std::vector<Queue<Message>> * message_queue = nullptr ;    
};

class BasicWorker : public BasicNode  {

public:
    BasicWorker(int _id) : id(_id) {};
    BasicWorker(BasicWorker&&) = default; 
    void setMessageQueue(Queue<Message> * _message_queue) { message_queue = _message_queue; }
    
protected:
    int id;
    Queue<Message> * message_queue = nullptr; 
};

#endif