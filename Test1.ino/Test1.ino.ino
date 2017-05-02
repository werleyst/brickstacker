#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <Box.h>

// If your 32x32 matrix has the SINGLE HEADER input,
// use this pinout:
#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3
// If your matrix has the DOUBLE HEADER input, use:
//#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
//#define LAT 9
//#define OE  10
//#define A   A3
//#define B   A2
//#define C   A1
//#define D   A0
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);
uint16_t black = matrix.Color444(0, 0, 0);
uint16_t white = matrix.Color444(15, 15, 15);
uint16_t yellow = matrix.Color444(15, 15, 0);

  int button = 52;
  int boxWidth = 8;
  int boxHeight = 4;
  int level = 1;
  int oldBoxX;
  Box *boxes[8];
  

void setup() {
  pinMode(button, INPUT);
  matrix.begin();
  for(int i=0; i<8; i++){
    boxes[i] =  new Box(12,28-i*4,boxWidth);
  }
  drawBox(boxes[0]);
}

void loop() {
  cycleBox(boxes[level%8],min(level+3,10));
  stopBox();
}

void stopBox(){
  if(digitalRead(button) == HIGH){
    if(level==0){
      delay(1000);
      level++;
      drawBox(boxes[level]);
    }
    else if(level%8==7){   //resets screen after top layer
      checkAlign();

      matrix.fillScreen(matrix.Color333(0,0,0));
      boxes[0]->xpos = boxes[7]->xpos;
      boxes[0]->width = boxes[7]->width;
      boxes[1]->width = boxes[0]->width;
      boxes[1]->xpos = 28;
      boxes[1]->dir = 1;
      delay(500);
      drawBox(boxes[0]);
      drawBox(boxes[1]);
      level++;
    }
    else{
      checkAlign();
      delay(500);
      drawBox(boxes[level%8]);
    }
  }
}

void checkAlign(){
  int error = boxes[level%8]->xpos - boxes[(level-1)%8]->xpos;
  int xpos = boxes[level%8]->xpos;
  int ypos = boxes[level%8]->ypos;
  int width = boxes[level%8]->width;
  Box *fall;

  if(abs(error)>width){
    loseGame();
  }
  else if(error < 0){
    boxes[level%8]->xpos+=error;
    boxes[level%8]->width-=error;
    fall = new Box(xpos,ypos,error);
    boxFall(fall);
  }
  else if(error > 0){
    boxes[level%8]->width-=error;
    fall = new Box(xpos+width-error,ypos,error);
    boxFall(fall);
  }
  level++;
  boxes[level%8]->width=width-error;
  boxes[level%8]->xpos=28;
}

void drawBox(Box *b){
  matrix.fillRect(b->xpos, b->ypos, b->width, boxHeight,white); 
}

void moveBox(Box *b){
  if(b->dir == 1){
    matrix.fillRect(b->xpos+b->width, b->ypos, 1, boxHeight,white);
    matrix.fillRect(b->xpos, b->ypos, 1, boxHeight,black); 
    b->xpos = b->xpos+1;
  }
  if(b->dir == -1){
    matrix.fillRect(b->xpos-1, b->ypos, 1, boxHeight,white); 
    matrix.fillRect(b->xpos+b->width-1, b->ypos, 1, boxHeight,black);
    b->xpos = b->xpos-1;
  }
}

void cycleBox(Box *b, int speed){
  if(b->xpos > matrix.width() || b->xpos < 0-b->width){
    b->dir = -b->dir;
  }
  moveBox(b);
  delay(100.0/speed);
}

void boxFall(Box *b){
  while(b->ypos <= 32){
    matrix.fillRect(b->xpos, b->ypos+boxHeight, b->width, 1,white); 
    matrix.fillRect(b->xpos, b->ypos, b->width, 1,black);
    b->ypos = b->ypos+1;
    delay(50);
  }
}


void loseGame(){
  //screen turns black, says "game over" and gives score
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  
  // draw some text!
  matrix.setCursor(1, 0);    // start at top left, with one pixel of spacing
  matrix.setTextSize(1);     // size 1 == 8 pixels high
  //matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves

  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.println("Game");
  matrix.setCursor(1, 9);
  matrix.println("Over!");
  matrix.setCursor(1,18);
  matrix.println("Score:"+level);
  delay(100000000);
}


