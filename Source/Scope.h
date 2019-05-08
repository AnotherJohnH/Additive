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

#ifndef SCOPE_H
#define SCOPE_H

#include "GUI/GUI.h"

#include <vector>

class Scope
   : public GUI::Widget
   , public GUI::ColourPair
{
private:
   static const STB::Colour grid_col    = STB::RGB(0x30, 0x30, 0x30);
   static const STB::Colour grid_col_br = STB::RGB(0x80, 0x80, 0x80);
   static const STB::Colour trace_col   = STB::RGB(0x40, 0xFF, 0x40);

   const double* table{};
   size_t        table_size = 0;

   virtual void eventDraw(GUI::Canvas& canvas) override
   {
      canvas.fillRect(bg_colour, pos.x, pos.y, pos.x + size.x, pos.y + size.y);

      drawGrid(canvas, pos.x, pos.y, size.x, size.y, 16, 8);

      for(size_t i=0; i<table_size*2; ++i)
      {
         unsigned j = i % table_size;
         unsigned x = i * size.x / (table_size*2);
         unsigned y = (-table[j]*size.y + size.y) / 2;

         canvas.drawPoint(trace_col, x + pos.x, y + pos.y);
      }
   }

   void drawGrid(GUI::Canvas&  canvas,
                 unsigned      ox,    unsigned oy,
                 unsigned      width, unsigned height,
                 unsigned      xdiv,  unsigned ydiv)
   {
      for(unsigned x=0; x<=width; x+=width/xdiv)
      {
         bool border = (x % (width/2)) == 0;
         canvas.drawLine(border ? grid_col_br : grid_col, ox+x, oy+1, ox+x, oy+height);
      }

      for(unsigned y=0; y<=height; y+=height/ydiv)
      {
         bool border = (y % (height/2)) == 0;
         canvas.drawLine(border ? grid_col_br : grid_col, ox, oy+y, ox+width, oy+y);
      }
   }

public:
   Scope(GUI::Widget* parent)
      : GUI::Widget(parent)
   {
   }

   void setTable(const double* table_, size_t size_)
   {
      table      = table_;
      table_size = size_;
   }
};

#endif
