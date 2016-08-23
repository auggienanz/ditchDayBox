/*
   Ditch Day 2016
   Auggie Nanz
   Mission Impossible/The Resistance

*/

#include "countDownTimer.h"
#include <Keypad.h>
#include <LiquidCrystal.h>

#define NUM_WIRES 6
const int wirePins[NUM_WIRES] = {8, 9, 10, 11, 12, 13};

// States 0 to NUM_WIRES-1 are the wire states. Rest are number of button presses
int currentWireState[NUM_WIRES];
const int solutionWireState[NUM_WIRES] = {1, 0, 0, 1, 0, 1};

// Sequence of buttons
bool hasPushedGreen = false;
bool hasPushedBlue = false;
bool hasPushedYellow = false;
bool hasPushedBlack = false;
bool hasPushedWhite = false;

// Buttons
const byte numRows = 2;
const byte numCols = 4;

// keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols] =
{
  {'b', 'B', 'g', 'r'},
  {' ', 'y', 'w', ' '}
};

// Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {5, 7}; // Rows 0 to 1
byte colPins[numCols] = {2, 3, 4, 6}; // Columns 0 to 3

// Initializes an instance of the Keypad class
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(A5, A4, A3, A2, A1, A0);

void setup()
{
  // Set the wire pins to be input with internal pull up resistors
  for (int i = 0; i < NUM_WIRES; i++)
    pinMode(wirePins[i], INPUT_PULLUP);

  // Initialize the current state
  resetState();

  SetTimer(0, 15, 0); // Set timer for 15 minutes
  StartTimer(); // Start the timer

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ARMED");
  Serial.begin(9600);
  delay(10);
  Serial.println("ARMED");
}

void loop()
{
  if ( !CountDownTimer() )
  {
    // Timer has gone off. Game over :(
    lcd.clear();
    lcd.print("GAME OVER :(");
    while(1) {}
  }

  updateButtonState();
  updateWireState();
  updateTimerDisplay();

  if (checkSolution())
  {
    // problem solved, need to do something about it
    lcd.clear();
    lcd.print("DISARMED");
    lcd.setCursor(0,1);
    lcd.print("The secret is 42");
    Serial.println("DISARMED");
    delay((unsigned long) 5 * 60 * 1000);
  }
  delay(1);
}

bool checkSolution()
{
  // Check if the correct wires have been cut
  for (int i = 0; i < NUM_WIRES; i++)
  {
    if (currentWireState[i] != solutionWireState[i]) {
      return false;
    }
  }
  // Check if the right buttons have been pressed
  if ( hasPushedGreen == true && hasPushedBlue == true && hasPushedBlack == true && hasPushedWhite == true && hasPushedYellow == true)
    return true;
  return false;
}

void resetState()
{
  for (int i = 0; i < NUM_WIRES; i++)
    currentWireState[i] = 0;
  hasPushedGreen = false;
  hasPushedBlue = false;
  hasPushedYellow = false;
  hasPushedWhite = false;
  hasPushedBlack = false;
}

void updateButtonState()
{
  char currentButton = myKeypad.getKey();
  if (currentButton == NO_KEY)
    return;

  if (checkGreen(currentButton)) {
    Serial.println("Pushed Green");
    return;
  } else if (checkBlue(currentButton)) {
    Serial.println("Pushed Blue");
    return;
  } else if (checkYellow(currentButton)) {
    Serial.println("Pushed Yellow");
    return;
  }  else if (checkBlack(currentButton)) {
    Serial.println("Pushed Black");
    return;
  }  else if (checkWhite(currentButton)) {
    Serial.println("Pushed White");
    return;
  } else {
    resetState();
    lcd.clear();
    lcd.print("BOOM!!!!");
    delay(750);
    lcd.setCursor(0, 1);
    lcd.print("lol, jk. 322 322");
    delay(1500);
    lcd.clear();
    lcd.print("ARMED");
    return;
  }
}

bool checkGreen(char key)
{
  if (!hasPushedGreen && key == 'g') {
    hasPushedGreen = true;
    return true;
  }
  return false;
}

bool checkBlue(char key)
{
  if (hasPushedGreen && !hasPushedBlue && key == 'b') {
    hasPushedBlue = true;
    return true;
  }
  return false;
}

bool checkYellow(char key)
{
  if (hasPushedBlue && !hasPushedYellow && key == 'y') {
    hasPushedYellow = true;
    return true;
  }
  return false;
}

bool checkBlack(char key)
{
  if (hasPushedYellow && !hasPushedBlack && key == 'B') {
    hasPushedBlack = true;
    return true;
  }
  return false;
}

bool checkWhite(char key)
{
  if (hasPushedBlack && !hasPushedWhite && key == 'w') {
    hasPushedWhite = true;
    return true;
  }
  return false;
}

void updateTimerDisplay()
{
  // Update the timer display
  lcd.clear();
  lcd.print("ARMED");
  lcd.setCursor(0, 1);
  lcd.print(ShowMinutes());
  lcd.print(':');
  lcd.print(ShowSeconds());
  lcd.print(':');
  int ms = 1000 - ShowMilliSeconds();
  ms = (ms == 1000 ? 999 : ms);
  lcd.print(ms/100);
}

void updateWireState()
{
  for (int i = 0; i < NUM_WIRES; i++) {
    currentWireState[i] = digitalRead(wirePins[i]);
    if (currentWireState[i] == 1 && solutionWireState[i] == 0) {
      lcd.clear();
      lcd.print("BOOM!!!");
      delay(750);
      lcd.setCursor(0, 1);
      lcd.print("Replace Wire Cut");
      while (digitalRead(wirePins[i]) == 1)
        delay(10);
      lcd.clear();
      lcd.print("ARMED");
    }
  }
}
