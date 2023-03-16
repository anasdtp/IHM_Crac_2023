//--------------------------------------------------------------
//  F746-DISCO の GUI 用のインクルード文とそれらを使うための
//  using 宣言をまとめたもの
//  Include statements for F746-DISCO GUI and using directive
//
//  2016/08/15, Copyright (c) 2016 MIKAMI, Naoki
//  2016/09/21, modifyed for use whith F469I-DISCO Axel Karch
//
//--------------------------------------------------------------

#include "mbed.h"
#ifndef __STM32F469xx_H
#error Platform is not F469I-DISCOVERY
#endif

#include "BlinkLabel.hpp"
#include "ButtonGroup.hpp"
#include "NumericLabel.hpp"
#include "ResetButton.hpp"
#include "SeekbarGroup.hpp"

using namespace Mikami;
