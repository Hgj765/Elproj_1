// testar kod for frågeläge
// man kan spara i Progmem och man kan kontrollera vad som visas

// TODO
// kunna visa 3 svar, gå framåt om man svarar rätt
// en fråga ska visas. svar ska visas. svar ska väljas. vilket ska få nästa fråga att visas
// om användaren har fel eller rätt ska sparas

// en till skärm med svårighetsuppskattning ska visas





#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// varje objekt har
// fråga
// svarsalternativ
// korrekt svar
// användarens rating - det enda som ändras

// allt förutom användarens rating kan vara i progmem istället för eeprom

int question_pin =11;
int alt_1=10;
int alt_2=9;
int alt_3=8;
int alt_4=7;
int led=2;
int led2=12;

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



void setup() {

    lcd.init();
    lcd.backlight();

    pinMode(alt_1, INPUT);
    pinMode(alt_2, INPUT);
    pinMode(alt_3, INPUT);
    pinMode(alt_4, INPUT);

}

void loop() {
    printAns();

}

void printQue() {
    printProgmem(question_r0, 0, 0);
    printProgmem(question_r1, 0, 1);
}

void printAns() {
    printProgmem(optionAB, 0, 0)
    printProgmem(optionCD, 0, 1)
}

void printProgmem(const char* const* table, int index, int row) {
    strcpy_P(buf, (char*)pgm_read_word(&table[index]));
    lcd.setCursor(0, row);
    lcd.print(buf);

}






