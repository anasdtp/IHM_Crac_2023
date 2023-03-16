//-----------------------------------------------------------
//  SeekBar class
//
//  2016/07/12, Copyright (c) 2016 MIKAMI, Naoki
//-----------------------------------------------------------

#include "SeekBar.hpp"

namespace Mikami
{
    // Constructor with scale value (only horizontal)
    SeekBar::SeekBar(uint16_t x, uint16_t y, uint16_t length,
                     float min, float max, float initialValue,
                     string left, string center, string right,
                     uint32_t thumbColor,
                     uint16_t thumbSize, uint16_t width,
                     uint32_t colorL, uint32_t colorH,
                     uint32_t backColor)
        : GuiBase(x, y, Font12, GuiBase::ENUM_TEXT, backColor, thumbColor),
          L_(length), W_(width),
          SIZE_(thumbSize), COLOR_L_(colorL), COLOR_H_(colorH),
          MIN_(min), MAX_(max), ORIENT_(Holizontal), v_(initialValue),
          labelOn_(true), slided_(false), active_(true)
    {
        Draw(initialValue);
        uint16_t y0 = y - thumbSize/2 - 13;
        labelLCR_[0] = new Label(x, y0, left, Label::CENTER);
        labelLCR_[1] = new Label(x+length/2, y0, center, Label::CENTER);
        labelLCR_[2] = new Label(x+length, y0, right, Label::CENTER);
    }

    SeekBar::~SeekBar()
    {
        if (labelOn_)
            for (int n=0; n<3; n++) delete labelLCR_[n];
    }

    // Slide thumb
    //      If the thumb is not touched, return false
    bool SeekBar::Slide()
    {
        if (!active_) return false;

        if (!PanelTouched())
        {
            if (slided_) Draw(v_);
            slided_ = false;
            return false;
        }

        uint16_t x, y;
        bool rtn = IsOnThumb(x, y);
        if (rtn || slided_)
        {
            if (rtn) v_ = ToValue(Point(x, y));
            Draw(v_, rtn);
            slided_ = rtn;
        }
        return rtn;
    }

    void SeekBar::Activate()
    {
        active_ = true;
        Draw(v_);
        if (labelOn_)
            for (int n=0; n<3; n++) labelLCR_[n]->Draw(TEXT_COLOR_);
    }

    void SeekBar::Inactivate()
    {
        active_ = false;
        Draw(v_);
        if (labelOn_)
            for (int n=0; n<3; n++) labelLCR_[n]->Draw(INACTIVE_TEXT_COLOR_);
    }

    // If touched position is on the thumb, return true
    bool SeekBar::IsOnThumb(uint16_t &x, uint16_t &y)
    {
        x = state_.touchX[0];
        y = state_.touchY[0];

        uint16_t th = SIZE_/2;
        Point pt = ToPoint(v_);
        if (ORIENT_ == Holizontal)
        {
            if ( (pt.x-th <= x) && (x <= pt.x+th) &&
                 (pt.y-th <= y) && (y <= pt.y+th) ) return true;
        }
        else
        {
            if ( (pt.x-th <= x) && (x <= pt.x+th) &&
                 (pt.y-th <= y) && (y <= pt.y+th) ) return true;
        }

        return false;
    }

    // Draw seekbar
    void SeekBar::Draw(float value, bool fill)
    {
        uint16_t sizeS = (uint16_t)(SIZE_*0.6f);
        // Erase previous seekbar
        lcd_.SetTextColor(BACK_COLOR_);
        if (ORIENT_ == Holizontal)
            lcd_.FillRect(X_-sizeS/2, Y_-SIZE_/2, L_+sizeS+1, SIZE_+1);
        else
            lcd_.FillRect(X_-SIZE_/2, Y_-sizeS/2, SIZE_+1, L_+sizeS+1);

        v_ = Saturate(value);       // current value
        Point pt = ToPoint(v_);     // Position of thumb

        // Draw upper line
        if (active_) lcd_.SetTextColor(COLOR_H_);
        else         lcd_.SetTextColor(INACTIVE_TEXT_COLOR_-0x404040);
        if ((ORIENT_ == Holizontal) && ((X_+L_-pt.x) > 0))
            lcd_.FillRect(pt.x, Y_-W_/4, X_+L_-pt.x, W_/2);
        if ((ORIENT_ == Vertical) && ((pt.y-Y_) > 0))
            lcd_.FillRect(X_-W_/4, Y_, W_/2, pt.y-Y_);

        // Draw lower line
        if (active_) lcd_.SetTextColor(COLOR_L_);
        else         lcd_.SetTextColor(INACTIVE_TEXT_COLOR_-0x202020);
        if ((ORIENT_ == Holizontal) && ((pt.x-X_) > 0))
            lcd_.FillRect(X_, Y_-W_/2, pt.x-X_, W_);
        if ((ORIENT_ == Vertical) && ((Y_+L_-pt.y) > 0))
            lcd_.FillRect(X_-W_/2, pt.y, W_, Y_+L_-pt.y);

        // Draw thumb
        if (active_) lcd_.SetTextColor(CREATED_COLOR_);
        else         lcd_.SetTextColor(INACTIVE_TEXT_COLOR_);
        uint16_t width = SIZE_;
        uint16_t height = SIZE_;
        if (ORIENT_ == Holizontal) width = sizeS;
        else                       height = sizeS;
        uint16_t xPos = pt.x - width/2;
        uint16_t yPos = pt.y - height/2;
        
        if (fill)
            lcd_.FillRect(xPos, yPos, width+1, height+1);
        else
        {
            lcd_.DrawRect(xPos, yPos, width, height);
            lcd_.DrawHLine(pt.x+width/2, pt.y+height/2, 1);
            lcd_.DrawRect(xPos+1, yPos+1, width-2, height-2);
            lcd_.DrawHLine(pt.x+width/2-1, pt.y+height/2-1, 1);
            if (ORIENT_ == Holizontal)
                lcd_.DrawVLine(pt.x, yPos+4, SIZE_-7);
            else
                lcd_.DrawHLine(xPos+4, pt.y, SIZE_-7);
        }
    }

    void SeekBar::Redraw(bool fill)
    {
        Draw(GetValue(), fill);
        if (labelOn_)
            for (int n=0; n<3; n++) labelLCR_[n]->Draw(TEXT_COLOR_);
    }

    SeekBar::Point SeekBar::ToPoint(float value)
    {
        if (ORIENT_ == Holizontal)
            return Point(X_ + Round(L_*(value - MIN_)/(MAX_ - MIN_)), Y_);
        else
            return Point(X_, Y_ + L_ - Round(L_*(value - MIN_)/(MAX_ - MIN_)));
    }

    float SeekBar::ToValue(Point pt)
    {
        float value;
        if (ORIENT_ == Holizontal)
            value = (pt.x - X_)*(MAX_ - MIN_)/(float)L_ + MIN_;
        else
            value = -(pt.y - Y_ - L_)*(MAX_ - MIN_)/(float)L_ + MIN_;
        return Saturate(value);
    }

    float SeekBar::Saturate(float value)
    {
        if (value < MIN_) value = MIN_;
        if (value > MAX_) value = MAX_;
        return value;
    }
}

