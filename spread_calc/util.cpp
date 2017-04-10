#include <iostream>
#include <sstream>
#include <utility>
#include <map>
#include <chrono>
#include <thread>
#include <mutex>
#include <regex>
#include <list>
#include <condition_variable>
#include <atomic>

#include "message_queue.h"
#include "util.h"

using namespace std;

void Message::print() {
    std::cout << "Message: [";
    std::cout << "Key=" << key.first << key.second << ";";
    if (type == MessageType::VALUE)
        std::cout << "Value=" << value << "]";
     else if(type == MessageType::FORMULA)
         std::cout << "Formula=" << formula << "]";
     else if(type == MessageType::TERMINATE)
         std::cout << "Terminate]";            
    else
        std::cout << "None]";
    std::cout << std::endl;
}

std::ostream& Message::operator<<(std::ostream& out)
{
    if (type == MessageType::VALUE)
        out << value;
      else 
           out << formula;
    return out;
}

bool replaceAll(std::string& str, const std::string from, const std::string to) {
    size_t start_pos = 0;
    bool result = false;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
        result = true;
    }
    return result;
}

std::string to_string (Error code)
{
    std::string result="";
    switch (code)
    {
        case Error::SUCCESS: result = "Success."; break;
        case Error::LOOP: result = "There is a loop in an input data."; break;
        case Error::SIZE: result = "Incorrect size of input data or input file."; break;
        case Error::FORMAT: result = "One of formulas format is incorrect."; break;
    }
    return result;
}

float stringToFloat(const std::string& in_str) {
    char * end;
    float value = strtof (in_str.c_str(), &end);

    if(!isnormal(value) || strlen(end)!=0)
        throw std::exception();

    return value;
}