#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
int frog = 0;
int timer = 100;

int frog_pin =11;


int alt_1=10;
int alt_2=9;
int alt_3=8;
int alt_4=7;

int qna =0;
int anser =0;

// heloooeheleoeoehekeo//this is a usles comment and sould be removed//merge conflict
// hej
unsigned long startTime= millis(); // store start time;

void setup() {
  lcd.init();        
  lcd.backlight();
  
  
  
  pinMode(alt_1, INPUT);
  pinMode(alt_2, INPUT);
  pinMode(alt_3, INPUT);
  pinMode(alt_4, INPUT);
  pinMode(frog_pin, INPUT);
  
  
}

void loop() {
  
  
  if (digitalRead(frog_pin) == HIGH)
    {   
    if (frog == 1){frog =0;}
    else{frog =1;}
    }
    
  switch (frog)
  {
  case 0 frog:
    /* code */
    timer_mode();
  case 1 frog:
    /* code */
    froge_mode();
  
  default:
    break;
  }

  delay(100);
}

void froge_mode()
{
  if (100 >= qna >=50)
  {
    qna +=1;
    lcd.setCursor(0, 0);
    lcd.print("fancy question        ");
    lcd.setCursor(0, 1);
    lcd.print("whats the answer          ");
  }
  else if (qna < 50){
  lcd.setCursor(0, 0);
  lcd.print("Alternatives        ");

  lcd.setCursor(0, 1);
  lcd.print("A B  C  D          ");
  qna +=1;
  }

  else{qna =0;}
  if (digitalRead(alt_1) == HIGH)
  { 
    frog=0;      
  }

      }

void timer_mode(){
  

    
  if (digitalRead(alt_4) == HIGH)
    {timer += 10;
    /*
    if (frog == 1){frog =0;}
    else{frog =1;}
    */
      }
  
  
  unsigned long currentTime = millis();
  if ((currentTime - startTime)/1000 >= timer) { // 20 minutes
  
  lcd.setCursor(0, 0);
  lcd.print("Done                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  
  
    }
  else{
    
    
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
}