#ifndef _UTIL_
#define _UTIL_
#include <cmath>
#include <utility>
#include <iostream>
#include <iomanip>

typedef std::pair<char,int> Key;

enum class Error {
    SUCCESS = 0,
    LOOP = 100, // there are loops in spreadsheet cells formulas 
    SIZE = 101, // the size of the data not equal to format definition
    FORMAT = 102, // cell formula evaluation impossible due to incorrect format
};

enum class MessageType {VALUE=0, FORMULA=1, TERMINATE=2,NONE=-1};

struct Message {
    Message() = default;
    Message(const Message&) = default;
    Message& operator=(const Message&) = default;
    Message( const Key _key,const std::string _formula,const float _value,const MessageType _mtype) : 
            key(_key), formula(_formula),value(_value),type(_mtype) {}; 
    std::ostream& operator<<(std::ostream& out);
    void print();
    Key key;
    std::string formula;
    float value = 0.0;
    MessageType type=MessageType::NONE; 
    
};

std::string to_string (Error code);
float stringToFloat(const std::string& in_str);
bool replaceAll(std::string& str, const std::string from, const std::string to);

#endif