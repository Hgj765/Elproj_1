// testar kod for frågeläge
// man kan spara i Progmem och man kan kontrollera vad som visas





#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// spara frågor
// algoritm för att visa frågor

// varje objekt har
// fråga
// svarsalternativ
// korrekt svar
// användarens rating - det enda som ändras

// allt förutom användarens rating kan vara i progmem ist för eeprom

// progmem delen
// antag att varje fråga är 1 rad

int question_pin =11;
int alt_1=10;
int alt_2=9;
int alt_3=8;
int alt_4=7;
int led=2;
int led2=12;

const char q00[] PROGMEM = "0. 16 symbols???";
const char qa00[] PROGMEM = "A0.ans";
const char qb00[] PROGMEM = "B0.ans";
const char qc00[] PROGMEM = "C0.ans";
const char qd00[] PROGMEM = "D0.ans";

const char q01[] PROGMEM = "1. 16 symbols???";
const char qa01[] PROGMEM = "A1.ans";
const char qb01[] PROGMEM = "B1.ans";
const char qc01[] PROGMEM = "C1.ans";
const char qd01[] PROGMEM = "D1.ans";

const char q02[] PROGMEM = "2. 16 symbols???";
const char qa02[] PROGMEM = "A2.ans";
const char qb02[] PROGMEM = "B2.ans";
const char qc02[] PROGMEM = "C2.ans";
const char qd02[] PROGMEM = "D2.ans";

const uint8_t correctAns[] PROGMEM = {0, 2, 1}; // rätt svar är 0.A, 1.C, 2.B

// pointers
const char* const question[] PROGMEM = {q00, q01, q02};
const char* const optionA[] PROGMEM = {qa00, qa01, qa02};
const char* const optionB[] PROGMEM = {qb00, qb01, qb02};
const char* const optionC[] PROGMEM = {qc00, qc01, qc02};
const char* const optionD[] PROGMEM = {qd00, qd01, qd02};

char buf[17];



void setup() {

    lcd.init();
    lcd.backlight();

    pinMode(alt_1, INPUT);
    pinMode(alt_2, INPUT);
    pinMode(alt_3, INPUT);
    pinMode(alt_4, INPUT);

}

void loop() {
    printProgmem(question, 0, 0);
    printProgmem(optionA, 0, 1);

}

void printProgmem(const char* const* table, int index, int row) {
    strcpy_P(buf, (char*)pgm_read_word(&table[index]));
    lcd.setCursor(0, row);
    lcd.print(buf);
    
    //for (int i = strlen(buf); i < 16; i++) lcd.print(' ');
}






