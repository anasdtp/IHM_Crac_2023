//-----------------------------------------------------------
//  Button class handling multi-touch
//      Multi-touch: Enabled (default)
//
//  2016/04/08, Copyright (c) 2016 MIKAMI, Naoki
//-----------------------------------------------------------

#include "Button.hpp"

namespace Mikami
{
    // Draw button
    void Button::Draw(uint32_t color, uint32_t textColor)
    {
        if (color == BACK_COLOR_) active_ = true;
        if (!active_) return;
        lcd_.SetTextColor(color);
        lcd_.FillRect(X_, Y_, W_, H_);
        lcd_.FillRect(X_, Y_, W_, H_);

        if (STR_.length() != 0)
        {
            lcd_.SetFont(FONTS_);
            lcd_.SetBackColor(color);
            lcd_.SetTextColor(textColor);
            uint16_t x0 = X_ + (W_ - FONTS_->Width*(STR_.length()))/2;
            uint16_t y0 = Y_ + (H_ - FONTS_->Height)/2 + 1;
            DrawString(x0, y0, STR_);
            DrawString(x0, y0, STR_);
            lcd_.SetBackColor(BACK_COLOR_);  // recover back color
        }            
    }

    // Check touch detected
    bool Button::Touched()
    {
        if (!active_) return false;
        if (!PanelTouched()) return false;
        if (!IsOnButton()) return false;
        Draw(TOUCHED_COLOR_, TEXT_COLOR_);
        return true;
    }

    // If touched position is on the button, return true
    bool Button::IsOnButton()
    {
        int nTouch = multiTouch_ ? state_.touchDetected : 1;
        for (int n=0; n<nTouch; n++)
        {
            uint16_t x = state_.touchX[n];
            uint16_t y = state_.touchY[n];
            if ( (X_ <= x) && (x <= X_+W_) &&
                 (Y_ <= y) && (y <= Y_+H_) ) return true;
        }
        return false;
    }

    void Button::Activate()
    {
        active_ = true;
        Draw();
    }

    void Button::Inactivate()
    {
        Draw(INACTIVE_COLOR_, INACTIVE_TEXT_COLOR_);
        active_ = false;
    }
}
