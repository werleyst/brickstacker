#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

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
  int boxX = 14;
  int boxY = 28;
  int boxWidth = 8;
  int boxHeight = 4;
  int boxDirection = 1 ;
  int level = 1;
  int oldBoxX;
  int savedBoxX = 0;
  int boxDelete = 0;
  float    dx, dy, d;
  uint16_t blockColor;
  //float delayTime = .5 * (7* (9-level));
  bool buttonPressed = false;

void setup() {
  pinMode(button, INPUT);
  matrix.begin();
}

void loop() {
  Serial.print("serial test");
  //matrix.fillRect(boxY, boxX, boxWidth, boxHeight, matrix.Color333(0, 7, 0));
  checkButton();
  moveBox();
  
}

void loseGame(){
  //screen turns black, says "game over" and gives score
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  
  // draw some text!
  matrix.setCursor(1, 0);    // start at top left, with one pixel of spacing
  matrix.setTextSize(1);     // size 1 == 8 pixels high
  matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves

  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.println(" You");
  matrix.setCursor(2, 9);
  matrix.println("Lose!");
  delay(100000000);
}

void moveBox() {

  //checkButton();
  stopBox();
  
  // if the box goes offscreen, reverse the direction:
  if (boxX > matrix.width()-1 || boxX < 1-boxWidth) {
    boxDirection = -boxDirection;
    
  } 
 
  // update the box's position
  boxX += boxDirection;
  delay(.5 * (7* (9-level)));
 
  // erase the box's previous position
  if (oldBoxX != boxX ) {
    matrix.fillRect(oldBoxX, boxY, boxWidth, boxHeight,black);
  }
  // draw the box's current position
  matrix.fillRect(boxX, boxY, boxWidth, boxHeight,white);
 
  oldBoxX = boxX;
}

void checkButton(){
  if(digitalRead(button) == HIGH && buttonPressed == false){
    buttonPressed = true;
    Serial.println("Button Pressed");
  }
}

void stopBox(){
  if(digitalRead(button) == HIGH){
    boxDirection = 0;
    
    delay(500);
    boxY = boxY-4;
    level++;
    boxDirection = 1;
    checkAlign();
    buttonPressed == false;
    saveBox();
  }
  
}

void saveBox(){
  savedBoxX = boxX;
}

void checkAlign(){
  if(savedBoxX == 0){
      return; //do nothing
  }
  else if(boxX != savedBoxX){
    boxDelete = abs(boxX - savedBoxX);
    if(boxDelete > (boxWidth - 1)){
      loseGame();
    }
    else{
      boxWidth = boxWidth - boxDelete;
    }
  }
}

void draw(){
  //dope
}




