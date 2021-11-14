// #define DEBUG
#include <Button.h>
#include <StateMachine.h> // https://github.com/jrullan/StateMachine
#include <neotimer.h> // https://github.com/jrullan/neotimer

const int BUTTON1_PIN = 2;
const int RELAIS1_PIN = 12;

Button button1(BUTTON1_PIN); // Connect your button between pin 2 and GND
StateMachine machine = StateMachine();
Neotimer button1Timer = Neotimer();
Neotimer ventilatorTimer = Neotimer();

State* S0 = machine.addState(&ventilatieUit);
State* S1 = machine.addState(&observingButton1PressDuration);
State* S2 = machine.addState(&ventilatieAan);

void setup() {
  button1.begin();
  pinMode(RELAIS1_PIN, OUTPUT);

  //#ifdef DEBUG
  while (!Serial) { }; // for Leos
  Serial.begin(9600);
  //#endif

  S0->addTransition(&wachtenOpStartSignaal,S1);
  S1->addTransition(&isButton1Released,S2);
  S2->addTransition(&wachtenOpStopSignaal, S0);
}

void loop() {
  button1.poll();
  machine.run();
}

// state
void ventilatieUit() {
  if(machine.executeOnce) {
    #ifdef DEBUG
    Serial.println("STATE: ventilatieUit");
    #endif
    digitalWrite(RELAIS1_PIN, LOW);
    // ventilatorTimer.stop();
    ventilatorTimer.reset();
  }
}

// transition
bool wachtenOpStartSignaal() {
  // TODO: add humidity sensor level exeeded event
  if(button1.pressed()){
    #ifdef DEBUG
    Serial.println("TRANS: wachtenOpStartSignaal - button1.pressed");
    #endif
    return true;
  }
  return false;
}

// state
void observingButton1PressDuration() {
  if(machine.executeOnce) {
    #ifdef DEBUG
    Serial.println("STATE: observingButton1PressDuration");
    #endif
    button1Timer.start();
  }
}

// transition
bool isButton1Released() {
  if(button1.released()) {
    
    unsigned long buttonPressDurationInMillis = (unsigned long)button1Timer.stop();
    #ifdef DEBUG
    Serial.print("TRANS: isButton1Released - buttonPressDurationInMillis:");
    Serial.println(buttonPressDurationInMillis);
    #endif
    button1Timer.reset();
    unsigned long oneMinute = 60;
    // bepaal hoe lang de ventilator moet blijven draaien (in milliseconden)
    unsigned long ventilationDuration = (unsigned long)10 * oneMinute * (unsigned long)1000; // by default 10 minuten
    if(buttonPressDurationInMillis>1000) {
      ventilationDuration = buttonPressDurationInMillis * oneMinute; // convert seconds to minutes
    }
    
    // stel de ventilator timer in 
    ventilatorTimer.set((long)ventilationDuration);
    ventilatorTimer.reset();
    #ifdef DEBUG
    Serial.print("TRANS: isButton1Released - set timer:");
    Serial.println(ventilationDuration);
    #endif
    return true; // goto next state
  }

  return false;
}

// state
void ventilatieAan() {
  if(machine.executeOnce) {
    #ifdef DEBUG
    Serial.println("STATE: VentilatieAan");
    #endif
    digitalWrite(RELAIS1_PIN,HIGH);
    ventilatorTimer.start();
  }
}

// transitie
bool wachtenOpStopSignaal() {
  if(ventilatorTimer.done()) {
    #ifdef DEBUG
    Serial.println("TRANS: wachtenOpStopSignaal: ventilatorTimer.done");
    #endif
    return true;
  }

  if(button1.pressed()) {
    #ifdef DEBUG
    Serial.println("TRANS: wachtenOpStopSignaal: button1.pressed");
    #endif
    return true;
  }

  return false;
}
