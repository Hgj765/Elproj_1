#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int timer = 100;
unsigned long startTime= millis(); // store start time;


void setup() {
  lcd.init();        
  lcd.backlight();
  
  
  
}

void loop() {
    timer_mode();

}

void timer_mode(){
    unsigned long currentTime = millis();
  
    unsigned long time_gone = (currentTime - startTime)/1000;

    int seconds = time_gone % 60;
    int minutes = (time_gone / 60) % 60;
    int hours = time_gone / 3600;
    
    lcd.setCursor(0, 0);
    lcd.print(hours);
    lcd.print("H: ");
    lcd.print(minutes);
    lcd.print("M: ");
    lcd.print(seconds);
    lcd.print("S Past");
    
    seconds = timer % 60;
    minutes = (timer / 60) % 60;
    hours = timer / 3600;
    
    lcd.setCursor(0, 1);
    lcd.print(hours);
    lcd.print("H: ");
    lcd.print(minutes);
    lcd.print("M: ");
    lcd.print(seconds);
    lcd.print("S All");
  
  
}