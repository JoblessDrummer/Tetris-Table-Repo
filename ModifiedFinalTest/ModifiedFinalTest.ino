
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "DFRobotDFPlayerMini.h"
#include <LiquidCrystal.h>
#include "EEPROMAnything.h"
#include <SoftwareSerial.h>
#include <Adafruit_SSD1306.h>
#define GRID_W              (10) //Width of tetris grid
#define GRID_H              (20) //Height of tetris grid
#define STRAND_LENGTH       (GRID_W*GRID_H)
#define LED_DATA_PIN        (6)  // this pin is connected to the first neopixel input. 
#define BACKWARDS           (0)
#define PIECE_W             (4) //Width of matrix (4x4)
#define PIECE_H             (4) //Height of matrix
#define NUM_PIECE_TYPES     (7) // number of tetris pieces is seven
#define arr_len( x )  ( sizeof( x ) / sizeof( *x ) )
#define DROP_MINIMUM        (100)  // top speed gravity can reach (1 Drop per 0.1 seconds)
#define DROP_ACCELERATION   (200)  // how fast gravity increases (-0.2 seconds every level)
#define INITIAL_MOVE_DELAY  (50)
#define INITIAL_DROP_DELAY  (900) //start speed (1 Drop per 0.9 seconds)
#define INITIAL_DRAW_DELAY  (30)
#define SCREEN_WIDTH 128 //Width of Tetris Piece OLED Screen
#define SCREEN_HEIGHT 64 // Height of Tetris Piece OLED Screen
#define SCREEN_GRID_COLS 4 //Number of Columns configured in the OLED Screen
#define SCREEN_GRID_ROWS 2 //Number of Columns configured in the OLED Screen
#define BLOCK_WIDTH (SCREEN_WIDTH / SCREEN_GRID_COLS) //Width of each block drawn (32) 
#define BLOCK_HEIGHT (SCREEN_HEIGHT / SCREEN_GRID_ROWS) //Height of each block drawn (32)
#define OLED_SDA 20 //SDA Pin to OLED Screen
#define OLED_SCK 21 //SCK Pin to OLED Screen
#define OLED_DC 3 //DC Pin to OLED Screen
#define OLED_CS 4 //CS Pin to OLED Screen
#define OLED_RESET 2 //Reset Pin to OLED Screen
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(20, 10, LED_DATA_PIN, NEO_MATRIX_TOP + NEO_MATRIX_RIGHT 
+NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE + NEO_MATRIX_ZIGZAG, NEO_GRB + NEO_KHZ800); //Initializes Matrix
SoftwareSerial mySoftwareSerial(12, 13);  //Initializes SoftwareSerial Communication for DF Player(Pin 12 to RX and Pin 13 to TX)
DFRobotDFPlayerMini myDFPlayer;
LiquidCrystal lcdScreen(49, 47, 35, 33, 31, 29); //rs, en, d4, d5, d6, d7
Adafruit_SSD1306 tetrisDisplay
(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_SDA, OLED_SCK, OLED_DC, OLED_RESET, OLED_CS); //wire SCK to 21 and SDA to 20.
void function1();    
void function2();
void function3();
void function4();     
void function5();
void function6();
void (*functionPtr[])() = { function1, function2, function3, function4, function5, function6};
char* Text[]= { "GAME OVER!!" };
const uint16_t colors[] = 
{matrix.Color(255, 255, 255)};
int brightness = 105;
int piece_id;
int next_piece_id;
int wipepixel = 0;
int wipepixelend = 0; 
int volumeLevel = 20;  // Default volume (0 to 30)
int exponent = 0;
int numMode = arr_len(Text)-1;
int numColor = arr_len(colors)-1;
int pixelPerChar =4;
int maxDisplacement;
int mode = 0;
int y = matrix.height();
int x = matrix.width();
int line_pass = 0;
int button_left = 11;       //pull this pin to low with a button. 
int button_right = 5;
int button_up = 9;
int button_down = 10;
int button_pause = 30;
int button_start = 7;
int volUpButton = 53;  // Volume up button
int volDownButton = 51; // Volume down button
int lastPauseTrack = -1;
byte adr = 0x08;
byte num = 0x00;
int i = 0;
int level = 1;
int lineThreshold = 4;
int linesCleared = 0;
bool Pause = false;
bool onStartScreen = true;
bool pause_onece = false;
bool pause_pressed = false;
bool dfPlayerInitialized = false;
bool isPauseMusicPlaying = false;
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
unsigned long lastVolUpPress = 0;
unsigned long lastVolDownPress = 0;
unsigned long lastPausePress = 0;
unsigned long lastStartPress = 0;
const unsigned long debounceDelay = 800;
const unsigned long rotateDelay = 100;

const char piece_I[] = {
  0, 0, 0, 0,
  1, 1, 1, 1,
  0, 0, 0, 0,
  0, 0, 0, 0,

  0, 0, 1, 0,
  0, 0, 1, 0,
  0, 0, 1, 0,
  0, 0, 1, 0,

  0, 0, 0, 0,
  0, 0, 0, 0,
  1, 1, 1, 1,
  0, 0, 0, 0,

  0, 1, 0, 0,
  0, 1, 0, 0,
  0, 1, 0, 0,
  0, 1, 0, 0,
};

const char piece_L[] = {
  0, 0, 1, 0,
  1, 1, 1, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,

  0, 1, 0, 0,
  0, 1, 0, 0,
  0, 1, 1, 0,
  0, 0, 0, 0,

  0, 0, 0, 0,
  1, 1, 1, 0,
  1, 0, 0, 0,
  0, 0, 0, 0,

  1, 1, 0, 0,
  0, 1, 0, 0,
  0, 1, 0, 0,
  0, 0, 0, 0,
};

const char piece_J[] = {
  1, 0, 0, 0,
  1, 1, 1, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,

  0, 1, 1, 0,
  0, 1, 0, 0,
  0, 1, 0, 0,
  0, 0, 0, 0,

  0, 0, 0, 0,
  1, 1, 1, 0,
  0, 0, 1, 0,
  0, 0, 0, 0,

  0, 1, 0, 0,
  0, 1, 0, 0,
  1, 1, 0, 0,
  0, 0, 0, 0,

};

const char piece_T[] = {
  0, 1, 0, 0,
  1, 1, 1, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,

  0, 1, 0, 0,
  0, 1, 1, 0,
  0, 1, 0, 0,
  0, 0, 0, 0,

  0, 0, 0, 0,
  1, 1, 1, 0,
  0, 1, 0, 0,
  0, 0, 0, 0,

  0, 1, 0, 0,
  1, 1, 0, 0,
  0, 1, 0, 0,
  0, 0, 0, 0,

};

const char piece_S[] = {
  0, 1, 1, 0,
  1, 1, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,

  0, 1, 0, 0,
  0, 1, 1, 0,
  0, 0, 1, 0,
  0, 0, 0, 0,

  0, 0, 0, 0,
  0, 1, 1, 0,
  1, 1, 0, 0,
  0, 0, 0, 0,

  1, 0, 0, 0,
  1, 1, 0, 0,
  0, 1, 0, 0,
  0, 0, 0, 0,
};

const char piece_Z[] = {
  1, 1, 0, 0,
  0, 1, 1, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,

  0, 0, 1, 0,
  0, 1, 1, 0,
  0, 1, 0, 0,
  0, 0, 0, 0,

  0, 0, 0, 0,
  1, 1, 0, 0,
  0, 1, 1, 0,
  0, 0, 0, 0,

  0, 1, 0, 0,
  1, 1, 0, 0,
  1, 0, 0, 0,
  0, 0, 0, 0,
};

const char piece_O[] = {
  1, 1, 0, 0,
  1, 1, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,

  1, 1, 0, 0,
  1, 1, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,

  1, 1, 0, 0,
  1, 1, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,

  1, 1, 0, 0,
  1, 1, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
};

const char *pieces[NUM_PIECE_TYPES] = {
  piece_S,
  piece_Z,
  piece_L,
  piece_J,
  piece_O,
  piece_T,
  piece_I,
};

const long piece_colors[NUM_PIECE_TYPES] = 
{
  0x009900, // green 
  0xFF0000, // red 
  0xFF8000, // orange 
  0x0000FF, // blue 
  0xFFFF00, // yellow 
  0xFF00FF, // purple 
  0x00FFFF,  // cyan 
};
const int numColors = sizeof(piece_colors) / sizeof(piece_colors[0]);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRAND_LENGTH, LED_DATA_PIN, NEO_RGB + NEO_KHZ800);
int old_px = 0;
int piece_rotation;
int old_pr;
int piece_x;
int piece_y;
char piece_sequence[NUM_PIECE_TYPES];
char sequence_i = NUM_PIECE_TYPES;
long currentPieceColor;
long last_move;
long move_delay;  // 100ms = 5 times a second
long last_drop;
long drop_delay;  //1000, initialized to INTIAL_DROP_DELAY of 900 at setup
long last_draw;
long draw_delay;  // 60 fps
long grid[GRID_W * GRID_H];


void p(int x, int y, long color) 
{
  int a = (GRID_H - 1 - y) * GRID_W;
  if ( ( y % 2 ) == BACKWARDS ) { // % is modulus.
    // y%2 is false when y is an even number - rows 0,2,4,6.
    a += x;
  } else {
    // y%2 is true when y is an odd number - rows 1,3,5,7.
    a += GRID_W - 1 - x;
  }
  a %= STRAND_LENGTH;
  strip.setPixelColor(a, color);
}
void draw_grid() 
{
  int x, y;
  for (y = 0; y < GRID_H; ++y) {
    for (x = 0; x < GRID_W; ++x) {
      if ( grid[y * GRID_W + x] != 0 ) {
        p(x, y, grid[y * GRID_W + x]);
      } else {
        p(x, y, 0);
      }
    }
  }
  strip.show();
}

void choose_new_piece() 
{
  long randomColor;
  if ( sequence_i >= NUM_PIECE_TYPES ) 
  {
     int i, j, k;
    for (i = 0; i < NUM_PIECE_TYPES; ++i) 
    {
      do 
      {
        j = rand() % NUM_PIECE_TYPES;
        // make sure it isn't already in the sequence.
        for (k = 0; k < i; ++k) 
        {
          if (piece_sequence[k] == j) break; // already in sequence
        }
      } while (k < i);
      // not in sequence.  Add it.
      piece_sequence[i] = j;
    }
    // rewind sequence counter
    sequence_i = 0;
  }
  piece_id = next_piece_id;
  // get the next piece in the sequence.
  next_piece_id = piece_sequence[sequence_i++];
  Serial.print("Next Piece ID is ");
  Serial.println(next_piece_id);
  displayNext();
  randomColor = random(numColors);
  Serial.print("Random Color: ");
  Serial.println(randomColor);
  Serial.print("Current Piece Color is ");
  Serial.println(currentPieceColor);
  if(piece_colors[randomColor] != currentPieceColor)
  {
    currentPieceColor = piece_colors[randomColor];
  }
  //currentPieceColor = piece_colors[random(numColors)];
  

  piece_y = -1; // -1 squares off the top of the screen.
  piece_x = 3;
  // always start in the same orientation.
  piece_rotation = 0;
}
void displayNext()
{
  if(next_piece_id == 1)
  {
  //Next Piece is Z
  tetrisDisplay.clearDisplay();
  drawBlock(0, 1); 
  drawBlock(1, 0);
  drawBlock(1, 1);
  drawBlock(2, 0);  
  tetrisDisplay.display();
  }
  else if (next_piece_id == 0)
  {
  //Next Piece is S
  tetrisDisplay.clearDisplay();
  drawBlock(0, 0); 
  drawBlock(1, 0);
  drawBlock(1, 1);
  drawBlock(2, 1);  
  tetrisDisplay.display();
  }
  else if(next_piece_id == 2)
  {
  //Next Piece is L
  tetrisDisplay.clearDisplay();
  drawBlock(0, 0); 
  drawBlock(0, 1);
  drawBlock(1, 1);
  drawBlock(2, 1);  
  tetrisDisplay.display();
  }
  else if(next_piece_id == 3)
  {
    //Next Piece is J
  tetrisDisplay.clearDisplay();
  drawBlock(2, 0);  
  drawBlock(0, 1);
  drawBlock(1, 1);
  drawBlock(2, 1);
  tetrisDisplay.display();
  }
  else if(next_piece_id == 5)
  {
    //Next Piece is T
  tetrisDisplay.clearDisplay();
  drawBlock(1, 0);  
  drawBlock(1, 1);
  drawBlock(2, 1);
  drawBlock(0, 1);
  tetrisDisplay.display();
  }
  else if(next_piece_id == 4)
  {
    //Next Piece is O
  tetrisDisplay.clearDisplay();
  drawBlock(1, 0); 
  drawBlock(1, 1);
  drawBlock(2, 1);
  drawBlock(2, 0);  
  tetrisDisplay.display();
  }
  else if(next_piece_id == 6)
  {
    //Next Piece is I
  tetrisDisplay.clearDisplay();
  drawBlock(0, 1); 
  drawBlock(1, 1);
  drawBlock(2, 1);
  drawBlock(3, 1);  
  tetrisDisplay.display();
  }
}
void drawBlock(int col, int row) 
{
  int x = col * BLOCK_WIDTH;
  int y = row * BLOCK_HEIGHT;
  tetrisDisplay.fillRect(x, y, BLOCK_WIDTH, BLOCK_HEIGHT, SSD1306_WHITE);
  tetrisDisplay.drawRect(x, y, BLOCK_WIDTH, BLOCK_HEIGHT, SSD1306_BLACK); 
}
void erase_piece_from_grid() {
  int x, y;

  const char *piece = pieces[piece_id] + (piece_rotation * PIECE_H * PIECE_W);

  for (y = 0; y < PIECE_H; ++y) {
    for (x = 0; x < PIECE_W; ++x) {
      int nx = piece_x + x;
      int ny = piece_y + y;
      if (ny < 0 || ny > GRID_H) continue;
      if (nx < 0 || nx > GRID_W) continue;
      if (piece[y * PIECE_W + x] == 1) {
        grid[ny * GRID_W + nx] = 0; // zero erases the grid location.
      }
    }
  }
}


void add_piece_to_grid() {
  int x, y;

  const char *piece = pieces[piece_id] + (piece_rotation * PIECE_H * PIECE_W);

  for (y = 0; y < PIECE_H; ++y) {
    for (x = 0; x < PIECE_W; ++x) {
      int nx = piece_x + x;
      int ny = piece_y + y;
      if (ny < 0 || ny >= GRID_H) continue;
      if (nx < 0 || nx >= GRID_W) continue;
      if (piece[y * PIECE_W + x] == 1) {
        grid[ny * GRID_W + nx] = currentPieceColor; // zero erases the grid location.
      }
    }
  }
}



void delete_row(int y) 
{
  wipepixel = 200 - (y*10+10);
wipepixelend = wipepixel + 10;
Serial.println(wipepixel);
for (int i = wipepixel; i < wipepixelend; i++) 
{
    strip.setPixelColor(i, strip.Color(255, 255, 255));
    strip.show(); 
    delay(25);
}
    
    
  int x;
  for (; y > 0; --y) {
    for (x = 0; x < GRID_W; ++x) {
      grid[y * GRID_W + x] = grid[(y - 1) * GRID_W + x];
    }
  }
  // everything moved down 1, so the top row must be empty or the game would be over.
  for (x = 0; x < GRID_W; ++x) {
    grid[x] = 0;
  }
}


void fall_faster() {
  if (drop_delay > DROP_MINIMUM) 
  {
    if(level >= 4)
    {
      drop_delay -= 50;
    }
    else
    {
      drop_delay -= DROP_ACCELERATION;
    }

  }
  Serial.print("Drop Delay: ");
  Serial.println(drop_delay);
}


void remove_full_rows() {
  int x, y, c;
  char row_removed = 0;
  int baseIncrease = 2;
  for (y = 0; y < GRID_H; ++y) {
    // count the full spaces in this row
    c = 0;
    for (x = 0; x < GRID_W; ++x) {
      if ( grid[y * GRID_W + x] > 0 ) c++;
    }
    if (c == GRID_W) 
    {
      // row full!
      delete_row(y);
      delay(100);
      linesCleared++;
      Serial.print("Rows Cleared: ");
      Serial.println(linesCleared);
      lcdScreen.clear();
      lcdScreen.setCursor(0,0);
      lcdScreen.print("Level: ");
      lcdScreen.print(level);
      lcdScreen.setCursor(0,2);
      lcdScreen.print("Lines Left: ");
      lcdScreen.print(lineThreshold - linesCleared);
      if(linesCleared >= lineThreshold)
      {
        level++;
        double mathIncrease = pow(baseIncrease, exponent);
        int increase = (int)mathIncrease;
        exponent++;
        linesCleared -= lineThreshold;
        lineThreshold = lineThreshold + increase;
        fall_faster();
        lcdScreen.clear();
        lcdScreen.setCursor(0,0);
        lcdScreen.print("Level: ");
        lcdScreen.print(level); 
        lcdScreen.setCursor(0,2);
        lcdScreen.print("Lines Left: ");
        lcdScreen.print(lineThreshold - linesCleared);
        delay(200);
      }
    }
  }
}
void try_to_move_piece_sideways() {
   int new_px = 0;
   unsigned long lastDebounceTime = 0;
  if(millis() - lastDebounceTime >= debounceDelay)
  {
  if (!digitalRead(button_left))
    {
    Serial.println("Right Pressed");
    lastDebounceTime = millis();
  
    new_px = -1;
    
   
    }

  if (!digitalRead(button_right))
    {
    Serial.println("Left Pressed");
    lastDebounceTime = millis();
    new_px = 1;
    }


  if (new_px != old_px && piece_can_fit(piece_x + new_px, piece_y, piece_rotation) == 1) {
    piece_x += new_px;
  }
  old_px = new_px;
  }
}

 // adjust as needed

void try_to_rotate_piece() {
  int new_pr = 0; // this will be our rotation flag
  static unsigned long lastDebounceTime = 0;

  if (millis() - lastDebounceTime >= rotateDelay) {
    if (!digitalRead(button_up)) 
    {
      Serial.println("Rotate Pressed");
      lastDebounceTime = millis();
      new_pr = 1; // signal that we want to rotate
    }

    if (new_pr != old_pr && new_pr == 1) {
      int rotated = (piece_rotation + 1) % 4;

      if (piece_can_fit(piece_x, piece_y, rotated)) {
        piece_rotation = rotated;
      } else {
        // Try wall kicks
        if (piece_can_fit(piece_x - 1, piece_y, rotated)) {
          piece_x -= 1;
          piece_rotation = rotated;
        } else if (piece_can_fit(piece_x + 1, piece_y, rotated)) {
          piece_x += 1;
          piece_rotation = rotated;
        }
      }
    }

    old_pr = new_pr;
  }
}


// can the piece fit in this new location?
int piece_can_fit(int px, int py, int pr) {
  if ( piece_off_edge(px, py, pr) ) return 0;
  if ( piece_hits_rubble(px, py, pr) ) return 0;
  return 1;
}


int piece_off_edge(int px, int py, int pr) {
  int x, y;
  const char *piece = pieces[piece_id] + (pr * PIECE_H * PIECE_W);

  for (y = 0; y < PIECE_H; ++y) {
    for (x = 0; x < PIECE_W; ++x) {
      int nx = px + x;
      int ny = py + y;
      if (ny < 0) continue; // off top, don't care
      if (piece[y * PIECE_W + x] > 0) {
        if (nx < 0) return 1; // yes: off left side
        if (nx >= GRID_W ) return 1; // yes: off right side
      }
    }
  }

  return 0;  // inside limits
}


int piece_hits_rubble(int px, int py, int pr) {
  int x, y;
  const char *piece = pieces[piece_id] + (pr * PIECE_H * PIECE_W);

  for (y = 0; y < PIECE_H; ++y) {
    int ny = py + y;
    if (ny < 0) continue;
    for (x = 0; x < PIECE_W; ++x) {
      int nx = px + x;
      if (piece[y * PIECE_W + x] > 0) {
        if (ny >= GRID_H ) return 1; // yes: goes off bottom of grid
        if (grid[ny * GRID_W + nx] != 0 ) return 1; // yes: grid already full in this space
      }
    }
  }

  return 0;  // doesn't hit
}



void draw_restart()
{
  long rnd = random(1, 5);   
  Serial.print("Random Function Number is: ");
  Serial.println(rnd); 
  functionPtr[rnd](); 
}
void all_white()
{
  strip.clear();
  for (int i = 0; i < STRAND_LENGTH; i++) {

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    strip.setPixelColor(i, strip.Color(80, 80, 80)); // Moderately bright green color.
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(2);
    
  }
for (int i = 0; i < STRAND_LENGTH; i++) 
{

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    strip.setPixelColor(i, strip.Color(0, 0, 0)); // Moderately bright green color.
    strip.show(); // This sends the updated pixel color to the hardware.
}
  }


void game_over() 
{
  long randomGameOver = random(3, 8);
  lcdScreen.clear();
  lcdScreen.print("Game Over!!!");
  game_over_loop_leds();
  myDFPlayer.stop();
  delay(200);
  myDFPlayer.play(randomGameOver);
  delay(1000);
  int x, y;

  long over_time = millis();
  draw_restart();
  currentMillis = millis();
  previousMillis = currentMillis;
  int led_number = 1;
  while (millis() - over_time < 10000) 
  {
    currentMillis = millis();
    if (digitalRead(button_start) == LOW) {
      // restart!
      Serial.println("Restart Pressed");
      delay(400);
      break;
    }
  }
  all_white();
  Serial.println("Will Setup");
  runGame();
  return;
}


void game_over_loop_leds()
{
 for (int i = 0; i < STRAND_LENGTH; i++) 
  {

  
    strip.setPixelColor(i, strip.Color(0, 255, 0)); 
    strip.show();
  }

   for (int i = 0; i < STRAND_LENGTH; i++) 
  {

   
    strip.setPixelColor(i, strip.Color(0, 0, 0)); // Moderately bright green color.

    strip.show(); // This sends the updated pixel color to the hardware.
  }

}


void try_to_drop_piece() {
  erase_piece_from_grid();
  if (piece_can_fit(piece_x, piece_y + 1, piece_rotation)) 
  {
    piece_y++;  // move piece down
    add_piece_to_grid();
  } else {
    // hit something!
    // put it back
    add_piece_to_grid();
    remove_full_rows();
    if (game_is_over() == 1) {
      game_over();
    }
    // game isn't over, choose a new piece
    choose_new_piece();
  }
}


void try_to_drop_faster() {

  unsigned long lastDebounceTime = 0;
  if (!digitalRead(button_down) && millis() - lastDebounceTime >= debounceDelay)
  {
    Serial.println("Dropping");
    try_to_drop_piece();
    lastDebounceTime = millis();
  }


}


void react_to_player() {
  erase_piece_from_grid();
  try_to_move_piece_sideways();
  try_to_rotate_piece();
  add_piece_to_grid();
  increaseVolume();
  decreaseVolume();
  try_to_drop_faster();
}


// can the piece fit in this new location
int game_is_over() {
  int x, y;
  const char *piece = pieces[piece_id] + (piece_rotation * PIECE_H * PIECE_W);

  for (y = 0; y < PIECE_H; ++y) {
    for (x = 0; x < PIECE_W; ++x) {
      int ny = piece_y + y;
      int nx = piece_x + x;
      if (piece[y * PIECE_W + x] > 0) {
        if (ny < 0) return 1; // yes: off the top!
      }
    }
  }

  return 0;  // not over yet...
}

void setup() 
{
  Serial.begin(9600);
  mySoftwareSerial.begin(9600);
  buttonConfiguration();
  initializeStartMenu();
  Serial.println("Setup is Done");
}
void initializeStartMenu()
{
int i;
Serial.println("Initializing");
strip.begin();
strip.show();
matrix.begin();
matrix.setTextWrap(false);
Serial.print("Brightness Level: ");
Serial.println(brightness);
matrix.setRotation(0);
Serial.print("Rotation: ");
Serial.println(matrix.getRotation());
matrix.setBrightness(brightness);
matrix.setTextColor(colors[0]);

  for (i = 0; i < GRID_W * GRID_H; ++i) 
  {
    grid[i] = 0;
  }
if(!dfPlayerInitialized)
  {
    if (!myDFPlayer.begin(mySoftwareSerial)) 
    {
    Serial.println(F("Not initialized:"));
    Serial.println(F("1. Check the DFPlayer Mini connections"));
    Serial.println(F("2. Insert an SD card"));
    while (true);
    }
  Serial.println(F("DFPlayer Mini initialized!"));
  }
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(volumeLevel); // Set initial volume
  myDFPlayer.EQ(0);
  if(myDFPlayer.readState() == 1) //check to see if the df player is currently playing a file
  {
    myDFPlayer.stop(); //stop the current track
  }
  // Loop from a random array of music
  myDFPlayer.loop(1); //loop file 1
  dfPlayerInitialized = true; //
  drawStartMenu(); 

}

void runGame()
{
  onStartScreen = false;
  strip.clear();
  for (i = 0; i < GRID_W * GRID_H; ++i) 
  {
    grid[i] = 0;
  }
  delay(500);
  Serial.print("Number of Colors: ");
  Serial.println(numColors);
  lcdScreen.begin(16, 2);
  Serial.println("GAME START!");
  lcdScreen.print("GAME START!");
  myDFPlayer.stop();
  delay(1000);
  myDFPlayer.loop(2);
  if (!tetrisDisplay.begin(SSD1306_SWITCHCAPVCC))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  tetrisDisplay.clearDisplay();
  level = 1;
  lcdScreen.clear();
  lcdScreen.setCursor(0,0);
  lcdScreen.print("Level: ");
  lcdScreen.print(level);
  lcdScreen.setCursor(0,2);
  lcdScreen.print("Lines Left: ");
  lcdScreen.print(lineThreshold - linesCleared); 
  randomSeed(analogRead(A0));
  next_piece_id = random(NUM_PIECE_TYPES);
  choose_new_piece();
  move_delay = INITIAL_MOVE_DELAY;
  drop_delay = INITIAL_DROP_DELAY;
  draw_delay = INITIAL_DRAW_DELAY;
  last_draw = last_drop = last_move = millis();

}
void draw_pause_overlay() 
{
  for (int i = 0; i < STRAND_LENGTH; i++) 
  {
    uint32_t original = strip.getPixelColor(i);
    uint8_t r = (original >> 16) & 0xFF;
    uint8_t g = (original >> 8) & 0xFF;
    uint8_t b = original & 0xFF;
    strip.setPixelColor(i, strip.Color(r / 2, g / 2, b / 2));  // dim by 50%
  }
  strip.show();
}

void drawStartMenu()
{
  Serial.println("Start Menu");
  for (int i = 0; i < 200; i += 10) 
  {

    strip.setPixelColor(i, strip.Color(255, 255, 255));
    strip.setPixelColor(i+1, strip.Color(255, 255, 255));
    strip.setPixelColor(i+2, strip.Color(255, 255, 255));
    strip.setPixelColor(i+3, strip.Color(255, 255, 255));
    strip.setPixelColor(i+4, strip.Color(255, 255, 255));
    strip.setPixelColor(i+5, strip.Color(255, 255, 255));
    strip.setPixelColor(i+6, strip.Color(255, 255, 255));
    strip.setPixelColor(i+7, strip.Color(255, 255, 255));
    strip.setPixelColor(i+8, strip.Color(255, 255, 255));
    strip.setPixelColor(i+9, strip.Color(255, 255, 255));
    strip.show();
    strip.clear();
    delay(50);
  }
  strip.clear();
  for(int i = 200; i > 0; i -= 10)
  {
    strip.setPixelColor(i, strip.Color(255, 255, 255));
    strip.setPixelColor(i+1, strip.Color(255, 255, 255));
    strip.setPixelColor(i+2, strip.Color(255, 255, 255));
    strip.setPixelColor(i+3, strip.Color(255, 255, 255));
    strip.setPixelColor(i+4, strip.Color(255, 255, 255));
    strip.setPixelColor(i+5, strip.Color(255, 255, 255));
    strip.setPixelColor(i+6, strip.Color(255, 255, 255));
    strip.setPixelColor(i+7, strip.Color(255, 255, 255));
    strip.setPixelColor(i+8, strip.Color(255, 255, 255));
    strip.setPixelColor(i+9, strip.Color(255, 255, 255));
    strip.show(); 
    strip.clear();
    delay(50); 
  }
  for (int i = 0; i < 200; i += 10) 
  {

    strip.setPixelColor(i, strip.Color(255, 255, 255));
    strip.setPixelColor(i+1, strip.Color(255, 255, 255));
    strip.setPixelColor(i+2, strip.Color(255, 255, 255));
    strip.setPixelColor(i+3, strip.Color(255, 255, 255));
    strip.setPixelColor(i+4, strip.Color(255, 255, 255));
    strip.setPixelColor(i+5, strip.Color(255, 255, 255));
    strip.setPixelColor(i+6, strip.Color(255, 255, 255));
    strip.setPixelColor(i+7, strip.Color(255, 255, 255));
    strip.setPixelColor(i+8, strip.Color(255, 255, 255));
    strip.setPixelColor(i+9, strip.Color(255, 255, 255));
    strip.show();
    strip.clear();
    delay(50);
  }
  for(int i = 200; i > 0; i -= 10)
  {
    strip.setPixelColor(i, strip.Color(255, 255, 255));
    strip.setPixelColor(i+1, strip.Color(255, 255, 255));
    strip.setPixelColor(i+2, strip.Color(255, 255, 255));
    strip.setPixelColor(i+3, strip.Color(255, 255, 255));
    strip.setPixelColor(i+4, strip.Color(255, 255, 255));
    strip.setPixelColor(i+5, strip.Color(255, 255, 255));
    strip.setPixelColor(i+6, strip.Color(255, 255, 255));
    strip.setPixelColor(i+7, strip.Color(255, 255, 255));
    strip.setPixelColor(i+8, strip.Color(255, 255, 255));
    strip.setPixelColor(i+9, strip.Color(255, 255, 255));
    strip.show(); 
    strip.clear();
    delay(50); 
  }
  for(int i = 0; i < 200; i++)
  {
    strip.setPixelColor(i, strip.Color(10, 5, 166));
    strip.show();
  }
  strip.setPixelColor(42, strip.Color(255, 215, 0));
  strip.setPixelColor(56, strip.Color(255, 215, 0));
  strip.setPixelColor(57, strip.Color(255, 215, 0));
  strip.setPixelColor(62, strip.Color(255, 215, 0));
  strip.setPixelColor(63, strip.Color(255, 215, 0));
  strip.setPixelColor(64, strip.Color(255, 215, 0));
  strip.setPixelColor(74, strip.Color(255, 215, 0));
  strip.setPixelColor(75, strip.Color(255, 215, 0));
  strip.setPixelColor(76, strip.Color(255, 215, 0));
  strip.setPixelColor(77, strip.Color(255, 215, 0));
  strip.setPixelColor(82, strip.Color(255, 215, 0));
  strip.setPixelColor(83, strip.Color(255, 215, 0));
  strip.setPixelColor(84, strip.Color(255, 215, 0));
  strip.setPixelColor(85, strip.Color(255, 215, 0));
  strip.setPixelColor(86, strip.Color(255, 215, 0));
  strip.setPixelColor(92, strip.Color(255, 215, 0));
  strip.setPixelColor(93, strip.Color(255, 215, 0));
  strip.setPixelColor(94, strip.Color(255, 215, 0));
  strip.setPixelColor(95, strip.Color(255, 215, 0));
  strip.setPixelColor(96, strip.Color(255, 215, 0));
  strip.setPixelColor(97, strip.Color(255, 215, 0));
  strip.setPixelColor(102, strip.Color(255, 215, 0));
  strip.setPixelColor(103, strip.Color(255, 215, 0));
  strip.setPixelColor(104, strip.Color(255, 215, 0));
  strip.setPixelColor(105, strip.Color(255, 215, 0));
  strip.setPixelColor(106, strip.Color(255, 215, 0));
  strip.setPixelColor(107, strip.Color(255, 215, 0));
  strip.setPixelColor(113, strip.Color(255, 215, 0));
  strip.setPixelColor(114, strip.Color(255, 215, 0));
  strip.setPixelColor(115, strip.Color(255, 215, 0));
  strip.setPixelColor(116, strip.Color(255, 215, 0));
  strip.setPixelColor(117, strip.Color(255, 215, 0));
  strip.setPixelColor(122, strip.Color(255, 215, 0));
  strip.setPixelColor(123, strip.Color(255, 215, 0));
  strip.setPixelColor(124, strip.Color(255, 215, 0));
  strip.setPixelColor(125, strip.Color(255, 215, 0));
  strip.setPixelColor(135, strip.Color(255, 215, 0));
  strip.setPixelColor(136, strip.Color(255, 215, 0));
  strip.setPixelColor(137, strip.Color(255, 215, 0));
  strip.setPixelColor(142, strip.Color(255, 215, 0));
  strip.setPixelColor(143, strip.Color(255, 215, 0));
  strip.setPixelColor(157, strip.Color(255, 215, 0));
  strip.show();

} 

 
void buttonConfiguration()
{
  Serial.println("Configured");
  pinMode(volUpButton, INPUT_PULLUP);
  pinMode(volDownButton, INPUT_PULLUP);
  pinMode(button_pause, INPUT_PULLUP);
  pinMode(button_start, INPUT_PULLUP);
  pinMode(button_left, INPUT_PULLUP);
  pinMode(button_right, INPUT_PULLUP);
  pinMode(button_up, INPUT_PULLUP);
  pinMode(button_down, INPUT_PULLUP);
}
void startGameButton()
{
  currentMillis = millis();
  if(digitalRead(button_start) == LOW && (currentMillis - lastStartPress >= debounceDelay))
  {

    Serial.println("Start Button Pressed");
    lastStartPress = currentMillis;
    all_white();
    runGame();
    delay(150);

  }
}
void increaseVolume()
{
  currentMillis = millis();
  if (digitalRead(volUpButton) == LOW && (currentMillis - lastVolUpPress >= debounceDelay)) {
    Serial.println("Button Up Pressed");
    lastVolUpPress = currentMillis;
    if (volumeLevel < 30) 
    { // Max volume limit
      volumeLevel += 2;
      Serial.println("Volume Down Changed");
      volumeLevel = constrain(volumeLevel, 0, 30);
      myDFPlayer.volume(volumeLevel);
      delay(150);
      Serial.print("Volume Up: ");
      Serial.println(volumeLevel);
    }
  }
}

void decreaseVolume()
{
  currentMillis = millis();
  if (digitalRead(volDownButton) == LOW && (currentMillis - lastVolDownPress >= debounceDelay))
  {
    lastVolDownPress = currentMillis;
    if (volumeLevel > 0) { // Min volume limit
      volumeLevel -= 2;
      Serial.println("Volume Change Down");
      volumeLevel = constrain(volumeLevel, 0, 30);
      myDFPlayer.volume(volumeLevel);
      delay(150);
      Serial.print("Volume Down: ");
      Serial.println(volumeLevel);
    }
    
  }
}
void pauseGame() 
{
  currentMillis = millis();
  if (digitalRead(button_pause) == LOW && (currentMillis - lastPausePress >= debounceDelay)) {
    lastPausePress = currentMillis;
    Pause = !Pause;
    Serial.print("Pause: ");
    Serial.println(Pause);

    if (Pause) {
      Serial.println("Pausing Game - stop and play pause music");
      //myDFPlayer.stop();
      delay(100);
      int track = random(3,6);
      Serial.print("Playing pause track: ");
      Serial.println(track);
      //myDFPlayer.play(track);
      draw_pause_overlay();
    } else {
      Serial.println("Resuming Game - stop and play game music");
      //myDFPlayer.stop();
      delay(100);
      //myDFPlayer.loop(2);
      draw_grid();
      add_piece_to_grid();
      strip.show();
      Serial.println("Game resumed");
    }
  }
}
void loop() 
{   
  long t = millis();
  if(onStartScreen)
  {
    startGameButton();
    return;
  }
  if (digitalRead(button_start) == LOW) 
  {
    Serial.println("Detected LOW");
    delay(200);
  }
  
  pauseGame();
  if (!Pause)
  {
    // the game plays at one speed,
    if (t - last_move > move_delay ) {
      last_move = t;
      react_to_player();
    }

    // ...and drops the falling block at a different speed.
    if (t - last_drop > drop_delay ) {
      last_drop = t;
      try_to_drop_piece();
    }

    // when it isn't doing those two things, it's redrawing the grid.
    if (t - last_draw > draw_delay ) {
      last_draw = t;
      draw_grid();
    }
  }

 
}

