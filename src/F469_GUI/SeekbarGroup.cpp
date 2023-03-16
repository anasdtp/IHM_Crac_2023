//-----------------------------------------------------------
//  SeekbarGroup class
//
//  2016/04/30, Copyright (c) 2016 MIKAMI, Naoki
//-----------------------------------------------------------

#include "SeekbarGroup.hpp"

namespace Mikami
{
    SeekbarGroup::SeekbarGroup(
            uint16_t x0, uint16_t y0, uint16_t length,
            uint16_t number, uint16_t space,
            float min, float max, float initialValue,
            SeekBar::Orientation hv,
            uint32_t thumbColor,
            uint16_t thumbSize, uint16_t width,
            uint32_t colorL, uint32_t colorH,
            uint32_t backColor)
        : GuiBase(x0, y0, Font12, GuiBase::ENUM_TEXT, backColor, thumbColor),
          numberOfSeekBar_(number)
    {
        seekBars_ = new SeekBar *[number];
        for (int n=0; n<number; n++)
        {
            uint16_t x = x0;
            uint16_t y = y0;
            if (hv == SeekBar::Holizontal) y += space*n;
            else                           x += space*n;
            seekBars_[n] =
                new SeekBar(x, y, length, min, max, initialValue, hv,
                            thumbColor, thumbSize, width,
                            colorL, colorH, backColor);
        }
    }

    // Destructor
    SeekbarGroup::~SeekbarGroup()
    {
        for (int n=0; n<numberOfSeekBar_; n++) delete seekBars_[n];
        delete[] seekBars_;
    }

    // Get slided number
    bool SeekbarGroup::GetSlidedNumber(int &num)
    {
        bool active = false;
        for (int n=0; n<numberOfSeekBar_; n++)
            if (seekBars_[n]->IsActive()) active = true;
        if (!active) return false;

        if (!PanelTouched())
        {
            for (int n=0; n<numberOfSeekBar_; n++)
            {
                if (seekBars_[n]->GetSlided())
                    seekBars_[n]->Draw(seekBars_[n]->GetValue());
                seekBars_[n]->SetSlided(false);
            }
            return false;
        }

        bool rtn = false;
        uint16_t x, y;
        for (int n=0; n<numberOfSeekBar_; n++)
        {
            if (seekBars_[n]->IsOnThumb(x, y))
            {
                if ((num != n) && Range(num))
                    seekBars_[num]->Draw(seekBars_[num]->GetValue());
                num = n;
                seekBars_[n]->SetValue(seekBars_[n]->ToValue(SeekBar::Point(x, y)));
                seekBars_[n]->Draw(seekBars_[n]->GetValue(), true);
                seekBars_[n]->SetSlided(true);
                rtn = true;
            }
            if (rtn) break;
        }
        return rtn;
    }
}
