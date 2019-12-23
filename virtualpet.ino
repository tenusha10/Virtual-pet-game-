

// include the library code:
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <Time.h>
#include <TimerOne.h>
#include <TimeLib.h>
#include <EEPROM.h>
#include <math.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

int devstage, happiness, fullness,period = 1000;
int saved_Devstage, saved_Happiness, saved_Fullness;
float mins ,secs;
boolean npet=false, loadpet=false, pause=false,new_in_game_pet=false, off=false , newage=false;
unsigned long timer, timer2, elapsedTime=0, cpy_time=0, age=0, time_now = 0,saved_Age=0;
  int lvl,happy, full;
  unsigned long t ;

//defining functions
void newpet(){ //this function creates a new pet when called 
  devstage=0;
  happiness=2;
  fullness=3;
  age=0;
  }

void reducehappiness(){ // this function reduces the happiness by 1 
  if (happiness >0){
    happiness--;
    }
  }
void reducefullness(){ // function reduces fullness by 1 
  if(fullness>0){
    fullness--;
    }else{
      happiness=0;
      }
  }

void grow(){ //function grows pet by 1 
  if(devstage=0){
    devstage++;
    }
  }

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  lcd.print(":");
  if(digits < 10)
    lcd.print('0');
  lcd.print(digits);
}

void displayBlink(int blinks, int duration) //function to blink the screen 
{
  lcd.setBacklight(RED);
  while(blinks--)
  {
    lcd.noDisplay();
    time_now = millis();
    while(millis() < time_now + duration){
        //wait approx. [period] ms
    }
    lcd.display();
      time_now = millis();
    while(millis() < time_now + duration){
        //wait approx. [period] ms
    }
  }
}


void copyStats(){ //function makes stores all the cuurent values of the given variables when called 
  lvl=devstage;
  happy=happiness;
  full=fullness;
  t=age;
  }
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16,2);
  Serial.begin(9600);
  lcd.setBacklight(WHITE);
  lcd.setCursor(0,0);
  lcd.print("LOAD:UP");
  lcd.setCursor(0,7);
  lcd.print("NEW PET:DOWN");
  
  while(npet!=true && loadpet!=true){ //eneters a loop until user selects an option
  uint8_t buttons = lcd.readButtons();
  if (buttons){
    if(buttons & BUTTON_UP){
      loadpet=true; // flag used to dtermined what user selected 
      }
    if (buttons & BUTTON_DOWN){
      npet=true;
      }
}
  }


if(loadpet==true){
  if (EEPROM.read(1)==0){ // EEPROM is read for previously stored data
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("NO SAVE FOUND"); // if EEPROM is empty a new pet is started instead 
    lcd.setCursor(0,1);
    lcd.print("Starting new pet..");
    time_now = millis();
    while(millis() < time_now + period){ // this function is used for delay instead of delay()
        //wait approx. [period] ms
    }
    newpet(); // newpet created
    }else{
  devstage=EEPROM.read(1); // otherwise data in EEPROM read to variables
  fullness=EEPROM.read(2);
  happiness=EEPROM.read(3);
  saved_Age=EEPROM.read(4);
    }
}
if(npet==true){ //new pet created 
  newpet();
      }
 lcd.clear();
 timer2=millis(); // time is copied to see how long user spent in starting menu 
}






void loop() {
   //main code here, to run repeatedly:
  if (new_in_game_pet==true){ // this method is set to true if user starts a new pet from in-game menu
    newpet();
    new_in_game_pet=false;
    newage=true;
    }
  uint8_t buttons = lcd.readButtons(); // read buttons on lcd sheild 
    lcd.setCursor(0, 0);
    timer=(millis()-timer2); //timer varaible stores the time since game is started(millis) and the diffrence in time spent in menu is reduced 
    timer=timer/1000; //milliseconds turned to seconds 
    age=(((timer+saved_Age)-elapsedTime)-cpy_time); //pets age is determined by reducing time spent in menus
    
   lcd.print("DEV:");//displays the stats
   lcd.print(devstage);
   
   lcd.setCursor(7,0);
   lcd.print("Happy:");
   lcd.print(happiness);
   
   lcd.setCursor(0,7);
   lcd.print("Full:");
   lcd.print(fullness);

   lcd.setCursor(7,7);
   lcd.print("Age");
   mins=floorf(age/60); //displays age in minutes:seconds format
   secs=age%60;
   printDigits(mins);
   printDigits(round(secs));

   if (age==5){ // when age reaches 5 devstage is set to 1
    devstage=1;
    lcd.setBacklight(GREEN); //sets lED colour to green
    }
    
   if(devstage>=1){
    lcd.setBacklight(GREEN);
    Timer1.initialize(7000000); //every 7 seconds happiness is  reduced by 1 
    Timer1.attachInterrupt(reducehappiness);
    
    Timer1.initialize(11000000); // every 11 seconds fullness is reduced by 1
    Timer1.attachInterrupt(reducefullness);
    }
    if(fullness==0){
      happiness=0; 
      }
    if(happiness==0 && fullness==0){
      displayBlink(1,100); // when all happiness and fullness are 0 display is set to red and blinked
      }
      
   if ((buttons)&&(devstage>=1)) { //user can only press buttons if devstage is greater than 1 
    lcd.clear();
    lcd.setCursor(0,0);
    if (buttons & BUTTON_UP) { // feeds the pet
      if (fullness<3){
        lcd.setBacklight(VIOLET);
        fullness++;
        lcd.print("YAY FOOD");
        time_now = millis();
        while(millis() < time_now + period){
        //wait approx. [period] ms
        }
        lcd.clear();
        }
      if (fullness==3){
        lcd.setBacklight(RED);
        fullness++;
        happiness=0;
        lcd.print("OVERFULL");
            time_now = millis();
    while(millis() < time_now + period){
        //wait approx. [period] ms
    }
        lcd.clear();
        }
         
         lcd.clear();
    }
    if (buttons & BUTTON_DOWN) { //plays with pet
      if((happiness<2) &&( fullness>=2)){
        lcd.setBacklight(YELLOW);
        happiness++;
        lcd.print("PLAY");
            time_now = millis();
    while(millis() < time_now + period){
        //wait approx. [period] ms
    }
        lcd.clear();
        }else{
          lcd.setBacklight(RED);
          lcd.print("Cant play now :(");
              time_now = millis();
    while(millis() < time_now + period){
        //wait approx. [period] ms
    }
          }
       lcd.clear();
    }
    if (buttons & BUTTON_LEFT) { //grows pet
      if(devstage==2){
        lcd.clear();
        lcd.setBacklight(TEAL);
        lcd.print("I Cant't grow anymore");
        }
      if((age>=35)&&(happiness>=1)&&(fullness>=3)){
        lcd.clear();
        lcd.setBacklight(TEAL);
        lcd.print("YAY IM GROWING");
           time_now = millis();
    while(millis() < time_now + period){
        //wait approx. [period] ms
    }
        devstage=2;
        }else{
          lcd.print("Im still young! ;)");
    time_now = millis();
    while(millis() < time_now + period){
        //wait approx. [period] ms
    }
          }
      
      lcd.clear();
    }
    if (buttons & BUTTON_RIGHT) {
    }
    if (buttons & BUTTON_SELECT) {
      copyStats(); 
      pause=true; //pauses simulation
      lcd.clear();
    }

    while(pause!=false){ // game enters menu 
      elapsedTime=(millis()/1000)-timer; // tracks how long user was in menu
      uint8_t buttons = lcd.readButtons();
      lcd.setCursor(0,0); // menu displayed
      lcd.print("ESC:UP");
      lcd.setCursor(0,7);
      lcd.print("NPET:DWN");
      lcd.setCursor(7,0);
      lcd.print("SAVE:RHT");
      lcd.setCursor(9,7);
      lcd.print("DEL:LFT");
      lcd.setBacklight(VIOLET);
      if (buttons & BUTTON_UP) { //escpae button goes back to game 
        pause=false;
        lcd.clear();
        happiness=happy;
        devstage=lvl;
        fullness=full;
        }
      if (buttons & BUTTON_RIGHT){ // saves the pet stats and  switch display off
        //for(i=0;i<4;i++){
          EEPROM.write(1,devstage);
          EEPROM.write(2,fullness);
          EEPROM.write(3,happiness);
          EEPROM.write(4,age);
          pause=false;
          lcd.clear();
          off=true;
         // }
        }
       if (buttons & BUTTON_LEFT){ //delete stats from memory
          for (int i = 0 ; i < EEPROM.length() ; i++) {
            EEPROM.write(i, 0);
            }
            pause=false;
        lcd.clear();
        }
         if (buttons & BUTTON_DOWN){ //starts new pet
        new_in_game_pet=true;
        pause=false;
        lcd.clear();
        lcd.setBacklight(WHITE);
        cpy_time=timer-elapsedTime;
        
        loop();
        }
       }
       
      }

      while(off==true){ // turns display off 
        lcd.setBacklight(WHITE);
        lcd.noDisplay();
        }
      }
      
