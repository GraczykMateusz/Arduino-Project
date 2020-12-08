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

#define COLOR CRGB::Blue

bool left  = false;
bool right = false;
bool up    = false;
bool down  = false;

// The array of leds. One item for each led in strip.
CRGB leds[NUM_LEDS];

//Needed to start game correctly
bool is_start_game = false; 
bool is_only_once  = true;

//Wrong SW_PIN input defender
bool is_button_pressed = false;

//Game 
unsigned int headPositionX = sqrt(NUM_LEDS) / 2;
unsigned int headPositionY = sqrt(NUM_LEDS) / 2;

int positionX;
int oldPositionX;

//Keep current leds to display
bool readValue[NUM_LEDS];

const bool start[NUM_LEDS] PROGMEM = {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
                                      1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,
                                      0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
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
                                      0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,
                                      1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,
                                      1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1};

const bool three[NUM_LEDS] PROGMEM = {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
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
                                      0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1};

const bool two[NUM_LEDS] PROGMEM  =  {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
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
                                      0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1};

const bool one[NUM_LEDS] PROGMEM  =  {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
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
                                      0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,
                                      1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                                      1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1};

void check_exit() {
  Serial.println("checking");
  if(is_button_pressed) {
    is_start_game = false;
    is_only_once = true;
    is_button_pressed = false;
    Serial.println("EXIT");
    delay(3000);
  }
}

/*--------> INPUT CONTROLLER / TIMER <--------*/
void inputController() {
 
  if(analogRead(X_PIN) < 50) { //Left
    left  = true;
    right = false;
    up    = false;
    down  = false;
    Serial.println("LEFT JOYSTICK");
  }
  
  if(analogRead(X_PIN) > 972) { //Right
    left  = false;
    right = true;
    up    = false;
    down  = false;
    Serial.println("RIGHT JOYSTICK");
  }
  
  if(analogRead(Y_PIN) < 50) { //Up
    left  = false;
    right = false;
    up    = true;
    down  = false;
    Serial.println("UP JOYSTICK");
  }
  if(analogRead(Y_PIN) > 972) { //Down
    left  = false;
    right = false;
    up    = false;
    down  = true;
    Serial.println("DOWN JOYSTICK");
  }
}
  

ISR(TIMER1_OVF_vect) 
{
  TCNT1 = 0;   // preload timer
  
  if(analogRead(SW_PIN) <  30  &&  
     analogRead(X_PIN)  >  411 &&
     analogRead(X_PIN)  <  611 &&
     analogRead(Y_PIN)  >  411 &&
     analogRead(Y_PIN)  <  611) {
    is_button_pressed = true;
    Serial.println("Switch");
  } else {
    is_button_pressed = false;
  }
  
  inputController();
}

/*----------------> DISPLAY <----------------*/
void displayer(bool* turnOnOffLeds) {
  FastLED.clear();
  
  int row = -1;
  
  for(int i = 0; i < NUM_LEDS; ++i) { 
    if(i % 16 == 0) {
      ++row;
    }
    if(turnOnOffLeds[i] && row % 2 == 0) {
      if(row == 0) {
        leds[15-i] = COLOR;
      } else if(row == 2) {
        leds[47 - i + LED_OFFSET] = COLOR;
      } else if(row == 4) {
        leds[79 - i + 2 * LED_OFFSET] = COLOR;
      } else if(row == 6) {
        leds[111 - i + 3 * LED_OFFSET] = COLOR;
      } else if(row == 8) {
        leds[143 - i + 4 * LED_OFFSET] = COLOR;
      } else if(row == 10) {
        leds[175 - i + 5 * LED_OFFSET] = COLOR;
      } else if(row == 12) {
        leds[207 - i + 6 * LED_OFFSET] = COLOR;
      } else if(row == 14) {
        leds[239 - i + 7 * LED_OFFSET] = COLOR;
      }
    } else if(turnOnOffLeds[i]){
      leds[i] = COLOR;
    }
  }
  row = 0;

  FastLED.show();
}

void start_display() {
  for(int i = 0; i < NUM_LEDS; ++i) {
    readValue[i] = pgm_read_byte(&start[i]);
  }
  displayer(readValue);
}

void starting_timer_display() {
  for(int i = 0; i < NUM_LEDS; ++i) {
    readValue[i] = pgm_read_byte(&three[i]);
  }
  displayer(readValue);

  delay(1000);
  
  for(int i = 0; i < NUM_LEDS; ++i) {
    readValue[i] = pgm_read_byte(&two[i]);
  }
  displayer(readValue);

  delay(1000);

  for(int i = 0; i < NUM_LEDS; ++i) {
    readValue[i] = pgm_read_byte(&one[i]);
  }
  displayer(readValue);

  delay(1000);

  for(int i = 0; i < NUM_LEDS; ++i) {
    readValue[i] = 0;
  }
}

void test() {
  moveSnake();
  readValue[oldPositionX] = 0;
  
  positionX = convertXYposToXpos(headPositionX, headPositionY);
  readValue[positionX] = 1;
  
  displayer(readValue);

  delay(500);

  oldPositionX = positionX;
}

/*----------------> GAMEPLAY <---------------*/
void moveSnake() {
  if(left) {
    --headPositionX;
    Serial.println("LEFT");
  }
  else if(right) {
    ++headPositionX;
    Serial.println("RIGTH");
  }
  else if(up) {
    ++headPositionY;
    Serial.println("UP");
  }
  else if(down) {
    --headPositionY;
    Serial.println("DOWN");
  }
}

int convertXYposToXpos(int headPositionX, int headPositionY) {
  const int ROW_OFFSET = 16;
  const int ROW_NUM = 16;
  
  int position;

  for(int i = 0; i < ROW_NUM; ++i) {
    if(headPositionY == i)
      position = headPositionX + i * ROW_OFFSET;
  }
  Serial.println(position);
  return position;
}

/*-------------> CONFIGURATION <-------------*/
void setup() {
  Serial.begin(9600);

  FastLED.setBrightness(10);

  //Clock type
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);

  //Controller Input
  pinMode     (X_PIN,  INPUT);
  pinMode     (Y_PIN,  INPUT);
  pinMode     (SW_PIN, INPUT);

  //Data to WS2812b panel
  pinMode     (DATA_PIN, OUTPUT);

  //Timer config
  timer_config();                   /* The timer is used as a guard to get correct SW_PIN input.
                                    It's necessary because sometimes the joystick has input issue.
                                    When his position is UP -> sets SW_PIN to zero. */
}

void config_game() {
  is_start_game = true;
  is_only_once = false;
}

void timer_config() {
  noInterrupts();
  
  TCCR1A  = 0;
  TCCR1B  = 0;

  TCNT1   = 0;
  TCCR1B |= (0 << CS02) | (0 << CS01) | (1 << CS00);
  TIMSK1 |= (1 << TOIE1);

  interrupts();
}

/*---------------> MAIN LOOP <---------------*/
void loop() {
  FastLED.clear();
  FastLED.show();
 
  if(is_start_game || is_button_pressed) {
    //Set up game data and display the timer (only once after starting the game).
    if(is_only_once) {
      config_game();
      starting_timer_display();  
    }
    //Game//
    test();
    
    //Hold the button to exit the game
    check_exit();
  } else if(!is_start_game) {
    start_display();
  }
}
