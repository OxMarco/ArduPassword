/*
 * Arduino password generator
 * Created: 01/01/2017
 * Author: grcasanova
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <LiquidCrystal.h>

#define PASSLENGTH 6

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 7);
char *alphabeth = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'?^&/%$£-+";
const long alphabethLength = 70;//sizeof(alphabeth);
char pass[PASSLENGTH+1];  //we add 1 extra for the NULL
volatile bool generatePass;
const byte interruptPin = 2;
const byte ledPin = 8;
int seconds;

void setup()
{
  // internal timer setup, courtesy of: http://www.engblaze.com/microcontroller-tutorial-avr-and-arduino-timer-interrupts/
  // initialize Timer1
  cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B

  // set compare match register to desired timer count:
  OCR1A = 15624;
  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  // enable global interrupts:
  sei();

  // enable watchdog to avoid uP hanging
  wdt_enable(WDTO_4S);
  // initialise internal pseudo-random number generator
  randomSeed(analogRead(A0));
  // initialise lcd row and lines
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Password:");
  // pin mode setup
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  // assign button interrupt
  attachInterrupt(digitalPinToInterrupt(interruptPin), newPass, CHANGE);
  // assign initial values to local vars
  seconds = 0;
  generatePass = 1;
}

void loop()
{
  // function that generates a new password
  if (generatePass == 1)
  {
    for (int n = 0; n < PASSLENGTH; n++)
    {
      pass[n] = alphabeth[random(0, alphabethLength)];
    }

    pass[PASSLENGTH] = '\0';

    // show password on the display
    lcd.setCursor(0, 1);
    lcd.print(pass);

    // erase password from memory
    memset(pass, '\0', PASSLENGTH);

    generatePass = 0;

    // reset the internal watchdog timer
    wdt_reset();

    // blink a led on successful password generation
    blink();
  }

  wdt_reset();
  delay(150);
}

// intercept button press event
void newPass()
{
  seconds = 0;
  generatePass = 1;
}

// timer to regenerate a new password every 10 seconds 
ISR(TIMER1_COMPA_vect)
{
  lcd.setCursor(15, 1);
  lcd.print(seconds);

  if (seconds == 9)
  {
    seconds = 0;
    generatePass = 1;
  }
  else
    seconds++;

  wdt_reset();
}

void blink()
{
  for(int i = 0; i<4; i++)
  {
    delay(200);
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
  }
}
