// Microprocessors II
// Lab 1 - Traffic Light Controller
// Ronan Dunn
// Patrick Hoey
// Peter Lynch

// global variables
int timer = 0; // keeps track of the current time
int STATE = 0; // maintains the state of the traffic controller
int INIT = 0; // initial state
int STOP = 1; // stop state (red led)
int SLOW = 2; // slow state (yellow led)
int GO = 3; // go state (green led)
bool toggle = 0;

// establish pin numbers
int GREEN_LED = 2;
int YELLOW_LED = 3;
int RED_LED = 4;
int BUZZER = 5;
int SWITCH = 20;

void setup(){

  // DEBUGGING
  Serial.begin(9600);
  Serial.print("Start");
  
  // setup output pins
  pinMode(GREEN_LED, OUTPUT); // green led
  pinMode(YELLOW_LED, OUTPUT); // yellow led
  pinMode(RED_LED, OUTPUT); // red led
  pinMode(BUZZER, OUTPUT); // buzzer

  // setup input pin
  pinMode(SWITCH, INPUT_PULLUP); // switch

  // attach an interrupt for the switch press
  attachInterrupt(digitalPinToInterrupt(SWITCH), start, LOW);
  
  cli(); // stop interrupts

  // set timer1 interrupt at 1Hz
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  
  // set compare match register for 1hz increments
  OCR1A = 15624; // = (16*10^6) / (1*1024) - 1 (must be <65536)
  
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  
  // set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10); 
   
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei(); // allow interrupts

} // end setup

// Timer1's interrupt service routing (ISR)
// The code in ISR will be executed every time timer1 interrupt occurs
// That is, the code will be called once every second
//
ISR(TIMER1_COMPA_vect){ // timer1 interrupt 1Hz controls flow of the traffic light controller
  
  // increment the timer
  timer = timer + 1;

  // toggle
  toggle = !toggle;

}

// called by interrupt when switch is pressed
void start() {

  // this conditional prohibits multiple switch presses from having another impact on the program
  if (STATE == INIT) {

    // turn off red led
    digitalWrite(RED_LED, LOW);
  
    // start the controller by exiting the initial state
    STATE = STOP;

    // reset timer
    timer = 0;
  
  }
}
  
void loop(){
  
  // determine state
  if (STATE == INIT) {

    // toggle red led
    if (toggle) {
      digitalWrite(RED_LED, HIGH);
    }
    else {
      digitalWrite(RED_LED, LOW);
    }
    
  }
  else if (STATE == STOP) {

    // reset timer
    timer = 0;

    while (timer < 20) {

      // DEBUGGING
      Serial.println(timer);
      
      // turn on red LED
      digitalWrite(RED_LED, HIGH);
      
      // turn on buzzer for last three seconds
      if (timer >= 17) {
        digitalWrite(BUZZER, HIGH);
      }
      
    }
    
    // turn off red led and buzzer, switch to the go state
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
    STATE = GO;
      
  }
  else if (STATE == SLOW) {
    
    // reset timer
    timer = 0;

    while (timer < 3) {

      // DEBUGGING
      Serial.println(timer);
      
      // turn on yellow LED
      digitalWrite(YELLOW_LED, HIGH);

      // turn on buzzer
      digitalWrite(BUZZER, HIGH);
      
    }
    
    // turn off yellow led and buzzer, switch to the stop state
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(BUZZER, LOW);
    STATE = STOP;
    
  }
  else {

    // reset timer
    timer = 0;

    while (timer < 20) {

      // DEBUGGING
      Serial.println(timer);
      
      // turn on green LED
      digitalWrite(GREEN_LED, HIGH);
      
      // turn on buzzer for last three seconds
      if (timer >= 17) {
        digitalWrite(BUZZER, HIGH);
      }
      
    }
    
    // turn off green led and buzzer, switch to the slow state
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER, LOW);
    STATE = SLOW;
      
  }
  
}
