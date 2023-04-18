#include <ihm.h>

Ihm::Ihm(ThreadLvgl *t) {
    m_threadLvgl = t;

    t->lock();

    lv_style_init(&styleTitre);
    lv_style_set_text_font(&styleTitre, FONT_LARGE);
    tabView = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 70);
    lv_obj_set_style_text_font(lv_scr_act(), FONT_NORMAL, 0);
    lv_obj_t *tab_btns = lv_tabview_get_tab_btns(tabView);
    lv_obj_t *label = lv_label_create(tab_btns);
    //    lv_obj_add_style(label, &styleTitre, 0);
    lv_label_set_text(label, "CRAC");
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 10, 0);
    tabSdInit = lv_tabview_add_tab(tabView, "Carte SD");
    sdInit(tabSdInit);

    t->unlock();
}

void Ihm::sdInit(lv_obj_t *parent) {
    lv_obj_t *titre = lv_label_create(parent);
    lv_label_set_text(titre, "Initialisation carte SD");
    lv_obj_align(titre, LV_ALIGN_CENTER, 0, -60);
    msgSdInit1 = lv_label_create(parent);
    lv_label_set_text(msgSdInit1, "");
    lv_obj_align(msgSdInit1, LV_ALIGN_CENTER, 0, -10);
    msgSdInit2 = lv_label_create(parent);
    lv_label_set_text(msgSdInit2, "");
    lv_obj_align(msgSdInit2, LV_ALIGN_CENTER, 0, 40);

    // exemple image

    // lv_obj_t *img1 = lv_img_create(parent);
    // lv_img_set_src(img1, "A:/images/minion.bin");
    // lv_obj_align(img1, LV_ALIGN_TOP_LEFT, 0, 0);
    // lv_obj_set_size(img1, 255, 255);
}

void Ihm::sdMsg(const char *msg1, const char *msg2) {
    m_threadLvgl->lock();
    if (msg1 != nullptr)
        lv_label_set_text(msgSdInit1, msg1);
    if (msg2 != nullptr)
        lv_label_set_text(msgSdInit2, msg2);
    m_threadLvgl->unlock();
}

void Ihm::matchInit(const vector<string> fichiers) {
    m_threadLvgl->lock();

    // Ajout d'un bouton sur l'onglet SD Init
    refreshSD = lv_btn_create(tabSdInit);
    lv_obj_t *label = lv_label_create(refreshSD);
    lv_label_set_text(label, "Rafraichir SD");
    lv_obj_center(label);
    lv_obj_align(refreshSD, LV_ALIGN_BOTTOM_MID, 0, -30);
    lv_obj_add_event_cb(refreshSD, Ihm::eventHandler, LV_EVENT_CLICKED, this);

    // Création de l'onglet tabMatch
    tabMatch = lv_tabview_add_tab(tabView, "Match");

    /*Column 1: 5 unit from the remaining free space
     *Column 2: 1 unit from the remaining free space*/
    static lv_coord_t col_dsc[] = {LV_GRID_FR(5), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    /*Row 1: 30 pixels
     *Row 2: 2 unit from the remaining free space
     *Row 3: 1 unit from the remaining free space*/
    static lv_coord_t row_dsc[] = {30, LV_GRID_FR(2), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    /*Create a container with grid*/
    lv_obj_t *cont = lv_obj_create(tabMatch);
    lv_obj_center(cont);
    lv_obj_update_layout(tabMatch);
    lv_obj_set_size(cont, lv_obj_get_content_width(tabMatch), lv_obj_get_content_height(tabMatch));
    lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);

    lv_obj_t *titre = lv_label_create(cont);
    lv_obj_set_style_text_font(titre, &liberation_24, 0);
    lv_label_set_text(titre, LV_SYMBOL_SD_CARD " Stratégie");
    lv_obj_set_grid_cell(titre, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);

    roller = lv_roller_create(cont);
    matchRollerSetOptions(fichiers, false);
    lv_obj_set_style_text_font(roller, &liberation_24, 0);
    lv_roller_set_selected(roller, 1, LV_ANIM_OFF);
    lv_obj_set_grid_cell(roller, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 2);

    couleur = lv_btn_create(cont);
    label = lv_label_create(couleur);
    lv_label_set_text(label, "Couleur");
    lv_obj_add_flag(couleur, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_bg_color(couleur, lv_color_make(0, 170, 18), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(couleur, lv_color_make(0, 92, 230), LV_STATE_CHECKED);
    lv_obj_center(label);
    lv_obj_set_grid_cell(couleur, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 2);

    depart = lv_btn_create(cont);
    label = lv_label_create(depart);
    lv_label_set_text(label, "Départ");
    lv_obj_set_style_bg_color(depart, lv_palette_main(LV_PALETTE_RED), LV_STATE_DEFAULT);
    lv_obj_center(label);
    lv_obj_set_grid_cell(depart, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 2, 1);
    lv_obj_add_event_cb(depart, Ihm::eventHandler, LV_EVENT_CLICKED, this);

    m_threadLvgl->unlock();
}

void Ihm::matchRollerSetOptions(const vector<string> fichiers, bool lock) {
    string choix;
    for (auto f : fichiers)
        choix += "\n" + f;
    if (lock)
        m_threadLvgl->lock();
    lv_roller_set_options(roller, choix.c_str(), LV_ROLLER_MODE_NORMAL);
    lv_tabview_set_act(tabView, 1, LV_ANIM_ON);
    if (lock)
        m_threadLvgl->unlock();
}

void Ihm::recalagePositionInit() {
    m_threadLvgl->lock();

    /*Column 1: 1 unit from the remaining free space
     *Column 2: 1 unit from the remaining free space*/
    static lv_coord_t col[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    /*Row 1: 30 pixels
     *Row 2: 1 unit from the remaining free space
     *Row 3: 1 unit from the remaining free space*/
    static lv_coord_t row[] = {30, LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    tabRecalage = lv_tabview_add_tab(tabView, "Recalage");
    /*Create a container with grid*/
    lv_obj_t *container = lv_obj_create(tabRecalage);
    lv_obj_center(container);
    lv_obj_update_layout(tabRecalage);
    lv_obj_set_size(container, lv_obj_get_content_width(tabRecalage), lv_obj_get_content_height(tabRecalage));
    lv_obj_set_grid_dsc_array(container, col, row);

    lv_obj_t *titreRecalage = lv_label_create(container);
    lv_obj_set_style_text_font(titreRecalage, &liberation_24, 0);
    lv_label_set_text(titreRecalage, LV_SYMBOL_EDIT " Choisir position robot");  // LV_SYMBOL_ENVELOPE LV_SYMBOL_DRIVE LV_SYMBOL_EDIT
    lv_obj_set_grid_cell(titreRecalage, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);

    hautGauche = lv_btn_create(container);
    lv_obj_t *label = lv_label_create(hautGauche);
    lv_label_set_text(label, "En Haut à gauche");
    lv_obj_set_style_bg_color(hautGauche, lv_palette_main(LV_PALETTE_LIGHT_BLUE), LV_STATE_DEFAULT);
    lv_obj_center(label);
    lv_obj_set_grid_cell(hautGauche, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_add_event_cb(hautGauche, Ihm::eventHandler, LV_EVENT_CLICKED, this);

    basGauche = lv_btn_create(container);
    label = lv_label_create(basGauche);
    lv_label_set_text(label, "En Bas à gauche");
    lv_obj_set_style_bg_color(basGauche, lv_palette_main(LV_PALETTE_LIGHT_BLUE), LV_STATE_DEFAULT);
    lv_obj_center(label);
    lv_obj_set_grid_cell(basGauche, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 2, 1);
    lv_obj_add_event_cb(basGauche, Ihm::eventHandler, LV_EVENT_CLICKED, this);

    hautDroit = lv_btn_create(container);
    label = lv_label_create(hautDroit);
    lv_label_set_text(label, "En Haut à droite");
    lv_obj_set_style_bg_color(hautDroit, lv_palette_main(LV_PALETTE_LIGHT_BLUE), LV_STATE_DEFAULT);
    lv_obj_center(label);
    lv_obj_set_grid_cell(hautDroit, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_add_event_cb(hautDroit, Ihm::eventHandler, LV_EVENT_CLICKED, this);

    basDroit = lv_btn_create(container);
    label = lv_label_create(basDroit);
    lv_label_set_text(label, "En Bas à droite");
    lv_obj_set_style_bg_color(basDroit, lv_palette_main(LV_PALETTE_LIGHT_BLUE), LV_STATE_DEFAULT);
    lv_obj_center(label);
    lv_obj_set_grid_cell(basDroit, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 2, 1);
    lv_obj_add_event_cb(basDroit, Ihm::eventHandler, LV_EVENT_CLICKED, this);

    etatRecalage = lv_btn_create(container);
    label = lv_label_create(etatRecalage);
    lv_label_set_text(label, "Activation Recalage");
    lv_obj_add_flag(etatRecalage, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_bg_color(etatRecalage, lv_palette_main(LV_PALETTE_GREEN), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(etatRecalage, lv_palette_main(LV_PALETTE_RED), LV_STATE_CHECKED);
    lv_obj_center(label);
    lv_obj_set_grid_cell(etatRecalage, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_add_event_cb(etatRecalage, Ihm::eventHandler, LV_EVENT_CLICKED, this);

    m_threadLvgl->unlock();
}

void Ihm::configInit(const vector<string> fichiers, int volume) {
    m_threadLvgl->lock();

    // Création de l'onglet tabConfig
    tabConfig = lv_tabview_add_tab(tabView, "Config");

    /*Column 1: 1 unit from the remaining free space
     ...
     *Column 5: 1 unit from the remaining free space*/
    static lv_coord_t col_dsc[] = {LV_GRID_FR(2), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    /*7 rows same height */
    static lv_coord_t row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
                                   LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    /*Create a container with grid*/
    lv_obj_t *cont = lv_obj_create(tabConfig);
    lv_obj_center(cont);
    lv_obj_update_layout(tabConfig);
    lv_obj_set_size(cont, lv_obj_get_content_width(tabConfig), lv_obj_get_content_height(tabConfig));
    lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);

    configRoller = lv_roller_create(cont);
    configRollerSetOptions(fichiers, false);
    lv_obj_set_style_text_font(configRoller, &liberation_24, 0);
    lv_roller_set_selected(configRoller, 1, LV_ANIM_OFF);
    lv_obj_set_grid_cell(configRoller, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 5);

    configPlay = lv_btn_create(cont);
    labelPlay = lv_label_create(configPlay);
    lv_label_set_text(labelPlay, LV_SYMBOL_PLAY);
    lv_obj_add_flag(configPlay, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_center(labelPlay);
    lv_obj_set_grid_cell(configPlay, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 6, 1);
    lv_obj_add_event_cb(configPlay, Ihm::eventHandler, LV_EVENT_CLICKED, this);

    /*Create a slider */
    configVolume = lv_slider_create(cont);
    /*Create a label */
    lv_obj_t *slider_label = lv_label_create(cont);
    lv_label_set_text_fmt(slider_label, "Volume : %d", volume);
    lv_obj_set_grid_cell(slider_label, LV_GRID_ALIGN_CENTER, 0, 1,
                         LV_GRID_ALIGN_START, 0, 1);
    lv_obj_set_grid_cell(configVolume, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_END, 0, 1);
    // lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    // depart = lv_btn_create(cont);
    // label = lv_label_create(depart);
    // lv_label_set_text(label, "Départ");
    // lv_obj_set_style_bg_color(depart, lv_palette_main(LV_PALETTE_RED), LV_STATE_DEFAULT);
    // lv_obj_center(label);
    // lv_obj_set_grid_cell(depart, LV_GRID_ALIGN_STRETCH, 1, 1,
    //                      LV_GRID_ALIGN_STRETCH, 2, 1);
    // lv_obj_add_event_cb(depart, Ihm::eventHandler, LV_EVENT_CLICKED, this);

    m_threadLvgl->unlock();
}

void Ihm::configRollerSetOptions(const vector<string> fichiers, bool lock) {
    string choix = "";
    for (auto f : fichiers)
        choix += f + "\n";
    if (choix != "") choix.pop_back();
    if (lock)
        m_threadLvgl->lock();
    lv_roller_set_options(configRoller, choix.c_str(), LV_ROLLER_MODE_NORMAL);
    if (lock)
        m_threadLvgl->unlock();
}

void Ihm::eventHandler(lv_event_t *e) {
    Ihm *ihm = static_cast<Ihm *>(lv_event_get_user_data(e));
    lv_obj_t *emetteur = lv_event_get_current_target(e);

    if (emetteur == ihm->depart) {
        ihm->departCouleur = lv_obj_get_state(ihm->couleur);
        ihm->departStrategie = int(lv_roller_get_selected(ihm->roller)) - 1;
        ihm->flags.set(IHM_FLAG_DEPART);
    } else if (emetteur == ihm->refreshSD) {
        ihm->flags.set(IHM_FLAG_REFRESH_SD);
    } else if (emetteur == ihm->msgBoxRecalage) {
        ihm->flags.set(IHM_FLAG_RECALAGE);
    } else if (emetteur == ihm->msgBoxJack) {
        if (lv_event_get_code(e) == LV_EVENT_DELETE) {
            ihm->flags.set(IHM_FLAG_START_CANCEL);
            ihm->msgBoxJack = nullptr;
        } else {
            ihm->flags.set(IHM_FLAG_START);
        }
    } else if (emetteur == ihm->msgBox) {
        ihm->flags.set(IHM_FLAG_MSGBOX_CANCEL);
        ihm->msgBox = nullptr;
    } else if (emetteur == ihm->hautGauche) {
        ihm->flags.set(IHM_FLAG_RECALAGE_HAUTGAUCHE);
    } else if (emetteur == ihm->hautDroit) {
        ihm->flags.set(IHM_FLAG_RECALAGE_HAUTDROIT);
    } else if (emetteur == ihm->basGauche) {
        ihm->flags.set(IHM_FLAG_RECALAGE_BASGAUCHE);
    } else if (emetteur == ihm->basDroit) {
        ihm->flags.set(IHM_FLAG_RECALAGE_BASDROIT);
    } else if (emetteur == ihm->etatRecalage) {
        ihm->flags.set(IHM_FLAG_RECALAGE_ETAT);
    } else if (emetteur == ihm->configPlay) {
        lv_state_t etat = lv_obj_get_state(emetteur);
        if (etat & LV_STATE_CHECKED) {
            lv_label_set_text(ihm->labelPlay, LV_SYMBOL_STOP);
            ihm->flags.set(IHM_FLAG_PLAY);
        } else {
            lv_label_set_text(ihm->labelPlay, LV_SYMBOL_PLAY);
            ihm->flags.set(IHM_FLAG_STOP);
        }
    }
}

bool Ihm::getFlag(IhmFlag f, bool clearIfSet) {
    if (flags.get() & f) {
        if (clearIfSet)
            flags.clear(f);
        return true;
    }
    return false;
}

void Ihm::msgBoxRecalageInit(const string &strategie) {
    m_threadLvgl->lock();
    static const char *btns[] = {"Ok", ""};
    string str = "Stratégie : " + strategie;
    str += string("\nCouleur : ") + ((departCouleur == 0) ? string("VERT\n") : string("BLEU\n"));
    msgBoxRecalage = lv_msgbox_create(NULL, "Recalage\n", str.c_str(), btns, true);
    lv_obj_set_size(msgBoxRecalage, 740, 420);
    lv_obj_center(msgBoxRecalage);
    lv_obj_set_style_bg_color(msgBoxRecalage, (departCouleur == 0) ? lv_color_make(0, 170, 18) : lv_color_make(0, 92, 230), 0);
    lv_obj_set_style_text_color(msgBoxRecalage, lv_color_white(), 0);
    lv_obj_t *boxbtns = lv_msgbox_get_btns(msgBoxRecalage);
    lv_obj_update_layout(msgBoxRecalage);
    lv_obj_set_width(boxbtns, lv_obj_get_content_width(msgBoxRecalage));
    lv_obj_set_height(boxbtns, lv_obj_get_content_height(msgBoxRecalage) - lv_obj_get_y(boxbtns));
    lv_obj_align(boxbtns, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(msgBoxRecalage, eventHandler, LV_EVENT_VALUE_CHANGED, this);
    m_threadLvgl->unlock();
}

void Ihm::msgBoxRecalageClose() {
    m_threadLvgl->lock();
    lv_msgbox_close(msgBoxRecalage);
    msgBoxRecalage = nullptr;
    m_threadLvgl->unlock();
}

void Ihm::msgBoxJackInit() {
    m_threadLvgl->lock();
    static const char *btns[] = {"Jack simulé", ""};
    msgBoxJack = lv_msgbox_create(NULL, "\n" LV_SYMBOL_WARNING " Attention au départ " LV_SYMBOL_WARNING "\n", "Tirer le jack pour démarrer !\n", btns, true);
    lv_obj_set_size(msgBoxJack, 740, 420);
    lv_obj_center(msgBoxJack);
    lv_obj_t *titre = lv_msgbox_get_title(msgBoxJack);
    lv_obj_set_style_text_color(titre, lv_color_make(255, 0, 0), 0);
    lv_obj_t *boxbtns = lv_msgbox_get_btns(msgBoxJack);
    lv_obj_update_layout(msgBoxJack);
    lv_obj_set_width(boxbtns, lv_obj_get_content_width(msgBoxJack));
    lv_obj_set_height(boxbtns, lv_obj_get_content_height(msgBoxJack) - lv_obj_get_y(boxbtns));
    lv_obj_align(boxbtns, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(msgBoxJack, eventHandler, LV_EVENT_VALUE_CHANGED, this);
    lv_obj_add_event_cb(msgBoxJack, eventHandler, LV_EVENT_DELETE, this);
    m_threadLvgl->unlock();
}

void Ihm::msgBoxJackClose() {
    m_threadLvgl->lock();
    lv_msgbox_close(msgBoxJack);
    // Efface le flag de fermeture de la message box
    jackAnnuleClicked();
    msgBoxJack = nullptr;
    m_threadLvgl->unlock();
}

void Ihm::msgBoxInit(const char *titre, const char *msg, bool boutonAnnuler) {
    m_threadLvgl->lock();
    static const char *btns[] = {""};
    msgBox = lv_msgbox_create(NULL, titre, msg, btns, boutonAnnuler);
    lv_obj_set_size(msgBox, 740, 420);
    lv_obj_center(msgBox);
    lv_obj_t *titreObj = lv_msgbox_get_title(msgBox);
    lv_obj_set_style_text_color(titreObj, lv_color_make(255, 0, 0), 0);
    lv_obj_add_event_cb(msgBox, eventHandler, LV_EVENT_DELETE, this);
    m_threadLvgl->unlock();
}

void Ihm::msgBoxClose() {
    m_threadLvgl->lock();
    lv_msgbox_close(msgBox);
    // Efface le flag de fermeture de la message box
    msgBoxCancelClicked();
    msgBox = nullptr;
    m_threadLvgl->unlock();
}

void Ihm::msgBoxMessage(const char *msg) {
    if (msgBox) {
        m_threadLvgl->lock();
        lv_obj_t *txt = lv_msgbox_get_text(msgBox);
        lv_label_set_text(txt, msg);
        m_threadLvgl->unlock();
    }
}
