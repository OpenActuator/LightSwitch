#include <Servo.h> 

// I need to use #define for using below expressions in switch.
#define   TURN_ON     1
#define   TURN_OFF    2
#define   TIMER_10    3
#define   TIMER_30    4
#define   NO_COMMAND  0
 
Servo servo;

int DI_3_Pin = 6;   // DI_3 : Pin 6, "A", TURN_ON,  iKey = 1  
int DI_2_Pin = 7;   // DI_2 : Pin 7, "B", TURN_OFF, iKey = 2  
int DI_1_Pin = 8;   // DI_1 : Pin 8, "C", TIMER_10, iKey = 3  
int DI_0_Pin = 9;   // DI_0 : Pin 9, "D", TIMER_30, iKey = 4    

int g_iPreviousKey;
long g_lCountReservation;
bool g_bStartReservation;
long g_lLimitTime;              // to need 60,000 or more, I use long type instead of int.

void setup() 
{
  g_iPreviousKey = NO_COMMAND;
  g_lCountReservation = 0;
  g_bStartReservation = false;
  g_lLimitTime = 0;
  
  Serial.begin(9600);
  
  pinMode(DI_0_Pin, INPUT);  
  pinMode(DI_1_Pin, INPUT);
  pinMode(DI_2_Pin, INPUT);
  pinMode(DI_3_Pin, INPUT);
}

void loop() 
{
  int iKey = NO_COMMAND;
  int servoPin = 3;  
  
  int iOpreatingTime = 50;         // ms
  int iCenterAngle = 83;
  int iRotaionAngle = 25;  

  iKey = digitalRead(DI_3_Pin) * TURN_ON;    // DI_3 : Pin 6, "A", iKey = 1  

  // "if code" prevent double commands.
  if(iKey == NO_COMMAND)
    iKey = digitalRead(DI_2_Pin) * TURN_OFF;   // DI_2 : Pin 7, "B", iKey = 2   

  // "if code" prevent double commands.
  if(iKey == NO_COMMAND)
    iKey = digitalRead(DI_1_Pin) * TIMER_10;   // DI_1 : Pin 8, "C", iKey = 3  

  // "if code" prevent double commands.
  if(iKey == NO_COMMAND)
    iKey = digitalRead(DI_0_Pin) * TIMER_30;   // DI_0 : Pin 9, "D", iKey = 4  

  // This delay is timer base delay.
  delay(100);

  // Timer Operation
  if(g_lCountReservation > g_lLimitTime && g_bStartReservation == true)
  {
      g_lCountReservation = 0;
      g_bStartReservation = false;
      
      iKey = TURN_OFF;
  }  

  switch (iKey)
  {
    case TURN_ON:
      if(g_iPreviousKey != TURN_ON)   // To prevent double operatings
      {
        servo.attach(servoPin);
        delay(50);
        
        servo.write(iCenterAngle - iRotaionAngle);
        delay(iOpreatingTime);
        
        g_bStartReservation = false;
        g_lCountReservation = 0;      
      }
      break;
      
    case TURN_OFF:
      if(g_iPreviousKey != TURN_OFF)   // To prevent double operatings   
      { 
        servo.attach(servoPin);
        delay(50);
        
        servo.write(iCenterAngle + iRotaionAngle);
        delay(iOpreatingTime);
        
        g_bStartReservation = false;
        g_lCountReservation = 0;      
      }
      break;
      
    case TIMER_10:
      if(g_iPreviousKey != TIMER_10)   // To prevent double operatings   
      { 
        if(g_bStartReservation == false)
          g_bStartReservation = true;
          
        delay(50);        
        g_lLimitTime = 10 * 60 * 10;            // 10 : min, 60 : min → sec, 10 : sec → 100 ms       
      }
      break;
      
    case TIMER_30:
      if(g_iPreviousKey != TIMER_30)   // To prevent double operatings  
      {
        if(g_bStartReservation == false)
          g_bStartReservation = true;
          
        delay(50);        
        g_lLimitTime = 30 * 60 * 10;     // 30 : min, 60 : min → sec, 10 : sec → 100 ms   
      }        
      break;

    case NO_COMMAND:
      if(g_iPreviousKey == TURN_ON || g_iPreviousKey == TURN_OFF)
      {
        servo.write(iCenterAngle);
        delay(iOpreatingTime);
        
        servo.detach();               // sevo motor detaching is for saving power.
        delay(50);
      }
      break;
      
    default:
      break;
  }

  if(g_bStartReservation == true)
    g_lCountReservation ++;

  g_iPreviousKey = iKey;

//  Serial.print(iKey);
//  Serial.print(", ");    
//  Serial.println(g_lCountReservation);  
}
