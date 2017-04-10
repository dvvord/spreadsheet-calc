#ifndef _WAIT_LIST_
#define _WAIT_LIST_

#include <map>
#include <mutex>
#include <condition_variable>

#include "util.h"

using namespace std;

class WaitList
{
public:
    void erase(Key k) { std::lock_guard<std::mutex> locker(waiting_list_lock); waiting_list.erase(k);}
    unsigned int count(Key k) { std::lock_guard<std::mutex> locker(waiting_list_lock); return waiting_list.count(k);}
    map<Key,vector<int>>::iterator begin() { std::lock_guard<std::mutex> locker(waiting_list_lock); return waiting_list.begin();}
    map<Key,vector<int>>::iterator end() { std::lock_guard<std::mutex> locker(waiting_list_lock);return waiting_list.end();}
    void get(Key k, vector<int>& ids) { std::lock_guard<std::mutex> locker(waiting_list_lock); ids = waiting_list[k]; }
    void push_back(Key k, int value) { std::lock_guard<std::mutex> locker(waiting_list_lock); waiting_list[k].push_back(value); }
private:
    mutex waiting_list_lock;
    map<Key,vector<int>> waiting_list;
};
#endif