#ifndef __IHM_H
#define __IHM_H

#include "mbed.h"
#include "lvgl.h"
#include <threadLvgl.h>
#include <vector>


#define FONT_NORMAL &liberation_24
#define FONT_LARGE &liberation_40

class Ihm
{
protected:
    typedef enum
    {
        IHM_FLAG_DEPART =                   (1UL << 0),
        IHM_FLAG_REFRESH_SD =               (1UL << 1),
        IHM_FLAG_RECALAGE =                 (1UL << 2),
        IHM_FLAG_START =                    (1UL << 3),
        IHM_FLAG_START_CANCEL =             (1UL << 4),
        IHM_FLAG_MSGBOX_CANCEL =            (1UL << 5),
        IHM_FLAG_RECALAGE_HAUTGAUCHE =      (1UL << 6),
        IHM_FLAG_RECALAGE_HAUTDROIT =       (1UL << 7),
        IHM_FLAG_RECALAGE_BASGAUCHE =       (1UL << 8),
        IHM_FLAG_RECALAGE_BASDROIT =        (1UL << 9),
        IHM_FLAG_RECALAGE_ETAT =            (1UL << 10),
        IHM_FLAG_PLAY =                     (1UL << 11),
        IHM_FLAG_STOP =                     (1UL << 12),
        IHM_FLAG_SAVE_CONFIG =              (1UL << 13),
        IHM_FLAG_RESET =                    (1UL << 14),
        IHM_FLAG_VOLUME =                   (1UL << 15),
        IHM_FLAG_ACTIONNEUR_POSE_CERISE =   (1UL << 16),
        IHM_FLAG_ACTIONNEUR_PINCE_ARRIERE = (1UL << 17),
        IHM_FLAG_ACTIONNEUR_PINCE_AVANT =   (1UL << 18),
        IHM_FLAG_ACTIONNEUR_STEP_MOTOR =    (1UL << 19),
        
    } IhmFlag;
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
    // Onglet "recalage"
    lv_obj_t *tabRecalage;
    lv_obj_t *hautGauche;
    lv_obj_t *basGauche;
    lv_obj_t *hautDroit;
    lv_obj_t *basDroit;
    lv_obj_t *etatRecalage;
    // Onglet "config"
    lv_obj_t *tabConfig;
    lv_obj_t *configRoller;
    lv_obj_t *configPlay;
    lv_obj_t *labelPlay;
    lv_obj_t *configVolume;
    lv_obj_t *labelVolume;
    lv_obj_t *configSave;
    lv_obj_t *configReset;
    //Onglet "test actionneur"
    lv_obj_t *tabActionneur;
    lv_obj_t *poseCerise;
    lv_obj_t *PinceArriere;
    lv_obj_t *PinceAvant;
    lv_obj_t *stepMotor;
    
    int volume;
    int mp3;

    // Message Box recalage
    lv_obj_t *msgBoxRecalage;
    // Message Box jack
    lv_obj_t *msgBoxJack;
    // Message Box générique
    lv_obj_t *msgBox;

    void sdInit(lv_obj_t *parent);
    static void eventHandler(lv_event_t *e);
    bool getFlag(IhmFlag f, bool clearIfSet = true);

public:
    enum
    {
        VERT = 0,
        BLEU = 1
    };
    Ihm(ThreadLvgl *t);
    void sdMsg(const char *msg1, const char *msg2 = "");
    void matchInit(const vector<string> fichiers);
    void matchRollerSetOptions(const vector<string> fichiers, bool lock = true);
    void recalagePositionInit();
    bool departClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_DEPART, clearIfSet); }
    bool refreshSDClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_REFRESH_SD, clearIfSet); }
    bool recalageClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_RECALAGE, clearIfSet); }
    bool jackSimuleClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_START, clearIfSet); }
    bool jackAnnuleClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_START_CANCEL, clearIfSet); }
    bool msgBoxCancelClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_MSGBOX_CANCEL, clearIfSet); }
    bool recalageHautGaucheClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_RECALAGE_HAUTGAUCHE, clearIfSet); }
    bool recalageBasGaucheClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_RECALAGE_BASGAUCHE, clearIfSet); }
    bool recalageHautDroitClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_RECALAGE_HAUTDROIT, clearIfSet); }
    bool recalageBasDroitClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_RECALAGE_BASDROIT, clearIfSet); }
    bool activationRecalageClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_RECALAGE_ETAT, clearIfSet); }
    bool playClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_PLAY, clearIfSet); }
    bool stopClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_STOP, clearIfSet); }
    bool saveConfigClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_SAVE_CONFIG, clearIfSet); }
    bool resetClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_RESET, clearIfSet); }
    bool volumeChanged(bool clearIfSet = true) { return getFlag(IHM_FLAG_VOLUME, clearIfSet); }
    bool actionneurPoseCeriseClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_ACTIONNEUR_POSE_CERISE, clearIfSet); }
    bool actionneurPinceArriereClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_ACTIONNEUR_PINCE_ARRIERE, clearIfSet); }
    bool actionneurPinceAvantClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_ACTIONNEUR_PINCE_AVANT, clearIfSet); }
    bool actionneurStepMotorClicked(bool clearIfSet = true) { return getFlag(IHM_FLAG_ACTIONNEUR_STEP_MOTOR, clearIfSet); }
    int choixStrategie() { return departStrategie; }
    int choixCouleur() { return departCouleur; }
    int choixVolume() { return volume; }
    int choixMp3() { return mp3; }
    void msgBoxRecalageInit(const string &strategie);
    void msgBoxRecalageClose();
    void msgBoxJackInit();
    void msgBoxJackClose();
    void msgBoxInit(const char *titre, const char *msg, bool boutonAnnuler);
    void msgBoxMessage(const char *msg);
    void msgBoxClose();
    void configInit(const vector<string> fichiers, int v);
    void ActionneurInit();
    void configRollerSetOptions(const vector<string> fichiers, bool lock = true);
    void configStopPlaying();
};

#endif
