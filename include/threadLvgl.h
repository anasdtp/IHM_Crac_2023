#ifndef __THREADLVGL_H
#define __THREADLVGL_H

#include "mbed.h"

class ThreadLvgl
{
protected:
    void runLvgl();
    static void run(ThreadLvgl *p)
    {
        p->runLvgl();
    }
    Thread m_mainThread;
    void touchpadInit();  
public:
    Mutex mutex;
    ThreadLvgl();
    ~ThreadLvgl();
};

#endif
