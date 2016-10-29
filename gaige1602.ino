#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#define LOG_PERIOD 15000  //Logging period in milliseconds, recommended value 15000-60000.
#define MAX_PERIOD 60000  //Maximum logging period without modifying this sketch

unsigned long counts;     //variable for GM Tube events
unsigned long cpm;        //variable for CPM
unsigned int multiplier;  //variable for calculation CPM in this sketch
unsigned long previousMillis;  //variable for time measurement
float usv;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void tube_impulse() {      //subprocedure for capturing events from Geiger Kit
  counts++;
}

void setup()
{
  counts = 0;
  cpm = 0;
  multiplier = MAX_PERIOD / LOG_PERIOD;      //calculating multiplier, depend on your log period
  //Serial.begin(9600);
  attachInterrupt(0, tube_impulse, FALLING); //define external interrupts

  //////////////////
  lcd.init();                     
  lcd.backlight();
  lcd.setCursor(5, 0);
  lcd.print("Boot...");
   lcd.setCursor(0, 1);
  for(int i=0;i<16;i++)
  {
  lcd.write(0xff);
  delay(250);
  }
}

void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > LOG_PERIOD) {
    previousMillis = currentMillis;
    cpm = counts * multiplier;
    usv = float(cpm) / 151;
    lcd.clear();
    lcd.print("CPM=");
    lcd.print(cpm);
    lcd.setCursor(0, 1);
    lcd.print(usv);
    lcd.print(" uSv/h");
    counts = 0;
    if (usv >= 10)
    {
      lcd.setCursor(9, 0);
      lcd.print("Danger!");
      delay(10);
    }
    else if (usv < 10 && usv >= 0.52)
      {
        lcd.setCursor(10, 0);
        lcd.print("Unsafe");
        delay(10);
      }
      else if (usv < 0.52)
        {
          lcd.setCursor(10, 0);
          lcd.print("Safety");
          delay(10);
        }
      }
}
