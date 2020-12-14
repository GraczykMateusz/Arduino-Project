#include <FastLED.h>

//Leds number
#define NUM_LEDS 256

//Controller input
#define X_PIN      A5
#define Y_PIN      A6
#define SW_PIN     A7

//Data to WS2812b panel
#define DATA_PIN    3

#define LED_OFFSET 32

//Colors
#define COLOR       CRGB::Red //Red is Green
#define FRUIT_COLOR CRGB::Green // Green is Red
#define POINT_COLOR CRGB::Blue

#define MAX_SNAKE_TAIL 256

//Snake tail length
int snake_tail = 0;

//Snake's movement
bool left;
bool right;
bool up;
bool down;

//Snake speed
int delay_time;

//collision
bool is_collision = false;

//Fruit
int fruit_position_X = -1;
bool is_spawn_fruit;

//The array of leds. One item for each led in strip.
CRGB leds[NUM_LEDS];

//Needed to start game correctly
bool is_start_game = false; 
bool is_only_once  = true;
bool is_start_display = false;

//Wrong SW_PIN input defender
bool is_button_pressed = false;

//Snake head position 
unsigned int head_position_X;
unsigned int head_position_Y;

unsigned int old_head_position_X;
unsigned int old_head_position_Y;

//Snake position in one line (the same as array)
int position_X;
int old_position_X[MAX_SNAKE_TAIL];

//Score
int score = 0;

//Keep current leds to display
bool read_value[NUM_LEDS];

const bool start[NUM_LEDS] PROGMEM = {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,
                                      1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,
                                      1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,
                                      0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,
                                      0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
                                      0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
                                      0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                                      1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,
                                      1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,
                                      1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1};

const bool three[NUM_LEDS] PROGMEM = {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,
                                      0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,
                                      0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
                                      0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,
                                      0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,
                                      0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
                                      1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1};

const bool two[NUM_LEDS] PROGMEM  =  {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,
                                      0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,
                                      0,0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,
                                      0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,
                                      0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,
                                      0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,
                                      0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,
                                      0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
                                      0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,
                                      0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
                                      1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1};

const bool one[NUM_LEDS] PROGMEM  =  {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,
                                      0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
                                      1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1};

const bool out[NUM_LEDS] PROGMEM  =  {1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,
                                      0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,
                                      0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,
                                      0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,
                                      0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                                      0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                                      0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,
                                      0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,
                                      0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,
                                      0,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,
                                      1,0,1,1,0,0,0,0,0,0,0,0,1,1,0,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1};
                                      

/*----------------> EXIT <----------------*/
void check_exit() {
  if(is_button_pressed) {
    reset();
    
    for(int i = 0; i < NUM_LEDS; ++i) {
      read_value[i] = pgm_read_byte(&out[i]);
    }
    displayer(read_value);
    
    delay(3000);
  }
}

void reset() {
  is_start_game = false;
  is_only_once = true;
  is_button_pressed = false;

  fruit_position_X = -1;
  is_spawn_fruit = false;

  score = 0;

  snake_tail = 0;

  is_collision = false;

  delay_time = 250;
}

/*--------> INPUT CONTROLLER / TIMER <--------*/
void inputController() {
  if(analogRead(X_PIN) < 50 && !right) { //Left
    left  = true;
    right = false;
    up    = false;
    down  = false;
  }
  else if(analogRead(X_PIN) > 972 && !left) { //Right
    left  = false;
    right = true;
    up    = false;
    down  = false;
  }
  else if(analogRead(Y_PIN) < 50 && !down) { //Up
    left  = false;
    right = false;
    up    = true;
    down  = false;
  }
  else if(analogRead(Y_PIN) > 972 && !up) { //Down
    left  = false;
    right = false; 
    up    = false;
    down  = true;
  } 
  if(analogRead(SW_PIN) <  30  &&  
     analogRead(X_PIN)  >  411 &&
     analogRead(X_PIN)  <  611 &&
     analogRead(Y_PIN)  >  411 &&
     analogRead(Y_PIN)  <  611) {
    is_button_pressed = true;
  } else {
    is_button_pressed = false;
  }
}

/*----------------> DISPLAY <----------------*/
void displayer(bool* turnOnOffLeds) {
  FastLED.clear();

  CRGB color;
  
  int row = -1;
  
  for(int i = 0; i < NUM_LEDS; ++i) {
    if(i == fruit_position_X && position_X != fruit_position_X)
      color = FRUIT_COLOR;

    //if(is_collision)
      //color = CRGB::Blue;
      
    if(i % 16 == 0) {
      ++row;
    }
    if(turnOnOffLeds[i] && row % 2 == 0) {
      if(row == 0) {
        leds[15-i] = color;
      } else if(row == 2) {
        leds[47 - i + LED_OFFSET] = color;
      } else if(row == 4) {
        leds[79 - i + 2 * LED_OFFSET] = color;
      } else if(row == 6) {
        leds[111 - i + 3 * LED_OFFSET] = color;
      } else if(row == 8) {
        leds[143 - i + 4 * LED_OFFSET] = color;
      } else if(row == 10) {
        leds[175 - i + 5 * LED_OFFSET] = color;
      } else if(row == 12) {
        leds[207 - i + 6 * LED_OFFSET] = color;
      } else if(row == 14) {
        leds[239 - i + 7 * LED_OFFSET] = color;
      }
    } else if(turnOnOffLeds[i]){
      leds[i] = color;
    }
    color = COLOR;
  }
  row = 0;

  FastLED.show();
}

void start_display() {
  is_start_display = true;
  
  for(int i = 0; i < NUM_LEDS; ++i) {
    read_value[i] = pgm_read_byte(&start[i]);
  }
  displayer(read_value);
}

void starting_timer_display() {
  for(int i = 0; i < NUM_LEDS; ++i) {
    read_value[i] = pgm_read_byte(&three[i]);
  }
  displayer(read_value);

  delay(1000);
  
  for(int i = 0; i < NUM_LEDS; ++i) {
    read_value[i] = pgm_read_byte(&two[i]);
  }
  displayer(read_value);

  delay(1000);

  for(int i = 0; i < NUM_LEDS; ++i) {
    read_value[i] = pgm_read_byte(&one[i]);
  }
  displayer(read_value);

  delay(1000);

  for(int i = 0; i < NUM_LEDS; ++i) {
    read_value[i] = 0;
  }
}

/*----------------> GAMEPLAY <---------------*/
void gameplay() {
  //Movement
  move_snake();
  
  position_X = convert_XY_to_X(head_position_X, head_position_Y);
  
  if(read_value[position_X] == 1 && position_X != fruit_position_X)
    is_collision = true;
  
  collision();
  read_value[position_X] = 1;

  displayer(read_value);

  //Fruit genarates
  if(position_X == fruit_position_X) {
    ++score;
    ++snake_tail;
    
    is_spawn_fruit = false;
    generate_fruit_position();
  }

  tail();
  
  change_speed();
}

void tail() {  
  if(snake_tail == 0) {
    old_position_X[0] = position_X;
    read_value[old_position_X[0]] = 0;
  }
  else if(snake_tail == 1) {
    old_position_X[1] = old_position_X[0];
    old_position_X[0] = position_X;
    
    read_value[old_position_X[1]] = 0;
  }
  else {    
    for(int i = snake_tail; i > 0; --i) {
      old_position_X[i] = old_position_X[i-1];
    }
    old_position_X[0] = position_X;
    read_value[old_position_X[snake_tail]] = 0;
  }
}

void move_snake() {
  if(left) {
    if(head_position_X > 0)
      --head_position_X;
    else
      is_collision = true;
  }
  else if(right) {
    if(head_position_X < 15)
      ++head_position_X;
    else
      is_collision = true;
  }
  else if(up) {
    if(head_position_Y > 0)
      --head_position_Y;
    else
      is_collision = true;
  }
  else if(down) {
    if(head_position_Y < 15)
      ++head_position_Y;
    else
      is_collision = true;
  }
}

void generate_fruit_position() {
  do {
    long randPosition = random(NUM_LEDS);
    fruit_position_X = int(randPosition);

    if(read_value[fruit_position_X] == 0)
      is_spawn_fruit = true;
    
  } while(!is_spawn_fruit);
  
  read_value[fruit_position_X] = 1;
}

int convert_XY_to_X(int headPositionX, int headPositionY) {
  const int ROW_OFFSET = 16;
  const int ROW_NUM = 16;
  
  int posX;

  for(int i = 0; i < ROW_NUM; ++i) {
    if(head_position_Y == i)
      posX = head_position_X + i * ROW_OFFSET;
  }
  return posX;
}

void collision() {
  if(is_collision) {
    is_button_pressed = true;

    FastLED.clear();
    FastLED.show();
    delay(500);
    displayer(read_value);
    delay(500);
    
    FastLED.clear();
    FastLED.show();
    delay(500);
    displayer(read_value);
    delay(500);

    FastLED.clear();
    FastLED.show();
    delay(500);
    displayer(read_value);
    delay(500);
    
    check_exit();
  }
}

void change_speed() {
  if(score == 0)
    delay_time = 250;
  else if(score == 1)
    delay_time = 235;
  else if(score == 3)
    delay_time = 220;
  else if(score == 5)
    delay_time = 205;
  else if(score == 7)
    delay_time = 190;
  else if(score == 9)
    delay_time = 175;
  else if(score == 11)
    delay_time = 160;
  else if(score == 13)
    delay_time = 145;
  else if(score == 15)
    delay_time = 130;
  else if(score == 17)
    delay_time = 115;
  else if(score == 19)
    delay_time = 100;

  delay(delay_time);
}

/*-------------> CONFIGURATION <-------------*/
void setup() {
  Serial.begin(9600);

  //Clear display
  FastLED.clear();
  FastLED.show();

  FastLED.setBrightness(1);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);

  randomSeed(analogRead(0));

  //Controller Input
  pinMode     (X_PIN,  INPUT);
  pinMode     (Y_PIN,  INPUT);
  pinMode     (SW_PIN, INPUT);

  //Data to WS2812b panel
  pinMode     (DATA_PIN, OUTPUT);
}

void config_game() {
  is_start_game = true;
  is_only_once = false;
  is_start_display = false;

  left  = true; //Snake starts left movement
  right = false;
  up    = false;
  down  = false;

  head_position_X = sqrt(NUM_LEDS) / 2;
  head_position_Y = sqrt(NUM_LEDS) / 2;
}

/*---------------> MAIN LOOP <---------------*/
void loop() {
  inputController();
  
  if(is_start_game || is_button_pressed) {
    //Set up game data and display the timer (only once after starting the game).
    if(is_only_once) {
      config_game();
      starting_timer_display();
      generate_fruit_position(); 
    }
    //Game//
    inputController();
    gameplay();
    
    //Hold the button to exit the game
    inputController();
    check_exit();
  } else if(!is_start_game && !is_start_display) {
    start_display();
  }
}
