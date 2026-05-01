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
// Kan förstå när man trycker på rätt och fel svar
// Kan lägga till 5 min vid fel svar
// Kan fråga användaren om feedback
// Kan ändra sparad svårighetsgrad efter använderns input

// ska vara pullup istället för pulldown (active low??)
// undvik flickering på något sätt
// man kan inte skriva å, ä, ö och förmodligen inga fancy matte tecken. lös eller tänk på att använda en annan sorts skärm 
// den ska kunna veta när något behöver flera rader? eller man ska kunna ha frågor med fler än 16x2 tecken
// den resettar inte question_time efter tiden är ute. den resettar inte inställda tiden heller. sluta

// fråga, svarsalternativ, rätt svar, vikt
// lätt = 1, mellan = 2, svår = 3, oprövad/fel = 4
// TODO
// 1. X skriv fler frågor + skriva vikter till alla (skriv till progmem)
// 2. X skriva algoritm som slumpar fram 3 styck från vikter
// 3. X kolla så man kan få plats med alla frågor
// 4. X correct ska bara visas om det är rätt svar
// 5. X den ska fråga hur svår frågan var
// 6. X när användaren trycker på en knapp ska den uppdatera vikten med värdet av den knappen
// 7. X motor funktion
// 8. X LEDs
// 9. X printa vad användaren tryckte på
// 10. X byt till pull down
// 11. skriv riktiga frågor
// 12. säkerställ att det är möjligt att lägga till en ny sida av frågor om det behövs
// 13. snyggare å, ä, ö


// den säger done för tidigt
// 
 







#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int timer = 1500; // default tid i setup mode
#define TIMER_INTERVALL 300 // hur mycket man ökar och minskar timern med
#define OPEN_TIME 1200 // hur länge motorn öppnar 
#define CLOSE_TIME 1500 // hur länge motorn stänger
#define BLINKS 5 // hur ofta lampan ska blinka vid färdig timer (sekunder typ)
#define PAGE_TIME 20 // hur länge frågor och svar visas

#define AA byte(0)  // å
#define AE byte(1)  // ä
#define OE byte(2)  // ö

unsigned long startTime; // store start time;

int led1=12;
int led2=11;
int led3=10;
int alt_1=9;
int alt_2=8;
int alt_3=7;
int alt_4=6;
int question_pin =5;


int question_time = 0; 
int timer_on =0;
int index=0;
int chosenDifficulty = 0;
bool difficultyChosen = false;
bool timerStarted = false; 
bool sessionPicked = false;


// MOTOR
const int motor_pin3 = 3;  
const int motor_pin4 = 4;
bool AnsCorrect = false;
int correctSoFar = 0;

#define NUM_QUESTIONS 20
int selectedQuestions[3];
bool usedInSession[NUM_QUESTIONS];
int sessionIndex = 0;

#define EEPROM_START 0

///////////// FRÅGELÄGE ///////////////////////////////
const char q000[] PROGMEM = "C=600nF, L=6mH, ";
const char q001[] PROGMEM = "R=6ohm in series";
const char q000_p2[] PROGMEM = "For which angula"; // sida 2 test
const char q001_p2[] PROGMEM = "r freq is the   "; // sida 2 test
const char q000_p3[] PROGMEM = "impedence purely"; // sida 2 test
const char q001_p3[] PROGMEM = "real? (μrad/s)  ";
const char qab00[] PROGMEM = "A:60    B:360   ";
const char qcd00[] PROGMEM = "C:36    D:600   ";

const char q010[] PROGMEM = "Amplitude of    ";
const char q011[] PROGMEM = "current in serie";
const char q010_p2[] PROGMEM = "R=3Ω ωL=1Ω      "; // sida 2 test
const char q011_p2[] PROGMEM = "U(top)=9cos(ωt) "; // sida 2 test
const char q010_p3[] PROGMEM = "Answer in A     "; // sida 2 test
const char q011_p3[] PROGMEM = "                ";
const char qab01[] PROGMEM = "A:5.92  B:2.84  ";
const char qcd01[] PROGMEM = "C:2.45  D:3.34   ";

const char q020[] PROGMEM = "2. this is a    ";
const char q021[] PROGMEM = "symbol2question?";
const char qab02[] PROGMEM = "A2.ans B2.ans   ";
const char qcd02[] PROGMEM = "C2.ans D2.ans   ";

const char q030[] PROGMEM = "3. this is a    ";
const char q031[] PROGMEM = "symbol3question?";
const char qab03[] PROGMEM = "A3.ans B3.ans   ";
const char qcd03[] PROGMEM = "C3.ans D3.ans   ";

const char q040[] PROGMEM = "4. this is a    ";
const char q041[] PROGMEM = "symbol4question?";
const char qab04[] PROGMEM = "A4.ans B4.ans   ";
const char qcd04[] PROGMEM = "C4.ans D4.ans   ";

const char q050[] PROGMEM = "5. this is a    ";
const char q051[] PROGMEM = "symbol5question?";
const char qab05[] PROGMEM = "A5.ans B5.ans   ";
const char qcd05[] PROGMEM = "C5.ans D5.ans   ";

const char q060[] PROGMEM = "6. this is a    ";
const char q061[] PROGMEM = "symbol6question?";
const char qab06[] PROGMEM = "A6.ans B6.ans   ";
const char qcd06[] PROGMEM = "C6.ans D6.ans   ";

const char q070[] PROGMEM = "7. this is a    ";
const char q071[] PROGMEM = "symbol7question?";
const char qab07[] PROGMEM = "A7.ans B7.ans   ";
const char qcd07[] PROGMEM = "C7.ans D7.ans   ";

const char q080[] PROGMEM = "8. this is a    ";
const char q081[] PROGMEM = "symbol8question?";
const char qab08[] PROGMEM = "A8.ans B8.ans   ";
const char qcd08[] PROGMEM = "C8.ans D8.ans   ";

const char q090[] PROGMEM = "9. this is a    ";
const char q091[] PROGMEM = "symbol9question?";
const char qab09[] PROGMEM = "A9.ans B9.ans   ";
const char qcd09[] PROGMEM = "C9.ans D9.ans   ";

const char q100[] PROGMEM = "10.this is a    ";
const char q101[] PROGMEM = "symbol10question";
const char qab10[] PROGMEM = "A10ans B10ans   ";
const char qcd10[] PROGMEM = "C10ans D10ans   ";

const char q110[] PROGMEM = "11. this is a    ";
const char q111[] PROGMEM = "symbo11question?";
const char qab11[] PROGMEM = "A11ans B11ans   ";
const char qcd11[] PROGMEM = "C4.ans D4.ans   ";

const char q120[] PROGMEM = "12 this is a    ";
const char q121[] PROGMEM = "symbol5question?";
const char qab12[] PROGMEM = "A12ans B5.ans   ";
const char qcd12[] PROGMEM = "C12ans D5.ans   ";

const char q130[] PROGMEM = "13this is a    ";
const char q131[] PROGMEM = "symbol6question?";
const char qab13[] PROGMEM = "A13ans B6.ans   ";
const char qcd13[] PROGMEM = "C13ans D6.ans   ";

const char q140[] PROGMEM = "14this is a    ";
const char q141[] PROGMEM = "symbol0question?";
const char qab14[] PROGMEM = "A14ans B0.ans   ";
const char qcd14[] PROGMEM = "C14ans D0.ans   ";

const char q150[] PROGMEM = "15this is a    ";
const char q151[] PROGMEM = "symbol1question?";
const char qab15[] PROGMEM = "A15ans B1.ans   ";
const char qcd15[] PROGMEM = "C15ans D1.ans   ";

const char q160[] PROGMEM = "16this is a    ";
const char q161[] PROGMEM = "symbol2question?";
const char qab16[] PROGMEM = "A16ans B2.ans   ";
const char qcd16[] PROGMEM = "C16ans D2.ans   ";

const char q170[] PROGMEM = "17this is a    ";
const char q171[] PROGMEM = "symbol3question?";
const char qab17[] PROGMEM = "A17ans B3.ans   ";
const char qcd17[] PROGMEM = "C17ans D3.ans   ";

const char q180[] PROGMEM = "18this is a    ";
const char q181[] PROGMEM = "symbol4question?";
const char qab18[] PROGMEM = "A18ans B4.ans   ";
const char qcd18[] PROGMEM = "C18ans D4.ans   ";

const char q190[] PROGMEM = "19this is a    ";
const char q191[] PROGMEM = "symbol5question?";
const char qab19[] PROGMEM = "A19ans B5.ans   ";
const char qcd19[] PROGMEM = "C19ans D5.ans   ";

//                             fråga: 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19
const uint8_t correctAns[] PROGMEM = {0, 1, 1, 2, 3, 0, 3, 2, 3, 1, 0, 2, 1, 2, 0, 2, 1, 0, 2, 3}; // rätt svar är 0.A, 1.C, 2.B
////                         fråga: 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19
//const uint8_t weight[] PROGMEM = {1, 2, 3, 4, 1, 2, 3, 2, 1, 4, 2, 4, 1, 4, 2, 3, 4, 2, 1, 4};

//                           fråga: 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19
const uint8_t numPages[] PROGMEM = {3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
#define MAX_PAGES 3 // max antal sidor som en fråga kan ha

const char* const qp_r0[] PROGMEM = { 
        q000, q000_p2, q000_p3,
        q010, nullptr, nullptr,
        q020, nullptr, nullptr,
        q030, nullptr, nullptr,
        q040, nullptr, nullptr,
        q050, nullptr, nullptr,
        q060, nullptr, nullptr,
        q070, nullptr, nullptr,
        q080, nullptr, nullptr,
        q090, nullptr, nullptr,
        q100, nullptr, nullptr,
        q110, nullptr, nullptr,
        q120, nullptr, nullptr,
        q130, nullptr, nullptr,
        q140, nullptr, nullptr,
        q150, nullptr, nullptr,
        q160, nullptr, nullptr,
        q170, nullptr, nullptr,
        q180, nullptr, nullptr,
        q190, nullptr, nullptr,
};

const char* const qp_r1[] PROGMEM = { 
        q001, q001_p2, nullptr,
        q011, nullptr, nullptr,
        q021, nullptr, nullptr,
        q031, nullptr, nullptr,
        q041, nullptr, nullptr,
        q051, nullptr, nullptr,
        q061, nullptr, nullptr,
        q071, nullptr, nullptr,
        q081, nullptr, nullptr,
        q091, nullptr, nullptr,
        q101, nullptr, nullptr,
        q111, nullptr, nullptr,
        q121, nullptr, nullptr,
        q131, nullptr, nullptr,
        q141, nullptr, nullptr,
        q151, nullptr, nullptr,
        q161, nullptr, nullptr,
        q171, nullptr, nullptr,
        q181, nullptr, nullptr,
        q191, nullptr, nullptr,
};

// pointers
const char* const question_r0[] PROGMEM = {q000, q010, q020, q030, q040, q050, q060, q070, q080, q090, q100, q110, q120, q130, q140, q150, q160, q170, q180, q190};
const char* const question_r1[] PROGMEM = {q001, q011, q021, q031, q041, q051, q061, q071, q081, q091, q101, q111, q121, q131, q141, q151, q161, q171, q181, q191};

const char* const optionAB[] PROGMEM = {qab00, qab01, qab02, qab03, qab04, qab05, qab06, qab07, qab08, qab09, qab10, qab11, qab12, qab13, qab14, qab15, qab16, qab17, qab18, qab19};
const char* const optionCD[] PROGMEM = {qcd00, qcd01, qcd02, qcd03, qcd04, qcd05, qcd06, qcd07, qcd08, qcd09, qcd10, qcd11, qcd12, qcd13, qcd14, qcd15, qcd16, qcd17, qcd18, qcd19};


uint8_t weight[NUM_QUESTIONS];
int  currentPage = 0;
int  page_ticks  = 0;
char buf[17];
////////////////////////////////////////////////////////////


void loadWeights() {
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        uint8_t val = EEPROM.read(EEPROM_START + i);
        if (val < 1 || val > 4) {
            weight[i] = 4; // default till svår/otestad
            EEPROM.update(EEPROM_START + i, 4);
        } else {
            weight[i] = val;
        }
    }
}

void printChar(const char* s) {
    while (*s) {
        if (*s == '{') {
            s++;
            if (*s == 'a') lcd.write(AE);
            else if (*s == 'o') lcd.write(OE);
            else if (*s == 'A') lcd.write(AA);
            s++; // skip letter
            if (*s == '}') s++;
        } else {
            lcd.print(*s);
            s++;
        }
    }
}

/////////// SPECIELLA TECKEN ///////////
byte ae[8] = {
    B00000,
    B01010,
    B00000,
    B01110,
    B00001,
    B01111,
    B10001,
    B01111
};

byte oe[8] = {
    B00000,
    B01010,
    B00000,
    B01110,
    B10001,
    B10001,
    B10001,
    B01110
};

byte aa[8] = {
    B00000,
    B00100,
    B00000,
    B01110,
    B00001,
    B11111,
    B10001,
    B01111

};
/////////////////////////


void setup() {

    lcd.init();        
    lcd.backlight();
    lcd.createChar(0, aa);
    lcd.createChar(1, ae);
    lcd.createChar(2, oe);
    
    startTime = millis();
    
    pinMode(alt_1, INPUT_PULLUP);
    pinMode(alt_2, INPUT_PULLUP);
    pinMode(alt_3, INPUT_PULLUP);
    pinMode(alt_4, INPUT_PULLUP);
    pinMode(question_pin, INPUT_PULLUP);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);

    randomSeed(analogRead(A0));
    loadWeights();

    // MOTOR
    pinMode(motor_pin3, OUTPUT);
    pinMode(motor_pin4, OUTPUT);

    close();
    open();

  
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
    case 2:
        question_mode();
        break;
    case 3:
        feedback_mode();
        break;
    }

    updateLEDs();
    delay(100);

}

void updateLEDs() {
    switch (timer_on) {
        case 0: // setup
            digitalWrite(led1, HIGH);
            digitalWrite(led2, LOW);
            digitalWrite(led3, LOW);
            break;

        case 1: // timer
            digitalWrite(led1, LOW);
            digitalWrite(led2, LOW);
            digitalWrite(led3, LOW);
            break;

        case 2: // question
            digitalWrite(led1, LOW);
            digitalWrite(led2, HIGH);
            digitalWrite(led3, LOW);
            break;

        case 3: // feedback
            if (question_time <= 10 || !AnsCorrect) {
            digitalWrite(led1, LOW);
            digitalWrite(led2, LOW);
            digitalWrite(led3, LOW);
        } else {
            digitalWrite(led1, LOW);
            digitalWrite(led2, LOW);
            digitalWrite(led3, HIGH);
        }
    }
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
    lcd.print(" Left    ");
    
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
    lcd.print(" Total      ");

    if (digitalRead(question_pin) == LOW) { // pullup
        timer_on=2;
        if (!sessionPicked) {
        pick3Questions();
        sessionPicked = true;
        }
        delay(200);
        return;

    }
    
    if (timerStarted) {
    timer_done(elapsed, timer);
    }
}

void set_timer() {

    int seconds = timer % 60;
    int minutes = (timer / 60) % 60;
    int hours = timer / 3600;

    lcd.setCursor(0, 0);
    lcd.print("    ");
    lcd.print(hours < 10 ? "0" : "");
    lcd.print(hours);
    lcd.print(":");
    lcd.print(minutes < 10 ? "0" : "");
    lcd.print(minutes);
    lcd.print(":");
    lcd.print(seconds < 10 ? "0" : "");
    lcd.print(seconds);
    lcd.print("    ");


    lcd.setCursor(0, 1);
    lcd.print("   Set timer!     ");

    

    if (digitalRead(alt_2) == LOW) // ta bort 10 sek // pullup
    {
        if (timer > 0) timer -= TIMER_INTERVALL;
        delay(100);
      
    }

    if (digitalRead(alt_1) == LOW) // lägg till 10 sek // pullup
    {
        timer += TIMER_INTERVALL;
        delay(100);

    }

    if (digitalRead(alt_4) == LOW) // confirm tid // pullup
    {
        startTime = millis();
        timerStarted = true;
        timer_on=1;
        close();
        delay(100);

    }

    if (digitalRead(question_pin) == LOW && timer_on != 0) // gå till frågemode // pullup
    {
        close();
        timer_on=2;
        //pick3Questions();
        delay(100);
    }
}

void timer_done(unsigned long elapsed, int timer) {
// visar att tiden är ute. stannar på den sidan i några sekunder innan den går tillbaka till setup mode
    if (elapsed > timer) {
        lcd.setCursor(0, 0);
        lcd.print("Done!              ");
        lcd.setCursor(0, 1);
        lcd.print("                   ");
        

    for (int i = 0; i < BLINKS; i++) {
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, HIGH);

        delay(500);
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        digitalWrite(led3, LOW);
        delay(500);
    }
    
    open();

    // nästa fråga
    timer_on = 0;
    index = 0;
    question_time = 0;
    currentPage = 0;
    page_ticks = 0;
    }
}


// FRÅGEMODE

void question_mode() {
    show_qna();
    choose_ans();
    
    if (digitalRead(question_pin) == LOW) { // pullup
        timer_on = 1;      // go back to timer
        currentPage = 0;
        page_ticks = 0;
        delay(200);
        return;
    }
    if (timerStarted) {  
        unsigned long elapsed = (millis() - startTime) / 1000;
        timer_done(elapsed, timer);
    }
}


void printPageRow(const char* const* table, int qIdx, int pageIdx, int lcdRow)
{
    const char* str = (const char*)pgm_read_word(&table[qIdx * MAX_PAGES + pageIdx]);
    if (!str) return;  // nullptr → no page, skip

    strcpy_P(buf, str);
    lcd.setCursor(0, lcdRow);
    printChar(buf);   // handles {a}/{o}/{A} escape sequences
}

void show_qna() {
    uint8_t np = pgm_read_byte(&numPages[index]);   // question page count

    if (currentPage < np) {
        // visa frågor
        printPageRow(qp_r0, index, currentPage, 0);
        printPageRow(qp_r1, index, currentPage, 1);

        page_ticks++;
        if (page_ticks >= PAGE_TIME) {
            page_ticks = 0;
            currentPage++;          // gå vidare till nästa sida/svar
        }

    } else {
        // visa svarsalternativ
        strcpy_P(buf, (char*)pgm_read_word(&optionAB[index]));
        lcd.setCursor(0, 0); lcd.print(buf);

        strcpy_P(buf, (char*)pgm_read_word(&optionCD[index]));
        lcd.setCursor(0, 1); lcd.print(buf);

        page_ticks++;
        if (page_ticks >= PAGE_TIME) { // tbx till första sidan
            page_ticks  = 0;
            currentPage = 0;
        }
    }
}

void choose_ans() {
    uint8_t np = pgm_read_byte(&numPages[index]);

    // Don't register answers while question text pages are shown
    if (currentPage < np) return;

    uint8_t correct = pgm_read_byte(&correctAns[index]);
    int pressed = -1;

    if      (digitalRead(alt_1) == LOW) pressed = 0;
    else if (digitalRead(alt_2) == LOW) pressed = 1;
    else if (digitalRead(alt_3) == LOW) pressed = 2;
    else if (digitalRead(alt_4) == LOW) pressed = 3;

    if (pressed >= 0) {
        AnsCorrect = (correct == pressed);
        if (AnsCorrect) correctSoFar++;

        timer_on      = 3;
        question_time = 0;
        currentPage   = 0;      // ← reset for next question
        page_ticks    = 0;
    }
}



void feedback_mode() {
    if (question_time <= 10) {
        lcd.setCursor(0, 0);
        if (AnsCorrect) {
            lcd.print("Correct!         ");
            lcd.setCursor(0, 1);
            lcd.print("                   ");
            
        } else {
            lcd.print("Incorrect         ");
            lcd.setCursor(0, 1);
            lcd.print("                 ");
        }
        
        question_time++;
        return;
    }

    if (question_time > 10 && !difficultyChosen && AnsCorrect) {
        lcd.setCursor(0, 0);
        lcd.print("How hard was    ");
        lcd.setCursor(0, 1);
        lcd.print("this question?  ");
   
        if (digitalRead(alt_1) == LOW) { //pullup
            chosenDifficulty = 1;
            difficultyChosen = true;
        }
        else if (digitalRead(alt_2) == LOW) { //pullup
            chosenDifficulty = 2;
            difficultyChosen = true;
        }
        else if (digitalRead(alt_3) == LOW) { //pullup
            chosenDifficulty = 3;
            difficultyChosen = true;
        } else {
            return;
        }

        } else if (!AnsCorrect && !difficultyChosen) {
            weight[index] = 4;
            EEPROM.update(EEPROM_START + index, 4);
            difficultyChosen = true;
            return;
        }
    
    if (difficultyChosen) {

        if (AnsCorrect && chosenDifficulty > 0) {
            weight[index] = chosenDifficulty;
            EEPROM.update(EEPROM_START + index, chosenDifficulty);
        }

        lcd.setCursor(0, 0);
        if (chosenDifficulty == 1) lcd.print("Easy            ");
        else if (chosenDifficulty == 2) lcd.print("Medium          ");
        else if (chosenDifficulty == 3) lcd.print("Hard            ");

        lcd.setCursor(0,1);
        lcd.print("                  ");

        if (question_time < 20) {
            question_time++;
            return; 
        }
        difficultyChosen = false;
        chosenDifficulty = 0;
    }

    sessionIndex++; 

    if (sessionIndex >= 3) {
        sessionPicked = false;
        if (correctSoFar >= 3) {
            timer_done(timer + 1, timer);
            timer_on = 0;
        } else {
            timer += 300;  
            timer_on = 1;
        }
        sessionIndex = 0;
        index = 0;
        correctSoFar = 0;
        question_time = 0;
        currentPage = 0;
        page_ticks = 0;

        return;
    }

        index = selectedQuestions[sessionIndex];
        timer_on = 2;
        question_time = 0;
        currentPage = 0;
        page_ticks = 0;

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
            //total += pgm_read_byte(&weight[i]);
            total += weight[i];
        
        }
    }

   
    if (total == 0) return 0; // säkerhetsskull
    int r = random(total); // random nummer mellan 0-total

    for (int i = 0; i < NUM_QUESTIONS; i++) {
        // subtrahera vikt från random tal. frågan som random tal blir negativ är frågan som ska visas
        if (usedInSession[i]) continue;
        //uint8_t w = pgm_read_byte(&weight[i]); 
        uint8_t w = weight[i];

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

void updateWeightFromUser() {
    uint8_t newWeight = 0;

    if (digitalRead(alt_1) == LOW) newWeight = 1; // pullup
    else if (digitalRead(alt_2) == LOW) newWeight = 2; // pullup
    else if (digitalRead(alt_3) == LOW) newWeight = 3; // pullup
    else return;

    if (weight[index] != newWeight) {
        weight[index] = newWeight;
        EEPROM.update(EEPROM_START + index, newWeight);
    }
}

void open(){

        digitalWrite(motor_pin3, HIGH);
        digitalWrite(motor_pin4, LOW);
        delay(OPEN_TIME);
        digitalWrite(motor_pin3, LOW);
}
void close(){

        digitalWrite(motor_pin4, HIGH);
        digitalWrite(motor_pin3, LOW);
        delay(CLOSE_TIME);
        digitalWrite(motor_pin4, LOW);
}

