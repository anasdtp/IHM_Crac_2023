//-----------------------------------------------------------
//  NumericLabel class -- derived class of Label class
//
//  2016/04/12, Copyright (c) 2016 MIKAMI, Naoki
//-----------------------------------------------------------

#ifndef F746_NUMERIC_LABEL_HPP
#define F746_NUMERIC_LABEL_HPP

#include "Label.hpp"

namespace Mikami
{
    template <typename T> class NumericLabel : public Label
    {
    public:
        // Constructor without drawing value
        NumericLabel(uint16_t x, uint16_t y,
                     TextAlignMode mode = LEFT,
                     sFONT &fonts = Font12,
                     uint32_t textColor = GuiBase::ENUM_TEXT,
                     uint32_t backColor = GuiBase::ENUM_BACK)
            : Label(x, y, "", mode, fonts, textColor, backColor) {}

        // Constructor with drawing value
        NumericLabel(uint16_t x, uint16_t y,
                     const char fmt[], T val,
                     TextAlignMode mode = LEFT,
                     sFONT &fonts = Font12,
                     uint32_t textColor = GuiBase::ENUM_TEXT,
                     uint32_t backColor = GuiBase::ENUM_BACK)
            : Label(x, y, "", mode, fonts, textColor, backColor)
        {   Draw(fmt, val); }

        // Draw value
        void Draw(const char fmt[], T val)
        {
            sprintf(str_, fmt, val);
            Label::Draw(str_);
        }

        // Draw previous value with specified color
        void Draw(uint32_t color)
        {   Label::Draw(str_, color); }

    private:
        char str_[81];

        // disallow copy constructor and assignment operator
        NumericLabel(const NumericLabel&);
        NumericLabel& operator=(const NumericLabel&);
    };
}
#endif  // F746_NUMERIC_LABEL_HPP
