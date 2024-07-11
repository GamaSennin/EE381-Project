#define NOTE_G4  392

#include <SoftwareSerial.h>

//Create software serial object to communicate with HC-05
SoftwareSerial mySerial(0, 1); //HC-05 Tx & Rx is connected to Arduino #0 & #1

int code[] = {1, 2, 3, 4}; //the desired code is entered  in this array,
//separated by commas

int entered[5]; //create a new empty  array for the code entered by
//the user (has 4 elements)
// Define pins
const int trigPin = 11;
const int echoPin = 9;


// Variables to hold the duration and distance
long duration;
long dist;

int flag1 = 1, flag2 = 1;
const int LED1  = 13;
const int LED2 = 3;
const int LED3 = 2;
const int buzzerPin = 8;
const  int button1 = 4;
const int button2 = 5;
const int button3 = 6;
const int  button4 = 7;
const int timerdelay = 25;
String command;

const int redLED = 12;
const  int greenLED = 10;
void checkEntered1(int button);

void setup() {
  // put your setup code here, to run once:
  pinMode(LED1, OUTPUT);
  pinMode(LED2,  OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3,  LOW);
  delay(1000);
  Serial.begin(9600); //begin Serial
  mySerial.begin(9600);

  pinMode  (button1, INPUT);
  pinMode (button2, INPUT);
  pinMode (button3, INPUT);
  pinMode (button4, INPUT);
  pinMode (greenLED, OUTPUT);
  pinMode (redLED,  OUTPUT);

  pinMode (trigPin, OUTPUT);
  pinMode (echoPin, INPUT);

  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  for (int i = 0; i < 4; i++) { //work through numbers 0-3
    Serial.println(code[i]);  //print each digit of the code
    Serial.println(entered[i]); //print each element  of the entered[]

  }
}

void loop() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the duration of the pulse from echoPin
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in centimeters
  dist = duration * 0.034 / 2;

  // Print the distance to the serial monitor
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.println(" cm");

  // Wait a short time before taking the next measurement
  delay(1000);
  if (dist > 100) {
    digitalWrite(LED1,  LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
  }
  if (dist >= 2 && dist <= 25) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2,  LOW);
    digitalWrite(LED3, LOW);
    tone(buzzerPin, NOTE_G4, 15000);
    if (Serial.available() && flag1 == 1)
    {
      command = Serial.readStringUntil('\n');
      mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
      flag1 = 0;
    }
    //delay(2000);
    if (mySerial.available())
    {
      entered[0] = command[0] - '0';
      entered[1] = command[1] - '0';
      entered[2] = command[2] - '0';
      entered[3] = command[3] - '0';
      //      delay(100);
      //Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
      Serial.print(command);
      compareCode();
    }
    //delay(5000);
    //delay(20);
  }
  else if (dist > 25 && dist <= 75) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
  }
  else if  (dist > 75 && dist <= 100) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2,  LOW);
    digitalWrite(LED3, HIGH);
  }

  if (digitalRead(button1)  == HIGH) { //if button1 is pressed
    checkEntered1(1); //call checkEntered  and pass it a 1

    delay(timerdelay);//wait, needed for correct functioning,  otherwise
    //buttons are deemed to be pressed more than once

  }
  else if (digitalRead(button2) == HIGH) { //if button2 is pressed
    checkEntered1(2);  //call checkEntered1 and pass it a 2

    delay(timerdelay); //wait

  }
  else if (digitalRead(button3) == HIGH) { //if button3 is pressed
    checkEntered1(3);  //call checkEntered1 and pass it a 3

    delay(timerdelay); //wait

  }
  else if (digitalRead(button4) == HIGH) { //if button4 is pressed
    checkEntered1(4);  //call checkEntered1 and pass it a 4

    delay(timerdelay); //wait

  }
}

void checkEntered1(int button) { //check the first element of the  entered[] array
  if (entered[0] != 0) { //if it is not a zero, i.e. it has already  been inputted
    checkEntered2(button); //move on to checkEntered2, passing  it "button"
  }

  else if (entered[0] == 0) { //if it is zero, i.e.  if it hasn't been defined with a button yet
    entered[0] = button; //set the  first element as the button that has been pressed
    Serial.print("1: ");  Serial.println(entered[0]); //for debugging
  }

}

void checkEntered2(int  button) { //check the second element of the entered[] array

  if (entered[1]  != 0) { //if it is not a zero, i.e. it has already been inputted
    checkEntered3(button);  //move on to checkEntered3, passing it "button"
  }

  else if (entered[1]  == 0) { //if it is zero, i.e. if it hasn't been defined with a button yet
    entered[1]  = button; //set the second element as the button that has been pressed
    Serial.print("2:  "); Serial.println(entered[1]); //for debugging
  }

}

void checkEntered3(int  button) { //check the third element of the entered[] array
  if (entered[2] !=  0) { //if it is not a zero, i.e. it has already been inputted
    checkEntered4(button);  //move on to checkEntered4, passing it "button"
  }

  else if (entered[2]  == 0) { //if it is zero, i.e. if it hasn't been defined with a button yet
    entered[2]  = button; //set the third element as the button that has been pressed
    Serial.print("3:  "); Serial.println(entered[2]); //for debugging
  }

}

void checkEntered4(int  button) { //check the third element of the entered[] array
  if (entered[3] ==  0) { //if it is zero, i.e. if it hasn't been defined with a button yet
    entered[3]  = button; //set the final element as the button that has been pressed
    Serial.print("4:  "); Serial.println(entered[3]); //for debugging
    delay(100); //allow time  for processing
    compareCode(); //call the compareCode function
  }
}



void  compareCode() { //checks if the code entered is correct by comparing the code[]  array with the entered[] array

  for (int i = 0; i < 4; i++) { //these three  lines are for debugging
    Serial.println(entered[i]);
  }
  if ((entered[0]  == code[0]) && (entered[1] == code[1]) && (entered[2] == code[2]) && (entered[3]  == code[3])) { //if all the elements of each array are equal
    digitalWrite(redLED,  LOW); // turn the red LED off
    digitalWrite(LED1,  LOW);
    digitalWrite(LED2,  LOW);
    digitalWrite(LED3,  LOW);
    noTone(buzzerPin);
    digitalWrite(greenLED, HIGH); //turn the green  LED on
    delay(10000); //wait for a bit
    digitalWrite(greenLED, LOW);  //turn the green LED off




    for (int i  = 0; i < 5; i++) { //this next loop is for debugging
      entered[i] = 0;

    }

    //loop(); //return to loop() (not really necessary)

  }
  else { //if you (or the intruder) get the code wrong

    digitalWrite(redLED,  HIGH);
    digitalWrite(LED1,  HIGH);
    digitalWrite(LED2,  HIGH);
    digitalWrite(LED3,  HIGH);
    delay(1000);
    //digitalWrite(redLED, LOW);
    Serial.println("Warning : Intruder!");
    for (int i = 0; i < 5; i++) { //this next loop is for debugging
      entered[i] = 0;

    }
    flag1 = 1;

  }

}
