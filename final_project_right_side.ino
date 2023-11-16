#include <Servo.h>

Servo gripper; 
Servo ax1;
Servo ax2;

int rx1 = 3;
int rx2 = 4;
int tx1 = 6;
int tx2 = 7;

bool isNew = false;

//Variables to store the servo angle associated with the gripper open and closed positions
int gr_open = 30;
int gr_close = 180;

//Variables to store the servo angles associated with the home location
float ax1_home = 0; 
float ax2_home = 86;

//Variables to store the servo angles associated with position 1
float ax1_pos1 = 100;
float ax2_pos1 = 138;

//Variables to store the servo angles associated with position 2
float ax1_pos2 = 70;
float ax2_pos2 = 110;

//Variables to store the servo angles associated with position 3
float ax1_pos3 = 50;
float ax2_pos3 = 90;

//Variables to store the servo angles associated with the current payload location
float ax1_pos_payload = ax1_pos1;
float ax2_pos_payload = ax2_pos1;

int payload = 1;

//Variables to store the servo angles associated with the current location
float ax1_current;
float ax2_current;

void moveTopos(float ax1s, float ax2s, float ax1e, float ax2e) {
  /*
   This function taskes the starting and ending angles for both axes and
   divides them into an even number of steps. This ensures that the movement
   from both motors occurs simultaneously. It also reduces the speed of
   movement to help protect the mechanism. This is a workable but sub-optimal
   way of moving the mechanism.

   ax1s -> Axis 1 Start Position
   ax1e -> Axis 1 End Position
   ax2s -> Axis 2 Start Position
   ax2e -> Axis 2 End Position
   */
  float num_steps_1 = ax1e - ax1s;
  float num_steps_2 = ax2e - ax2s;
  float num_steps = 0;

  if (abs(num_steps_1) >= abs(num_steps_2)){
    num_steps = abs(num_steps_1);
  }
  else {
     num_steps = abs(num_steps_2);
  }
  float inc1 = num_steps_1 / num_steps;
  float inc2 = num_steps_2 / num_steps;
  float stp = 0;
  for(stp = 0; stp<=num_steps; stp += 1){
    ax1_current += inc1;
    ax1.write(ax1_current);
    ax2_current += inc2;
    ax2.write(ax2_current);
    delay(25);
  }
  ax1_current = ax1e;
  ax1.write(ax1_current);
  ax2_current = ax2e;
  ax2.write(ax2_current);
  delay(25);
}

void setup() {
  pinMode(rx1, INPUT_PULLUP);
  pinMode(rx2, INPUT_PULLUP);
  pinMode(tx1, OUTPUT);
  pinMode(tx2, OUTPUT);
  //What pins are the servos attached to?
  gripper.attach(12); 
  ax1.attach(10);
  ax2.attach(11);

  //Setting the axes to the home position
  ax1_current = ax1_home;
  ax2_current = ax2_home;

  ax1.write(ax1_current);
  ax2.write(ax2_current);

  ax1_current = ax1_pos1;
  ax2_current = ax2_pos1;
  
  gripper.write(gr_open);
  
  Serial.begin(9600);

}

void loop() {
    Serial.println(payload);
    Serial.println("rx1 " + String(digitalRead(rx1)) + ", " + String(digitalRead(rx2)));

    //Write your actuation code here!
    //while (Serial.available() == 0) {}     //wait for data available
    String input = Serial.readString();  //read until timeout
    input.trim();                        // remove any \r \n whitespace at the end of the String

    if (digitalRead(rx1) and !digitalRead(rx2)){
      payload =1;
    } else if (!digitalRead(rx1) and digitalRead(rx2)){
      payload =2;
    } else if (digitalRead(rx1) and digitalRead(rx2)){
      payload =3;
    }
    switch(payload){
      case 1:
        ax1_pos_payload = ax1_pos1;
        ax2_pos_payload = ax2_pos1;
        digitalWrite(tx1,HIGH);
        digitalWrite(tx2,LOW);
        break;
      case 2:
        ax1_pos_payload = ax1_pos2;
        ax2_pos_payload = ax2_pos2;
        digitalWrite(tx1,LOW);
        digitalWrite(tx2,HIGH);
        break;
      case 3:
        ax1_pos_payload = ax1_pos3;
        ax2_pos_payload = ax2_pos3;
        digitalWrite(tx1,HIGH);
        digitalWrite(tx2,HIGH);
        break;
    }

    
  
  if (input == "1"){
    
    //Go to current
    moveTopos(ax1_current, ax2_current, ax1_pos_payload, ax2_pos_payload);
    Serial.println("moved to current");
    delay(200);
    //Close gripper
    gripper.write(gr_close);
    Serial.println("close gripper");
    delay(400);
    //Go to target
    moveTopos(ax1_current, ax2_current, ax1_pos1, ax2_pos1);
    Serial.println("target 1");
    delay(200);
    //Open gripper
    gripper.write(gr_open);
    Serial.println("open gripper");
    delay(200);
    //Go to home
    moveTopos(ax1_current, ax2_current, ax1_home, ax2_home);
    Serial.println("home");
    delay(200);
    payload = 1;
    digitalWrite(tx1,HIGH);
    digitalWrite(tx2,LOW);
    delay(500);
    
  } else if (input == "2"){
    //Go to current
    moveTopos(ax1_current, ax2_current, ax1_pos_payload, ax2_pos_payload);
    Serial.println("moved to current");
    delay(200);
    //Close gripper
    gripper.write(gr_close);
    Serial.println("close gripper");
    delay(400);
    //Go to target
    moveTopos(ax1_current, ax2_current, ax1_pos2, ax2_pos2);
    Serial.println("target 1");
    delay(200);
    //Open gripper
    gripper.write(gr_open);
    Serial.println("open gripper");
    delay(200);
    //Go to home
    moveTopos(ax1_current, ax2_current, ax1_home, ax2_home);
    Serial.println("home");
    delay(200);
    payload = 2;
    digitalWrite(tx1,LOW);
    digitalWrite(tx2,HIGH);
    delay(500);
    
  } else if (input == "3"){
    
    //Go to current
    moveTopos(ax1_current, ax2_current, ax1_pos_payload, ax2_pos_payload);
    Serial.println("moved to current");
    delay(200);
    //Close gripper
    gripper.write(gr_close);
    Serial.println("close gripper");
    delay(400);
    //Go to target
    moveTopos(ax1_current, ax2_current, ax1_pos3, ax2_pos3);
    Serial.println("target 1");
    delay(200);
    //Open gripper
    gripper.write(gr_open);
    Serial.println("open gripper");
    delay(200);
    //Go to home
    moveTopos(ax1_current, ax2_current, ax1_home, ax2_home);
    Serial.println("home");
    delay(200);
    payload = 3;
    digitalWrite(tx1,HIGH);
    digitalWrite(tx2,HIGH);
    delay(500);
    
  } else {
    Serial.println("invalid");

  }
  
}
