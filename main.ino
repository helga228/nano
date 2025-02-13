#include <iarduino_MultiServo.h>

int trigPin = 15;
int echoPin = 7;

iarduino_MultiServo MSS;

struct ServoMoveTask {
    int servoIndex;
    int startAngle;
    int endAngle;
    int currentAngle;
    int step;
    unsigned long lastUpdateTime;
    unsigned long updateInterval;
    bool isMoving;
};

ServoMoveTask tasks[4];

void setup() {
    MSS.begin();
    for (int i = 0; i < 8; i++) {
        MSS.servoSet(i, SERVO_SG90);
    }

    for (int i = 0; i < 8; i++) {
        MSS.servoWrite(i, 90);
        delay(300);
    }

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

int servoIndices[] = {0, 1, 2, 3, 4, 5, 6, 7};

void smoothServoMove(int servoIndex, int startAngle, int endAngle) {
    int step = (startAngle < endAngle) ? 1 : -1;
    for (int angle = startAngle; angle != endAngle + step; angle += step) {
        MSS.servoWrite(servoIndex, angle);
        delay(10);
    }
}

void loop() {
  smoothServoMove(1, 90, 50);
  smoothServoMove(0, 90, 50);
  smoothServoMove(1, 50, 90);
  smoothServoMove(3, 90, 120);
  smoothServoMove(2, 90, 120);
  smoothServoMove(3, 120, 90);

  tasks[0] = {0, 50, 90, 50, (50 < 90) ? 1 : -1, millis(), 10, true};
  tasks[1] = {2, 90, 50, 90, (90 > 50) ? -1 : 1, millis(), 10, true};
  tasks[2] = {4, 90, 120, 90, (90 < 120) ? 1 : -1, millis(), 10, true};
  tasks[3] = {6, 90, 120, 90, (90 < 120) ? -1 : 1, millis(), 10, true};
  for (int i = 0; i < 4; i++) {
    if (tasks[i].isMoving && millis() - tasks[i].lastUpdateTime >= tasks[i].updateInterval) {
      tasks[i].lastUpdateTime = millis();

      MSS.servoWrite(tasks[i].servoIndex, tasks[i].currentAngle);
      if (tasks[i].currentAngle != tasks[i].endAngle) {
          tasks[i].currentAngle += tasks[i].step;
      } else {
          tasks[i].isMoving = false;
      }
    }
  }
}