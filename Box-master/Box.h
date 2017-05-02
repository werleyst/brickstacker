#ifndef Box_H
#define Box_H

#include <Arduino.h>

class Box{
  public:
     int xpos;
     int ypos;
     int width;
     int dir;
     Box();
     Box(int x, int y, int w);
};

#endif