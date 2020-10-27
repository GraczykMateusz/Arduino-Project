//Controller input
#define X_PIN     A0
#define Y_PIN     A1
#define SW_PIN    A2

//#define DATA_PIN  A3

bool is_start_game = false; 
bool is_only_once = true;

void setup() {
  Serial.begin(9600);

  //Controller Input
  pinMode     (X_PIN, INPUT);
  pinMode     (Y_PIN, INPUT);
  pinMode     (SW_PIN, INPUT);

  //pinMode     (DATA_PIN, OUTPUT);
}

void start_display() {
  Serial.print("START\n");
}

void starting_timer_display() {
  Serial.print("3\n");
  delay(1000);
  Serial.print("2\n");
  delay(1000);
  Serial.print("1\n");
  delay(1000);
}

void check_exit() {
  if(analogRead(SW_PIN) == 0) {
      delay(2000);
      if(analogRead(SW_PIN) == 0) {
        is_start_game = false;
        is_only_once = true;
        Serial.print("EXIT\n");
      }
    }
}

void config_game() {
  is_start_game = true;
  is_only_once = false;
}

void inputController() {
  
}



void loop() {
  Serial.println(analogRead(SW_PIN));
  if(analogRead(SW_PIN) == 0)
    Serial.println("dddddddddddddduppppppppaaaaaaaaaa");

  /*
  if(!is_start_game){
    start_display();
    Serial.println(SW_PIN);
    delay(2000);
  }
  if(analogRead(SW_PIN) == 0 || is_start_game) {
    //Set up game data and display the timer (only once after starting the game).
    if(is_only_once) {
      config_game();
      starting_timer_display();  
    }
    if(analogRead(X_PIN) < 211)
      Serial.println("LEFT");
    if(analogRead(X_PIN) > 811)
      Serial.println("RIGHT");
    if(analogRead(Y_PIN) < 211) // DELAY ISSUE
      Serial.println("UP");
    if(analogRead(Y_PIN) > 811)
      Serial.println("DOWN");
      
    //Hold the button to exit the game
    check_exit();
  }
  */
}
