/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include <instruction.h>
#include <strategie.h>
//#include <debug.h>
#include <deplacement.h>
#include <global.h>
#include <herkulex.h>
#include <lvgl.h>
#include <config.h>
#include <threadCAN.h>
#include <threadLvgl.h>
#include <ihm.h>
#include <threadSD.h>
#include <threadSound.h>

#include <fstream>
#include <sstream>

#include "mbed.h"


BufferedSerial pc(SERIAL_TX, SERIAL_RX,921600);

ThreadCAN threadCAN;
ThreadSD threadSD;
ThreadLvgl threadLvgl;
Ihm ihm(&threadLvgl);
Deplacement deplacement(threadCAN);
Herkulex herkulex(threadCAN);

DigitalIn jackPin(D7);

Thread *recalage;
void runRecalage();
int recalageErreur;  // 0 si recalage réussi, valeur négative sinon

Thread *match;
void runMatch();
bool jack();
Timeout timerMatch;
int tempsRestant();
void forceFinMatch();
volatile bool flagForceFinMatch = false;
bool Recalage = true;

vector<string> fichiers;
bool listeFichiers();
bool lectureFichier(int choix);

vector<string> fichiersMp3;
bool listeFichiersMp3();

int actionneurPinceArriere[2]={2,1}, actionneurPinceArriereTab = 0, actionneurPinceAavant = 0, actionneurStepMotor = 0;

int main() {
    char buf[100];


    threadSD.registerCANControl(threadCAN);
    threadCAN.registerIds(0x01, 0x7FF, canProcessRx);
    int secReboot = 60;
    while (1) {
        int flag;
        flag = threadSD.status();
        if (flag & ThreadSD::FLAG_NO_CARD) {
            sprintf(buf, "Reboot dans %2ds", secReboot);
            ihm.sdMsg("Carte SD absente", buf);
        } else {
            ihm.sdMsg("Carte SD présente");
        }
        if (flag & ThreadSD::FLAG_READY)
            break;
        ThisThread::sleep_for(1s);
        if (secReboot-- <= 0)
            NVIC_SystemReset();
    }

    readConfig();

    if (!listeFichiers()) {
        string msg = "Dossier " + config["Dossiers"]["strategie"] + " non trouvé";
        ihm.sdMsg(nullptr, msg.c_str());
    } else if (fichiers.size() == 0) {
        ihm.sdMsg(nullptr, "Aucun fichier trouvé");
    } else {
        int nb = fichiers.size();
        if (nb == 1) {
            sprintf(buf, "Un fichier trouvé");
        } else {
            sprintf(buf, "%d fichiers trouvés", nb);
        }
        ihm.sdMsg(nullptr, buf);
    }
    
    ihm.matchInit(fichiers);
    ihm.recalagePositionInit();
    ihm.ActionneurInit();
    listeFichiersMp3();
    ihm.configInit(fichiersMp3, ThreadSound::volume());

    int etat = 0;
    int choix = -1;
    int tempsAffiche = 100;

    while (1) {
        switch (etat) {
            case 0:
                if (ihm.departClicked()) {
                    choix = ihm.choixStrategie();
                    if (choix == -1) {
                        ihm.msgBoxRecalageInit("[Stratégie par défaut]");
                    } else {
                        ihm.msgBoxRecalageInit(fichiers[choix].c_str());
                    }
                } else if (ihm.refreshSDClicked()) {
                    listeFichiers();
                    ihm.matchRollerSetOptions(fichiers);
                } else if (ihm.recalageClicked()) {
                    ihm.msgBoxRecalageClose();
                    ihm.msgBoxInit("Lecture de la stratégie\n", "En cours", false);
                    ThisThread::sleep_for(1s);
                    etat = 10;
                } else if (ihm.recalageHautGaucheClicked()) {
                    // printf("recalage_HautGaucheClicked\n");
                    Hauteur = ROBOT_EN_HAUT;
                    Cote = ROBOT_A_GAUCHE;
                } else if (ihm.recalageBasGaucheClicked()) {
                    // printf("recalage_BasGaucheClicked\n");
                    Hauteur = ROBOT_EN_BAS;
                    Cote = ROBOT_A_GAUCHE;
                } else if (ihm.recalageHautDroitClicked()) {
                    // printf("recalage_HautDroitClicked\n");
                    Hauteur = ROBOT_EN_HAUT;
                    Cote = ROBOT_A_DROITE;
                } else if (ihm.recalageBasDroitClicked()) {
                    // printf("recalage_BasDroitClicked\n");
                    Hauteur = ROBOT_EN_BAS;
                    Cote = ROBOT_A_DROITE;
                } else if (ihm.activationRecalageClicked()) {
                    if (Recalage) {
                        Recalage = false;
                    } else {
                        Recalage = true;
                    }
                } else if(ihm.actionneurPinceArriereClicked()){
                    printf("actionneurPinceArriereClicked\n");
                    uint8_t etat_pince = actionneurPinceArriere[actionneurPinceArriereTab]; // Serrer : 1, lacher : 0
                    bool poseCerise = false;
                    if(etat_pince == 1){poseCerise = true;}
                    herkulex.controlePinceArriere(etat_pince, poseCerise);
                    actionneurPinceArriereTab = !actionneurPinceArriereTab;

                } else if(ihm.actionneurPinceAvantClicked()){
                    printf("actionneurPinceAvantClicked\n");
                    actionneurPinceAavant = !actionneurPinceAavant;
                    herkulex.controlePince(0, actionneurPinceAavant,0);
                } else if(ihm.actionneurPoseCeriseClicked()){
                    printf("actionneurPoseCeriseClicked\n");
                    herkulex.poseCerise(false);
                } else if(ihm.actionneurStepMotorClicked()){
                    printf("actionneurStepMotorClicked\n");
                    actionneurStepMotor = !actionneurStepMotor;
                    actionneurStepMotor = (actionneurStepMotor != 0) ? 4 : 0;
                    herkulex.controlePince(actionneurStepMotor, 0,0);
                } else if (ihm.playClicked()) {
                    if (fichiersMp3.size()>0) {
                        ThreadSound::playMp3(("/sd" + config["Dossiers"]["musique"] + "/" + fichiersMp3[ihm.choixMp3()]).c_str());
                    }
                } else if (ihm.stopClicked()) {
                    ThreadSound::stop();
                } else if (ihm.saveConfigClicked()) {
                    config["Audio"]["volume"] = std::to_string(ihm.choixVolume());
                    writeConfig();
                } else if (ihm.resetClicked()) {
                    NVIC_SystemReset();
                } else if (ihm.volumeChanged()) {
                    ThreadSound::setVolume(ihm.choixVolume());
                }
                if (!ThreadSound::isPlaying()) ihm.configStopPlaying();
                break;

            case 1:
                if (ihm.msgBoxCancelClicked()) {
                    // annuler le recalage en cours
                    recalage->terminate();
                    delete recalage;
                    recalage = nullptr;
                    etat = 0;
                } else if (!Recalage) {
                    ihm.msgBoxClose();
                    ihm.msgBoxJackInit();
                    etat = 2;
                } else {
                    // si fin recalage :
                    if (recalage->get_state() == Thread::Deleted) {
                        delete recalage;
                        recalage = nullptr;
                        ihm.msgBoxClose();
                        if (recalageErreur == 0) {
                            ihm.msgBoxJackInit();
                            etat = 2;
                        } else {
                            sprintf(buf, "Problème recalage %d", recalageErreur);
                            ihm.msgBoxInit("Erreur\n", buf, true);
                            etat = 5;
                        }
                    }
                }
                break;

            case 2:
                if (ihm.jackAnnuleClicked()) {
                    etat = 0;
                } else if (ihm.jackSimuleClicked() || jack()) {
                    flagForceFinMatch = false;
                    tempsAffiche = 100;
                    timerMatch.attach(forceFinMatch, std::chrono::seconds(tempsAffiche));
                    // démarrer le match
                    match = new Thread;
                    match->start(runMatch);
                    ihm.msgBoxJackClose();
                    ihm.msgBoxInit("Match\n", "En cours\n", true);
                    etat = 3;
                }
                break;

            case 3:
                // Le match a démarré
                if (ihm.msgBoxCancelClicked()) {
                    // Annuler le match en cours
                    match->terminate();
                    delete match;
                    match = nullptr;
                    etat = 0;
                } else {
                    // Si fin du match
                    if (flagForceFinMatch) {
                        match->terminate();
                        match->join();
                        delete match;
                        match = nullptr;
                        etat = 4;
                    } else if (match->get_state() == Thread::Deleted) {
                        delete match;
                        match = nullptr;
                        etat = 4;
                    } else {
                        int t = tempsRestant() + 1;
                        if (tempsAffiche != t) {
                            tempsAffiche = t;
                            if (tempsAffiche > 0) {
                                sprintf(buf, "Reste %d s", tempsAffiche);
                                ihm.msgBoxMessage(buf);
                            } else {
                                ihm.msgBoxMessage("Terminé");
                            }
                        }
                    }
                }
                break;

            case 4:
                // Affichage du score
                sprintf(buf, "Terminé\n\nScore = %d", SCORE_GLOBAL);
                ihm.msgBoxMessage(buf);
                etat = 5;
                break;

            case 5:
                if (ihm.msgBoxCancelClicked()) {
                    etat = 0;
                }
                break;

            case 10:
                if (lectureFichier(choix)) {
                    etat = 1;
                    if (Recalage) {
                        // démarrer le recalage
                        recalage = new Thread;
                        recalage->start(runRecalage);
                        ihm.msgBoxClose();
                        ihm.msgBoxInit("Recalage en cours\n", "Attente\n", true);
                    }
                } else {
                    etat = 5;
                    ihm.msgBoxClose();
                    ihm.msgBoxInit("Erreur\n", "Problème fichier\n", true);
                }
                break;
        }
        ThisThread::sleep_for(10ms);
    }
}

bool listeFichiers() {
    // Vide la liste de fichiers
    fichiers.clear();
    // Attend que la carte SD soit prête
    threadSD.waitReady();
    // Se déplace dans le dossier des stratégies et liste les fichiers présents
    string reply = threadSD.cdName(config["Dossiers"]["strategie"].c_str());
    // Vérifie que le dossier des stratégies existe
    if (reply.find(config["Dossiers"]["strategie"].c_str()) != 0) {
        return false;
    }
    // Récupère le résultat sous la forme /chemin*dossier1*dossier2*dossier3:fichier1:fichier2:fichier3?   * pour dossier  : pour fichier  ? pour fin
    // Enlève le ? à la fin
    if (!reply.empty()) {
        reply.pop_back();
    }
    stringstream txtStream(reply);
    string item;
    // Ignore tous les dossiers
    if (getline(txtStream, item, ':')) {
        while (getline(txtStream, item, ':')) {
            // Range chaque nom de fichier dans un tableau de string
            fichiers.push_back(item);
        }
    }
    return true;
}

bool lectureFichier(int choix) {
    string ficStrat;
    if (choix < 0) {
        // Que faire si choix == -1 ????
        return false;
    }
    ficStrat = "/sd" + config["Dossiers"]["strategie"] + "/" + fichiers[choix];
    ifstream monFlux(ficStrat);  // Ouverture d'un fichier en lecture
    if (monFlux) {
        // Tout est prêt pour la lecture.
        string ligne;
        while (getline(monFlux, ligne)) {  // On lit une ligne complète
            // printf("%s\n", ligne.c_str());
            listeInstructions.ajout(ligne.c_str());
            // debug_Instruction(listeInstructions.derniere());
        }
        monFlux.close();
        return true;
    }
    // ERREUR: Impossible d'ouvrir le fichier en lecture
    // On fait la même chose que pour choix == -1 ????
    return false;
}

bool listeFichiersMp3() {
    // Vide la liste de fichiers
    fichiersMp3.clear();
    // Attend que la carte SD soit prête
    threadSD.waitReady();
    // Se déplace dans le dossier des mp3 et liste les fichiers présents
    string reply = threadSD.cdName(config["Dossiers"]["musique"].c_str());
    // Vérifie que le dossier des musique existe
    if (reply.find(config["Dossiers"]["musique"].c_str()) != 0) {
        return false;
    }
    // Récupère le résultat sous la forme /chemin*dossier1*dossier2*dossier3:fichier1:fichier2:fichier3?   * pour dossier  : pour fichier  ? pour fin
    // Enlève le ? à la fin
    if (!reply.empty()) {
        reply.pop_back();
    }
    stringstream txtStream(reply);
    string item;
    // Ignore tous les dossiers
    if (getline(txtStream, item, ':')) {
        while (getline(txtStream, item, ':')) {
            // Range chaque nom de fichier dans un tableau de string si extension mp3
            if (item.substr(item.find_last_of(".") + 1) == "mp3") {
                fichiersMp3.push_back(item);
            }
        }
    }
    return true;
}

void runRecalage() {
    if (machineRecalageInit()) {
        while (machineRecalage())
            ;
    }
}

void runMatch() {
    gameEtat = ETAT_GAME_LOAD_NEXT_INSTRUCTION;
    while (machineStrategie());
}

// Retourne true si le jack est retiré
bool jack() {
    return (jackPin.read());
}

void forceFinMatch() {
    flagForceFinMatch = true;
}

int tempsRestant() {
    return std::chrono::duration_cast<std::chrono::seconds>(timerMatch.remaining_time()).count();
}
