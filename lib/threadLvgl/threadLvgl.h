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
    typedef enum {
          IHM_FLAG_DEPART           = 0x00000001,
          IHM_FLAG_REFRESH_SD       = 0x00000002,
          IHM_FLAG_RECALAGE         = 0x00000004,
          IHM_FLAG_START            = 0x00000008,
          IHM_FLAG_START_CANCEL     = 0x00000010 } IhmFlag;
    EventFlags flags;
    ThreadLvgl *m_threadLvgl;
    lv_style_t styleTitre;
    // Initialisation de la carte SD
    lv_obj_t *tabView;
    lv_obj_t *msgSdInit1, *msgSdInit2;
    lv_obj_t *tabSdInit;
    lv_obj_t *refreshSD;
    // Onglet "match"
    lv_obj_t *tabMatch;
    lv_obj_t *roller;
    lv_obj_t *couleur;
    lv_obj_t *depart;
    int departCouleur;
    int departStrategie;
    // Message Box recalage
    lv_obj_t *msgBoxRecalage;
    // Message Box jack
    lv_obj_t *msgBoxJack;

    void sdInit(lv_obj_t *parent);
    static void eventHandler(lv_event_t *e);
    bool getFlag(IhmFlag f, bool clearIfSet = true);
public:
    enum {VERT = 0, BLEU = 1};
    Ihm(ThreadLvgl *t);
    void sdMsg(const char *msg1, const char *msg2 = "");
    void matchInit(const vector <string> fichiers);
    void matchRollerSetOptions(const vector <string> fichiers, bool lock = true);
    bool departClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_DEPART, clearIfSet); }
    bool refreshSDClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_REFRESH_SD, clearIfSet); }
    bool recalageClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_RECALAGE, clearIfSet); }
    bool jackSimuleClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_START, clearIfSet); }
    bool jackAnnuleClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_START_CANCEL, clearIfSet); }
    int choixStrategie() { return departStrategie; }
    int choixCouleur() { return departCouleur; }
    void msgBoxRecalageInit(const string &strategie);
    void msgBoxJackInit();
    void msgBoxJackClose();
};

#endif
