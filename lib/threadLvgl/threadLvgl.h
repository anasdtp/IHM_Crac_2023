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
    enum {IHM_FLAG_DEPART = 1};
    EventFlags flags;
    ThreadLvgl *m_threadLvgl;
    lv_style_t styleTitre;
    // Initialisation de la carte SD
    lv_obj_t *tabView;
    lv_obj_t *msgSdInit1, *msgSdInit2;
    lv_obj_t *tabSdInit;
    // Onglet "match"
    lv_obj_t *tabMatch;
    lv_obj_t *roller;
    lv_obj_t *couleur;
    lv_obj_t *depart;
    int departCouleur;
    int departStrategie;
    void sdInit(lv_obj_t *parent);
    static void eventHandler(lv_event_t *e);
public:
    enum {VERT = 0, BLEU = 1};
    Ihm(ThreadLvgl *t);
    void sdMsg(const char *msg1, const char *msg2 = "");
    void matchInit(const vector <string> fichiers);
    void matchRollerSetOptions(const vector <string> fichiers, bool lock = true);
    bool departClicked(bool clearIfSet = true);
    int choixStrategie() { return departStrategie; }
    int choixCouleur() { return departCouleur; }
};

#endif
