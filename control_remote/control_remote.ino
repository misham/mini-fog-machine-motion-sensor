#include <Arduino.h>

#define SERIAL_BAUD (115200)

#define REMOTE_PIN (14) // D5
#define INTERRUPT_PIN (5) // D1

#define CALC_MINUTES(min) (1000 * 60 * min)

#define RUN_TIME (CALC_MINUTES(10))
#define OFF_TIME (CALC_MINUTES(15))

volatile bool motionDetected = false;
volatile bool isRunning = false;
volatile bool shouldRun = true;
volatile unsigned long startTime = 0;
volatile unsigned long offTime = 0;

void ICACHE_RAM_ATTR setRemote() {
  Serial.println("-----------------");
  Serial.println("motion detected....");
  if(isRunning == true) {
    Serial.println("already running");
    Serial.println("-----------------");
    return;
  }

  Serial.println("can activate fogger");
  motionDetected = true;
  Serial.println("-----------------");
}

void triggerRemote() {
  Serial.println("remote pin high");
  digitalWrite(REMOTE_PIN, HIGH);
  delay(2000);
  Serial.println("remote pin low");
  digitalWrite(REMOTE_PIN, LOW);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SERIAL_BAUD);
  pinMode(REMOTE_PIN, OUTPUT);
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);

  digitalWrite(REMOTE_PIN, LOW);

  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), setRemote, RISING);
}

void loop() {
  if(shouldRun == false) {
    // Check if we're in the off timeout & if the timeout expired, then allow to run
    unsigned long curTime = millis();
    if (curTime - offTime >= OFF_TIME) {
      Serial.println("=================");
      Serial.println("off timer expired");
      shouldRun = true;
      Serial.println("=================");
    }
  } else if(motionDetected == true) {
    Serial.println("+++++++++++++++++");
    Serial.println("activating fogger...");
    triggerRemote();
    startTime = millis(); // start timer by recording the current time
    motionDetected = false;
    isRunning = true;
    Serial.println("+++++++++++++++++");
  } else if(isRunning == true) {
    // if 15 minutes have elapsed
    unsigned long curTime = millis();
    if (curTime - startTime >= RUN_TIME) {
      Serial.println("*****************");
      Serial.println("deactivating fogger...");
      triggerRemote();

      isRunning = false;
      offTime = millis(); // start the timer so it doesn't run
      shouldRun = false;
      Serial.println("*****************");
    }
  }
}
