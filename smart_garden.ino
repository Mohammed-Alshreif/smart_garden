#define place 2
#define pump 3
#define soilmoister A0
#define rainsensor A1
#define light 5

#include "DHT.h"
#define DHTPIN 4 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#include <Wire.h>
#include <DS1307.h>
DS1307 rtc;

int temp =0, hum=0 ,soilhum=0,rain=0,desplaynum=0;
bool check=LOW,check1=LOW;
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  

uint8_t sec1, min1, hour1, day1, month1;
uint16_t year1;


void setup()
{
  
pinMode(place,INPUT_PULLUP);
pinMode(pump,OUTPUT);
digitalWrite(pump,HIGH);

pinMode(light,OUTPUT);
digitalWrite(light,HIGH);

  // rtc.set(0,59,17, 30, 12, 2021); //08:00:00 24.12.2014 //sec, min, hour, day, month, year

  //stop/pause RTC
  // rtc.stop();
  rtc.start();

  dht.begin();

  lcd.init();                      // initialize the lcd 
  lcd.init();lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(" SMART GARDEN ");
  lcd.setCursor(3,1);
  lcd.print(" CSED 24");
  delay(3000);
  lcd.clear();
}


void loop()
{
     rtc.get(&sec1, &min1, &hour1, &day1, &month1, &year1);
     soilhum=map(analogRead(soilmoister),0,1024,100,0);
     rain=map(analogRead(rainsensor),0,1024,100,0);
     hum = dht.readHumidity();
     temp = dht.readTemperature();
    
     watering_control ();
     light_control ();
     lcd_control();
    
}
// ======================================lcd content===================================
void desplay1()
{
  lcd.setCursor(0,0);
  lcd.print("Time: ");
  if(hour1<10) lcd.print('0'); //10:00:00 //10:00:00
  lcd.print(hour1, DEC);
  lcd.print(':');
  if(min1<10) lcd.print('0');
  lcd.print(min1, DEC);
  lcd.print(':');
  if(sec1<10) lcd.print('0');
  lcd.print(sec1, DEC);
  lcd.print("   ");

  lcd.setCursor(0,1);
  lcd.print("Date: ");
  if(day1<10) lcd.print('0');
  lcd.print(day1, DEC);
  lcd.print('/');
  if(month1<10) lcd.print('0');
  lcd.print(month1);
  lcd.print('/');
  lcd.print(year1);
}

void desplay2()
{
   
    lcd.setCursor(0,0);
    lcd.print("Temperature:");
    lcd.print(temp,1);lcd.print((char)223);lcd.print("C  ");
    lcd.setCursor(0,1);
    lcd.print("Humidity:");
    lcd.print(hum);
    lcd.print("%        ");
    delay(30);
}


void desplay3()
{
  lcd.setCursor(0,0);
   
    lcd.print("soil Hum:");
    lcd.print(soilhum);
    lcd.print(" %         ");
    lcd.setCursor(0,1);
    if(check==HIGH)lcd.print("pump: ON       ");
    else lcd.print("pump: Off     ");
    delay(30);
}

void desplay4()
{
  lcd.setCursor(0,0);
   
    
    if(check1==HIGH)lcd.print("LIGHT : ON       ");
    else lcd.print("LIGHT : Off     ");
    delay(40);
}

//============================lcd control =======================================
void lcd_control ()
{
   if ( digitalRead(place)==LOW)
   {
      delay(100);
      desplaynum++;
      if (desplaynum>3)desplaynum=0;
      lcd.clear();
      delay(60);
   }
     
     switch(desplaynum)
     {
      case 0:
      
      desplay1();break;
      case 1:
     
      desplay2();break;
      case 2:
      
      desplay3();break;

      case 3:
      
      desplay4();break;
     }
delay(60);
}

//=============================== watering control ====================================
void watering_control ()
{
   if (soilhum<15)
   {
      digitalWrite(pump,LOW);
      check=HIGH;
     }
     else 
     {
      if (temp<=25)
      {
        // open the pump two times in one day
      
          if ((hour1==17&&min1==51)&&(0>sec1||sec1<20)&&(soilhum<30))
          {
          digitalWrite(pump,LOW);check=HIGH;
          }
     
     
          else if ((hour1==5&&min1==30)&&(0>sec1||sec1<20)&&(soilhum<30))
          {
          digitalWrite(pump,LOW);check=HIGH;
          }
          
           else
          {
          digitalWrite(pump,HIGH);check=LOW;
          }

  }
            // open the pump 3 times in one day
         else 
        {
          if ((hour1==4&&min1==51)&&(0>sec1||sec1<20)&&(soilhum<30))
          {
          digitalWrite(pump,LOW);check=HIGH;
          }
     
     
          else if ((hour1==3&&min1==30)&&(0>sec1||sec1<20)&&(soilhum<30))
          {
          digitalWrite(pump,LOW);check=HIGH;
          }
          
          else if ((hour1==6&&min1==30)&&(0>sec1||sec1<20)&&(soilhum<30))
          {
          digitalWrite(pump,LOW);check=HIGH;
          }
          
           else
          {
          digitalWrite(pump,HIGH);check=LOW;
          }
          
        }
     }
    
}

//====================================light control =====================================
void light_control ()
{
   if ((hour1>=6)&&(hour1<=17))
          {
          digitalWrite(light,HIGH);check1=LOW;
          }
          
   else
          {
          digitalWrite(light,LOW);check1=HIGH;
          }
}
