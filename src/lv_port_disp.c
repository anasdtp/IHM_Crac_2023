/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include <stdbool.h>
#include "../BSP_DISCO_F469NI/Drivers/STM32469I-Discovery/stm32469i_discovery_lcd.h"
#include "../lvgl/src/draw/stm32_dma2d/lv_gpu_stm32_dma2d.h"

extern LTDC_HandleTypeDef hltdc_eval;

/*********************
 *      DEFINES
 *********************/
#ifndef MY_DISP_HOR_RES
#define MY_DISP_HOR_RES 800
#endif

#ifndef MY_DISP_VER_RES
#define MY_DISP_VER_RES 480
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
// static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//         const lv_area_t * fill_area, lv_color_t color);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_color_t *layer0; /*A screen sized buffer*/
static lv_color_t *layer1; /*Another screen sized buffer*/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /**
     * LVGL requires a buffer where it internally draws the widgets.
     * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
     * The buffer has to be greater than 1 display row
     *
     * There are 3 buffering configurations:
     * 1. Create ONE buffer:
     *      LVGL will draw the display's content here and writes it to your display
     *
     * 2. Create TWO buffer:
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     *
     * 3. Double buffering
     *      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
     *      This way LVGL will always provide the whole rendered screen in `flush_cb`
     *      and you only need to change the frame buffer's address.
     */

    /* Example for 1) */
    // static lv_color_t buf_1[MY_DISP_HOR_RES * 48];                          /*A buffer for 10 rows*/
    // static lv_disp_draw_buf_t draw_buf_dsc_1;
    // lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 48);   /*Initialize the display buffer*/

    /* Example for 2) */
    // static lv_disp_draw_buf_t draw_buf_dsc_1;
    // static lv_color_t buf_2_1[MY_DISP_HOR_RES * 24];                        /*A buffer for 10 rows*/
    // static lv_color_t buf_2_2[MY_DISP_HOR_RES * 24];                        /*An other buffer for 10 rows*/
    // lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_2_1, buf_2_2, MY_DISP_HOR_RES * 24);   /*Initialize the display buffer*/

    // /* Example for 3) also set disp_drv.full_refresh = 1 below*/
    layer0 = (lv_color_t *)(LCD_FB_START_ADDRESS+0x00200000); /*A screen sized buffer*/
    layer1 = (lv_color_t *)(LCD_FB_START_ADDRESS+0x00400000); /*Another screen sized buffer*/
    static lv_disp_draw_buf_t draw_buf_dsc_1;
    lv_disp_draw_buf_init(&draw_buf_dsc_1, layer0, layer1,
                          MY_DISP_HOR_RES * MY_DISP_VER_RES); /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    static lv_disp_drv_t disp_drv; /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);   /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_1;

    /*Required for Example 3)*/
    //disp_drv.full_refresh = 1;

    /* Fill a memory array with a color if you have GPU.
     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
     * But if you have a different GPU you can use with this callback.*/
    // disp_drv.gpu_fill_cb = gpu_fill;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    /*You code here*/
    // Init the touch screen display via the BSP driver. Based on ST's example.
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
//    BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS + 0x00200000);
    BSP_LCD_DisplayOn();
    BSP_LCD_SelectLayer(0);
    BSP_LCD_Clear(LCD_COLOR_TRANSPARENT);
    BSP_LCD_SetFont(&LCD_DEFAULT_FONT);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    if (disp_flush_enabled)
    {
        /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

        // int32_t x;
        // int32_t y;
        // for(y = area->y1; y <= area->y2; y++) {
        //     for(x = area->x1; x <= area->x2; x++) {
        //         /*Put a pixel to the display. For example:*/
        //         /*put_px(x, y, *color_p)*/
        //         BSP_LCD_DrawPixel( x, y, color_p->full);
        //         color_p++;
        //     }
        // }

        // int32_t y;
        // uint8_t *c = (uint8_t *)(color_p);
        // int d = (area->x2 - area->x1 + 1)*4;
        // uint32_t *p = (uint32_t *)(hltdc_eval.LayerCfg[LTDC_ACTIVE_LAYER_BACKGROUND].FBStartAdress + (4*(area->y1*BSP_LCD_GetXSize() + area->x1)));
        // for(y = area->y1; y <= area->y2; y++) {
        //     memcpy(p, c, d);
        //     p+=800;
        //     c+=d;
        // }

        lv_color_t *fbp32 = (lv_color_t *)hltdc_eval.LayerCfg[LTDC_ACTIVE_LAYER_BACKGROUND].FBStartAdress;
        lv_draw_stm32_dma2d_buffer_copy(NULL, fbp32 + area->y1 * LV_HOR_RES + area->x1,
        	(lv_coord_t) LV_HOR_RES, area, color_p, area->x2 - area->x1 + 1, area);

    }

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}

/*OPTIONAL: GPU INTERFACE*/

/*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
// static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//                    const lv_area_t * fill_area, lv_color_t color)
// {
//    /*It's an example code which should be done by your GPU*/
//    int32_t x, y;
//    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/

//    for(y = fill_area->y1; y <= fill_area->y2; y++) {
//        for(x = fill_area->x1; x <= fill_area->x2; x++) {
//            dest_buf[x] = color;
//        }
//        dest_buf+=dest_width;    /*Go to the next line*/
//    }
// }

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
