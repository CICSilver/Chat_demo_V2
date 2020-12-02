#ifndef _RESPTHREADPOOL_H
#define _RESPTHREADPOOL_H

#include "EvHttpResp.h"
#include <thread>
#include <vector>
#include <condition_variable>

using namespace std;

class RespThreadPool
{
private:
    vector<thread> pool;
    mutex m_lock;
    condition_variable cv;
    //收到消息的用户名
    vector<string> recv_msg_mem;
    int threadCount = 0;

    int threadNum;

    RespThreadPool(){};
    void Init();
    void Dispatch();

public:
    static RespThreadPool* Get()
    {
        static RespThreadPool rp;
        return &rp;
    }

    void AddThread(string to)
    {
        pool.emplace_back([this]() {
            unique_lock<mutex> u_lock(this->m_lock);
            this->cv.wait(u_lock, )
        });
    }
};

#endif