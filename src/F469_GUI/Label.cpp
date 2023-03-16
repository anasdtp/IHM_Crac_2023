//-----------------------------------------------------------
//  Label class
//
//  2016/04/24, Copyright (c) 2016 MIKAMI, Naoki
//-----------------------------------------------------------

#include "Label.hpp"

namespace Mikami
{
    // Constructor
    Label::Label(uint16_t x, uint16_t y,
                 const string str, TextAlignMode mode, sFONT &fonts,
                 uint32_t textColor, uint32_t backColor)
        : GuiBase(x, y, fonts, textColor, backColor),
          MODE_(mode), STR_(str)
    {
        length_ = str.length();
        lcd_.SetBackColor(backColor);
        lcd_.SetTextColor(textColor);
        lcd_.SetFont(&fonts);
        DrawString(PosX(x), y, str);
    }

    void Label::Draw(const string str, uint32_t textColor)
    {
        // Erase previously-drawn string
        lcd_.SetTextColor(BACK_COLOR_);
        length_ = (length_ > str.length()) ? length_ : str.length();
        lcd_.FillRect(PosX(X_), Y_, FONTS_->Width*length_+1, FONTS_->Height);

        // Draw new string
        length_ = str.length();
        lcd_.SetFont(FONTS_);
        lcd_.SetTextColor(textColor);
        DrawString(PosX(X_), Y_, str);
    }

    uint16_t Label::PosX(uint16_t x)
    {
        if (MODE_ == LEFT) return x;
        else
        {
            if (MODE_ == CENTER)
                return x - length_*FONTS_->Width/2;
            else
                return x - length_*FONTS_->Width;
        }
    }
}
