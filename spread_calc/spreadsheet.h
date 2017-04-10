#ifndef _SPREADSHEET_
#define _SPREADSHEET_
#include <map>
#include <mutex>
#include <condition_variable>

template< typename K, typename V>
class Spreadsheet {
public:
    void setValue(const K& k, V& value) {
        std::lock_guard<std::mutex> locker(spreadsheet_lock);
        spreadsheet[k].value=value;
        spreadsheet[k].has_value=true;
        spreadsheet_cond.notify_all();       
    }
    bool hasValue(const K& k) {        
        return spreadsheet[k].has_value; 
        }
    float getValue(const K& k) {
        std::unique_lock<std::mutex> locker(spreadsheet_lock);
       if(!hasValue(k)) {
            spreadsheet_cond.wait(locker,[&]{return hasValue(k);});
        }
        return  spreadsheet[k].value;     
    }
    float getValue(const K& k,bool wait) {
        std::unique_lock<std::mutex> locker(spreadsheet_lock);
       if(!hasValue(k)) {
            spreadsheet_cond.wait(locker,[&]{return !wait || hasValue(k);});
        }
        return  spreadsheet[k].value;     
    }    
private:
    struct Cell {
        V value = 0.0;
        bool has_value = false;
    };
    std::condition_variable spreadsheet_cond;
    std::mutex spreadsheet_lock;
    std::map<K,Cell> spreadsheet;
};

#endif