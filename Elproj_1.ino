// Elproj_1
// Kan lägga till och ta bort 10 sekunder från tiden. Inga negativa tider
// något ska hända när timern är klar
// Visar återstående tid
// ska vara pullup istället för pulldown (active low??)


#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int timer = 60;
unsigned long startTime; // store start time;

/*
int question_pin =11;
int alt_1=10;
int alt_2=9;
int alt_3=8;
int alt_4=7;
*/

// omdefinierar pinsen för en inte funkar och jag kan inte fixa
int question_pin =11;
int alt_1=10;
int alt_2=9;
int alt_4=8;
int alt_5=7;

int timer_on =0;

void setup() {

    lcd.init();        
    lcd.backlight();

    /*
    pinMode(alt_1, INPUT);
    pinMode(alt_2, INPUT);
    pinMode(alt_3, INPUT);
    pinMode(alt_4, INPUT);
    pinMode(question_pin, INPUT);
    */

    pinMode(alt_1, INPUT);
    pinMode(alt_2, INPUT);
    pinMode(alt_4, INPUT);
    pinMode(alt_5, INPUT);
    pinMode(question_pin, INPUT);
  
}

void loop() {
    switch (timer_on)
    {
        case 0:
        set_timer();//if the timer isnt on then the setup timer is shown 
        
        break;
    case 1:
        timer_mode();//if the timer is on the timer/question in shown
        break;
    }
    
    
    delay(100);

}

void timer_mode() {

    unsigned long currentTime = millis();
  
    unsigned long elapsed = (currentTime - startTime) / 1000;
    unsigned long time_left = (elapsed >= timer) ? 0 : timer - elapsed; 
    // när passerad tid större än/lika med inställd tid = visa 0
    // när passerad tid < inställd tid = visa återstående tid

    int seconds = time_left % 60;
    int minutes = (time_left / 60) % 60;
    int hours = time_left / 3600;
    
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

    

    // notera att det ska vara pin 3 och 4 istället för 4 och 5 efter jag fixat kretsen
    if (digitalRead(alt_4) == HIGH) // ta bort 10 sek
    {
        timer -= 10;
        if (timer < 0) timer = 0;
        delay(100);
      
        }

    if (digitalRead(alt_5) == HIGH) // lägg till 10 sek
    {
        timer += 10;
        delay(100);

    }

    if (digitalRead(alt_1) == HIGH) // confirm tid
    {
        startTime = millis();
        timer_on=1;
        delay(100);

    }


}