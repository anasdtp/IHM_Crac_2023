//-----------------------------------------------------------
//  ButtonGroup class -- Header
//
//  2016/07/12, Copyright (c) 2016 MIKAMI, Naoki
//-----------------------------------------------------------

#ifndef F746_BUTTON_GROUP_HPP
#define F746_BUTTON_GROUP_HPP

#include "Button.hpp"

namespace Mikami
{
    class ButtonGroup : public GuiBase
    {
    public:
        // Constructor
        ButtonGroup(uint16_t x0, uint16_t y0,
                    uint16_t width, uint16_t height,
                    uint16_t number, const string str[],
                    uint16_t spaceX = 0, uint16_t spaceY = 0,
                    uint16_t column = 1,
                    int touched = -1,   // number for button initially touched-color
                    sFONT &fonts = Font12,
                    uint32_t textColor         = GuiBase::ENUM_TEXT,
                    uint32_t backColor         = GuiBase::ENUM_BACK,
                    uint32_t createdColor      = GuiBase::ENUM_CREATED,
                    uint32_t touchedColor      = GuiBase::ENUM_TOUCHED,
                    uint32_t inactiveColor     = GuiBase::ENUM_INACTIVE,
                    uint32_t inactiveTextColor = GuiBase::ENUM_INACTIVE_TEXT);

        // Destructor
        virtual ~ButtonGroup();

        // Draw button
        bool Draw(int num, uint32_t color, uint32_t textColor);
        bool Draw(int num) { return Draw(num, CREATED_COLOR_, TEXT_COLOR_); }

        // Change to touched color
        bool TouchedColor(int num);

        // Draw all buttons
        void DrawAll(uint32_t color, uint32_t textColor)
        {   for (int n=0; n<number_; n++) Draw(n, color, textColor); }
        void DrawAll() { DrawAll(CREATED_COLOR_, TEXT_COLOR_); }

        // Erase button
        bool Erase(int num);
        void EraseAll()
        {   for (int n=0; n<number_; n++) Erase(n); }

        // Check touch detected for specified button
        bool Touched(int num);

        // Get touched number
        bool GetTouchedNumber(int &num);

        // Activate and inactivate button(s)
        bool Activate(int num);
        void ActivateAll()
        {   for (int n=0; n<number_; n++) Activate(n); }
        bool Inactivate(int num);
        void InactivateAll()
        {   for (int n=0; n<number_; n++) Inactivate(n); }

    private:
        Button **buttons_;
        int number_;
        __IO int prevNum_;

        // Check range of argument
        bool Range(int n)
        { return ((n >= 0) && (n < number_)); }

        // disallow copy constructor and assignment operator
        ButtonGroup(const ButtonGroup&);
        ButtonGroup& operator=(const ButtonGroup&);
    };
}
#endif  // F746_BUTTON_GROUP_HPP
