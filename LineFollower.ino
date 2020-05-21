// Köllő Magor Örs
// PoliHack v10, 2019.
#include <IRremote.h>

const int pinOUT1 = A0;
const int pinOUT2 = A1;
const int pinOUT3 = A2;
const int pinOUT4 = A3;
const int pinOUT5 = A4;
int S1 = 0;
int S2 = 0;
int S3 = 0;
int S4 = 0;
int S5 = 0;


const int RECV_PIN = 9;
IRrecv irrecv(RECV_PIN);
decode_results results;

int EN_A = 6;      //Enable pin for first motor
int IN1 = 5;       //control pin for first motor
int IN2 = 4;       //control pin for first motor
int IN3 = 3;        //control pin for second motor
int IN4 = 2;        //control pin for second motor
int EN_B = 10;      //Enable pin for second motor
   
   
   void setup()
{
   irrecv.enableIRIn();
  irrecv.blink13(true);
   Serial.begin (9600);
   pinMode(EN_A, OUTPUT);
   pinMode(IN1, OUTPUT);  
   pinMode(IN2, OUTPUT);
   pinMode(IN3, OUTPUT);  
   pinMode(IN4, OUTPUT);
   pinMode(EN_B, OUTPUT);

  pinMode(pinOUT1,INPUT);
  pinMode(pinOUT2,INPUT);
  pinMode(pinOUT3,INPUT);
  pinMode(pinOUT4,INPUT);
  pinMode(pinOUT5,INPUT);

}
  
  
  void loop()
{
   analogWrite(EN_A, 90);
   analogWrite(EN_B, 90);

    S1 = digitalRead(pinOUT1);
    S2 = digitalRead(pinOUT2);
    S3 = digitalRead(pinOUT3);
    S4 = digitalRead(pinOUT4);
    S5 = digitalRead(pinOUT5);

    if((S1 == 0) && (S2 == 0) && (S3 == 0) && (S4 == 0) && (S5 == 0 ))
    {
      stop1();
      delay(50);
    }
    
    else if((S1 == 1) && (S2 == 1) && (S3 == 0) && (S4 == 1) && (S5 == 1 ))
    {
      forward();
      delay(10);
    }
    else if((S1 == 1) && (S2 == 1) && (S3 == 1) && (S4 == 1) && (S5 == 1 ))
    {
      left();
      delay(10);
    }
    else if((S1 == 1) && (S2 == 1) && (S3 == 0) && (S4 == 0) && (S5 == 1 ))
    {
      forward();
      delay(10);
    }
     else if((S1 == 1) && (S2 == 1) && (S3 == 1) && (S4 == 0) && (S5 == 1 ))
    {
      right();
      delay(10);
    }
     else if((S1 == 1) && (S2 == 1) && (S3 == 1) && (S4 == 0) && (S5 == 0 ))
    {
      right();
      delay(10);
    }
    
      else if((S1 == 1) && (S2 == 1) && (S3 == 0) && (S4 == 0) && (S5 == 0 ))
    {
      right();
      delay(10);
    }
     else if((S1 == 1) && (S2 == 0) && (S3 == 0) && (S4 == 0) && (S5 == 0 ))
    {
      right();
      delay(10);
    }
    else if((S1 == 1) && (S2 == 1) && (S3 == 1) && (S4 == 1) && (S5 == 0 ))
    {
      right();
      delay(10);
    }
    else if((S1 == 1) && (S2 == 0) && (S3 == 0) && (S4 == 1) && (S5 == 1 ))
    {
      forward();
      delay(10);
    }
    else if((S1 == 1) && (S2 == 0) && (S3 == 0) && (S4 == 0) && (S5 == 1 ))
    {
      forward();
      delay(10);
    }
    
    else if((S1 == 1) && (S2 == 0) && (S3 == 1) && (S4 == 1) && (S5 == 1 ))
    {
      left();
      delay(10);
    }
      else if((S1 == 0) && (S2 == 0) && (S3 == 1) && (S4 == 1) && (S5 == 1 ))
    {
      left();
      delay(10);
    }
    else if((S1 == 0) && (S2 == 0) && (S3 == 0) && (S4 == 1) && (S5 == 1 ))
    {
      left();
      delay(10);
    }
    else if((S1 == 0) && (S2 == 0) && (S3 == 0) && (S4 == 0) && (S5 == 1 ))
    {
      left();
      delay(10);
    }
    else if((S1 == 0) && (S2 == 1) && (S3 == 1) && (S4 == 1) && (S5 == 1 ))
    {
      left();
      delay(10);
    }
    stop1();
   
    /*if (irrecv.decode(&results)){

        switch(results.value){
          case 0xFF4AB5: //BACK
          back(); delay(100);      
         break;
         case 0xFF18E7: //FORWARD
         forward();delay(100);
       break;
        case 0xFF5AA5: //RIGHT
        right();delay(100);
        break;
        case 0xFF10EF: //LEFT
        left();delay(100);break;
        case 0xFF38C7: //STOP
        stop1();delay(100);break;
        }
         
        irrecv.resume(); 
    }
 */
  
}
void forward()
{
       digitalWrite(IN1, HIGH);
       digitalWrite(IN2, LOW);
       digitalWrite(IN3, HIGH);
       digitalWrite(IN4, LOW);
}

void back()
{
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
}

void right()
{
   digitalWrite(IN3, HIGH);
   digitalWrite(IN4, LOW);
}

void left()
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
}

void stop1()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}
