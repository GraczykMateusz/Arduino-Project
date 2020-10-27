//Controller input
#define X_PIN     A5
#define Y_PIN     A6
#define SW_PIN    A7

bool is_start_game = false; 
bool is_only_once = true;

bool is_button_pressed = false;

int timer1_counter;

void setup() {
  Serial.begin(9600);

  //Controller Input
  pinMode     (X_PIN, INPUT);
  pinMode     (Y_PIN, INPUT);
  pinMode     (SW_PIN, INPUT);

  //Timer config
  //Used as a guard to get correct SW_PIN input.
  //It's necessary because sometimes the joystick has input issue.
  //When his position is UP -> sets SW_PIN to zero.
  timer_config();
}

void start_display() {
  Serial.println("START");
}

void starting_timer_display() {
  Serial.println("3");
  delay(1000);
  Serial.println("2");
  delay(1000);
  Serial.println("1");
  delay(1000);
}

void check_exit() {
  if(is_button_pressed) {
    is_start_game = false;
    is_only_once = true;
    is_button_pressed = false;
    Serial.println("EXIT");
    delay(3000);
  }
}

void timer_config() {
  noInterrupts();
  
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 0;
  TCCR1B |= (0 << CS02) | (0 << CS01) | (1 << CS00);
  TIMSK1 |= (1 << TOIE1);

  interrupts();
}

void config_game() {
  is_start_game = true;
  is_only_once = false;
}

void inputController() {
  if(analogRead(X_PIN) < 111)
    Serial.println("LEFT");
  if(analogRead(X_PIN) > 911)
    Serial.println("RIGHT");
  if(analogRead(Y_PIN) < 111)
    Serial.println("UP");
  if(analogRead(Y_PIN) > 911)
    Serial.println("DOWN");
}
int button_clicks_counter = 0;

ISR(TIMER1_OVF_vect)        // interrupt service routine 
{
  TCNT1 = 0;   // preload timer
  if(analogRead(SW_PIN) < 30  && 
     analogRead(X_PIN) > 411 &&
     analogRead(X_PIN) < 611 &&
     analogRead(Y_PIN) > 411 &&
     analogRead(Y_PIN) < 611) {
    is_button_pressed = true;
  }
  else {
    is_button_pressed = false;
  }
 //inputController();
}

void loop() {
  if(is_start_game || is_button_pressed) {
    //Set up game data and display the timer (only once after starting the game).
    if(is_only_once) {
      config_game();
      starting_timer_display();  
    }

    inputController();
      
    //Hold the button to exit the game
    check_exit();
  } else if(!is_start_game){
    start_display();
  }
}
