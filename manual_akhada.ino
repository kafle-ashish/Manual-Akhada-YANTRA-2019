


#define BRAKE 0
#define CW    1
#define CCW   2
#define CS_THRESHOLD 15   // Definition of safety current (Check: "1.3 Monster Shield Example").

//MOTOR 1
#define MOTOR_A1_PIN 7
#define MOTOR_B1_PIN 8

//MOTOR 2
#define MOTOR_A2_PIN 4
#define MOTOR_B2_PIN 9

#define PWM_MOTOR_1 5
#define PWM_MOTOR_2 6

#define CURRENT_SEN_1 A2
#define CURRENT_SEN_2 A3

#define EN_PIN_1 A0
#define EN_PIN_2 A1

#define MOTOR_1 0
#define MOTOR_2 1

//Channels to use
#define CHANNEL1 10
#define CHANNEL2 11
#define CHANNEL3 12
#define CHANNEL4 12


//Channel pulse read parameters
//Below CHANNEL_NEUTRAL_LOW robot moves backward.
#define CHANNEL_NEUTRAL_LOW 1400 //Lower limit to remain in stop state.
#define CHANNEL_NEUTRAL_HIGH 1550//Upper limit to remain in stop state.
//Above CHANNEL_NEUTRAL_HIGH robot moves forward.

//This parameter are not needed.
//1960 is the highest reading from channel-1 and channel-2.
#define CHANNEL_ONE_TWO_HIGH  1960
//990 is the lowest.
//During startup reading is 0 from each channel.
//This acts as a parameter to check if the robot should be moving backwards.
//The robot should not move backwards when value is less than 990 i.e 0
#define CHANNEL_ONE_TWO_LOW  990

short usSpeed = 150;  //default motor speed
unsigned short usMotor_Status = BRAKE;
//Array to store channel readings.
//Stores upto CHANNEL-1 to CHANNEL-4
int channelData[4];

void setup()                         
{
  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);

  pinMode(MOTOR_A2_PIN, OUTPUT);
  pinMode(MOTOR_B2_PIN, OUTPUT);

  pinMode(PWM_MOTOR_1, OUTPUT);
  pinMode(PWM_MOTOR_2, OUTPUT);

  pinMode(CURRENT_SEN_1, OUTPUT);
  pinMode(CURRENT_SEN_2, OUTPUT);  

  pinMode(EN_PIN_1, OUTPUT);
  pinMode(EN_PIN_2, OUTPUT);

 //for left right movement
  pinMode(CHANNEL1, INPUT);
  //for forward-Reverse movement
  pinMode(CHANNEL2, INPUT);

  //Not used right now.
  pinMode(CHANNEL3, INPUT);
  pinMode(CHANNEL4, INPUT);

  Serial.begin(9600);
}

void loop() 
{
    digitalWrite(EN_PIN_1, HIGH);
    digitalWrite(EN_PIN_2, HIGH);
    
    //This array element holds the reading of channnel-1.
    //Channel-1 is controlled by the left-right movement in the joy-stick.
    channelData[0] = pulseIn(CHANNEL1, HIGH);//left-right channel

    //This array element holds the reading of channel-2.
    //Channel-2 is controlled by the forward-backward movement in the joy-stick.
    channelData[1] = pulseIn(CHANNEL2, HIGH);//forward backward channel
    //Not used right now.
    //Couldn't calibrate,
    channelData[2] = pulseIn(CHANNEL3, HIGH);
    channelData[3] = pulseIn(CHANNEL4, HIGH);
    //****************************************//
    
    //Remain stopped while the value is between 1400, 1550
    if(channelData[1] >= CHANNEL_NEUTRAL_LOW && channelData[1] <= CHANNEL_NEUTRAL_HIGH){ 
      Stop();
    
    }
    //If the value is greater than 1550 move forward.
    if(channelData[1] > CHANNEL_NEUTRAL_HIGH ){
       Forward();
    }

    //If the value is less than 1400 move backward.
    if(channelData[1] < CHANNEL_NEUTRAL_LOW && channelData[1] > CHANNEL_ONE_TWO_LOW){
       Reverse();
    }

    if(channelData[0] >= CHANNEL_NEUTRAL_LOW && channelData[1] <= CHANNEL_NEUTRAL_HIGH){
       Stop();
    }

    //If channel 1 is greater than 1550 move right.
    if(channelData[0] > CHANNEL_NEUTRAL_HIGH ){
       Right();
    }
    //If channel 1 is less than 1400 move left.
    //This might need to be fixed for left right movement.
    //By changing functions called i.e. Left() or Right()
    if(channelData[0] < CHANNEL_NEUTRAL_LOW && channelData[1] > CHANNEL_ONE_TWO_LOW){
       Left();
    } 
}

void Stop()
{
  Serial.println("Stop");
  usMotor_Status = BRAKE;
  motorGo(MOTOR_1, usMotor_Status, 0);
  motorGo(MOTOR_2, usMotor_Status, 0);
}

void Forward()
{
  Serial.println("Forward");
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
}

void Reverse()
{
  Serial.println("Reverse");
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
}

void Left()
{
  Serial.println("Left");
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  usMotor_Status = CCW;
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
}

void Right()
{
  Serial.println("Right");
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  usMotor_Status = CW;
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
}

void IncreaseSpeed()
{
  usSpeed = usSpeed + 10;
  if(usSpeed > 255)
  {
    usSpeed = 255;  
  }
  
  Serial.print("Speed +: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);  
}

void DecreaseSpeed()
{
  usSpeed = usSpeed - 10;
  if(usSpeed < 0)
  {
    usSpeed = 0;  
  }
  
  Serial.print("Speed -: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);  
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)         //Function that controls the variables: motor(0 ou 1), direction (cw ou ccw) e pwm (entra 0 e 255);
{
  if(motor == MOTOR_1)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A1_PIN, LOW); 
      digitalWrite(MOTOR_B1_PIN, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A1_PIN, HIGH);
      digitalWrite(MOTOR_B1_PIN, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_B1_PIN, LOW);            
    }
    
    analogWrite(PWM_MOTOR_1, pwm); 
  }
  else if(motor == MOTOR_2)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A2_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A2_PIN, HIGH);
      digitalWrite(MOTOR_B2_PIN, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A2_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, LOW);            
    }
    
    analogWrite(PWM_MOTOR_2, pwm);
  }
}
