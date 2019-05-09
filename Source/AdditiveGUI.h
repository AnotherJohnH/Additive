//------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

#ifndef ADDITIVE_GUI_H
#define ADDITIVE_GUI_H

#include <functional>

#include "SND/SND.h"
#include "GUI/GUI.h"

#include "Scope.h"


template <unsigned HARMONICS>
class AdditiveGUI : public GUI::App
{
private:
   // Audio components
   SND::AdditiveOsc<HARMONICS>  osc{440.0};
   SND::Monitor                 monitor{};

   // GUI components
   GUI::MenuBar      menu_bar{this};
   GUI::TextButton   mute_btn{  &menu_bar, 'm', "Mute"};
   GUI::TextButton   reset_btn{ &menu_bar, 'R', "Reset"};
   GUI::TextButton   sine_btn{  &menu_bar, 's', "Sine"};
   GUI::TextButton   square_btn{&menu_bar, 'U', "Square"};
   GUI::TextButton   ramp_btn{  &menu_bar, 'V', "Ramp"};
   GUI::TextButton   pulse_btn{ &menu_bar, 'p', "Pulse"};
   Scope             scope{this};
   GUI::Row          controls{this};
   bool              mute{true};

   struct CoefControl
   {
      GUI::Col       col;
      std::string    label;
      GUI::Text      text;
      GUI::Slider    slider;
      GUI::Field<4>  field;
   };

   CoefControl coef_table[HARMONICS];

   //! Update all the sliders
   void setSliders(std::function<double(unsigned)> func)
   {
      for(unsigned n=1; n<=HARMONICS; ++n)
      {
         size_t i = n - 1;
         coef_table[i].slider.setValue(func(n) * 0.5 + 0.5);
      }
   }

   // Handle GUI application events
   virtual void appEvent(Widget* item_, unsigned code_) override
   {
      switch(code_)
      {
      case 'm':
         mute = !mute;
         mute_btn.setSelect(mute);
         if (mute)
            monitor.stop();
         else
            monitor.play();
         break;

      case 'R': setSliders([](unsigned n) -> double { return 0.0;                 }); break;
      case 's': setSliders([](unsigned n) -> double { return n == 1 ? 1.0 : 0.0;  }); break;
      case 'U': setSliders([](unsigned n) -> double { return n % 2 ? 1.0/n : 0.0; }); break;
      case 'V': setSliders([](unsigned n) -> double { return 1.0/n;               }); break;
      case 'p': setSliders([](unsigned n) -> double { return 1.0;                 }); break;

      default:
         if ((code_ >= 1) && (code_ <= HARMONICS))
         {
            GUI::Slider* slider = reinterpret_cast<GUI::Slider*>(item_);
            osc.a[code_] = slider->getValue() * 2.0 - 1.0;
            signed value = osc.a[code_] * 100;
            std::string value_str = std::to_string(value);
            size_t i = code_ - 1;
            coef_table[i].field.setValue(value_str.c_str());
         }
         break;
      }
   }

public:
   AdditiveGUI()
      : GUI::App("Fourier Synthesis", &GUI::font_teletext18)
   {
      setBorderAndGap(8);
      mute_btn.setSelect(mute);

      scope.setSize(1024, 256);
      scope.setForegroundColour(STB::GREEN);
      scope.setBackgroundColour(STB::BLACK);

      size_t n;
      scope.setTable(osc.getTable(n), n);

      for(size_t n=1; n<=HARMONICS; n++)
      {
         size_t i = n - 1;
         CoefControl& coef = coef_table[i];

         coef.label = std::to_string(n);
         coef.col.init(&controls);
         coef.text.init(&coef.col, coef.label.c_str());
         coef.text.setAlign(GUI::Align::CENTER);
         coef.slider.init(&coef.col, n, 400);
         coef.field.init(&coef.col, 0, " 100");
      }

      setSliders([](unsigned n) -> double { return 0.0; });

      monitor.in = osc;
   }
};

#endif
