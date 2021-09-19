#include <Arduino.h>

#define LED_GREEN 2
#define LED_YELLOW 3
#define LED_RED 4
#define SWITCH 20
#define BUZZER 24

#define INIT 0
#define STOP 1
#define SLOW 2
#define GO 3

// Globals, everyone loves em :)
int state = INIT;

void switch_event(){
    digitalWrite(LED_RED, LOW);
    state = STOP;
}

void setup(){
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(BUZZER, OUTPUT);

    pinMode(SWITCH, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(SWITCH), switch_event, CHANGE);
}

void loop(){
    while(true){
        if(state == INIT){
            digitalWrite(LED_RED, HIGH);
            delay(1000);
            digitalWrite(LED_RED, LOW);
            delay(1000);

        } else if (state == STOP){
            digitalWrite(LED_RED, HIGH);
            delay(17000);
            digitalWrite(BUZZER, HIGH);
            delay(3000);
            digitalWrite(LED_RED, LOW);
            digitalWrite(BUZZER, LOW);
            state = GO;

        } else if (state == SLOW){
            digitalWrite(LED_YELLOW, HIGH);
            digitalWrite(BUZZER, HIGH);
            delay(3000);
            digitalWrite(BUZZER, LOW);
            digitalWrite(LED_YELLOW, LOW);
            state = STOP;

        } else if (state == GO){
            digitalWrite(LED_GREEN, HIGH);
            delay(17000);
            digitalWrite(BUZZER, HIGH);
            delay(3000);
            digitalWrite(LED_GREEN, LOW);
            digitalWrite(BUZZER, LOW);
            state = SLOW;

        }
    }
}