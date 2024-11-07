#include <Servo.h>

// Arduino pin assignment

#define PIN_IR    0         // IR sensor at Pin A0
#define PIN_LED   9
#define PIN_SERVO 10

#define _DUTY_MIN 500  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1500  // servo neutral position (90 degree)
#define _DUTY_MAX 2500  // servo full counter-clockwise position (180 degree)

#define _DIST_MIN  100.0   // minimum distance 100mm
#define _DIST_MAX  250.0   // maximum distance 250mm

#define EMA_ALPHA  0.5      // for EMA Filter

#define LOOP_INTERVAL 20   // Loop Interval (unit: msec)

Servo myservo;
unsigned long last_loop_time;   // unit: msec

float dist_prev = _DIST_MIN;
float dist_ema = _DIST_MIN;

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  Serial.begin(1000000);    // 1,000,000 bps
}

void loop()
{
  unsigned long time_curr = millis();
  int duty;
  float a_value, dist_raw;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  a_value = analogRead(PIN_IR);
  dist_raw = ((6762.0 / (a_value - 9.0)) - 4.0) * 10.0;

  if (dist_ema < _DIST_MIN) {
    myservo.writeMicroseconds(_DUTY_MIN);
    pinMode(PIN_LED, INPUT);
  } else if (dist_ema < _DIST_MAX) {
    myservo.writeMicroseconds(_DUTY_MIN + ((dist_ema - _DUTY_MIN)*(_DUTY_MAX - _DUTY_MIN) / 180.0));
    pinMode(PIN_LED, OUTPUT);
  } else {
    myservo.writeMicroseconds(_DUTY_MAX);
    pinMode(PIN_LED, INPUT);
  }
  // and turn on LED if the distance is in the range 

  dist_ema = dist_raw * EMA_ALPHA + dist_prev * (1 - EMA_ALPHA);// Put EMA filter code here
      
  //duty = map(dist_ema, _DIST_MIN, _DIST_MAX, _DUTY_MIN, _DUTY_MAX);
  duty = map(dist_ema, _DIST_MIN, _DIST_MAX, _DUTY_MIN, _DUTY_MAX);// Put map() equivalent code here
  
  myservo.writeMicroseconds(duty);

  Serial.print("_DUTY_MIN:");  Serial.print(_DUTY_MIN);
  Serial.print("_DIST_MIN:");  Serial.print(_DIST_MIN);
  Serial.print(",IR:");        Serial.print(a_value);
  Serial.print(",dist_raw:");  Serial.print(dist_raw);
  Serial.print(",ema:");       Serial.print(dist_ema);
  Serial.print(",servo:");     Serial.print(duty);
  Serial.print(",_DIST_MAX:"); Serial.print(_DIST_MAX);
  Serial.print(",_DUTY_MAX:"); Serial.print(_DUTY_MAX);
  Serial.println("");
}
