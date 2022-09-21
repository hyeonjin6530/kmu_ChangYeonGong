#define PIN_LED 7
unsigned int count, toggle;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  toggle = 0;
  count = 0;
  digitalWrite(PIN_LED, toggle);
}

void loop() {
  // put your main code here, to run repeatedly:
  toggle = toggle_state(toggle);
  delay(1000);
  digitalWrite(PIN_LED, toggle);

  while (count<12){
      toggle = toggle_state(toggle);
      digitalWrite(PIN_LED, toggle);
      count++;
      delay(200);
  }

  while(1) {
    toggle == 0;
    digitalWrite(PIN_LED, toggle);
  } // infinite loop
}

int toggle_state(int toggle) {
  if (toggle == 0) {
    toggle = 1;
  }
  else if (toggle == 1) {
    toggle = 0;
  }
  return toggle; 
}