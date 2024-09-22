#define LED 7
unsigned int toggle;

void setup() {
  pinMode(LED, OUTPUT);
  toggle = 0;
  

}

void loop() {
  digitalWrite(LED, toggle);
  delay(1000);

  for (int i = 0; i < 10; i++) {
    toggle = toggle_state(toggle);
    toggle = toggle % 2;
    digitalWrite(LED, toggle);
    delay(100);
  }

  digitalWrite(LED, HIGH);
  while(1) {}
}

int toggle_state(int toggle) {
  return ++toggle;
}   
