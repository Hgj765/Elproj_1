
// Elproj_1.ino
// Erika Erwin och Daniel Wickleus
// Kod till K.E.T. för Elproj 1

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int timer = 1500; // default tid i setup mode
#define TIMER_INTERVALL 300 // hur mycket man ökar och minskar timern med
#define OPEN_TIME 1200 // hur länge motorn öppnar 
#define CLOSE_TIME 1500 // hur länge motorn stänger
#define BLINKS 5 // hur ofta lampan ska blinka vid färdig timer (sekunder typ)
#define PAGE_TIME 30 // hur länge frågor och svar visas

#define MICRO byte(0) 
#define OHM byte(1)  
#define OMEGA byte(2)  
#define PI byte(3)
#define PM byte(4)

unsigned long startTime; 

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


   const char q000[] PROGMEM = "C=600nF, L=6mH, ";      ///
   const char q001[] PROGMEM = "R=6{O} in series    ";
const char q000_p2[] PROGMEM = "For which angula"; // sida 2 test
const char q001_p2[] PROGMEM = "r freq is the   "; // sida 2 test
const char q000_p3[] PROGMEM = "impedence purely"; // sida 2 test
const char q001_p3[] PROGMEM = "real? ({u}rad/s)  ";
  const char qab00[] PROGMEM = "A:60    B:360   ";
  const char qcd00[] PROGMEM = "C:36    D:600   ";

   const char q010[] PROGMEM = "Amplitude of    ";
   const char q011[] PROGMEM = "current in serie";  ///
const char q010_p2[] PROGMEM = "R=3{O} {o}L=1{O}      "; 
const char q011_p2[] PROGMEM = "U(top)=9cos({o}t) "; 
const char q010_p3[] PROGMEM = "Answer in A     "; 
const char q011_p3[] PROGMEM = "                ";
  const char qab01[] PROGMEM = "A:5.92  B:2.84  ";
  const char qcd01[] PROGMEM = "C:2.45  D:3.34   ";


   const char q020[] PROGMEM = "What is the     "; // hela  
   const char q021[] PROGMEM = "equivalent resis";  
const char q020_p2[] PROGMEM = "tance of 4 10{O}   "; 
const char q021_p2[] PROGMEM = "resistors in     ";  
const char q020_p3[] PROGMEM = "parallell?       "; 
const char q021_p3[] PROGMEM = "                 ";
  const char qab02[] PROGMEM = "A:3.3{O}  B:1{O}     ";
  const char qcd02[] PROGMEM = "C:0.1{O}  D:2.5{O}   ";


   const char q030[] PROGMEM = "An OP-amp has a ";  
   const char q031[] PROGMEM = "{m}12V supply and";
const char q030_p2[] PROGMEM = "gain of 5. At   "; 
const char q031_p2[] PROGMEM = "what input      "; 
const char q030_p3[] PROGMEM = "signal does it  "; 
const char q031_p3[] PROGMEM = "saturate?       ";
  const char qab03[] PROGMEM = "A:{m}1.3V B:{m}12V";
  const char qcd03[] PROGMEM = "C:{m}2V  D:{m}2.4V";


   const char q040[] PROGMEM = "What is the stan";
   const char q041[] PROGMEM = "dard power grid ";
const char q040_p2[] PROGMEM = "frequency in    "; 
const char q041_p2[] PROGMEM = "Sweden?         "; 
//const char q040_p3[] PROGMEM = "                "; 
//const char q041_p3[] PROGMEM = "                ";
  const char qab04[] PROGMEM = "A:230Hz B:60Hz  ";
  const char qcd04[] PROGMEM = "C:50Hz  D:16.7Hz";

   const char q050[] PROGMEM = "3 parallell capa";      
   const char q051[] PROGMEM = "citors of 100{u}F,";
const char q050_p2[] PROGMEM = "200{u}F and 300{u}F "; 
const char q051_p2[] PROGMEM = "have what total "; 
const char q050_p3[] PROGMEM = "capacitance?    "; 
const char q051_p3[] PROGMEM = "                ";
  const char qab05[] PROGMEM = "A:54.5{u}F B:200{u}F"; 
  const char qcd05[] PROGMEM = "C:600{u}F D:6000{u}F";

  
   const char q060[] PROGMEM = "What is the     ";
   const char q061[] PROGMEM = "formula for     ";
const char q060_p2[] PROGMEM = "bandwidth in a  "; 
const char q061_p2[] PROGMEM = "resonance       "; 
const char q060_p3[] PROGMEM = "circuit?        "; 
const char q061_p3[] PROGMEM = "                ";
  const char qab06[] PROGMEM = "A:{o}2-{o}1 B:Q*L/C "; 
  const char qcd06[] PROGMEM = "C:E/Z   D:W/Z   ";


   const char q070[] PROGMEM = "What is the     ";
   const char q071[] PROGMEM = "charge of an    ";
const char q070_p2[] PROGMEM = "electron (in aC)"; 
const char q071_p2[] PROGMEM = "                "; 
//const char q070_p3[] PROGMEM = "              "; 
//const char q071_p3[] PROGMEM = "              ";
  const char qab07[] PROGMEM = "A:+3.20 B:-10.02";
  const char qcd07[] PROGMEM = "C:+1.99 D:-0.16 ";

   const char q080[] PROGMEM = "At the voltage  ";
   const char q081[] PROGMEM = "20V with the    ";
const char q080_p2[] PROGMEM = "current 2A, what"; 
const char q081_p2[] PROGMEM = "is the power?   "; 
//const char q080_p3[] PROGMEM = "              "; 
//const char q081_p3[] PROGMEM = "              ";
  const char qab08[] PROGMEM = "A:30W   B:40W   ";
  const char qcd08[] PROGMEM = "C:3KW   D:42W   ";

  
   const char q090[] PROGMEM = "Does the source ";
   const char q091[] PROGMEM = "deactivation    ";
const char q090_p2[] PROGMEM = "method require a"; 
const char q091_p2[] PROGMEM = "dependent source"; 
const char q090_p3[] PROGMEM = "in a circuit to "; 
const char q091_p3[] PROGMEM = "be applicable?  ";
  const char qab09[] PROGMEM = "A:Yes   B:Unsure";
  const char qcd09[] PROGMEM = "C:most time D:No";

  
   const char q100[] PROGMEM = "Which method is ";
   const char q101[] PROGMEM = "better, nodal   ";
const char q100_p2[] PROGMEM = "analysis or mesh"; 
const char q101_p2[] PROGMEM = "analysis?       "; 
//const char q100_p3[] PROGMEM = "                "; 
//const char q101_p3[] PROGMEM = "                ";
  const char qab10[] PROGMEM = "A:Nodal B:Equal ";
  const char qcd10[] PROGMEM = "C:Mesh D:Unknown";

   const char q110[] PROGMEM = "KCL states that ";
   const char q111[] PROGMEM = "charge is always";
const char q110_p2[] PROGMEM = "constant in a   "; 
const char q111_p2[] PROGMEM = "closed system,  ";
const char q110_p3[] PROGMEM = "is that true?   "; 
const char q111_p3[] PROGMEM = "                ";
  const char qab11[] PROGMEM = "A:Yes   B:Often ";
  const char qcd11[] PROGMEM = "C:No    D:Seldom";

   const char q120[] PROGMEM = "Thevenin's theor";
   const char q121[] PROGMEM = "em simplifies a ";
const char q120_p2[] PROGMEM = "system to a volt"; 
const char q121_p2[] PROGMEM = "age source in   "; 
const char q120_p3[] PROGMEM = "series with a   "; 
const char q121_p3[] PROGMEM = "capacitor       ";
  const char qab12[] PROGMEM = "A:No B:Sometimes";
  const char qcd12[] PROGMEM = "C:Yes D:Depends ";

   const char q130[] PROGMEM = "A transformer   ";
   const char q131[] PROGMEM = "steps up voltage";
const char q130_p2[] PROGMEM = "from 10V to 100V"; 
const char q131_p2[] PROGMEM = "What happens to "; 
const char q130_p3[] PROGMEM = "the current?    "; 
const char q131_p3[] PROGMEM = "                ";
  const char qab13[] PROGMEM = "A:Up   B:Down   ";
  const char qcd13[] PROGMEM = "C:Remain D:Zero ";

   const char q140[] PROGMEM = "R=100{O} and      ";    
   const char q141[] PROGMEM = "1/({o}C)=100{O} in  ";
const char q140_p2[] PROGMEM = "series. What is ";
const char q141_p2[] PROGMEM = "the circuits    ";
const char q140_p3[] PROGMEM = "admittance in   ";
const char q141_p3[] PROGMEM = "the unit mS?    ";
  const char qab14[] PROGMEM = "A:5      B:4    ";
  const char qcd14[] PROGMEM = "C:10     D:7    ";

   const char q150[] PROGMEM = "What is the     ";
   const char q151[] PROGMEM = "power factor if ";
const char q150_p2[] PROGMEM = "the active power";
const char q151_p2[] PROGMEM = "is 800W and the ";
const char q150_p3[] PROGMEM = "apparent power  ";
const char q151_p3[] PROGMEM = "is 1000VA?      ";
  const char qab15[] PROGMEM = "A:0.6 B:0.1     ";
  const char qcd15[] PROGMEM = "C:0.8 D:1.25    ";

   const char q160[] PROGMEM = "What is the     ";
   const char q161[] PROGMEM = "apparent power  ";
const char q160_p2[] PROGMEM = "at 600W active  ";
const char q161_p2[] PROGMEM = "and 800 VAr     ";
const char q160_p3[] PROGMEM = "reactive power? ";
const char q161_p3[] PROGMEM = "                ";
  const char qab16[] PROGMEM = "A:203VA B:1000VA";
  const char qcd16[] PROGMEM = "C:400VA D:200VA ";

   const char q170[] PROGMEM = "u1=100cos({o}t+arc";    
   const char q171[] PROGMEM = "tan(3/4)) V     ";
const char q170_p2[] PROGMEM = "u2=50cos({o}t+arct"; 
const char q171_p2[] PROGMEM = "an(4/3)) V      "; 
const char q170_p3[] PROGMEM = "What is U(top)  "; 
const char q171_p3[] PROGMEM = "of u1+u2?       ";
  const char qab17[] PROGMEM = "A:150V   B:149V ";
  const char qcd17[] PROGMEM = "C:75V    D:90V  ";

   const char q180[] PROGMEM = "What is the     ";
   const char q181[] PROGMEM = "line-to-line    ";
const char q180_p2[] PROGMEM = "voltage in a    ";
const char q181_p2[] PROGMEM = "standard Swedish";
const char q180_p3[] PROGMEM = "three-phase     ";
const char q181_p3[] PROGMEM = "system?         ";
  const char qab18[] PROGMEM = "A:230V B:400V   ";
  const char qcd18[] PROGMEM = "C:200V D:150V   ";

   const char q190[] PROGMEM = "What mathematica";
   const char q191[] PROGMEM = "l relationship  ";
const char q190_p2[] PROGMEM = "exists between  "; 
const char q191_p2[] PROGMEM = "admittance and  "; 
const char q190_p3[] PROGMEM = "impedance?      "; 
const char q191_p3[] PROGMEM = "                ";
  const char qab19[] PROGMEM = "A:Y=Z+1 B:Y=Z*Z ";
  const char qcd19[] PROGMEM = "C:Y=1/Z D:Y=Z/k ";

//                             fråga: 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19
const uint8_t correctAns[] PROGMEM = {0, 1, 3, 3, 2, 2, 0, 3, 1, 3, 1, 0, 0, 1, 0, 2, 1, 2, 1, 2}; // rätt svar är 0.A, 1.C, 2.B

//                           fråga: 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19
const uint8_t numPages[] PROGMEM = {3, 3, 3, 3, 2, 3, 3, 3, 2, 3, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3};
#define MAX_PAGES 3 // max antal sidor som en fråga kan ha

const char* const qp_r0[] PROGMEM = { 
        q000, q000_p2, q000_p3,
        q010, q010_p2, q010_p3,
        q020, q020_p2, q020_p3,
        q030, q030_p2, q030_p3,
        q040, q040_p2, nullptr,
        q050, q050_p2, q050_p3,
        q060, q060_p2, q060_p3,
        q070, q060_p2, nullptr,
        q080, q080_p2, nullptr,
        q090, q090_p2, q090_p3,
        q100, q100_p2, nullptr,
        q110, q110_p2, q110_p3,
        q120, q120_p2, q120_p3,
        q130, q130_p2, q130_p3,
        q140, q140_p2, q140_p3,
        q150, q150_p2, q150_p3,
        q160, q160_p2, q160_p3,
        q170, q170_p2, q170_p3,
        q180, q180_p2, q180_p3,
        q190, q190_p2, q190_p3,   
};

const char* const qp_r1[] PROGMEM = { 
        q001, q001_p2, q001_p3,
        q011, q011_p2, q011_p3,
        q021, q021_p2, q021_p3,
        q031, q031_p2, q031_p3,
        q041, q041_p2, nullptr,
        q051, q051_p2, q051_p3,
        q061, q061_p2, q061_p3,
        q071, q071_p2, nullptr,
        q081, q081_p2, nullptr,
        q091, q091_p2, q091_p3,
        q101, q101_p2, nullptr,
        q111, q111_p2, q111_p3,
        q121, q121_p2, q121_p3,
        q131, q131_p2, q131_p3,
        q141, q141_p2, q141_p3,
        q151, q151_p2, q151_p3,
        q161, q161_p2, q161_p3,
        q171, q171_p2, q171_p3,
        q181, q181_p2, q181_p3,
        q191, q191_p2, q191_p3,
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


void load_weights() {
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

void print_char(const char* s) {
    while (*s) {
        if (*s == '{') {
            s++;
            if (*s == 'u') lcd.write(MICRO);
            else if (*s == 'O') lcd.write(OHM);
            else if (*s == 'o') lcd.write(OMEGA);
            else if (*s == 'p') lcd.write(PI);
            else if (*s == 'm') lcd.write(PM);

            s++; // skip letter
            if (*s == '}') s++;
        } else {
            lcd.print(*s);
            s++;
        }
    }
}

/////////// SPECIELLA TECKEN ///////////

byte micro[8] = {
  B00000,
  B00000,
  B10010,
  B10010,
  B10010,
  B11110,
  B10001,
  B10000
};

byte ohm[8] = {
  B00000,
  B00000,
  B01110,
  B10001,
  B10001,
  B01010,
  B11011,
  B00000
};

byte omega[8] = {
  B00000,
  B00000,
  B00000,
  B01010,
  B10001,
  B10101,
  B01010,
  B00000
};

byte pi[8] = {
  B00000,
  B00000,
  B11111,
  B01010,
  B01010,
  B01010,
  B01001,
  B00000
};

byte pm[8] = {
  B00000,
  B00000,
  B00100,
  B01110,
  B00100,
  B00000,
  B01110,
  B00000
};

/////////////////////////


void setup() {

    lcd.init();        
    lcd.backlight();
    lcd.createChar(0, micro);
    lcd.createChar(1, ohm);
    lcd.createChar(2, omega);
    lcd.createChar(3, pi);
    lcd.createChar(4, pm);
    lcd.home();

    
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
    load_weights();

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

    update_LEDs();
    delay(100);

}

void update_LEDs() {
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
        pick_3_questions();
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


void print_page_row(const char* const* table, int qIdx, int pageIdx, int lcdRow)
{
    const char* str = (const char*)pgm_read_word(&table[qIdx * MAX_PAGES + pageIdx]);
    if (!str) return;  

    strcpy_P(buf, str);
    lcd.setCursor(0, lcdRow);
    print_char(buf);  
}

void show_qna() {
    uint8_t np = pgm_read_byte(&numPages[index]);   // question page count

    if (currentPage < np) {
        // visa frågor
        print_page_row(qp_r0, index, currentPage, 0);
        print_page_row(qp_r1, index, currentPage, 1);

        page_ticks++;
        if (page_ticks >= PAGE_TIME) {
            page_ticks = 0;
            currentPage++;          // gå vidare till nästa sida/svar
        }

    } else {
        // visa svarsalternativ
        strcpy_P(buf, (char*)pgm_read_word(&optionAB[index]));
        lcd.setCursor(0, 0); 
        print_char(buf);

        strcpy_P(buf, (char*)pgm_read_word(&optionCD[index]));
        lcd.setCursor(0, 1); 
        print_char(buf);

        page_ticks++;
        if (page_ticks >= PAGE_TIME) { // tbx till första sidan
            page_ticks  = 0;
            currentPage = 0;
        }
    }
}

void choose_ans() {
    uint8_t np = pgm_read_byte(&numPages[index]);

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
        currentPage   = 0;      
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
         
void reset_session() {
    // kollar om den är med i sessionen
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        usedInSession[i] = false;
    }
}

int pick_weighted_question() {
    int total = 0;
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (!usedInSession[i]) {
            total += weight[i];
        }
    }
    if (total == 0) return 0;
    int r = random(total); 
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (usedInSession[i]) continue;
        uint8_t w = weight[i];
        if (r < w) {
            usedInSession[i] = true;
            return i;
        }
        r -= w;
    }
    return 0;
}


void pick_3_questions() {
    // välj 3 styck
    reset_session();

    for (int i = 0; i < 3; i++) {
        selectedQuestions[i] = pick_weighted_question();
    }

    sessionIndex = 0;
    index = selectedQuestions[0];
    question_time = 0;
}

void update_weight_from_user() { 
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

