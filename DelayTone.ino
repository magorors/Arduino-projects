// Köllő Magor Örs - 2020
// Technical University of Cluj - Napoca

#include "wiring_private.h"

const int NOTE_B0 =  31;
const int NOTE_C1 = 33;
const int NOTE_CS1 = 35;
const int NOTE_D1 = 37;
const int NOTE_DS1 = 39;
const int NOTE_E1 = 41;
const int NOTE_F1 = 44;
const int NOTE_FS1 = 46;
const int NOTE_G1 = 49;
const int NOTE_GS1 = 52;
const int NOTE_A1 = 55;
const int NOTE_AS1 = 58;
const int NOTE_B1 = 62;
const int NOTE_C2  = 65;
const int NOTE_CS2= 69;
const int NOTE_D2  =73;;
const int NOTE_DS2 =78;
const int NOTE_E2  =82;
const int NOTE_F2  =87;
const int NOTE_FS2 = 93;
const int NOTE_G2  = 98;
const int NOTE_GS2 = 104;
const int NOTE_A2 = 110;
const int NOTE_AS2 = 117;
const int NOTE_B2 = 123;;
const int NOTE_C3 = 131;
const int NOTE_CS3 = 139;
const int NOTE_D3 = 147;
const int NOTE_DS3 = 156;
const int NOTE_E3 = 165;
const int NOTE_F3 = 175;
const int NOTE_FS3 = 185;
const int NOTE_G3 = 196;
const int NOTE_GS3 = 208;
const int NOTE_A3 = 220;
const int NOTE_AS3 = 233;
const int NOTE_B3 = 247;
const int NOTE_C4 = 262;
const int NOTE_CS4 = 277;
const int NOTE_D4 = 294;
const int NOTE_DS4 = 311;
const int NOTE_E4 = 330;
const int NOTE_F4 = 349;
const int NOTE_FS4 = 370;
const int NOTE_G4 = 392;
const int NOTE_GS4 = 415;
const int NOTE_A4 = 440;
const int NOTE_AS4 = 466;
const int NOTE_B4 = 494;
const int NOTE_C5 = 523;
const int NOTE_CS5 = 554;
const int NOTE_D5 = 587;;
const int NOTE_DS5 = 622;
const int NOTE_E5 = 659;
const int NOTE_F5 = 698;
const int NOTE_FS5 = 740;;
const int NOTE_G5 = 784;
const int NOTE_GS5 = 831;
const int NOTE_A5 = 880;
const int NOTE_AS5 = 932;
const int NOTE_B5 =  988;
const int NOTE_C6 = 1047;
const int NOTE_CS6 = 1109;
const int NOTE_D6 = 1175;
const int NOTE_DS6 = 1245;
const int NOTE_E6 = 1319;
const int NOTE_F6 = 1397;
const int NOTE_FS6 = 1480;
const int NOTE_G6 = 1568;
const int NOTE_GS6 = 1661;
const int NOTE_A6 = 1760;
const int NOTE_AS6 = 1865;
const int NOTE_B6 = 1976;
const int NOTE_C7 = 2093;
const int NOTE_CS7 = 2217;
const int NOTE_D7 = 2349;
const int NOTE_DS7 = 2489;
const int NOTE_E7 = 2637;
const int NOTE_F7 = 2794;
const int NOTE_FS7 = 2960;
const int NOTE_G7 = 3136;
const int NOTE_GS7 = 3322;
const int NOTE_A7 = 3520;
const int NOTE_AS7 = 3729;
const int NOTE_B7 = 3951;
const int NOTE_C8 = 4186;
const int NOTE_CS8 = 4435;
const int NOTE_D8 = 4699;
const int NOTE_DS8 = 4978;
const int REST    =  0;

#define MICROSECONDS_PER_TIMER2_OVERFLOW (clockCyclesToMicroseconds(64 * 256))
#define MILLIS_INC (MICROSECONDS_PER_TIMER2_OVERFLOW / 1000)

#define FRACT_INC ((MICROSECONDS_PER_TIMER2_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

volatile unsigned long overflowcount = 0;
volatile unsigned long tmillis2 = 0;
static unsigned char timer2_fract = 0;     
 
const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;

//PB5 = a - 16
//PB4 = b - 15
//PB3 = c - 3
//PB2 = d - 2
//PB1 = e - 1
//PB0 = f - 18
//PD2 = g - 17
//GND = 14

long cnt;
uint8_t pin = PD6; //PORTD |= 0b01000000;
int8_t timer = 0;

void setup() {
    TCCR0A = 0;
    TCCR0B = 0;
    TCCR0A = (1<<1);
    TCCR0B = (1<<0);
    DDRD |= (1<<6);

    TCCR2A |= (1<<0);
    TCCR2A |= (1<<1);
    TCCR2B |= (1<<2);
    TIMSK2 |= (1<<0);

    #if defined(ADCSRA)
    sbi(ADCSRA, ADPS2);
    sbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);
    sbi(ADCSRA, ADEN);
    #endif  

    #if defined(UCSRB)
    UCSRB = 0;
    #elif defined(UCSR0B)
    UCSR0B = 0;
    #endif

    DDRB |= 0b00111111;
    DDRD |= (1<<2);

    extinterrupt();
    DDRD &= ~(1<<3); //External input pin
    PORTD |= (1<<3); //Set PULL-UP Resistance
}

void sound( unsigned int frequency, unsigned long duration)
{
  uint8_t prescalarbits = 0b001;
  long toggle_count = 0;
  uint32_t ocr = 0;
      //F_CPU = 16MHz
      ocr = F_CPU / frequency / 2 - 1;
      prescalarbits = 0b001; 
      if (ocr > 255)
      {
        ocr = F_CPU / frequency / 2 / 8 - 1;
        prescalarbits = 0b010;
        //We scale the variable by the prescalers from 0 to 1024
        //To have an OCR value between 0 - 255 
        //We "translate" to a scale 0-255
        if (ocr > 255)
        {
          ocr = F_CPU / frequency / 2 / 64 - 1;
          prescalarbits = timer == 0 ? 0b011 : 0b100;
 
          if (ocr > 255)
          {
            ocr = F_CPU / frequency / 2 / 256 - 1;
            prescalarbits = timer == 0 ? 0b100 : 0b110;
            if (ocr > 255)
            {
              // can't do any better than /1024
              ocr = F_CPU / frequency / 2 / 1024 - 1;
              prescalarbits = timer == 0 ? 0b101 : 0b111;
            }
          }
        }
      }
 
        TCCR0B = (TCCR0B & 0b11111000) | prescalarbits;
  
    if (duration > 0)
    {
      toggle_count = 2 * frequency * duration / 1000;
    }
    else
    {
      toggle_count = -1;
    }
    
    OCR0A = ocr;
    cnt = toggle_count;
    TIMSK0 |= (1<<1);
}

void nosound()
{
  TIMSK0 |= (1<<0);
  PORTD &= ~(1<<6);
  
}

 
ISR(TIMER0_COMPA_vect)
{
 
  if (cnt != 0)
  {
    PORTD ^= (1<<6);
    if (cnt > 0)
      cnt--;
  }
  else
  { 
    nosound();
  }
}


ISR(TIMER2_OVF_vect)
{
 
  unsigned long m = tmillis2;
  unsigned char f = timer2_fract;

  m += MILLIS_INC;
  f += FRACT_INC;
  if (f >= FRACT_MAX) {
    f -= FRACT_MAX;
    m += 1;
  }

  timer2_fract = f;
  tmillis2 = m;
  overflowcount++;
  
}

unsigned long millis2()
{
  unsigned long m;
  uint8_t oldSREG = SREG;
  cli();
  m = tmillis2;
  SREG = oldSREG;

  return m;
}

unsigned long micros2() {
  unsigned long m;
  uint8_t oldSREG = SREG, t;
  cli();
  m = overflowcount;
  t = TCNT2;
  if ((TIFR2 & _BV(TOV2)) && (t < 255))
    m++;
  SREG = oldSREG;
  
  return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}
void delay2(unsigned long ms)
{
  uint32_t start = micros2();

  while (ms > 0) {
    yield();
    while ( ms > 0 && (micros2() - start) >= 1000) {
      ms--;
      start += 1000;
    }
  }
}

void display2(int code)
{
PORTD &= 0b11111011;
PORTB &= 0b11000000;
switch(code)
{
  case 0: //DO
  PORTB |= 0b00011110;
  PORTD |= (1<<2);
  break;
  case 1: //RE
  PORTB |= 0b00101011;;
  PORTD |= (1<<2);
  break;
  case 2: //MI
  PORTB |= 0b00001010;
  PORTD |= (1<<2);
  break;
  case 3: //FA
  PORTB |= 0b00100011;
  PORTD |= (1<<2);
  break;
  case 4: //SOL
  PORTB |= 0b00101101;
  PORTD |= (1<<2);
  break;
  case 5: //LA
  PORTB |= 0b00000111;
  //PORTD |= (1<<2);
  break;
  case 6: //TI
  PORTB |= 0b00011000;
  PORTD |= (1<<2);
  break;
}
}

void extinterrupt() {


      //EICRA = (EICRA & ~((1 << ISC10) | (1 << ISC11))) | (RISING << ISC10 );
      EICRA |= (1<<ISC11);
      EIMSK |= (1 << INT1);

}

int menu = 1;

ISR(INT1_vect)
{
  menu= menu+1;
  if(menu>2)
   menu = 0;
}

void loop()
{
 switch(menu)
 {
  case 0: starwars();break;
  case 1: wewishyou();break;
  case 2 : GoT();break;
 }
}

int melody1[] = {

  // We Wish You a Merry Christmas
  
  NOTE_C5,4, //1
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4, REST, 8

};
int melody2[]= {
  NOTE_F5,2, NOTE_C5,4, //8 
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4,
};
int melody3[]= {
  NOTE_F5,4, NOTE_F5,4, NOTE_F5,4,//17
  NOTE_E5,2, NOTE_E5,4,
  NOTE_F5,4, NOTE_E5,4, NOTE_D5,4,
  NOTE_C5,2, NOTE_A5,4,
  NOTE_AS5,4, NOTE_A5,4, NOTE_G5,4,
  NOTE_C6,4, NOTE_C5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4, 
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
};
int melody4[] ={
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8, //27
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4,
  NOTE_F5,4, NOTE_F5,4, NOTE_F5,4,
  NOTE_E5,2, NOTE_E5,4,
  NOTE_F5,4, NOTE_E5,4, NOTE_D5,4,
};

int melody5[]={
  NOTE_C5,2, NOTE_A5,4,//36
  NOTE_AS5,4, NOTE_A5,4, NOTE_G5,4,
  NOTE_C6,4, NOTE_C5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4, 
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8, 
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
};
int melody6[]={
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,//45
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, NOTE_C5,4,
  NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
  NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
  NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
};
int melody7[]={
  NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, //53
  NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
  NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  NOTE_F5,2, REST,4
};

void wewishyou()
{
  for(int i =0; i <sizeof(melody1); i++)
  {
  beep(melody1[i],melody1[i+1]*75);
  i++;
  }
  //nosound();
  //delay2(1000);
  
}

void starwars()
{
  //Play first section
  firstSection();
 
  //Play second section
  secondSection();
 
  //Variant 1
  beep(f, 250);  
  beep(gS, 500);  
  beep(f, 350);  
  beep(a, 125);
  beep(cH, 500);
  beep(a, 375);  
  beep(cH, 125);
  beep(eH, 650);
 
  delay2(500);
 
  //Repeat second section
  secondSection();
 
  //Variant 2
  beep(f, 250);  
  beep(gS, 500);  
  beep(f, 375);  
  beep(cH, 125);
  beep(a, 500);  
  beep(f, 375);  
  beep(cH, 125);
  beep(a, 650);  
 
  delay2(650);
}
void beep(int note, int duration)
{
  //Play sound on buzzerPin
  sound(note, duration);
  if(note == c || note == cH || note == cSH)
  {
  display2(0);
  delay2(duration);
  }
  else if(note == d || note == dH || note == dSH)
  {
  display2(1);
  delay2(duration);
  }
  else if(note == e || note == eH)
  {
  display2(2);
  delay2(duration);
  }
  else if(note == f || note == fH || note == fSH)
  {
  display2(3);
  delay2(duration);
  }
  else if(note == g || note == gH || note == gSH || note == gS)
  {
  display2(4);
  delay2(duration);
  }
  else if(note == a || note == aH || note == aS)
  {
  display2(5);
  delay2(duration);
  }
  else if(note == b)
  {
  display2(6);
  delay2(duration);
  }
  nosound();
  delay2(50);
  //Increment counter
}
 
void firstSection()
{
  beep(a, 500);
  beep(a, 500);    
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);  
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay2(500);
 
  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500);  
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);
 
  delay2(500);
}
 
void secondSection()
{
  beep(aH, 500);
  beep(a, 300);
  beep(a, 150);
  beep(aH, 500);
  beep(gSH, 325);
  beep(gH, 175);
  beep(fSH, 125);
  beep(fH, 125);    
  beep(fSH, 250);
 
  delay2(325);
 
  beep(aS, 250);
  beep(dSH, 500);
  beep(dH, 325);  
  beep(cSH, 175);  
  beep(cH, 125);  
  beep(b, 125);  
  beep(cH, 250);  
 
  delay2(350);
}


void GoT() {
  for(int i=0; i<4; i++) {
    sound(NOTE_G4,500);
    delay2(500);
    nosound();

    sound( NOTE_C4,500);
    delay2(500);
    nosound();


    sound( NOTE_DS4,250);
    delay2(250);
    nosound();

    sound( NOTE_F4,250);
    delay2(250);
    nosound();
  }

  for(int i=0; i<4; i++){
    sound( NOTE_G4,500);
    delay2(500);
    nosound();

    sound( NOTE_C4,500);
    delay2(500);
    nosound();
    
    sound( NOTE_E4,250);
    delay2(250);
    nosound();

    sound( NOTE_F4,250);
    delay2(250);
    nosound();
  }

    sound( NOTE_G4,1500);
    delay2(1500);
    nosound();

    sound( NOTE_C4,1500);
    delay2(1500);
    nosound();

//
    sound( NOTE_E4,250);
    delay2(250);

    sound( NOTE_F4,250);
    delay2(250);
//
    sound( NOTE_G4,1000);
    delay2(1000);
    nosound();

    sound( NOTE_C4,1000);
    delay2(1000);

    sound( NOTE_DS4,250);
    delay2(250);
    nosound();

    sound( NOTE_F4,250);
    delay2(250);
    nosound();
    
  for(int i=0; i<3; i++) {
    
    sound( NOTE_D4,500);
    delay2(500);
    nosound();
    
    sound( NOTE_G3,500);
    delay2(500);
    nosound();

    sound( NOTE_AS3,250);
    delay2(250);
    nosound();

    sound( NOTE_C4,250);
    delay2(250);
    nosound();
    
  }
  


      sound( NOTE_D4,1500);
      delay2(1500);
      nosound();
      
      sound( NOTE_F4,1500);
      delay2(1500);
      nosound();

      sound( NOTE_AS3,1000);
      delay2(1000);
      nosound();

      sound( NOTE_DS4,250);
      delay2(250);
      nosound();

      sound( NOTE_D4,250);
      delay2(250);
      nosound();

      sound(NOTE_F4,1000);
      delay2(1000);
      nosound();
      
      sound( NOTE_AS3,1000);
      delay2(1000);
      nosound();

      sound( NOTE_DS4,250);
      delay2(250);
      nosound();

      sound( NOTE_D4,250);
      delay2(250);
      nosound();
      
      sound( NOTE_C4,500);
      delay2(500);
      nosound();

  for(int i=0; i<3; i++) {
      sound( NOTE_GS3,250);
      delay2(250);
      nosound();

      sound( NOTE_AS3,250);
      delay2(250);
      nosound();

      sound( NOTE_C4,500);
      delay2(500);
      nosound();
      
      sound( NOTE_F3,500);
      delay2(500);
      nosound();
  }

    sound(NOTE_G4,1000);
    delay2(1000);
    nosound();

    sound( NOTE_C4,1000);
    delay2(1000);
    nosound();

    sound( NOTE_DS4,250);
    delay2(250);
    nosound();

    sound( NOTE_F4,250);
    delay2(250);
    nosound();

    sound( NOTE_G4,1000);
    delay2(1000);
    nosound();

    sound( NOTE_C4,1000);
    delay2(1000);
    nosound();

    sound( NOTE_DS4,250);
    delay2(250);
    nosound();

    sound(NOTE_F4,250);
    delay2(250);
    nosound();

    sound(NOTE_D4,500);
    delay2(500);
    nosound();

  for(int i=0; i<4; i++) {
    sound(NOTE_G3,500);
    delay2(500);
    nosound();


    sound(NOTE_AS3,250);
    delay2(250);
    nosound();

    sound(NOTE_C4,250);
    delay2(250);
    nosound();

    sound(NOTE_D4,500);
    delay2(500);
    nosound();
  }
}
