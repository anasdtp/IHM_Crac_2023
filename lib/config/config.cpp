#include <config.h>
#include <mIni.h>
#include <string>
#include <threadSound.h>

static int verifieInt(std::string &cle, int min, int max, int parDefaut);

bool readConfig() {
    mINI::INIFile file("/sd/config.ini");
    file.read(config);
    // Dossier des stratégies
    std::string &dirStrat = config["Dossiers"]["strategie"];
    if (dirStrat == "") dirStrat = "/strategie";
    // Volume initial
    std::string &volume = config["Audio"]["volume"];
    int vInit = verifieInt(volume, 0, 100, 75);
    ThreadSound::setVolume(vInit);

    file.write(config);
    return true;
}

bool writeConfig() {
    mINI::INIFile file("/sd/config.ini");
    return file.write(config);
}

int verifieInt(std::string &cle, int min, int max, int parDefaut)
{
    int valeur;
    int nbConv = sscanf(cle.c_str(), "%d", &valeur);
    if (nbConv != 1) {
        valeur = parDefaut;
    }
    if (valeur < min) {
        valeur = min;
    } else if (valeur > max) {
        valeur = max;
    }
    cle = std::to_string(valeur);
    return valeur;
}

