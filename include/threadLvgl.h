#ifndef __THREADLVGL_H
#define __THREADLVGL_H

#include "mbed.h"

class ThreadLvgl
{
protected:
    Mutex mutex;
    void runLvgl();
    static void run(ThreadLvgl *p)
    {
        p->runLvgl();
    }
    Thread m_mainThread;
    void displayInit();
    void touchpadInit();  
public:
    ThreadLvgl();
    ~ThreadLvgl();
    void lock() { mutex.lock(); }
    void unlock() { mutex.unlock(); }
};

#endif
