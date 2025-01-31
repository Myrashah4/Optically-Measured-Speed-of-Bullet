//CODE FOR OPTOELECTRONICS PROJECT : OPTICALLY MEASURED SPEED OF BULLET
// AUTHOR : MYRA SHAH

/*A speed measurement system using laser modules and LDRs. 
It calculates the speed of objects by measuring the time between interruptions of two laser beams.
*/

//Inputs/Outputs
int sensor_1 = 8;              //Front sensor (photoresistor)
int sensor_2 = 9;               //Rear sensor

//Variables
float distance_between = 0.1;    //Add your distance in "meter" units

//Variables used in the code...
unsigned long counter_1, current_time, time_interval;
byte last_CH1_state, last_CH2_state;
float Speed, last_speed;

void setup() {
  pinMode(sensor_1, INPUT);
  pinMode(sensor_2, INPUT);
  
  PCICR |= (1 << PCIE0);    //enable PCMSK0 scan  
  PCMSK0 |= (1 << PCINT0);  //Set pin D8 trigger an interrupt on state change. 
  PCMSK0 |= (1 << PCINT1);  //Set pin D9 trigger an interrupt on state change.      

  Serial.begin(9600);
}

void loop() {
  // Read the value from the photoresistor
  
  
  int sensorValue1 = digitalRead(sensor_1);
//  delay(500);
  // Read the value from the second sensor
  int sensorValue2 = digitalRead(sensor_2);
  
  // Print the sensor values to the Serial Monitor
  Serial.print("Sensor 1 Value: ");
  Serial.println(sensorValue1);
  Serial.print("Sensor 2 Value: ");
  Serial.println(sensorValue2);
  
 Speed = distance_between / (time_interval / 1E6);
// if (Speed != last_speed) {
    // Do something if speed changes (optional)
 //}

  last_speed = Speed;
  Serial.println(time_interval);
  Serial.println(Speed);
  delay(1000);
}

ISR(PCINT0_vect) {
  //First we take the current count value in micro seconds using the micros() function
  current_time = micros();
  
  if (PINB & B00000001) { //We make an AND with the pin state register, We verify if pin 8 is HIGH???
    if (last_CH1_state == 0) { //If the last state was 0, then we have a state change...
      last_CH1_state = 1; //Store the current state into the last state for the next loop
      counter_1 = current_time; //Set counter_1 to current value.
    }
  } else if (last_CH1_state == 1) { //If pin 8 is LOW and the last state was HIGH then we have a state change      
    last_CH1_state = 0; //Store the current state into the last state for the next loop    
  }

  if (PINB & B00000010) { //pin D9 -- B00000010                                              
    if (last_CH2_state == 0) {                                               
      last_CH2_state = 1;                                                   
      time_interval = current_time - counter_1;                                    
    }
  } else if (last_CH2_state == 1) {                                           
    last_CH2_state = 0;                                                     
  }

}
