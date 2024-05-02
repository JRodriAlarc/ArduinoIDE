#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int potPin1 = A0; // Pin para el potenciómetro 1
int potPin2 = A1; // Pin para el potenciómetro 2
int potPin3 = A2; // Pin para el potenciómetro 3
int potPin4 = A3; // Pin para el potenciómetro 4

int switchPin = 2; // Pin para el switch manual/automático

int buttonPin1 = 3; // Pin para el botón 1
int buttonPin2 = 4; // Pin para el botón 2
int buttonPin3 = 5; // Pin para el botón 3

int ledPin1 = 6; // Pin para el LED 1
int ledPin2 = 7; // Pin para el LED 2
int ledPin3 = 8; // Pin para el LED 3

int storedData1 = 0; // Variable para indicar si se ha almacenado la posición 1
int storedData2 = 0; // Variable para indicar si se ha almacenado la posición 2
int storedData3 = 0; // Variable para indicar si se ha almacenado la posición 3

int switchState = 0; // Variable para almacenar el estado del switch

int pos1, pos2, pos3, pos4; // Variables globales para las posiciones de los potenciómetros
int pos1Stored[4]; // Arreglo para almacenar la posición 1
int pos2Stored[4]; // Arreglo para almacenar la posición 2
int pos3Stored[4]; // Arreglo para almacenar la posición 3

void setup() {
  servo1.attach(9); // Pin de control del servo 1
  servo2.attach(10); // Pin de control del servo 2
  servo3.attach(11); // Pin de control del servo 3
  servo4.attach(12); // Pin de control del servo 4

  pinMode(switchPin, INPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
}

void loop() {
  switchState = digitalRead(switchPin);

  if (switchState == HIGH) {
    executeRoutine();
  } else {
    controlServosManually();
    checkButtons();
  }

  checkStoredData();
}

void controlServosManually() {
  pos1 = map(analogRead(potPin1), 0, 1023, 0, 180);
  pos2 = map(analogRead(potPin2), 0, 1023, 0, 180);
  pos3 = map(analogRead(potPin3), 0, 1023, 0, 180);
  pos4 = map(analogRead(potPin4), 0, 1023, 0, 180);

  servo1.write(pos1);
  servo2.write(pos2);
  servo3.write(pos3);
  servo4.write(pos4);
}

void executeRoutine() {
  if (storedData1 == 1 && storedData2 == 1 && storedData3 == 1) {
    // Si todas las posiciones están guardadas
    moveToPosition(1); // Mover a la primera posición guardada
    delay(2000); // Esperar 2 segundos antes de mover a la siguiente posición
    moveToPosition(2); // Mover a la segunda posición guardada
    delay(2000);
    moveToPosition(3); // Mover a la tercera posición guardada
    delay(2000);
  }
}

void moveToPosition(int positionNumber) {
  // Utiliza el arreglo de posiciones almacenado en la pos1Stored[] de la función savePosition()
  
  if (positionNumber == 1) {
    moveServos(pos1Stored);
  } else if (positionNumber == 2) {
    moveServos(pos2Stored);
  } else if (positionNumber == 3) {
    moveServos(pos3Stored);
  }
}

void moveServos(int positions[]) {
  servo1.write(positions[0]);
  servo2.write(positions[1]);
  servo3.write(positions[2]);
  servo4.write(positions[3]);
}

void checkButtons() {
  if (digitalRead(buttonPin1) == HIGH && storedData1 == 0) {
    savePosition(1);
  } else if (digitalRead(buttonPin2) == HIGH && storedData2 == 0) {
    savePosition(2);
  } else if (digitalRead(buttonPin3) == HIGH && storedData3 == 0) {
    savePosition(3);
  }
}

void savePosition(int positionNumber) {
  if (positionNumber == 1) {
    storedData1 = 1;
    pos1Stored[0] = pos1;
    pos1Stored[1] = pos2;
    pos1Stored[2] = pos3;
    pos1Stored[3] = pos4;
    }
  } else if (positionNumber == 2) {
    storedData2 = 1;
    pos2Stored[0] = pos1;
    pos2Stored[1] = pos2;
    pos2Stored[2] = pos3;
    pos2Stored[3] = pos4;
  } else if (positionNumber == 3) {
    storedData3 = 1;
    pos3Stored[0] = pos1;
    pos3Stored[1] = pos2;
    pos3Stored[2] = pos3;
    pos3Stored[3] = pos4;
  }
}

void checkStoredData() {
  // Encender o apagar los LEDs según los datos almacenados
  digitalWrite(ledPin1, storedData1 == 1 ? HIGH : LOW); // Enciende el LED1 si la posición 1 está guardada, de lo contrario, apágalo
  digitalWrite(ledPin2, storedData2 == 1 ? HIGH : LOW); // Enciende el LED2 si la posición 2 está guardada, de lo contrario, apágalo
  digitalWrite(ledPin3, storedData3 == 1 ? HIGH : LOW); // Enciende el LED3 si la posición 3 está guardada, de lo contrario, apágalo
}