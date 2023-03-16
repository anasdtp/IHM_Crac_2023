//-----------------------------------------------------------
//  Label class -- Header
//
//  2016/04/24, Copyright (c) 2016 MIKAMI, Naoki
//-----------------------------------------------------------

#ifndef F746_LABEL_HPP
#define F746_LABEL_HPP

#include "GuiBase.hpp"

namespace Mikami
{
    class Label : public GuiBase
    {
    public:
        enum TextAlignMode { LEFT, CENTER, RIGHT };
        // Constructor
        Label(uint16_t x, uint16_t y, const string str,
              TextAlignMode mode = LEFT,
              sFONT &fonts = Font12,
              uint32_t textColor = GuiBase::ENUM_TEXT,
              uint32_t backColor = GuiBase::ENUM_BACK);

        // Constructor without string
        Label(uint16_t x, uint16_t y,
              TextAlignMode mode = LEFT,
              sFONT &fonts = Font12,
              uint32_t textColor = GuiBase::ENUM_TEXT,
              uint32_t backColor = GuiBase::ENUM_BACK)
            : GuiBase(x, y, fonts, textColor, backColor),
              MODE_(mode), STR_("") {}

        void Draw()
        {   Draw(STR_, TEXT_COLOR_); }

        void Draw(const string str)
        {   Draw(str, TEXT_COLOR_); }

        void Draw(uint32_t textColor)
        {   Draw(STR_, textColor); }

        void Draw(const string str,
                  uint32_t textColor);
                  
    private:
        const TextAlignMode MODE_;
        const string STR_;
        
        uint8_t length_;
        uint16_t PosX(uint16_t x);
        
        // disallow copy constructor and assignment operator
        Label(const Label&);
        Label& operator=(const Label&);
    };
}
#endif  // F746_LABEL_HPP
