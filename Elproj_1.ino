// Elproj_1
// Kan lägga till och ta bort 10 sekunder från tiden. Inga negativa tider
// Efter timern är klar står det klar och lampa blinkar
// Visar återstående tid
// Kan växla till frågeläge skärm (gjort så man endast kan göra det i timerläge, ej i setup)
// Kan växla mellan frågor och svar

// jag gjorde en funktion för vad som händer när timern är klar
// daniel får skriva in outputsen till motorn

// ska vara pullup istället för pulldown (active low??)
// undvik flickering på något sätt
// man kan inte skriva å, ä, ö och förmodligen inga fancy matte tecken. lös eller tänk på att använda en annan sorts skärm 
// kanske behöver byta språk till engelska men vore synd

// den ska kunna veta när något behöver flera rader? eller man ska kunna ha frågor med fler än 16x2 tecken
// något ska hända när man trycker på rätt svar. en annan fråga ska komma upp
// den ska kunna dra frågor från frågebank




#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int timer = 0; // om man börjar på 0 får man inte konstiga negativa tider
unsigned long startTime; // store start time;


int question_pin =11;
int alt_1=10;
int alt_2=9;
int alt_3=8;
int alt_4=7;
int led=2;
int led2=12;

int question_time = 0; 
int timer_on =0;

// MOTOR
const int motor_pin3 = 3;  
const int motor_pin4 = 4;



///////////// FRÅGELÄGE ///////////////////////////////
const char q000[] PROGMEM = "0. this is a 32 ";
const char q001[] PROGMEM = "symbol0question?";
const char qab00[] PROGMEM = "A0.ans B0.ans   ";
const char qcd00[] PROGMEM = "C0.ans D0.ans   ";

const char q010[] PROGMEM = "1. this is a 32 ";
const char q011[] PROGMEM = "symbol1question?";
const char qab01[] PROGMEM = "A1.ans B1.ans   ";
const char qcd01[] PROGMEM = "C1.ans D1.ans   ";

const char q020[] PROGMEM = "2. this is a 32 ";
const char q021[] PROGMEM = "symbol2question?";
const char qab02[] PROGMEM = "A2.ans B2.ans   ";
const char qcd02[] PROGMEM = "C2.ans D2.ans   ";

const uint8_t correctAns[] PROGMEM = {0, 2, 1}; // rätt svar är 0.A, 1.C, 2.B

// pointers
const char* const question_r0[] PROGMEM = {q000, q010, q020};
const char* const question_r1[] PROGMEM = {q001, q011, q021};

const char* const optionAB[] PROGMEM = {qab00, qab01, qab02};
const char* const optionCD[] PROGMEM = {qcd00, qcd01, qcd02};

char buf[17];
////////////////////////////////////////////////////////////




void setup() {

    lcd.init();        
    lcd.backlight();

    
    pinMode(alt_1, INPUT);
    pinMode(alt_2, INPUT);
    pinMode(alt_3, INPUT);
    pinMode(alt_4, INPUT);
    pinMode(question_pin, INPUT);
    pinMode(led, OUTPUT);
    pinMode(led2, OUTPUT);

    // MOTOR
    pinMode(motor_pin3, OUTPUT);
    pinMode(motor_pin4, OUTPUT);
  
}



// TIMERMODE

void loop() {
    switch (timer_on)
    {
        case 0:
        set_timer();//if the timer isnt on then the setup timer is shown 
        
        break;
    case 1:
        timer_mode();//if the timer is on the timer/question in shown
    
        break;

    case 2:
        question_mode();
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
    lcd.print(hours < 10 ? "0" : "");
    lcd.print(hours);
    lcd.print(":");
    lcd.print(minutes < 10 ? "0" : "");
    lcd.print(minutes);
    lcd.print(":");
    lcd.print(seconds < 10 ? "0" : "");
    lcd.print(seconds);
    lcd.print(" Kvar    ");
    
    seconds = timer % 60;
    minutes = (timer / 60) % 60;
    hours = timer / 3600;
    
    lcd.setCursor(0, 1);
    lcd.print(hours < 10 ? "0" : "");
    lcd.print(hours);
    lcd.print(":");
    lcd.print(minutes < 10 ? "0" : "");
    lcd.print(minutes);
    lcd.print(":");
    lcd.print(seconds < 10 ? "0" : "");
    lcd.print(seconds);
    lcd.print(" Totalt   ");

    if (digitalRead(question_pin) == HIGH) {
        timer_on=2;
        delay(200);
        return;

    }
    
    timer_done(elapsed, timer);
}

void set_timer() {

    int seconds = timer % 60;
    int minutes = (timer / 60) % 60;
    int hours = timer / 3600;

    lcd.setCursor(0, 0);
    lcd.print(hours < 10 ? "0" : "");
    lcd.print(hours);
    lcd.print(":");
    lcd.print(minutes < 10 ? "0" : "");
    lcd.print(minutes);
    lcd.print(":");
    lcd.print(seconds < 10 ? "0" : "");
    lcd.print(seconds);
    lcd.print(" Totalt   ");

    lcd.setCursor(0, 1);
    lcd.print("OK -10 +10       ");

    

    if (digitalRead(alt_2) == HIGH) // ta bort 10 sek
    {
        if (timer > 0) timer -= 10;
        delay(100);
      
        }

    if (digitalRead(alt_3) == HIGH) // lägg till 10 sek
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

    if (digitalRead(question_pin) == HIGH && timer_on != 0) // gå till frågemode
    {
        timer_on=2;
        delay(100);
    }


}

void timer_done(unsigned long elapsed, int timer) {
// visar att tiden är ute. stannar på den sidan i 8s innan den går tillbaka till setup mode
    if (elapsed > timer) {
        lcd.setCursor(0, 0);
        lcd.print("KLAR! :D           ");
        lcd.setCursor(0, 1);
        lcd.print("                   ");


    for (int i = 0; i < 5; i++) {
        digitalWrite(led, HIGH);
        delay(500);
        digitalWrite(led, LOW);
        delay(500);
    }
    
    timer_on = 0;
    
    }


}


// FRÅGEMODE

void question_mode() {
    /* lcd.setCursor(0, 0);
    lcd.print("question               ");
    lcd.setCursor(0, 1);
    lcd.print("abcd                "); */

    show_qna();
    
    if (digitalRead(question_pin) == HIGH) {
        timer_on = 1;       // go back to timer
        delay(200);
        return;
    }

    unsigned long elapsed = (millis() - startTime) / 1000;
    timer_done(elapsed, timer);
}

void printQue() {
    printProgmem(question_r0, 0, 0);
    printProgmem(question_r1, 0, 1);
}

void printAns() {
    printProgmem(optionAB, 0, 0);
    printProgmem(optionCD, 0, 1);
}

void printProgmem(const char* const* table, int index, int row) {
    strcpy_P(buf, (char*)pgm_read_word(&table[index]));
    lcd.setCursor(0, row);
    lcd.print(buf);

}

// växlar mellan frågor och svar- vill att den ska kunna visa mer än 1 sida av frågor och svar, vill att något ska hända efter input
void show_qna() {
    if (question_time <50)
    {
        question_time +=1;
        printQue();
    }
    else if (question_time >= 50 && question_time <= 100 )
    {
        printAns();
        question_time +=1;
    }

  else{question_time = 0;}

// problem- den resettar inte question_time efter tiden är ute. den resettar inte inställda tiden heller

}