//-----------------------------------------------------------
//  Button class handling multi-touch -- Header
//      Multi-touch: Enabled (default)
//
//  2016/03/29, Copyright (c) 2016 MIKAMI, Naoki
//-----------------------------------------------------------

#ifndef F746_BUTTON_HPP
#define F746_BUTTON_HPP

#include "GuiBase.hpp"

namespace Mikami
{
    class Button : public GuiBase
    {
    public:
        // Constructor
        Button(uint16_t x, uint16_t y,
               uint16_t width, uint16_t height,
               const string str = "", sFONT &fonts = Font24,
               uint32_t textColor         = GuiBase::ENUM_TEXT,
               uint32_t backColor         = GuiBase::ENUM_BACK,
               uint32_t createdColor      = GuiBase::ENUM_CREATED,
               uint32_t touchedColor      = GuiBase::ENUM_TOUCHED,
               uint32_t inactiveColor     = GuiBase::ENUM_INACTIVE,
               uint32_t inactiveTextColor = GuiBase::ENUM_INACTIVE_TEXT)
              : GuiBase(x, y, fonts,
                        textColor, backColor, createdColor,
                        touchedColor, inactiveColor,
                        inactiveTextColor),
                W_(width), H_(height), STR_(str), active_(true)
        {   Draw(); }

        // Draw button
        void Draw(uint32_t color, uint32_t textColor);
        void Draw(uint32_t color) { Draw(color, TEXT_COLOR_); }
        void Draw() { Draw(CREATED_COLOR_, TEXT_COLOR_); }

        // Erase button
        void Erase() { Draw(BACK_COLOR_, BACK_COLOR_); }

        // Check touch detected and redraw button
        bool Touched();

        bool IsOnButton();

        void Activate();
        void Inactivate();
        bool IsActive() { return active_; }

        // Set or reset multi-touch
        static void SetMultiTouch(bool tf) { multiTouch_ = tf; }

    private:       
        const uint16_t W_, H_;
        const string STR_;
        bool active_;

        // disallow copy constructor and assignment operator
        Button(const Button&);
        Button& operator=(const Button&);
    };
}
#endif  // F746_BUTTON_HPP
