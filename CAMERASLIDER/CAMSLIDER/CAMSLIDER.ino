
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include "bitmap.h"
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


#define limitSwitch 10
#define PinSW 3
#define PinCLK 2  
#define PinDT 4
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

AccelStepper stepper1(1, 6, 5); // (Type:driver, STEP, DIR)
AccelStepper stepper2(1, 8, 7);

MultiStepper StepperControl; 

int direction  = 1;

long gotoposition[2]; 

volatile long XInPoint=0;
volatile long YInPoint=0;
volatile long XOutPoint=0;
volatile long YOutPoint=0;  
volatile long totaldistance=0;
int flag=0; 
int temp=0;
int i,j;
unsigned long switch0=0;
unsigned long rotary0=0;
float setspeed=200;
float motorspeed;
float timeinsec;
float timeinmins;
volatile boolean TurnDetected;  
volatile boolean rotationdirection;  


void Switch()  
{
 if(millis()-switch0>50)
 {
 flag=flag+1;
 }
 switch0=millis();
}

void Rotary()  
{
delay(75);
if (digitalRead(PinCLK))
rotationdirection= digitalRead(PinDT);
else
rotationdirection= !digitalRead(PinDT);
TurnDetected = true;
delay(75);
}

void setup() 
{

  Serial.begin(9600);
  stepper1.setMaxSpeed(6400);
  stepper1.setSpeed(6400);
  stepper2.setMaxSpeed(6400);
  stepper2.setSpeed(6400);
  
  pinMode(limitSwitch, INPUT_PULLUP);
  pinMode(PinSW,INPUT_PULLUP); 
  pinMode(PinCLK,INPUT_PULLUP); 
  pinMode(PinDT,INPUT_PULLUP);  
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  
  // Create instances for MultiStepper - Adding the 2 steppers to the StepperControl instance for multi control
  StepperControl.addStepper(stepper1);
  StepperControl.addStepper(stepper2);

  attachInterrupt (digitalPinToInterrupt(3),Switch,RISING); // SW connected to D2
  attachInterrupt (digitalPinToInterrupt(2),Rotary,RISING); // CLK Connected to D3
   
  // display Boot logo
  display.drawBitmap(0, 0, CamSlider, 128, 64, 1);
  display.display();
  delay(2000);
  display.clearDisplay(); 
  
  Home(); // Move the slider to the initial position - homing
  
}

void Home()
{
  stepper1.setMaxSpeed(6400);
  stepper1.setSpeed(3200);
  stepper2.setMaxSpeed(6400);
  stepper2.setSpeed(3200);
  if(digitalRead(limitSwitch)==1)
  {
    display.drawBitmap(0, 0, Homing, 128, 64, 1);
    display.display(); 
  }
  
  while (digitalRead(limitSwitch)== 1) 
  {
    stepper1.setSpeed(-3200);
    stepper1.runSpeed();
    
 }
  delay(20);
  stepper1.setCurrentPosition(0);
    stepper1.moveTo(200);
    while(stepper1.distanceToGo() != 0)
    {
      stepper1.setSpeed(3200);
      stepper1.runSpeed();
    }
    stepper1.setCurrentPosition(0);
    display.clearDisplay();
}

void SetSpeed()
{
  display.clearDisplay();
  while( flag==6)
  {
  if (TurnDetected)  
  {
        TurnDetected = false;  // do NOT repeat IF loop until new rotation detected
        if (rotationdirection) 
        { 
         setspeed = setspeed + 30;
        }
        if (!rotationdirection) 
        { 
         setspeed = setspeed - 30;        
         if (setspeed < 0) 
        { 
         setspeed = 0;
        }
        }

          display.clearDisplay();
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.setCursor(30,0);
          display.print("Speed");
          motorspeed=setspeed/80;
          display.setCursor(5,16);
          display.print(motorspeed);
          display.print(" mm/s");   
          totaldistance=XOutPoint-XInPoint;
          if(totaldistance<0)
            {
              totaldistance=totaldistance*(-1);
            }
            else
            {
              
            }
          timeinsec=(totaldistance/setspeed);
          timeinmins=timeinsec/60;
          display.setCursor(35,32);
          display.print("Time");
          display.setCursor(8,48);
          if(timeinmins>1)
          {
          display.print(timeinmins);
          display.print(" min");
          }
          else
          { 
          display.print(timeinsec);
          display.print(" sec");
          }
          display.display();
  }
          display.clearDisplay();
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.setCursor(30,0);
          display.print("Speed");
          motorspeed=setspeed/80;
          display.setCursor(5,16);
          display.print(motorspeed);
          display.print(" mm/s");   
          totaldistance=XOutPoint-XInPoint;
           if(totaldistance<0)
            {
              totaldistance=totaldistance*(-1);
            }
            else
            {
              
            }
          timeinsec=(totaldistance/setspeed);
          timeinmins=timeinsec/60;
          display.setCursor(35,32);
          display.print("Time");
          display.setCursor(8,48);
          if(timeinmins>1)
          {
          display.print(timeinmins);
          display.print(" min");
          }
          else
          { 
          display.print(timeinsec);
          display.print(" sec");
          }
          display.display();
  }
 
}

void stepperposition(int n)
{
  stepper1.setMaxSpeed(6400);
  stepper1.setSpeed(3200);
  stepper2.setMaxSpeed(6400);
  stepper2.setSpeed(3200);
  if (TurnDetected)  
  {
        TurnDetected = false;  // do NOT repeat IF loop until new rotation detected
     if(n==1)
     {
        if (!rotationdirection) 
        { 
          if( stepper1.currentPosition()-500>0 )
          {
          stepper1.move(-500);
          while(stepper1.distanceToGo() != 0)
            {
              stepper1.setSpeed(-3000);
              stepper1.runSpeed();
            }
          }
            else
            {
                while (stepper1.currentPosition()!=0) 
                 {
                      stepper1.setSpeed(-3000);
                      stepper1.runSpeed();
                 }
            }
        }

        if (rotationdirection) 
        { 
          if( stepper1.currentPosition()+500<61000 )
          {
          stepper1.move(500);
          while(stepper1.distanceToGo() != 0)
            {
              stepper1.setSpeed(3200);
              stepper1.runSpeed();
            }
          }
          else
          {
            while (stepper1.currentPosition()!= 61000) 
             {
                  stepper1.setSpeed(3000);
                  stepper1.runSpeed();
    
             } 
          }
        }
     }
     if(n==2)
     {
       if (rotationdirection) 
       { 
         stepper2.move(-100);
         while(stepper2.distanceToGo() != 0)
           {
             stepper2.setSpeed(-3000);
             stepper2.runSpeed();
           }      
       }
        if (!rotationdirection) 
       { 
         stepper2.move(100);
         while(stepper2.distanceToGo() != 0)
           {
             stepper2.setSpeed(3000);
             stepper2.runSpeed();
           } 
       }
     }
     }
} 


void loop()
{ 
  
  //Begin Setup
  if(flag==0)
  {
    display.clearDisplay();
    display.drawBitmap(0, 0, BeginSetup, 128, 64, 1);
    display.display(); 
    setspeed=200;
  }
  
  //SetXin
  if(flag==1)
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10,28);
    display.println("Set X In");
    display.display(); 
    while(flag==1)
    {
    stepperposition(1); 
    }
    XInPoint=stepper1.currentPosition();
  }
  //SetYin
  if(flag==2)
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10,28);
    display.println("Set Y In");
    display.display(); 
    while(flag==2)
    {
    stepperposition(2);
    }
    stepper2.setCurrentPosition(0);
    YInPoint=stepper2.currentPosition();
  }
  //SetXout
  if(flag==3)
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10,28);
    display.println("Set X Out");
    display.display(); 
    while(flag==3)
    {
    stepperposition(1);
    Serial.println(stepper1.currentPosition());
    }
    XOutPoint=stepper1.currentPosition();
    
  }
  //SetYout
  if(flag==4)
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10,28);
    display.println("Set Y Out");
    display.display();
    while(flag==4)
    {
    stepperposition(2);
    }
    YOutPoint=stepper2.currentPosition();
    display.clearDisplay();
        
    // Go to IN position
    gotoposition[0]=XInPoint;
    gotoposition[1]=YInPoint;    
    display.clearDisplay();
    display.setCursor(8,28);
    display.println(" Preview  ");
    display.display(); 
    stepper1.setMaxSpeed(3000);
    StepperControl.moveTo(gotoposition);
    StepperControl.runSpeedToPosition();
  }

  //Display Set Speed
  if(flag==5)
  {
    display.clearDisplay();
    display.setCursor(8,28);
    display.println("Set Speed");
    display.display();  
  }
  //Change Speed
  if(flag==6)
  {
    display.clearDisplay();
    SetSpeed();
  }
  //DisplayStart
  if(flag==7)
  {
    display.clearDisplay();
    display.setCursor(30,27);
    display.println("Start");
    display.display();
  }
  //Start
  if(flag==8)
  { 
    display.clearDisplay();
    display.setCursor(20,27);
    display.println("Running");
    display.display();
    Serial.println(XInPoint);
    Serial.println(XOutPoint);
    Serial.println(YInPoint);
    Serial.println(YOutPoint);

    gotoposition[0]=XOutPoint;
    gotoposition[1]=YOutPoint;
    stepper1.setMaxSpeed(setspeed);
    StepperControl.moveTo(gotoposition);
    StepperControl.runSpeedToPosition();
    flag=flag+1;
  }
  //Slide Finish
   if(flag==9)
   {
    display.clearDisplay();
    display.setCursor(24,26);
    display.println("Finish");
    display.display();
   }
  //Return to start
   if(flag==10)
   {
    display.clearDisplay();
    Home();
    flag=0;
   }  
}
