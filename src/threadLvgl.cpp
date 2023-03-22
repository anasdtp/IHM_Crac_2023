#include "threadLvgl.h"
#include "../BSP_DISCO_F469NI/Drivers/STM32469I-Discovery/stm32469i_discovery_lcd.h"
#include "../BSP_DISCO_F469NI/Drivers/STM32469I-Discovery/stm32469i_discovery_ts.h"
#include "lvgl.h"

TS_StateTypeDef TS_State; 
    
ThreadLvgl::ThreadLvgl()
{
    displayInit();
    touchpadInit();
    lv_task_handler();
    m_mainThread.start(callback(ThreadLvgl::run, this));
}

ThreadLvgl::~ThreadLvgl()
{
    m_mainThread.join();
}

void ThreadLvgl::runLvgl()
{
    while (1) {
        lv_tick_inc(10); 
        //Call lv_tick_inc(x) every x milliseconds in a Timer or Task (x should be between 1 and 10). 
        //It is required for the internal timing of LittlevGL.
        mutex.lock();
        lv_task_handler();
        mutex.unlock(); 
        //Call lv_task_handler() periodically every few milliseconds. 
        //It will redraw the screen if required, handle input devices etc.
        ThisThread::sleep_for(10ms);
    }
}

extern LTDC_HandleTypeDef  hltdc_eval;

void my_disp_flush_cb(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p)
{
    //The most simple case (but also the slowest) to put all pixels to the screen one-by-one
    uint16_t x, y;
 /*   for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            //put_px(x, y, *color_p)
            BSP_LCD_DrawPixel( x, y, color_p->full);
            color_p++;
        }
    }
*/
/*
    uint32_t *c = (uint32_t *)(color_p);
    for(y = area->y1; y <= area->y2; y++) {
        uint32_t *p = (uint32_t *)(hltdc_eval.LayerCfg[LTDC_ACTIVE_LAYER_BACKGROUND].FBStartAdress + (4*(y*BSP_LCD_GetXSize() + area->x1)));
        for(x = area->x1; x <= area->x2; x++) {
            *p = *c;
            p++;
            c++;
        }
    }
*/
    uint8_t *c = (uint8_t *)(color_p);
    int d = (area->x2 - area->x1 + 1)*4;
    uint32_t *p = (uint32_t *)(hltdc_eval.LayerCfg[LTDC_ACTIVE_LAYER_BACKGROUND].FBStartAdress + (4*(area->y1*BSP_LCD_GetXSize() + area->x1)));
    for(y = area->y1; y <= area->y2; y++) {
        memcpy(p, c, d);
        p+=800;
        c+=d;
    }

    //IMPORTANT!!!* Inform the graphics library that you are ready with the flushing
    lv_disp_flush_ready(disp_drv);
}

static bool touchpad_read(lv_indev_drv_t *indev, lv_indev_data_t *data){
    // Read your touchpad
    BSP_TS_GetState(&TS_State);
    if(TS_State.touchDetected) {
        data->point.x = TS_State.touchX[0];
        data->point.y = TS_State.touchY[0];
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->point.x = 0;
        data->point.y = 0;
        data->state = LV_INDEV_STATE_REL;
    }
    return false;   //false: no more data to read because we are no buffering
}

void ThreadLvgl::displayInit(void){
    //Init the touch screen display via the BSP driver. Based on ST's example.
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
    BSP_LCD_DisplayOn();
    BSP_LCD_SelectLayer(0);
    BSP_LCD_Clear(LCD_COLOR_TRANSPARENT );
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
    
    lv_init();                                  //Initialize the LittlevGL
    static lv_disp_buf_t disp_buf;
    static lv_color_t buf[LV_HOR_RES_MAX * 10]; //Declare a buffer for 10 lines                                                              
    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10); //Initialize the display buffer
    
    //Implement and register a function which can copy a pixel array to an area of your display
    lv_disp_drv_t disp_drv;                     //Descriptor of a display driver
    lv_disp_drv_init(&disp_drv);                //Basic initialization
    disp_drv.flush_cb = my_disp_flush_cb;       //Set your driver function
    disp_drv.buffer = &disp_buf;                //Assign the buffer to the display
    lv_disp_drv_register(&disp_drv);            //Finally register the driver
}

void ThreadLvgl::touchpadInit(void){
    BSP_TS_Init(420, 272);
    lv_indev_drv_t indev_drv;                       //Descriptor of an input device driver
    lv_indev_drv_init(&indev_drv);                  //Basic initialization
    indev_drv.type = LV_INDEV_TYPE_POINTER;         //The touchpad is pointer type device
    indev_drv.read_cb = touchpad_read;              //Set the touchpad_read function
    lv_indev_drv_register(&indev_drv);              //Register touch driver in LvGL
}
