#include <Servo.h>

// Arduino pin assignment

#define PIN_LED 9 // LED 연결하기
#define IR 0 // IR센서 연결

// #define PIN_POTENTIOMETER 3 // Potentiometer at Pin A3
// Add IR Sensor Definition Here !!!
#define PIN_SERVO 10

#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 250.0   // maximum distance to be measured (unit: mm)

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)

#define LOOP_INTERVAL 50   // Loop Interval (unit: msec)

#define _EMA_ALPHA 0.3

#define _TARGET_LOW  100.0
#define _TARGET_HIGH 250.0

float  dist_ema, dist_prev = _DIST_MAX;

Servo myservo;
unsigned long last_loop_time;   // unit: msec

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  Serial.begin(1000000);
}

void loop()
{
  float  dist;
  unsigned long time_curr = millis();
  int a_value, duty;

  a_value = analogRead(IR);

  dist = (6762.0/(a_value-9)-4.0)*10.0 - 60.0;


if (dist < _DIST_MIN) { // 범위필터: 거리를 벗어나면 이전 값 가져오는 거
    digitalWrite(PIN_LED, 1);
    dist = dist_prev;           // cut lower than minimum
  } else if (dist > _DIST_MAX) {
    digitalWrite(PIN_LED, 1);
    dist = dist_prev;           // Cut higher than maximum
  } else {    // In desired Range
    digitalWrite(PIN_LED, 0);       // LED ON  범위안에 들어오면 led 켜    
    dist_prev = dist;
  }

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  // Remove Next line !!!
  // Read IR Sensor value !!!
  // Convert IR sensor value into distance !!!
  // we need distance range filter here !!!
  // we need EMA filter here !!!
  dist_ema = _EMA_ALPHA * dist + (1 - _EMA_ALPHA) * dist_ema; //ema 필터
  // map distance into duty
  duty = (dist_ema - _DIST_MIN)/(_TARGET_HIGH - _TARGET_LOW)*(_DUTY_MAX - _DUTY_MIN) + _DUTY_MIN;

  if (dist_ema <= _TARGET_LOW){
    myservo.writeMicroseconds(_DUTY_MIN);
  }
  else if (dist_ema > _TARGET_LOW && dist_ema < _TARGET_HIGH ){
    // digitalWrite(PIN_LED, 0); 
    myservo.writeMicroseconds(duty);    
  }
  else if (dist_ema >= _TARGET_HIGH){
    // digitalWrite(PIN_LED, 1);
    myservo.writeMicroseconds(_DUTY_MAX);
  }

  // print IR sensor value, distnace, duty !!!
  Serial.print("MIN:"); Serial.print(_DIST_MIN);
  Serial.print(",IR:"); Serial.print(a_value);
  Serial.print(",dist:"); Serial.print(dist);
  Serial.print(",ema:"); Serial.print(dist_ema);
  Serial.print(",servo:"); Serial.print(duty);
  Serial.println(",MAX:"); Serial.print(_DIST_MAX);
  Serial.print("");
}
