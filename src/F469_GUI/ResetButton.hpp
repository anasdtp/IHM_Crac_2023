//-----------------------------------------------------------
//  ResetButton class -- derived class of Button class
//
//  2016/08/15, Copyright (c) 2016 MIKAMI, Naoki
//-----------------------------------------------------------

#ifndef F746_RESET_BUTTON_HPP
#define F746_RESET_BUTTON_HPP

#include "Button.hpp"

namespace Mikami
{
    class ResetButton : public Button
    {
    public:
        ResetButton(uint16_t x = 0, uint16_t y = 240,
                    uint16_t w = 32, uint16_t h = 32)
            : Button(x, y, w, h)
        {
            pt_[0] = (Point){x+0.2f*w, y+h/2};
            pt_[1] = (Point){x+0.75f*w, y+0.175f*h};
            pt_[2] = (Point){x+0.75f*w, y+0.825f*h};
            Draw();
        }

        void Do()
        {
            if (!Touched()) return;

//            ThisThread::sleep_for(0.2s);
            NVIC_SystemReset();
        }
        
        void Draw()
        {
            Activate();
            lcd_.SetTextColor(LCD_COLOR_WHITE);
            lcd_.FillPolygon(pt_, 3);
        }

    private:
        Point pt_[3];
    };
}
#endif  // F746_RESET_BUTTON_HPP
