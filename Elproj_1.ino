#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int timer = 100;
unsigned long startTime; // store start time;

//hehehe
int question_pin =11;
int alt_1=10;
int alt_2=9;
int alt_3=8;
int alt_4=7;

int timer_on =0;

void setup() {

    startTime = millis();

    lcd.init();        
    lcd.backlight();
    
    pinMode(alt_1, INPUT);
    pinMode(alt_2, INPUT);
    pinMode(alt_3, INPUT);
    pinMode(alt_4, INPUT);
    pinMode(question_pin, INPUT);
  
}

void loop() {
    switch (timer_on)
    {
        case 0:
        set_timer();//if the timer isnt on then the setup timer is sown 
        
        break;
    case 1:
        timer_mode();//if the timer is on the timer/question in shown
        break;
    }
    
    
    delay(100);

}

void timer_mode() {

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

void set_timer() {

    int seconds = timer % 60;
    int minutes = (timer / 60) % 60;
    int hours = timer / 3600;

    lcd.setCursor(0, 0);
    lcd.print(hours);
    lcd.print("H: ");
    lcd.print(minutes);
    lcd.print("M: ");
    lcd.print(seconds);
    lcd.print("S All");

    lcd.setCursor(0, 1);
    lcd.print("-10 +10 c    ");

    


    if (digitalRead(alt_4) == HIGH) // ta bort 10 sek
    {
        timer -= 10;
        delay(100);
      
        }

    if (digitalRead(alt_3) == HIGH) // ta bort 10 sek
    {
        timer += 10;
        if (timer < 0) timer = 0;
        delay(100);

    }
    if (digitalRead(alt_1) == HIGH) // ta bort 10 sek
    {
        timer_on=1;
        delay(100);

    }
}