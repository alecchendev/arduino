#define ENCA 2 // YELLOW
#define ENCB 3 // WHITE
#define PWM 5
#define IN2 6
#define IN1 7

int pos = 0;
long prev_time = 0; // microseconds
float err_prev = 0;
float err_integral = 0;
float err_threshold = 3;

// 1. Make the PID general thing
// 2. Use it to make an automatic rotary telephone thingy

void setup() {
  Serial.begin(9600);
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);

  pinMode(PWM, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  delay(1000);
}

String phone_number = "1236549708";

void loop() {
  for (size_t i = 0; i < phone_number.length(); i++) {
    int digit = phone_number.substring(i, i+1).toInt();
    int target = target_from_num(digit);
    move_to_target(target);
    move_to_target(0);
    delay(200);
  }
  delay(1000);
}

int target_from_num(int num) {
  int full_rotation = 1200; // approxiamately
  if (num == 0) num = 10; // put 0 after 9
  return full_rotation / 12 * num;
}

void move_to_target(int target) {
  while (!step_to_target(target));
  setMotor(0, 0, PWM, IN1, IN2);
}

bool step_to_target(int target) {
  float kp = 1.5;
  float kd = 0.05;
  float ki = 0;

  long curr_time = micros();
  float delta_time_micros = (float)(curr_time - prev_time);
  float delta_time_secs = delta_time_micros / 1.0e6;
  prev_time = curr_time;

  int err = target - pos;
  // Finite difference approxiamation
  float de_dt = (err - err_prev) / delta_time_secs;
  err_integral = err_integral + err * delta_time_secs;

  // Control signal
  float u = kp * err + kd * de_dt + ki * err_integral;

  float power = fabs(u);
  power = power >= 255 ? 255 : power; // Cap the power

  // Motor direction
  int dir = u >= 0 ? 1 : -1;

  setMotor(dir, power, PWM, IN1, IN2);

  err_prev = err;

  Serial.print(target);
  Serial.print(" ");
  Serial.print(pos);
  Serial.println();

  return fabs(err) < err_threshold;
}

void setMotor(int dir, int pwm_val, int pwm, int in1, int in2) {
  analogWrite(pwm, pwm_val);
  if (dir == 1) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else if (dir == -1) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
}

void readEncoder() {
  int b = digitalRead(ENCB);
  if (b > 0) {
    pos += 1;
  } else {
    pos -= 1;
  }
}
