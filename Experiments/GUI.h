#pragma once

#include "TArray.h"

struct point
{
  point(int _x = 0, int _y = 0) : x(_x), y(_y) {}
  int x = 0;
  int y = 0;
};

point operator+(const point &a, const point &b) { return point(a.x + b.x, a.y + b.y); }
point operator-(const point &a, const point &b) { return point(a.x - b.x, a.y - b.y); }

struct rect
{
  rect(point _center = point(), int _w = 0, int _h = 0) : center(_center), w(_w), h(_h) {}
  rect(int _x = 0, int _y = 0, int _w = 0, int _h = 0) : center(_x, _y), w(_w), h(_h) {}
  point center;
  size_t w, h;

  bool is_in(const point &p) 
  { 
    point diff = p - center;
    // not strict, can be on border
    return 
      (diff.x >= 0) && 
      (diff.y >= 0) && 
      (diff.x <= w) && 
      (diff.y <= h);
  }
};

struct key_state
{
  bool pressed;
  bool released;
};

struct click_event
{
  point p;
  key_state buttons[8];
};

class widget
{
  rect rect_;
  TArray<widget*> children_;

  void handle_mouse_event(const click_event& e) 
  {
    for (auto w : children_)
      w->handle_mouse_event(e);

    if (rect_.is_in(e.p))
    {
      if (e.buttons[0].pressed)   on_lmb_press    (e);
      if (e.buttons[1].pressed)   on_rmb_press    (e);
      if (e.buttons[0].released)  on_lmb_released (e);
      if (e.buttons[1].released)  on_rmb_released (e);
    }
  }

  virtual void on_lmb_press   (const click_event& e) {}
  virtual void on_rmb_press   (const click_event& e) {}
  virtual void on_lmb_released(const click_event& e) {}
  virtual void on_rmb_released(const click_event& e) {}
};

class GUI : private widget
{  

};