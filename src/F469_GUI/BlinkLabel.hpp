//-----------------------------------------------------------
//  BlinkLabel class -- derived class of Label class
//      For displaying error message
//
//  2016/08/16, Copyright (c) 2016 MIKAMI, Naoki
//-----------------------------------------------------------

#ifndef F746_BLINK_LABEL_HPP
#define F746_BLINK_LABEL_HPP

#include "Label.hpp"
#include "ResetButton.hpp"

namespace Mikami
{
    class BlinkLabel : public Label
    {
    public:
        // Constructor
        BlinkLabel(uint16_t x, uint16_t y, const string str,
                   TextAlignMode mode = LEFT,
                   sFONT &fonts = Font20,
                   uint32_t textColor = LCD_COLOR_RED,
                   uint32_t backColor = GuiBase::ENUM_BACK,
                   uint32_t on = 500, uint32_t off = 200)
            : Label(x, y, str, mode, fonts, textColor, backColor)
        {
            ResetButton *reset = new ResetButton();
            while (true)    // Blinking here
            {
                wait_ms(on);
                Draw(backColor);
                wait_ms(off);
                Draw(textColor);
                reset->Do();
            }
        }

    private:
        // disallow copy constructor and assignment operator
        BlinkLabel(const BlinkLabel&);
        BlinkLabel& operator=(const BlinkLabel&);
    };
}
#endif  // F746_BLINK_LABEL_HPP
