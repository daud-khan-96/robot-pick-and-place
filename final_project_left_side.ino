/* SYDE 263 Final Project Skeleton
    Copyright (C) 2023  Calvin Young

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3 (GNU-GPLv3)
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
s
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>. 
*/

#include <Servo.h>

Servo gripper; 
Servo ax1;
Servo ax2;

// setting up the rx and tx pins
int rx1 = 5;
int rx2 = 6;
int tx1 = 7;
int tx2 = 8;

//Variables to store the servo angle associated with the gripper open and closed positions
int gr_open = 0;
int gr_close = 180;

//Variables to store the servo angles associated with the home location
float ax1_home = 85+5;
float ax2_home = -30+30;

//Variables to store the servo angles associated with position 1
float ax1_pos1 = 103.5+5;
float ax2_pos1 = 31+30;

//Variables to store the servo angles associated with position 2
float ax1_pos2 = 127+5;
float ax2_pos2 = 55+30;

//Variables to store the servo angles associated with position 3
float ax1_pos3 = 157.8+5;
float ax2_pos3 = 87+38;

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

  // setting the number of steps to the largest value
  if (abs(num_steps_1) >= abs(num_steps_2)){
    num_steps = abs(num_steps_1);
  }
  else {
     num_steps = abs(num_steps_2);
  }
  // defining ratios to increment by for each servo
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
  // final adjustment to ensure correct position
  ax1_current = ax1e;
  ax1.write(ax1_current);
  ax2_current = ax2e;
  ax2.write(ax2_current);
  delay(25);
}

void setup() {
  // setting rx and tx I/O
  pinMode(rx1, INPUT_PULLUP);
  pinMode(rx2, INPUT_PULLUP);
  pinMode(tx1, OUTPUT);
  pinMode(tx2, OUTPUT);
  // initializing tx pins to position 1
  digitalWrite(tx1,HIGH);
  digitalWrite(tx2,LOW);
  //What pins are the servos attached to?
  gripper.attach(12); 
  ax1.attach(11);
  ax2.attach(10);

  //Setting the axes to the home position
  ax1_current = ax1_home;
  ax2_current = ax2_home;

  ax1.write(ax1_current);
  ax2.write(ax2_current);
  
  gripper.write(gr_open);
  
  Serial.begin(9600);

}

void loop() {

    //Write your actuation code here!
    String input = Serial.readString();  //read until timeout
    input.trim();                        // remove any \r \n whitespace at the end of the String

    // checking rx, changing payload variable, setting tx to rx, covering all cases
    if (digitalRead(rx1) and !digitalRead(rx2)){
      payload =1;
      digitalWrite(tx1,HIGH);
      digitalWrite(tx2,LOW);
    } else if (!digitalRead(rx1) and digitalRead(rx2)){
      payload =2;
      digitalWrite(tx1,LOW);
      digitalWrite(tx2,HIGH); 
    } else if (digitalRead(rx1) and digitalRead(rx2)){
      payload =3;
      digitalWrite(tx1,HIGH);
      digitalWrite(tx2,HIGH);
    }
    // setting angles depending on where the payload is
    switch(payload){
      case 1:
        ax1_pos_payload = ax1_pos1;
        ax2_pos_payload = ax2_pos1;
        break;
      case 2:
        ax1_pos_payload = ax1_pos2;
        ax2_pos_payload = ax2_pos2;
        break;
      case 3:
        ax1_pos_payload = ax1_pos3;
        ax2_pos_payload = ax2_pos3;
        break;
    }
  // movement depending on serial input
  if (input == "1"){
    
    Serial.println((String)"Moving payload form position " + payload + " to position " + input);
    //Go to current
    moveTopos(ax1_current, ax2_current, ax1_pos_payload, ax2_pos_payload);
    delay(200);
    //Close gripper
    gripper.write(gr_close);
    delay(400);
    //Go to target
    moveTopos(ax1_current, ax2_current, ax1_pos1, ax2_pos1);
    delay(200);
    //Open gripper
    gripper.write(gr_open);
    delay(300);
    //Go to home
    moveTopos(ax1_current, ax2_current, ax1_home, ax2_home);
    delay(200);
    payload = 1;
    // setting the correct tx pin states after move to end up as rx for next iteration of loop by other mechanism
    digitalWrite(tx1,HIGH);
    digitalWrite(tx2,LOW);
    delay(500);
    
  } else if (input == "2"){
    
    Serial.println((String)"Moving payload form position " + payload + " to position " + input);
    //Go to current
    moveTopos(ax1_current, ax2_current, ax1_pos_payload, ax2_pos_payload);
    delay(200);
    //Close gripper
    gripper.write(gr_close);
    delay(400);
    //Go to target
    moveTopos(ax1_current, ax2_current, ax1_pos2, ax2_pos2);
    delay(200);
    //Open gripper
    gripper.write(gr_open);
    delay(300);
    //Go to home
    moveTopos(ax1_current, ax2_current, ax1_home, ax2_home);
    delay(200);
    payload = 2;
    // setting the correct tx pin states after move to end up as rx for next iteration of loop by other mechanism
    digitalWrite(tx1,LOW);
    digitalWrite(tx2,HIGH);
    delay(500);
    
  } else if (input == "3"){
    
    Serial.println((String)"Moving payload form position " + payload + " to position " + input);
    //Go to current
    moveTopos(ax1_current, ax2_current, ax1_pos_payload, ax2_pos_payload);
    delay(200);
    //Close gripper
    gripper.write(gr_close);
    delay(400);
    //Go to target
    moveTopos(ax1_current, ax2_current, ax1_pos3, ax2_pos3);
    delay(200);
    //Open gripper
    gripper.write(gr_open);
    delay(300);
    //Go to home
    moveTopos(ax1_current, ax2_current, ax1_home, ax2_home);
    delay(200);
    payload = 3;
    // setting the correct tx pin states after move to end up as rx for next iteration of loop by other mechanism
    digitalWrite(tx1,HIGH);
    digitalWrite(tx2,HIGH);
    delay(500);
  }
}
