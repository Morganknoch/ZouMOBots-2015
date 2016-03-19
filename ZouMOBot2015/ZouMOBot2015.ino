#include <QTRSensors.h>

int driveDisable = 2; //standby		stby

					  //Left Motor
int L_Speed = 10; //Speed control 	PWMA
int L_Dir1 = 11; //Direction		Ain1
int L_Dir2 = 12; //Direction 		Ain2

				 //Right Motor; aka B
int R_Speed = 6; //Speed control	PWMB
int R_Dir1 = 7; //Direction			Bin1
int R_Dir2 = 8; //Direction		Bin2

#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low

unsigned char pins[] = { 4 };
QTRSensorsRC qtrrc(pins, 1, TIMEOUT, 3);  //Init the reflectanec sensor on pin 4 and emmiter pin to 3
unsigned int sensorValues[1];
							 //ir prox stuff
int irLeft = 3;  //connected to output of Vishay TSSP58P38 reciever 
int emitterLeft = 13;  //connected to enable of Pololu 38 kHz IR Proximity Sensor
int ir_sense_Left;  //variable to store if an object is detected
int i = 0;

int irRight = 5;  //connected to output of Vishay TSSP58P38 reciever 
int emitterRight = 9;  //connected to enable of Pololu 38 kHz IR Proximity Sensor
int ir_sense_Right;  //variable to store if an object is detected
int j = 0;

void setup() {
	pinMode(driveDisable, OUTPUT);

	pinMode(L_Speed, OUTPUT);
	pinMode(L_Dir1, OUTPUT);
	pinMode(L_Dir2, OUTPUT);

	pinMode(R_Speed, OUTPUT);
	pinMode(R_Dir1, OUTPUT);
	pinMode(R_Dir2, OUTPUT);

	pinMode(irLeft, INPUT);
	pinMode(emitterLeft, OUTPUT);

	pinMode(irRight, INPUT);
	pinMode(emitterRight, OUTPUT);

	Serial.begin(9600);
}

void loop() {

	/* if(checkLine()){
	drive(-255, -255);
	delay(500);
	drive(-255, 255);
	delay(1000);
	stopMotors();
	}
	*/

	rotateCorrect();


	delay(100);
}

boolean checkLine() {
	qtrrc.read(sensorValues);

	Serial.print(sensorValues[0]);
	Serial.println();

	if (sensorValues[0] < 400) {
		return true;
	}
	else {
		return false;
	}

}

void rotateCorrect() {
	digitalWrite(emitterLeft, HIGH);
	delay(120);
	digitalWrite(emitterLeft, LOW);
	ir_sense_Left = digitalRead(irLeft);

	digitalWrite(emitterRight, HIGH);
	delay(120);
	digitalWrite(emitterRight, LOW);
	ir_sense_Right = digitalRead(irRight);

	if (ir_sense_Left == LOW) {
		Serial.print("left: ");
		Serial.println(++i);

		if (ir_sense_Right == LOW) {
			drive(255, 255);
		}
		else {
			spinCCW();
		}
	}
	else {

		if (ir_sense_Right == LOW) {
			Serial.print("Right: ");
			Serial.println(++i);
			spinCW();
		}
		else {
			drive(255, 255);
		}
	}
}

void spinCCW() {
	drive(-250, 250);
}

void spinCW() {
	drive(250, -250);
}


void drive(float motorLeft, float motorRight) {
	//values: 1 = full forward, -1 = full reverse, 0 stop

	digitalWrite(driveDisable, HIGH);

	if (motorLeft < 0) {
		motorLeft = -1 * motorLeft;
		digitalWrite(L_Dir1, HIGH);
		digitalWrite(L_Dir2, LOW);
		analogWrite(L_Speed, motorLeft);
	}
	else {
		digitalWrite(L_Dir1, LOW);
		digitalWrite(L_Dir2, HIGH);
		analogWrite(L_Speed, motorLeft);
	}

	if (motorRight < 0) {
		motorRight = -1 * motorRight;
		digitalWrite(R_Dir1, LOW);
		digitalWrite(R_Dir2, HIGH);
		analogWrite(R_Speed, motorRight);
	}
	else {
		digitalWrite(R_Dir1, HIGH);
		digitalWrite(R_Dir2, LOW);
		analogWrite(R_Speed, motorRight);
	}
}

void stopMotors() {
	//enable standby  
	digitalWrite(driveDisable, LOW);
}

