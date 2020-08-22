int in1 = 4;
int in2= 2;
int in3 = 7;
int in4 = 8;
int enA = 6;
int enB = 11;
#define ch1 10 //x axia
#define ch2 3 //y axis
#define ch3 5
#define ch4 9
#define Swing1 A0
#define Swing2 A1
#define Grab1 A2
#define Grab2 A3

int Channel1 ;
int Channel2 ;
int motorSpeedA = 0;
int motorSpeedB = 0;
void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(Swing1,OUTPUT);
  pinMode(Swing2,OUTPUT);
  pinMode(Grab1,OUTPUT);
  pinMode(Grab2,OUTPUT);
  Serial.begin(9600);
}
void loop() {
  int xAxis = (pulseIn (ch1, HIGH));
  int yAxis = (pulseIn (ch2, HIGH));
  int yMapped = 0;
  int xMapped = 0;
  // Motor A forward
  bool bin1 = false;
  bool bin2 = true;
  // Motor B forward
  bool bin3 = false;
  bool bin4 = true;
  
  // Y-axis used for forward and backward control
  if (yAxis < 1460) {
    bin1 = !bin1;
    bin2 = !bin2;
    bin3 = !bin3;
    bin4 = !bin4;    
    
    /*
    // Set Motor A backward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);*/
    // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    yMapped = map(yAxis, 1460, 990, 0, 255);
    
  }
  else if (yAxis > 1500) {
    /*
    // Set Motor A forward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Set Motor B forward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);*/
    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    yMapped = map(yAxis, 1500, 2000, 0, 255);
  }
  // If joystick stays in middle the motors are not moving
  else {
    yMapped = 0;    
  }
  motorSpeedA = yMapped;
  motorSpeedB = yMapped;
  
  // X-axis used for left and right control
  if (xAxis < 1460) {
    // Convert the declining X-axis readings from 470 to 0 into increasing 0 to 255 value
    xMapped = map(xAxis, 1460, 990, 0, 255);
    // Move to left - decrease left motor speed, increase right motor speed
    motorSpeedA = abs(motorSpeedA - xMapped);
    motorSpeedB = motorSpeedB + xMapped;
    
    if(xMapped>yMapped){
      //motor A rotates reverse    
        bin1 = !bin1;
        bin2 = !bin2;        
        motorSpeedA+=70; 
               
    }
    // Confine the range from 0 to 255
    if(motorSpeedA > 255){
      motorSpeedA = 255;
    }
    if (motorSpeedB > 255) {
      motorSpeedB = 255;
    }
  }
  if (xAxis > 1500) {
    // Convert the increasing X-axis readings from 550 to 1023 into 0 to 255 value
    int xMapped = map(xAxis, 1500, 2000, 0, 255);
    // Move right - decrease right motor speed, increase left motor speed
    motorSpeedA = motorSpeedA + xMapped;
    motorSpeedB = abs(motorSpeedB - xMapped);
    if(xMapped>yMapped){
      //motor B rotates reverse
        bin3 = !bin3;
        bin4 = !bin4;
        motorSpeedB+=70;
    }
    // Confine the range from 0 to 255
    if (motorSpeedA > 255) {
      motorSpeedA = 255;
    }
    if(motorSpeedB > 255){
      motorSpeedB = 255;
    }
  }
  // Prevent buzzing at low speeds (Adjust according to your motors. My motors couldn't start moving if PWM value was below value of 70)
  if (motorSpeedA < 70) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 70) {
    motorSpeedB = 0;
  }
  // Set Motor A
  digitalWrite(in1, bin1);
  digitalWrite(in2, bin2);
  // Set Motor B
  digitalWrite(in3, bin3);
  digitalWrite(in4, bin4);
  analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
  analogWrite(enB, motorSpeedB); // Send PWM signal to motor B
  
  Serial.println("yMapped");
  Serial.println(yMapped);
  Serial.println("xMapped");
  Serial.println(xMapped);
  Serial.println("motorSpeedA");
  Serial.println(motorSpeedA);
  Serial.println("motorSpeedB");
  Serial.println(motorSpeedB);
  int lyAxis = (pulseIn (ch3, HIGH));
  int lxAxis = (pulseIn (ch4, HIGH));
  if((lyAxis<1800 && lyAxis>1200)||(lxAxis<1800 && lxAxis>1200))
    armature(lyAxis,lxAxis);
  else{
    digitalWrite(Swing1,false);
    digitalWrite(Swing2,false);
    digitalWrite(Grab1,false);
    digitalWrite(Grab2,false); 
  } 
}
void armature(int lyAxis,int lxAxis){
  bool s1 = false;
  bool s2 = false;
  bool g1 = false;
  bool g2 = false;  
  if(lyAxis>1800){
    s1 = !s1;     
  }
  else if(lyAxis<1200){
    s2 = !s2;
  }
  if(lxAxis>1800){
    g1 = !g1;
  }
  else if(lxAxis<1200){
    g2 = !g2;
  }
  digitalWrite(Swing1,s1);
  digitalWrite(Swing2,s2);
  digitalWrite(Grab1,g1);
  digitalWrite(Grab2,g2);
}
