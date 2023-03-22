#include "threadLvgl.h"
#include "../BSP_DISCO_F469NI/Drivers/STM32469I-Discovery/stm32469i_discovery_lcd.h"
#include "../BSP_DISCO_F469NI/Drivers/STM32469I-Discovery/stm32469i_discovery_ts.h"
#include "lvgl.h"
#include "lv_port_disp.h"

lv_indev_t * indev_touchpad;

ThreadLvgl::ThreadLvgl()
{
    lv_init();
    lv_port_disp_init();
    touchpadInit();    
    m_mainThread.start(callback(ThreadLvgl::run, this));
}

ThreadLvgl::~ThreadLvgl()
{
    m_mainThread.join();
}

void ThreadLvgl::runLvgl()
{
    while (1) {
        lv_tick_inc(5); 
        //Call lv_tick_inc(x) every x milliseconds in a Timer or Task (x should be between 1 and 10). 
        //It is required for the internal timing of LittlevGL.
        mutex.lock();
        lv_task_handler();
        mutex.unlock(); 
        //Call lv_task_handler() periodically every few milliseconds. 
        //It will redraw the screen if required, handle input devices etc.
        ThisThread::sleep_for(5ms);
    }
}

// extern LTDC_HandleTypeDef  hltdc_eval;

// void my_disp_flush_cb(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p)
// {
//     //The most simple case (but also the slowest) to put all pixels to the screen one-by-one
//     uint16_t x, y;
//  /*   for(y = area->y1; y <= area->y2; y++) {
//         for(x = area->x1; x <= area->x2; x++) {
//             //put_px(x, y, *color_p)
//             BSP_LCD_DrawPixel( x, y, color_p->full);
//             color_p++;
//         }
//     }
// */
// /*
//     uint32_t *c = (uint32_t *)(color_p);
//     for(y = area->y1; y <= area->y2; y++) {
//         uint32_t *p = (uint32_t *)(hltdc_eval.LayerCfg[LTDC_ACTIVE_LAYER_BACKGROUND].FBStartAdress + (4*(y*BSP_LCD_GetXSize() + area->x1)));
//         for(x = area->x1; x <= area->x2; x++) {
//             *p = *c;
//             p++;
//             c++;
//         }
//     }
// */
//     uint8_t *c = (uint8_t *)(color_p);
//     int d = (area->x2 - area->x1 + 1)*4;
//     uint32_t *p = (uint32_t *)(hltdc_eval.LayerCfg[LTDC_ACTIVE_LAYER_BACKGROUND].FBStartAdress + (4*(area->y1*BSP_LCD_GetXSize() + area->x1)));
//     for(y = area->y1; y <= area->y2; y++) {
//         memcpy(p, c, d);
//         p+=800;
//         c+=d;
//     }

//     //IMPORTANT!!!* Inform the graphics library that you are ready with the flushing
//     lv_disp_flush_ready(disp_drv);
// }

/*------------------
 * Touchpad
 * -----------------*/

/*Will be called by the library to read the touchpad*/
static void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;

    /*Save the pressed coordinates and the state*/
    TS_StateTypeDef TS_State; 
    
    BSP_TS_GetState(&TS_State);
    if(TS_State.touchDetected) {
        last_x = TS_State.touchX[0];
        last_y = TS_State.touchY[0];
        data->state = LV_INDEV_STATE_PR;
    }
    else {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Set the last pressed coordinates*/
    data->point.x = last_x;
    data->point.y = last_y;
}

void ThreadLvgl::touchpadInit(void){
    static lv_indev_drv_t indev_drv;

    /*------------------
     * Touchpad
     * -----------------*/

    /*Initialize your touchpad if you have*/
    BSP_TS_Init(420, 272);

    /*Register a touchpad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    indev_touchpad = lv_indev_drv_register(&indev_drv);
}
