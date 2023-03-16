//-----------------------------------------------------------
//  SeekBar class -- Header
//
//  2016/07/12, Copyright (c) 2016 MIKAMI, Naoki
//-----------------------------------------------------------

#ifndef F746_SEEKBAR_HPP
#define F746_SEEKBAR_HPP

#include "GuiBase.hpp"
#include "Label.hpp"

namespace Mikami
{
    class SeekBar : public GuiBase
    {
    public:
        enum Orientation { Holizontal, Vertical };

        struct Point
        {
            uint16_t x, y;
            Point(uint16_t x0 = 0, uint16_t y0 = 0) : x(x0), y(y0) {}
        };

        // Constructor
        SeekBar(uint16_t x, uint16_t y, uint16_t length,
                float min, float max, float initialValue,
                Orientation hv = Holizontal,
                uint32_t thumbColor = 0xFFB0B0FF,
                uint16_t thumbSize = 30, uint16_t width = 4,
                uint32_t colorL = LCD_COLOR_LIGHTGRAY,
                uint32_t colorH = 0xFFB0B0B0,
                uint32_t backColor = GuiBase::ENUM_BACK)
            : GuiBase(x, y, Font12, 0, backColor, thumbColor),
              L_(length), W_(width),
              SIZE_(thumbSize), COLOR_L_(colorL), COLOR_H_(colorH),
              MIN_(min), MAX_(max), ORIENT_(hv), v_(initialValue),
                labelOn_(false), slided_(false), active_(true)
        {   Draw(initialValue); }

        // Constructor with scale value (only horizontal)
        SeekBar(uint16_t x, uint16_t y, uint16_t length,
                float min, float max, float initialValue,
                string left, string center, string right,
                uint32_t thumbColor = 0xFFB0B0FF,
                uint16_t thumbSize = 30, uint16_t width = 4,
                uint32_t colorL = LCD_COLOR_LIGHTGRAY,
                uint32_t colorH = 0xFFB0B0B0,
                uint32_t backColor = GuiBase::ENUM_BACK);

        virtual ~SeekBar();
        
        bool Slide();
        float GetValue() { return v_; }
        int GetIntValue() { return Round(v_); }

        void Activate();
        void Inactivate();
        bool IsActive() { return active_; }

        bool IsOnThumb(uint16_t &x, uint16_t &y);
        void Draw(float value, bool fill = false);
        void Redraw(bool fill = false);
        float ToValue(Point pt);

        void SetValue(float v) { v_ = v; }
        void SetSlided(bool tf) { slided_ = tf; }
        bool GetSlided() { return slided_; }

    private:
        const uint16_t L_, W_;
        const uint16_t SIZE_;           // Size of thumb
        const uint32_t COLOR_L_, COLOR_H_;
        const float MIN_, MAX_;
        const Orientation ORIENT_;

        Label *labelLCR_[3];
        float v_;             // value of seekbar
        bool labelOn_;
        bool slided_;
        bool active_;

        int Round(float x) { return x + 0.5f - (x < 0); }
        Point ToPoint(float value);
        float Saturate(float value);

        // disallow copy constructor and assignment operator
        SeekBar(const SeekBar&);
        SeekBar& operator=(const SeekBar&);
    };
}
#endif  // F746_SEEKBAR_HPP
