/******************************
*                             *
  Holonomic Driving Functions
*                             *
******************************/
/*
  HOW TO INSTALL MOTORS
  1   2

  4   3

  All motors move forward CLOCKWISE
*/
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);
int _stopMode = BRAKE;

// drive(x, y, r)
// x: x-vector, 0-100% of full speed
// y: y-vector, 0-100% of full speed
// r: clockwise rotation, 0-100% of full speed
void drive(int x, int y, int r) {
  if (x > 100) {
    x = 100;
  } else if (x < -100) {
    x = -100;
  }
  if (y > 100) {
    y = 100;
  } else if (y < -100) {
    y = -100;
  }

  int m1 = y + x + r;
  int m2 = (-1 * y) + x + r;
  int m3 = (-1 * y) + (-1 * x) + r;
  int m4 = y + (-1 * x) + r;

  int max = 0;
  if (abs(m1) > max) {
    max = abs(m1);
  }
  if (abs(m2) > max) {
    max = abs(m2);
  }
  if (abs(m3) > max) {
    max = abs(m3);
  }
  if (abs(m4) > max) {
    max = abs(m4);
  }

  if (max > 0) {
    double scale = 1;
    if (max > 100) {
      scale = 1 / (max / 100.0);
    }
    m1 = (scale * m1) * 2.55;
    m2 = (scale * m2) * 2.55;
    m3 = (scale * m3) * 2.55;
    m4 = (scale * m4) * 2.55;
  }

  int d1 = m1 == 0 ? _stopMode : m1 > 0 ? FORWARD : BACKWARD;
  int d2 = m2 == 0 ? _stopMode : m2 > 0 ? FORWARD : BACKWARD;
  int d3 = m3 == 0 ? _stopMode : m3 > 0 ? FORWARD : BACKWARD;
  int d4 = m4 == 0 ? _stopMode : m4 > 0 ? FORWARD : BACKWARD;

  motor1->setSpeed(abs(m1));
  motor2->setSpeed(abs(m2));
  motor3->setSpeed(abs(m3));
  motor4->setSpeed(abs(m4));

  motor1->run(d1);
  motor2->run(d2);
  motor3->run(d3);
  motor4->run(d4);
}

// rDrive(a, v, r)
// a: angle relative to robot's front, 0-360 degrees
// v: velocity, 0-100% of full speed
// r: clockwise rotation, 0-100% of full speed
void rDrive(int a, int v, int r) {
  float rad  = (a - 90) * PI / 180;
  float x = v * cos(rad);
  float y = v * sin(rad) * -1;
  drive((int)x, (int)y, r);
}

// spin(r):
// r: clockwise rotation, 0-100% of full speed
void spin(int r) {
  drive(0, 0, r);
}

void halt() {
  drive(0, 0, 0);
}

void testAllWheels() {
  motor1->setSpeed(128);
  motor1->run(FORWARD);
  delay(3000);
  motor1->run(RELEASE);

  motor2->setSpeed(128);
  motor2->run(FORWARD);
  delay(3000);
  motor2->run(RELEASE);

  motor3->setSpeed(128);
  motor3->run(FORWARD);
  delay(3000);
  motor3->run(RELEASE);

  motor4->setSpeed(128);
  motor4->run(FORWARD);
  delay(3000);
  motor4->run(RELEASE);
}


