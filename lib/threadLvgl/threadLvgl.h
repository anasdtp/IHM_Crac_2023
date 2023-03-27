#ifndef __THREADLVGL_H
#define __THREADLVGL_H

#include "mbed.h"
#include "lvgl.h"
#include <vector>

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
    static void refreshDisplay(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
    void touchpadInit();
    static void touchpadRead(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
    lv_indev_t *indevTouchpad;    
    static bool refreshEnabled;
    Ticker lvTicker;
    static int refreshTime;
    static void lvTimeCounter();
    void setRefreshEnable(bool enabled = true) { refreshEnabled = enabled; }
public:
    ThreadLvgl(int refreshTimeInit = 5);
    ~ThreadLvgl();
    void lock() { mutex.lock(); }
    void unlock() { mutex.unlock(); }
};

#define FONT_NORMAL     &liberation_24
#define FONT_LARGE      &liberation_40

class Ihm
{
protected:
    ThreadLvgl *m_threadLvgl;
    lv_style_t styleTitre;
    lv_obj_t *tabView;
    lv_obj_t *tabMatch;
    lv_obj_t *msgSdInit1, *msgSdInit2;
    lv_obj_t *tabSdInit;
    void sdInit(lv_obj_t *parent);
public:
    Ihm(ThreadLvgl *t);
    void sdMsg(const char *msg1, const char *msg2 = "");
    void matchInit(vector <string> fichiers);
};

#endif
