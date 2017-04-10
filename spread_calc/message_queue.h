#ifndef _MESSAGE_QUEUE_
#define _MESSAGE_QUEUE_
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <typeinfo>

template <typename T>
class Queue
{
 public:
 
  T recieve()
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    if (queue_.empty())
    {
      cond_.wait(mlock,[&]{return !queue_.empty();});
    }
    auto item = queue_.front();
    queue_.pop();
    return item;
  }
 
  void recieve(T& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);  
    if (queue_.empty())
    {
        cond_.wait(mlock, [&] {return !queue_.empty(); });
    }
    item = queue_.front();
    queue_.pop();
  }
 
  void send(const T& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(std::move(item));
    notify();
  }
 
  void send(T&& item)
  {
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push(std::move(item));
    notify();
  }

  unsigned int size()  { return queue_.size(); }
  bool empty()  { return queue_.empty(); } 


 private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;

  void notify() { cond_.notify_all(); }
};
#endif