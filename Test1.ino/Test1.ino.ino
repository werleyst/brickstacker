#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <Box.h>

//_______________________________________________________________________________________________________
//LED matrix set up
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


//_______________________________________________________________________________________________________
//Global variables
int button = 52;
int boxWidth = 16;
int boxHeight = 4;
int level = 0;
Box *boxes[8]; //array of each layer of boxes
  

//Set up matrix and array of boxes
void setup() {
  pinMode(button, INPUT);
  matrix.begin();
  matrix.fillScreen(matrix.Color333(0,0,0));
  level=0;
  for(int i=0; i<8; i++){
    boxes[i] =  new Box(rand()%32,28-i*4,boxWidth);
  }
  boxes[0]->dir = (rand()%2==0?-1:1); //boxes start in random position an direction
  drawBox(boxes[0]);
  delay(500);
}

//Cycle for gameplay
void loop() {
  cycleBox(boxes[level%8],min(level+2,20)); //moves current box back and forth
  checkButton(); //checks for button press
}


//_______________________________________________________________________________________________________
//Checks button, if pressed then checks box and handles leveling/game over
void checkButton(){
  if(digitalRead(button) == HIGH){ //looks for button press

    //first level player can go anywhere
    if(level==0){
      delay(1000);
      level++;
      drawBox(boxes[1]);
      boxes[1]->dir = (rand()%2==0?-1:1);
    }

    //resets screen after top layer, starts at bottom again
    else if(level%8==7){
      checkAlign();
      delay(500);

      matrix.fillScreen(matrix.Color333(0,0,0)); //reset screen
      boxes[0]->xpos = boxes[7]->xpos;
      boxes[0]->width = boxes[7]->width;
      boxes[1]->width = boxes[0]->width;
      boxes[1]->xpos = rand()%32;
      boxes[1]->dir = (rand()%2==0?-1:1); //boxes start in random position an direction

      level++;
      drawBox(boxes[0]);
      drawBox(boxes[1]);
    }

    //normal level, not first or top of screen
    else{
      checkAlign();
      delay(500);
      boxes[level%8]->xpos = rand()%32;
      drawBox(boxes[level%8]);
      boxes[level%8]->dir = (rand()%2==0?-1:1);//boxes start in random position an direction
    }
  }
}

//Checks alignment of box after press. If there is error, the box falls and shrinks. If complete miss, game is over. Otherwise the level increases
void checkAlign(){
  int error = boxes[level%8]->xpos - boxes[(level-1)%8]->xpos;
  int xpos = boxes[level%8]->xpos;
  int ypos = boxes[level%8]->ypos;
  int width = boxes[level%8]->width;
  Box *fall;

  if(abs(error)>=width){ //complete miss
    delay(1000);
    loseGame();
    
  }else if(error < 0){ //error on left side
    boxes[level%8]->xpos+=abs(error);
    boxes[level%8]->width-=abs(error);
    fall = new Box(xpos,ypos,abs(error));
    boxFall(fall);
    
  }else if(error > 0){ //error on ride side
    boxes[level%8]->width-=error;
    fall = new Box(xpos+width-error,ypos,error);
    boxFall(fall);
  }
  
  level++;
  boxes[level%8]->width=width-abs(error);
  boxes[level%8]->xpos=rand()%32;
}


//_______________________________________________________________________________________________________
//Methods for working with boxes

//draws the box in more concise way
void drawBox(Box *b){
  matrix.fillRect(b->xpos, b->ypos, b->width, boxHeight,white); 
}

//moves box one pixel left or right based on direction
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

//moves box back and forth across entire screen, speed can change based on level
void cycleBox(Box *b, int speed){
  if(b->xpos > matrix.width() || b->xpos < 0-b->width){
    b->dir = -b->dir;
  }
  moveBox(b);
  delay(200.0/speed);
}

//used for making piece of box fall off the screen
void boxFall(Box *b){
  while(b->ypos <= 32){
    matrix.fillRect(b->xpos, b->ypos+boxHeight, b->width, 1,white); 
    matrix.fillRect(b->xpos, b->ypos, b->width, 1,black);
    b->ypos = b->ypos+1;
    delay(50);
  }
}

//_______________________________________________________________________________________________________
//Handles the game over screen and reset
void loseGame(){
  //screen turns black, says "game over" and gives score
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  
  // draw some text!
  matrix.setCursor(0, 0);    // start at top left, with one pixel of spacing
  matrix.setTextSize(1);     // size 1 == 8 pixels high
  //matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves

  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.println("Game");
  matrix.setCursor(0, 8);
  matrix.println("Over!");
  matrix.setCursor(0,16);
  matrix.print("Lv");
  matrix.setCursor(14,16);
  matrix.println(level);
  delay(1000000);
}


