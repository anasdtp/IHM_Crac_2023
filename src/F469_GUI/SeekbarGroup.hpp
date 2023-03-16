//-----------------------------------------------------------
//  SeekbarGroup class -- Header
//
//  2016/07/12, Copyright (c) 2016 MIKAMI, Naoki
//-----------------------------------------------------------

#ifndef F746_SEEKBAR_GROUP_HPP
#define F746_SEEKBAR_GROUP_HPP

#include "SeekBar.hpp"

namespace Mikami
{
    class SeekbarGroup : public GuiBase
    {
    public:
        SeekbarGroup(uint16_t x0, uint16_t y0,  uint16_t length,
                     uint16_t number, uint16_t space,
                     float min, float max, float initialValue,
                     SeekBar::Orientation hv = SeekBar::Holizontal,
                     uint32_t thumbColor = 0xFFB0B0FF,
                     uint16_t thumbSize = 30, uint16_t width = 4,
                     uint32_t colorL = LCD_COLOR_LIGHTGRAY,
                     uint32_t colorH = 0xFFB0B0B0,
                     uint32_t backColor = GuiBase::ENUM_BACK);

        virtual ~SeekbarGroup();
        
        bool Slide(int num) { return seekBars_[num]->Slide(); }
        float GetValue(int num) { return seekBars_[num]->GetValue(); }
        int GetIntValue(int num) { return seekBars_[num]->GetIntValue(); }

        // Get slided number
        bool GetSlidedNumber(int &num);
        
        void Draw(int num, float value, bool fill = false)
        {   seekBars_[num]->Draw(value, fill); }

        // Draw all thumbs with same value
        void DrawAll(float value, bool fill = false)
        {   for (int n=0; n<numberOfSeekBar_; n++) Draw(n, value, fill); }

        void Redraw(int num, bool fill = false)
        {   seekBars_[num]->Draw(seekBars_[num]->GetValue(), fill); }

        void RedrawAll(bool fill = false)
        {   for (int n=0; n<numberOfSeekBar_; n++) Redraw(n, fill); }

        // Activate and inactivate
        void Activate(int num) { seekBars_[num]->Activate(); }
        void Inactivate(int num) { seekBars_[num]->Inactivate(); }
        void ActivateAll()
        {   
            for (int n=0; n<numberOfSeekBar_; n++)
                seekBars_[n]->Activate();
        }
        void InactivateAll()
        {
            for (int n=0; n<numberOfSeekBar_; n++)
                seekBars_[n]->Inactivate();
        }

    private:
        SeekBar **seekBars_;
        int numberOfSeekBar_;

        // Check range of argument
        bool Range(int n)
        { return ((n >= 0) && (n < numberOfSeekBar_)); }

        // disallow copy constructor and assignment operator
        SeekbarGroup(const SeekbarGroup&);
        SeekbarGroup& operator=(const SeekbarGroup&);
    };
}
#endif  //  F746_SEEKBAR_GROUP_HPP
