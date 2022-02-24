// Error value
int prevoffset;
int offset;
// Pins for the half bridge
int motor1 = 0;
int motor2 = 1;
// Pin for the potentiometer
int pot = A3;
// Pin for the signal
int input = A1;
// Inputted angle 
int setangle = 0;
// Time measurements
int previousTime = 0;
int currentTime; 
// Delta time used for integration and differentiation 
int elapsedTime;  
// PID gains for controller
int P = 10;
int I = 1/100;
int D = 0;
void setup() {
  // Setting the timer/counter 0 registers to enable fast PWM mode with no prescalar
  TCCR0A = 2<<COM0A0 | 2<<COM0B0 | 3<<WGM00;
  TCCR0B = 0<<WGM02 | 1<<CS00;
  // Defining pin states
  pinMode(input, INPUT);
  pinMode(pot, INPUT);
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
}
// Function for running the servo motor 
int servo_run(int offset){
  // Calculates delta time
  currentTime = millis();
  elapsedTime = currentTime - previousTime;
  previousTime = currentTime; 
  // Calculates the motor correction using PID gains given 
  // Present response 
  int present = P*offset;
  // Past response 
  int past = I*offset*elapsedTime;
  // Future response 
  int future = D*(offset-prevoffset)/elapsedTime;
  // Saturates the PID output and scales it to match the 
  offset = present + past + future;
  offset = constrain(offset,-980,980);
  offset = map(offset,-980,980,-255,255);
  // Applies the PID output to the half bridge 
  if(offset>0){
    analogWrite(motor2,0);
    analogWrite(motor1,abs(offset));
  }
  else if(offset<0){
    analogWrite(motor2,abs(offset));
    analogWrite(motor1,0);
  }
  else if(offset==0){
    analogWrite(motor1,0);
    analogWrite(motor2,0);
  }
  return offset;
}
void loop() {
  // Calculates the error and then runs the servo 
  setangle = analogRead(input);
  offset = setangle-analogRead(pot);
  offset = servo_run(offset);
  prevoffset = offset;
}
