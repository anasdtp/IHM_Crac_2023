//-----------------------------------------------------------
//  GuiBase class (abstract base class)
//
//  2016/03/29, Copyright (c) 2016 MIKAMI, Naoki
//-----------------------------------------------------------

#include "GuiBase.hpp"

namespace Mikami
{
    GuiBase::GuiBase(
        uint16_t x, uint16_t y, sFONT &fonts,
        uint32_t textColor, uint32_t backColor,
        uint32_t createdColor, uint32_t touchedColor,
        uint32_t inactiveColor, uint32_t inactiveTextColor)
        : X_(x), Y_(y), FONTS_(&fonts),
          TEXT_COLOR_(textColor), BACK_COLOR_(backColor),
          CREATED_COLOR_(createdColor),
          TOUCHED_COLOR_(touchedColor),
          INACTIVE_COLOR_(inactiveColor),
          INACTIVE_TEXT_COLOR_(inactiveTextColor)
    {
        if (first_)
        {
            lcd_.Clear(backColor);
            first_ = false;
        }
    }

    // If panel touched, return true
    bool GuiBase::PanelTouched()
    {
        ts_.GetState(&state_);
        return (bool)(state_.touchDetected);
    }

    LCD_DISCO_F469NI GuiBase::lcd_;
    TS_DISCO_F469NI GuiBase::ts_;

    TS_StateTypeDef GuiBase::state_;

    bool GuiBase::multiTouch_ = false;
    bool GuiBase::first_ = true;
}
