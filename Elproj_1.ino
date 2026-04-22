// Elproj_1
// Kan lägga till och ta bort 10 sekunder från tiden. Inga negativa tider
// Efter timern är klar står det klar och lampa blinkar
// Visar återstående tid
// Kan växla till frågeläge skärm (gjort så man endast kan göra det i timerläge, ej i setup)
// Kan växla mellan frågor och svar
// Kan visa frågor sparade på minnet
// Kan visa tre frågor efter varandra
// Kan visa att den är klar efter 3 rätta svar (efter 3 tryck på alt A)
// Kan välja 3 frågor baserat på vikt.

// ska vara pullup istället för pulldown (active low??)
// undvik flickering på något sätt
// man kan inte skriva å, ä, ö och förmodligen inga fancy matte tecken. lös eller tänk på att använda en annan sorts skärm 
// den ska kunna veta när något behöver flera rader? eller man ska kunna ha frågor med fler än 16x2 tecken
// den resettar inte question_time efter tiden är ute. den resettar inte inställda tiden heller. sluta
// får den plats med 20 frågor?
// default tid på 25 min. max tid 10 timmar
// den ska autoöppna i början??  motor grejer jag behöver hjälp

// den ska kunna spara användarens input om rätt och fel ?? EEPROM SKITEN?


// fråga, svarsalternativ, rätt svar, vikt
// lätt = 1, mellan = 2, svår = 3, oprövad/fel = 4
// TODO
// 1. X skriv fler frågor + skriva vikter till alla (skriv till progmem)
// 2. X skriva algoritm som slumpar fram 3 styck från vikter
// 3. kolla så man kan få plats med alla frågor
// 4. correct ska bara visas om det är rätt svar
// 5. den ska fråga hur svår frågan var
// 6. den ska 
// 
// 







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
int index=0;

// MOTOR
const int motor_pin3 = 3;  
const int motor_pin4 = 4;

#define NUM_QUESTIONS 20
int selectedQuestions[3];
bool usedInSession[NUM_QUESTIONS];
int sessionIndex = 0;

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

const char q030[] PROGMEM = "3. this is a 32 ";
const char q031[] PROGMEM = "symbol3question?";
const char qab03[] PROGMEM = "A3.ans B3.ans   ";
const char qcd03[] PROGMEM = "C3.ans D3.ans   ";

const char q040[] PROGMEM = "4. this is a 32 ";
const char q041[] PROGMEM = "symbol4question?";
const char qab04[] PROGMEM = "A4.ans B4.ans   ";
const char qcd04[] PROGMEM = "C4.ans D4.ans   ";

const char q050[] PROGMEM = "5. this is a 32 ";
const char q051[] PROGMEM = "symbol5question?";
const char qab05[] PROGMEM = "A5.ans B5.ans   ";
const char qcd05[] PROGMEM = "C5.ans D5.ans   ";

const char q060[] PROGMEM = "6. this is a 32 ";
const char q061[] PROGMEM = "symbol6question?";
const char qab06[] PROGMEM = "A6.ans B6.ans   ";
const char qcd06[] PROGMEM = "C6.ans D6.ans   ";

const char q070[] PROGMEM = "7. this is a 32 ";
const char q071[] PROGMEM = "symbol7question?";
const char qab07[] PROGMEM = "A7.ans B7.ans   ";
const char qcd07[] PROGMEM = "C7.ans D7.ans   ";

const char q080[] PROGMEM = "8. this is a 32 ";
const char q081[] PROGMEM = "symbol8question?";
const char qab08[] PROGMEM = "A8.ans B8.ans   ";
const char qcd08[] PROGMEM = "C8.ans D8.ans   ";

const char q090[] PROGMEM = "9. this is a 32 ";
const char q091[] PROGMEM = "symbol9question?";
const char qab09[] PROGMEM = "A9.ans B9.ans   ";
const char qcd09[] PROGMEM = "C9.ans D9.ans   ";

const char q100[] PROGMEM = "10.this is a 32 ";
const char q101[] PROGMEM = "symbol10question";
const char qab10[] PROGMEM = "A10ans B10ans   ";
const char qcd10[] PROGMEM = "C10ans D10ans   ";

const char q110[] PROGMEM = "11. this is a 32 ";
const char q111[] PROGMEM = "symbo11question?";
const char qab11[] PROGMEM = "A11ans B11ans   ";
const char qcd11[] PROGMEM = "C4.ans D4.ans   ";

const char q120[] PROGMEM = "12 this is a 32 ";
const char q121[] PROGMEM = "symbol5question?";
const char qab12[] PROGMEM = "A12ans B5.ans   ";
const char qcd12[] PROGMEM = "C12ans D5.ans   ";

const char q130[] PROGMEM = "13this is a 32 ";
const char q131[] PROGMEM = "symbol6question?";
const char qab13[] PROGMEM = "A13ans B6.ans   ";
const char qcd13[] PROGMEM = "C13ans D6.ans   ";

const char q140[] PROGMEM = "14this is a 32 ";
const char q141[] PROGMEM = "symbol0question?";
const char qab14[] PROGMEM = "A14ans B0.ans   ";
const char qcd14[] PROGMEM = "C14ans D0.ans   ";

const char q150[] PROGMEM = "15this is a 32 ";
const char q151[] PROGMEM = "symbol1question?";
const char qab15[] PROGMEM = "A15ans B1.ans   ";
const char qcd15[] PROGMEM = "C15ans D1.ans   ";

const char q160[] PROGMEM = "16this is a 32 ";
const char q161[] PROGMEM = "symbol2question?";
const char qab16[] PROGMEM = "A16ans B2.ans   ";
const char qcd16[] PROGMEM = "C16ans D2.ans   ";

const char q170[] PROGMEM = "17this is a 32 ";
const char q171[] PROGMEM = "symbol3question?";
const char qab17[] PROGMEM = "A17ans B3.ans   ";
const char qcd17[] PROGMEM = "C17ans D3.ans   ";

const char q180[] PROGMEM = "18this is a 32 ";
const char q181[] PROGMEM = "symbol4question?";
const char qab18[] PROGMEM = "A18ans B4.ans   ";
const char qcd18[] PROGMEM = "C18ans D4.ans   ";

const char q190[] PROGMEM = "19this is a 32 ";
const char q191[] PROGMEM = "symbol5question?";
const char qab19[] PROGMEM = "A19ans B5.ans   ";
const char qcd19[] PROGMEM = "C19ans D5.ans   ";
//                             fråga: 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19
const uint8_t correctAns[] PROGMEM = {0, 2, 1, 2, 3, 0, 3, 2, 3, 1, 0, 2, 1, 2, 0, 2, 1, 0, 2, 3}; // rätt svar är 0.A, 1.C, 2.B
//                         fråga: 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19
const uint8_t weight[] PROGMEM = {1, 2, 3, 4, 1, 2, 3, 2, 1, 4, 2, 4, 1, 4, 2, 3, 4, 2, 1, 4};

// pointers
const char* const question_r0[] PROGMEM = {q000, q010, q020, q030, q040, q050, q060, q070, q080, q090, q100, q110, q120, q130, q140, q150, q160, q170, q180, q190};
const char* const question_r1[] PROGMEM = {q001, q011, q021, q031, q041, q051, q061, q071, q081, q091, q101, q111, q121, q131, q141, q151, q161, q171, q181, q191};

const char* const optionAB[] PROGMEM = {qab00, qab01, qab02, qab03, qab04, qab05, qab06, qab07, qab08, qab09, qab10, qab11, qab12, qab13, qab14, qab15, qab16, qab17, qab18, qab19};
const char* const optionCD[] PROGMEM = {qcd00, qcd01, qcd02, qcd03, qcd04, qcd05, qcd06, qcd07, qcd08, qcd09, qcd10, qcd11, qcd12, qcd13, qcd14, qcd15, qcd16, qcd17, qcd18, qcd19};

char buf[17];
////////////////////////////////////////////////////////////





void setup() {

    lcd.init();        
    lcd.backlight();
    //close();
    //open();

    
    pinMode(alt_1, INPUT);
    pinMode(alt_2, INPUT);
    pinMode(alt_3, INPUT);
    pinMode(alt_4, INPUT);
    pinMode(question_pin, INPUT);
    pinMode(led, OUTPUT);
    pinMode(led2, OUTPUT);

    randomSeed(analogRead(A0));

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
    case 3:
        feedback_mode();
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
        pick3Questions();
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
        //pick3Questions();
        delay(100);
    }


}

void timer_done(unsigned long elapsed, int timer) {
// visar att tiden är ute. stannar på den sidan i några sekunder innan den går tillbaka till setup mode
    if (elapsed > timer) {
        lcd.setCursor(0, 0);
        lcd.print("KLAR! :D           ");
        lcd.setCursor(0, 1);
        lcd.print("                   ");
        

    for (int i = 0; i < 2; i++) {
        digitalWrite(led, HIGH);
        delay(500);
        digitalWrite(led, LOW);
        delay(500);
    }
    

    // nästa fråga
    timer_on = 0;
    index = 0;
    question_time = 0;
    
    }

}


// FRÅGEMODE

void question_mode() {

    show_qna();
    choose_ans();
    
    if (digitalRead(question_pin) == HIGH) {
        timer_on = 1;       // go back to timer
        delay(200);
        return;
    }

    unsigned long elapsed = (millis() - startTime) / 1000;
    timer_done(elapsed, timer);
}

void printProgmem(const char* const* table, int index, int row) {
    strcpy_P(buf, (char*)pgm_read_word(&table[index]));
    lcd.setCursor(0, row);
    lcd.print(buf);

}

void printQue() {
    printProgmem(question_r0, index, 0);
    printProgmem(question_r1, index, 1);
}

void printAns() {
    printProgmem(optionAB, index, 0);
    printProgmem(optionCD, index, 1);
}


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
}

// jag vill göra så att olika saker händer vid rätt och fel svar

void choose_ans() { // kommer eventuellt kontrollera om trycket är rätt svar
    if (digitalRead(alt_1) == HIGH){
        question_time = 0; // för hur länge "correct!" ska visas sen
        timer_on = 3; // gå till feedback mode
    }
    
}

void feedback_mode() {
    // om rätt - säger correct, om fel - inget händer for now
    
    // correct visas i 2 sek
    if (question_time <= 20) {
        lcd.setCursor(0, 0);
        lcd.print("correct!           ");
        lcd.setCursor(0, 1);
        lcd.print("                   ");
        question_time++;
    }

    else {
        sessionIndex++;

        if (sessionIndex >= 3) {
            // session done
            timer_done(timer + 1, timer);
            timer_on = 0; // back to timer
            sessionIndex = 0;
            question_time = 0;
            index = 0;
            return;

        } 


        index = selectedQuestions[sessionIndex];
        question_time = 0;
        timer_on = 2;
        }
    }

void resetSession() {
    // kollar om den är med i sessionen
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        usedInSession[i] = false;
    }
}

int pickWeightedQuestion() {
    int total = 0;

    for (int i = 0; i < NUM_QUESTIONS; i++) {
        // ser till så att den inte redan är med i omgången. summerar ihop alla vikter
        if (!usedInSession[i]) {
            total += pgm_read_byte(&weight[i]);
        }
    }

   
    if (total == 0) return 0; // säkerhetsskull
    int r = random(total); // random nummer mellan 0-total

    for (int i = 0; i < NUM_QUESTIONS; i++) {
        // subtrahera vikt från random tal. frågan som random tal blir negativ är frågan som ska visas
        if (usedInSession[i]) continue;

                uint8_t w = pgm_read_byte(&weight[i]); 


        if (r < w) {
            usedInSession[i] = true;
            return i;
        }
        r -= w;
    }

    return 0;
}

void pick3Questions() {
    // välj 3 styck
    resetSession();

    for (int i = 0; i < 3; i++) {
        selectedQuestions[i] = pickWeightedQuestion();
    }

    sessionIndex = 0;
    index = selectedQuestions[0];
    question_time = 0;
}

void open(){

        digitalWrite(motor_pin3, HIGH);
        digitalWrite(motor_pin4, LOW);
        delay(6000);
        digitalWrite(motor_pin3, LOW);
}
void close(){

        digitalWrite(motor_pin4, HIGH);
        digitalWrite(motor_pin3, LOW);
        delay(6000);
        digitalWrite(motor_pin4, LOW);
}

// kollar om git fortfarande är uppfuckad