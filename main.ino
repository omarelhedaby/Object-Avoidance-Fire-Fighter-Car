  char state;
String mode="Manual";
int fanMode=0;
//***************************UlTRASONIC PINS***********************
const int trigfront=8; //Trig pin of front ultrasonic
const int echofront=9;// Echo pin of front ultrasonic
const int trigright=10;//Trig pin of right ultrasonic
const int echoright=11;//Echo pin of right ultrasonic
const int trigleft=12;//Trig pin of left ultrasonic 
const int echoleft=13;//Echo pin of left ultrasonic
//***************************Automatic Mode Variables**********************
char defaultDirection='F';
int count=0;
int caseleftright=0;
int prev; //0 is right , 1 is left
void setup() {
  // put your setup code here, to run once:
pinMode(3,OUTPUT);//input3
pinMode(4,OUTPUT);//input4
pinMode(5,OUTPUT); //input2
pinMode(6,OUTPUT); //input3
pinMode(trigfront,OUTPUT);
pinMode(echofront,INPUT);
pinMode(trigright,OUTPUT);
pinMode(echoright,INPUT);
pinMode(trigleft,OUTPUT);
pinMode(echoleft,INPUT);
pinMode(2,OUTPUT); //Fan signal
pinMode(A0,INPUT);//Flame sensor signal 
//digitalWrite(2,HIGH);

Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(A0));
if(Serial.available()>0) 
{
    state=Serial.read();
// ***********************************CHECK OF MODE(AUTO OR MANUAL)**********************8
if(state=='X')
{
  mode="Auto";
  digitalWrite(2,LOW);
  Serial.println(mode);
}
else if(state=='x')
{
  mode="Manual";
  Serial.println(mode);
}
//**************************MANUAL MODE*******************************************
if(mode=="Manual") //Manual Mode
{
   int flameInput =analogRead(A0);
  if(flameInput<=36)
  {
    stopCar();
    Serial.println("FIRE");
    digitalWrite(2,HIGH);
    delay(1900);
    digitalWrite(2,LOW);
  }
 else if(state=='F')
{

 int distanceFront=getDistance(trigfront,echofront);
 if(distanceFront>19)
 {
forward();
Serial.print(distanceFront); Serial.println("   ");
 }
 else
 {
  stopCar();
Serial.print(distanceFront); Serial.println("   ");
Serial.println("F");
 }
}
else if(state=='B')
{
 backward();
}
else if(state=='G')
{
      digitalWrite(3,LOW); //Forward-LEFT
digitalWrite(4,LOW);
digitalWrite(5,HIGH);
digitalWrite(6,LOW);
Serial.println(state);
}
else if(state=='I')
{
digitalWrite(3,HIGH); //Forward-RIGHT
digitalWrite(4,LOW);
digitalWrite(5,LOW);
digitalWrite(6,LOW);
Serial.println(state);  
}
else if(state=='R')
{
  right();
}
else if(state=='L')
{
  left();
}
else if(state=='H')
{
 digitalWrite(3,LOW); //BACK lEFT
digitalWrite(4,LOW);
digitalWrite(5,LOW);
digitalWrite(6,HIGH);
Serial.println(state);
}
else if(state=='J')
{
  digitalWrite(3,LOW); //BACK RIGHT
digitalWrite(4,HIGH);
digitalWrite(5,LOW);
digitalWrite(6,LOW);
Serial.println("J"); 
}
else if(state=='V')
{
  digitalWrite(2,HIGH);
}
else if(state=='v')
{
  digitalWrite(2,LOW);
}
else if(state=='S')
{
 stopCar();
}
}
//*******************************************AUTOMATIC MODE*****************************************
if(mode=="Auto")
{
  int distanceFront=getDistance(trigfront,echofront);
  int distanceRight=getDistance(trigright,echoright);
  int distanceLeft=getDistance(trigleft,echoleft);
  Serial.print("Front: "); Serial.print(distanceFront); Serial.print("  ");
  Serial.print("Right: "); Serial.print(distanceRight); Serial.print("  ");
  Serial.print("Left: "); Serial.print(distanceLeft); Serial.print("  ");
  int flameInput =analogRead(A0);
  if(flameInput<=36)
  {
    stopCar();
    Serial.println("FIRE");
    digitalWrite(2,HIGH);
    delay(2200);
    
  }
  else
  {
    digitalWrite(2,LOW);
  if(distanceFront>18)
  {
    count=0;
     if(distanceLeft<16 && distanceRight<16)
    {
      Serial.print("   "); Serial.println("Both Blocked ,Forward is on");
      right();
      delay(520);
      stopCar();
      caseleftright=0;
    }
    else if(distanceRight<16)
    {
      if(prev==0)
      {
        caseleftright++;
      }
      else
      {
        prev=1;
      caseleftright=0;
      }
       if(caseleftright==6)
      {
         Serial.print("   "); Serial.print(caseleftright); Serial.println("case left right");
      right();
      delay(320);
      stopCar();
      }
      else
      {
      Serial.print("   "); Serial.println("Corner Case go left");
      left();
      delay(170);
      stopCar();
      }
    }
    else if(distanceLeft<16)
    {
      if(prev==1)
      {
        caseleftright++;
      }
      else
      {
      prev=0;
      caseleftright=0;
      }
      if(caseleftright==6)
      {
         Serial.print("   "); Serial.print(caseleftright); Serial.println("case left right");
      right();
      delay(320);
      stopCar();
      }
      else
      {
       Serial.print("   "); Serial.println("Corner Case go right");
      right();
      delay(170);
      stopCar();
      }
    }
    
    else
    {
       Serial.print("   "); Serial.println("All good , go forward");
       forward();
       count=1;
    }
  }
  else
  {
    if(count=1)
    {
      backward;
      delay(45);
      count=0;
    }
    stopCar();
     if(distanceLeft<16 && distanceRight<16)
    {
      Serial.print("   "); Serial.println("Both Corners Blocked");
      right();
      delay(520);
      stopCar();
      caseleftright=0;
    }

    else if(distanceRight<16)
    {
      if(prev==0)
      {
        caseleftright++;
      }
      else
      {
        prev=1;
      caseleftright=0;
      }
       if(caseleftright==6)
      {
         Serial.print("   "); Serial.print(caseleftright); Serial.println("case left right");
      left();
      delay(320);
      stopCar();
      }
      else
      {
      Serial.print("   "); Serial.println("Forward Block , Go left!!");
       left(); 
       delay(170);
       stopCar();
      }
    }
     else if(distanceLeft<16)
    {
      if(prev==1)
      {
        caseleftright++;
      }
      else
      {
      prev=0;
      caseleftright=0;
      }
        if(caseleftright==6)
      {
         Serial.print("   "); Serial.print(caseleftright); Serial.println("case left right");
      right();
      delay(320);
      stopCar();
      }
      else
      {
      right();
      delay(170);
      stopCar();
      Serial.print("   "); Serial.println("Forward Block , Go right!!");
      }
    }
    else if(distanceRight>14 || distanceLeft>14)
    {
      //Serial.print("   "); Serial.println("Forward Blocked GO RIGHT");
      right();
      delay(170);
      stopCar();
      caseleftright=0;
    }
    else{
      caseleftright=0;
      right();
      delay(170);
      stopCar();
    }
   
  }
  }
}
}


}
//*****************************END OF OBSTACLE AVOIDANCE CAR CODE *****************************************


//*************************FUNCTION AREA*************************************

//********************************ULTRA SONIC DISTANCE***************************
int getDistance(int trigPin,int echoPin)
{
  long duration;
int distance;
  digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
return distance;
}
//********************************MOVES FORWARD****************************
void forward()
{
  digitalWrite(3,HIGH); //Forward
digitalWrite(4,LOW);
digitalWrite(5,HIGH);
digitalWrite(6,LOW);
//Serial.println("Forward");
}
//******************************MOVES BACKWARD***************************
void backward()
{
   digitalWrite(3,LOW); //Back
digitalWrite(4,HIGH);
digitalWrite(5,LOW);
digitalWrite(6,HIGH);
Serial.println("Backward");
}
//************************MOVES RIGHT**********************
void right()
{
    digitalWrite(3,HIGH); //RIGHT
digitalWrite(4,LOW);
digitalWrite(5,LOW);
digitalWrite(6,HIGH);
//Serial.println("RIGHT");
}
//*********************MOVES LEFT********************
void left()
{
   digitalWrite(3,LOW); //Forward-LEFT
 digitalWrite(4,HIGH);
 digitalWrite(5,HIGH);
 digitalWrite(6,LOW);
 Serial.println("LEFT");
}
//********************STOPS CAR*************************
void stopCar()
{
  digitalWrite(3,LOW);
digitalWrite(4,LOW);
digitalWrite(5,LOW);
digitalWrite(6,LOW);
}
