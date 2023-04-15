#include "threadLvgl.h"
#include "Drivers/STM32469I-Discovery/stm32469i_discovery_lcd.h"
#include "Drivers/STM32469I-Discovery/stm32469i_discovery_ts.h"
#include "src/draw/stm32_dma2d/lv_gpu_stm32_dma2d.h"
#include "lvgl_fs_driver.h"

bool ThreadLvgl::refreshEnabled = true;
int ThreadLvgl::refreshTime = 30;

ThreadLvgl::ThreadLvgl(int refreshTimeInit) : m_mainThread(osPriorityBelowNormal)
{
    displayInit();
    touchpadInit();
    lv_fs_stdio_init();
    refreshTime = refreshTimeInit;
    lvTicker.attach(lvTimeCounter, chrono::milliseconds(refreshTime));
    m_mainThread.start(callback(ThreadLvgl::run, this));
}

ThreadLvgl::~ThreadLvgl()
{
    m_mainThread.join();
}

void ThreadLvgl::lvTimeCounter()
{
    lv_tick_inc(refreshTime);
}

void ThreadLvgl::runLvgl()
{
    while (1)
    {
        mutex.lock();
        lv_task_handler();
        mutex.unlock();
        // Call lv_task_handler() periodically every few milliseconds.
        // It will redraw the screen if required, handle input devices etc.
        ThisThread::sleep_for(chrono::milliseconds(refreshTime));
    }
}

void ThreadLvgl::displayInit(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    /*You code here*/
    // Init the touch screen display via the BSP driver. Based on ST's example.
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
    BSP_LCD_DisplayOn();
    BSP_LCD_SelectLayer(0);

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/
    lv_init();
    /**
     * LVGL requires a buffer where it internally draws the widgets.
     * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
     * The buffer has to be greater than 1 display row
     *
     * 2. Create TWO buffer:
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     */
    lv_color_t *buf0 = (lv_color_t *)(LCD_FB_START_ADDRESS + 0x00200000); /*A screen sized buffer*/
    lv_color_t *buf1 = (lv_color_t *)(LCD_FB_START_ADDRESS + 0x00400000); /*Another screen sized buffer*/
    static lv_disp_draw_buf_t draw_buf_dsc;
    lv_disp_draw_buf_init(&draw_buf_dsc, buf0, buf1,
                          BSP_LCD_GetXSize() * BSP_LCD_GetYSize()); /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    static lv_disp_drv_t disp_drv; /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);   /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = BSP_LCD_GetXSize();
    disp_drv.ver_res = BSP_LCD_GetYSize();

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = refreshDisplay;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
void ThreadLvgl::refreshDisplay(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    if (refreshEnabled)
    {
        lv_color_t *fbp32 = (lv_color_t *)LCD_FB_START_ADDRESS;
        lv_draw_stm32_dma2d_buffer_copy(NULL, fbp32 + area->y1 * LV_HOR_RES + area->x1,
                                        (lv_coord_t)LV_HOR_RES, area, color_p, area->x2 - area->x1 + 1, area);
    }

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}

/*------------------
 * Touchpad
 * -----------------*/

/*Will be called by the library to read the touchpad*/
void ThreadLvgl::touchpadRead(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    /*Save the pressed coordinates and the state*/
    TS_StateTypeDef TS_State;

    BSP_TS_GetState(&TS_State);
    if (TS_State.touchDetected)
    {
        last_x = TS_State.touchX[0];
        last_y = TS_State.touchY[0];
        data->state = LV_INDEV_STATE_PR;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Set the last pressed coordinates*/
    data->point.x = last_x;
    data->point.y = last_y;
}

void ThreadLvgl::touchpadInit(void)
{
    static lv_indev_drv_t indev_drv;

    /*------------------
     * Touchpad
     * -----------------*/

    /*Initialize your touchpad if you have*/
    BSP_TS_Init(420, 272);

    /*Register a touchpad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpadRead;
    indevTouchpad = lv_indev_drv_register(&indev_drv);
}

Ihm::Ihm(ThreadLvgl *t)
{
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

void Ihm::sdInit(lv_obj_t *parent)
{
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

void Ihm::sdMsg(const char *msg1, const char *msg2)
{
    m_threadLvgl->lock();
    if (msg1 != nullptr)
        lv_label_set_text(msgSdInit1, msg1);
    if (msg2 != nullptr)
        lv_label_set_text(msgSdInit2, msg2);
    m_threadLvgl->unlock();
}

void Ihm::matchInit(const vector<string> fichiers)
{
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

    /*Column 1: 3 unit from the remaining free space
     *Column 2: 1 unit from the remaining free space*/
    static lv_coord_t col_dsc[] = {LV_GRID_FR(5), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    /*Row 1: 30 pixels
     *Row 2: 1 unit from the remaining free space
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

void Ihm::matchRollerSetOptions(const vector<string> fichiers, bool lock)
{
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

void Ihm::recalagePositionInit()
{
    m_threadLvgl->lock();

    /*Column 1: 1 unit from the remaining free space
     *Column 2: 1 unit from the remaining free space*/
    static lv_coord_t col[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};

    /*Row 1: 30 pixels
     *Row 2: 1 unit from the remaining free space
     *Row 3: 1 unit from the remaining free space*/
    static lv_coord_t row[] = {30, LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    
    recalageView = lv_tabview_add_tab(tabView, "Recalage");
    /*Create a container with grid*/
    lv_obj_t *container = lv_obj_create(recalageView);
    lv_obj_center(container);
    lv_obj_update_layout(recalageView);
    lv_obj_set_size(container, lv_obj_get_content_width(recalageView), lv_obj_get_content_height(recalageView));
    lv_obj_set_grid_dsc_array(container, col, row);

    lv_obj_t *titreRecalage = lv_label_create(container);
    lv_obj_set_style_text_font(titreRecalage, &liberation_24, 0);
    lv_label_set_text(titreRecalage, LV_SYMBOL_EDIT " Choisir position robot"); // LV_SYMBOL_ENVELOPE LV_SYMBOL_DRIVE LV_SYMBOL_EDIT
    lv_obj_set_grid_cell(titreRecalage, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);

    HautGauche = lv_btn_create(container);
    lv_obj_t *label = lv_label_create(HautGauche);
    lv_label_set_text(label, "En Haut à gauche");
    lv_obj_set_style_bg_color(HautGauche, lv_palette_main(LV_PALETTE_LIGHT_BLUE), LV_STATE_DEFAULT);
    lv_obj_center(label);
    lv_obj_set_grid_cell(HautGauche, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_add_event_cb(HautGauche, Ihm::eventHandler, LV_EVENT_CLICKED, this);

    BasGauche = lv_btn_create(container);
    label = lv_label_create(BasGauche);
    lv_label_set_text(label, "En Bas à gauche");
    lv_obj_set_style_bg_color(BasGauche, lv_palette_main(LV_PALETTE_LIGHT_BLUE), LV_STATE_DEFAULT);
    lv_obj_center(label);
    lv_obj_set_grid_cell(BasGauche, LV_GRID_ALIGN_STRETCH, 0, 1,
                         LV_GRID_ALIGN_STRETCH, 2, 1);
    lv_obj_add_event_cb(BasGauche, Ihm::eventHandler, LV_EVENT_CLICKED, this);

    HautDroit = lv_btn_create(container);
    label = lv_label_create(HautDroit);
    lv_label_set_text(label, "En Haut à droite");
    lv_obj_set_style_bg_color(HautDroit, lv_palette_main(LV_PALETTE_LIGHT_BLUE), LV_STATE_DEFAULT);
    lv_obj_center(label);
    lv_obj_set_grid_cell(HautDroit, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 1, 1);
    lv_obj_add_event_cb(HautDroit, Ihm::eventHandler, LV_EVENT_CLICKED, this);

    BasDroit = lv_btn_create(container);
    label = lv_label_create(BasDroit);
    lv_label_set_text(label, "En Bas à droite");
    lv_obj_set_style_bg_color(BasDroit, lv_palette_main(LV_PALETTE_LIGHT_BLUE), LV_STATE_DEFAULT);
    lv_obj_center(label);
    lv_obj_set_grid_cell(BasDroit, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 2, 1);
    lv_obj_add_event_cb(BasDroit, Ihm::eventHandler, LV_EVENT_CLICKED, this);


    etat_recalage = lv_btn_create(container);
    label = lv_label_create(etat_recalage);
    lv_label_set_text(label, "Activation Recalage");
    lv_obj_add_flag(etat_recalage, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_bg_color(etat_recalage, lv_palette_main(LV_PALETTE_GREEN), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(etat_recalage, lv_palette_main(LV_PALETTE_RED), LV_STATE_CHECKED);
    lv_obj_center(label);
    lv_obj_set_grid_cell(etat_recalage, LV_GRID_ALIGN_STRETCH, 1, 1,
                         LV_GRID_ALIGN_STRETCH, 0, 1);
    lv_obj_add_event_cb(etat_recalage, Ihm::eventHandler, LV_EVENT_CLICKED, this);


    m_threadLvgl->unlock();
}

void Ihm::eventHandler(lv_event_t *e)
{
    Ihm *ihm = static_cast<Ihm *>(lv_event_get_user_data(e));
    lv_obj_t *emetteur = lv_event_get_current_target(e);

    if (emetteur == ihm->depart)
    {
        ihm->departCouleur = lv_obj_get_state(ihm->couleur);
        ihm->departStrategie = int(lv_roller_get_selected(ihm->roller)) - 1;
        ihm->flags.set(IHM_FLAG_DEPART);
    }
    else if (emetteur == ihm->refreshSD)
    {
        ihm->flags.set(IHM_FLAG_REFRESH_SD);
    }
    else if (emetteur == ihm->msgBoxRecalage)
    {
        ihm->flags.set(IHM_FLAG_RECALAGE);
    }
    else if (emetteur == ihm->msgBoxJack)
    {
        if (lv_event_get_code(e) == LV_EVENT_DELETE)
        {
            ihm->flags.set(IHM_FLAG_START_CANCEL);
            ihm->msgBoxJack = nullptr;
        }
        else
        {
            ihm->flags.set(IHM_FLAG_START);
        }
    }
    else if (emetteur == ihm->msgBox)
    {
        ihm->flags.set(IHM_FLAG_MSGBOX_CANCEL);
        ihm->msgBox = nullptr;
    }
    else if (emetteur == ihm->HautGauche)
    {
        ihm->flags.set(IHM_FLAG_RECALAGE_HAUTGAUCHE);
    }
    else if (emetteur == ihm->HautDroit)
    {
        ihm->flags.set(IHM_FLAG_RECALAGE_HAUTDROIT);
    }
    else if (emetteur == ihm->BasGauche)
    {
        ihm->flags.set(IHM_FLAG_RECALAGE_BASGAUCHE);
    }
    else if (emetteur == ihm->BasDroit)
    {
        ihm->flags.set(IHM_FLAG_RECALAGE_BASDROIT);
    }else if(emetteur == ihm->etat_recalage){
        ihm->flags.set(IHM_FLAG_RECALAGE_ETAT);
    }
}

bool Ihm::getFlag(IhmFlag f, bool clearIfSet)
{
    if (flags.get() & f)
    {
        if (clearIfSet)
            flags.clear(f);
        return true;
    }
    return false;
}

void Ihm::msgBoxRecalageInit(const string &strategie)
{
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

void Ihm::msgBoxRecalageClose()
{
    m_threadLvgl->lock();
    lv_msgbox_close(msgBoxRecalage);
    msgBoxRecalage = nullptr;
    m_threadLvgl->unlock();
}

void Ihm::msgBoxJackInit()
{
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

void Ihm::msgBoxJackClose()
{
    m_threadLvgl->lock();
    lv_msgbox_close(msgBoxJack);
    // Efface le flag de fermeture de la message box
    jackAnnuleClicked();
    msgBoxJack = nullptr;
    m_threadLvgl->unlock();
}

void Ihm::msgBoxInit(const char *titre, const char *msg, bool boutonAnnuler)
{
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

void Ihm::msgBoxClose()
{
    m_threadLvgl->lock();
    lv_msgbox_close(msgBox);
    // Efface le flag de fermeture de la message box
    msgBoxCancelClicked();
    msgBox = nullptr;
    m_threadLvgl->unlock();
}

void Ihm::msgBoxMessage(const char *msg)
{
    if (msgBox)
    {
        m_threadLvgl->lock();
        lv_obj_t *txt = lv_msgbox_get_text(msgBox);
        lv_label_set_text(txt, msg);
        m_threadLvgl->unlock();
    }
}
