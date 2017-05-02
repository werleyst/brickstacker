#include "Box.h"

  int xpos;
  int ypos;
  int width;
  int dir;

  Box::Box(){
    xpos=0;
    ypos=0;
    width=1;
    dir=1;
  }

  Box::Box(int x, int y, int w){
    xpos=x;
    ypos=y;
    width=w;
    dir=1;
  }