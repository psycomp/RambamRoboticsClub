#include "Arduino.h"
#include "Holonomic.h"

#include <Adafruit_MotorShield.h>
#include <math.h>

#define MIN_SPEED -255
#define MAX_SPEED  255

Holonomic::Holonomic(Adafruit_MotorShield afms) {
	_afms = afms;
	*motor1 = _afms.getMotor(1);
	*motor2 = _afms.getMotor(2);
	*motor3 = _afms.getMotor(3);
	*motor4 = _afms.getMotor(4);
}

void Holonomic::drive(int x, int y, int r) {
	if(x > 100) { x = 100; } else if(x < -100) { x = -100; }
	if(y > 100) { y = 100; } else if(y < -100) { y = -100; }

	int m1 = y + x + r;
	int m2 = (-1 * y) + x + r;
	int m3 = (-1 * y) + (-1 * x) + r;
	int m4 = y + (-1 * x) + r;

        m1 = map(m1, -300, 300, MIN_SPEED, MAX_SPEED);
        m2 = map(m2, -300, 300, MIN_SPEED, MAX_SPEED);
        m3 = map(m3, -300, 300, MIN_SPEED, MAX_SPEED);
        m4 = map(m4, -300, 300, MIN_SPEED, MAX_SPEED);

	int d1 = m1 == 0 ? RELEASE : m1 > 0 ? FORWARD : BACKWARD;
	int d2 = m2 == 0 ? RELEASE : m2 > 0 ? FORWARD : BACKWARD;
	int d3 = m3 == 0 ? RELEASE : m3 > 0 ? FORWARD : BACKWARD;
	int d4 = m4 == 0 ? RELEASE : m4 > 0 ? FORWARD : BACKWARD;

	motor1->run(d1);
	motor1->setSpeed(abs(m1));

	motor2->run(d2);
	motor2->setSpeed(abs(m2));

	motor3->run(d3);
	motor3->setSpeed(abs(m3));

	motor4->run(d4);
	motor4->setSpeed(abs(m4));
}

void Holonomic::rDrive(int a, int v, int r) {
	float rad  = a * PI / 180;
	float x = v * cos(rad - 90);
	float y = v * sin(rad - 90);
	drive((int)x, (int)y, r);
}

void Holonomic::halt() {
	drive(0, 0, 0);
}

void Holonomic::spin(int r) {
	drive(0, 0, r);
}

