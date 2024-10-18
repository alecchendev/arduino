void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

const String message = "hey sexy";

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  for (size_t msg_idx = 0; msg_idx < message.length(); msg_idx++) {
    String morse = char_to_morse(message.charAt(msg_idx));
    blink_morse_msg(morse);
    small_pause();
  }
  delay(3000);
}

void blink_morse_msg(String morse) {
  for (size_t morse_idx = 0; morse_idx < morse.length(); morse_idx++) {
    blink_morse(morse.charAt(morse_idx));
  }
}

String char_to_morse(char c) {
  switch (c) {
    case 'h':
      return "....";
    case 'e':
      return ".";
    case 'y':
      return "-.--";
    case 's':
      return "...";
    case 'x':
      return "-..-";
    case ' ':
      return " ";
  }
}

void blink_morse(char c) {
  switch (c) {
    case '.':
      dot();
      break;
    case '-':
      dash();
      break;
    case ' ':
      big_pause();
      break;
  }
}

void dot() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}

void dash() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}

void small_pause() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}

void big_pause() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
