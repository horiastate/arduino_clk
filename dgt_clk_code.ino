
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS1302RTC.h>
#include <Time.h>
#include <TimeLib.h>
#include <string.h>
#include <dht.h>

dht DHT;

#define DHT11_PIN 13


///// MAI JOS ESTE SEMNUL DE GRADE CELSIUS

byte celsius[]={
  
  0B00001,
  0B00000,
  0B11110,
  0B10000,
  0B10000,
  0B10000,
  0B11110,
  0B00000
};


////////LINIA DESPARTITOARE

byte linie1[]={
  
  0B00111,
  0B00100,
  0B00100,
  0B00100,
  0B00100,
  0B00100,
  0B00100,
  0B00111
};

byte linie2[]={
  0B11100,
  0B00100,
  0B00100,
  0B00100,
  0B00100,
  0B00100,
  0B00100,
  0B11100
};


// Init the DS1302
// Set pins:  RST, DAT, CLK
DS1302RTC RTC(4, 3, 2);

// Init the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);




void setup() {

  Serial.begin(9600);


  lcd.init();       
  lcd.backlight();  

  
  lcd.createChar(1,celsius); ////emoticonul
  lcd.createChar(2,linie1); ///LINIE DE DESCHIDERE
  lcd.createChar(3,linie2); ///LINIE DE INCHIDERE


  if (RTC.haltRTC()) {
    
    AutoSetBuildTime();
    RTC.haltRTC(false);

  } else
    

  delay(500);

  lcd.setCursor(0, 1);


  if (RTC.writeEN())
    lcd.print("Nu Merge");
  else
    lcd.print("Functioneaza");

  delay(2000);

  


  lcd.clear();
  
  setSyncProvider(RTC.get);  // functia care ia timpul de pe rtc(real time clock)






  if (timeStatus() == timeSet)     
    lcd.print(" FAIL");
  else
    lcd.print(" OK!");

  delay(2000);

  lcd.clear();
}







void loop() {

/////////////////////////////////////////////////////////////////////////

int chk = DHT.read11(DHT11_PIN);          //MODULUL DE TEMPERATURA

lcd.setCursor(6, 0);
lcd.write(2);
lcd.setCursor(7, 0);                      //TEMPERATURA
lcd.print(int(DHT.temperature));
lcd.write(1);
lcd.setCursor(10,0);
lcd.write(3);

lcd.setCursor(11,0);
lcd.write(2);

lcd.setCursor(12, 0);                     //UMIDITATE
lcd.print(int(DHT.humidity));
lcd.print("%");

lcd.setCursor(15, 0);
lcd.write(3);

delay (1000);






/////////////////////////////////////////////////////////////////////////
                                                                      
  static int sday = 0;  // salveaza ziua curenta pentru verificare 

  
  lcd.setCursor(0, 0);
  print2digits(hour());
  lcd.print(":");
  print2digits(minute());
  


////////////////////////////////////////////////////////////////////////



  if (sday != day()) {

    
    lcd.setCursor(0, 1);         ////afiseaza ziua
    lcd.print(dayShortStr(weekday()));

   
    lcd.setCursor(5, 1);      ///afiseaza data
    lcd.print(" ");
    print2digits(day());
    lcd.print("/");
    print2digits(month());
    lcd.print("/");
    lcd.print(year());
  }
  
  

  
  sday = day(); //salvam ziua din nou pentru verificare 

  
  delay(100);

}


// toata structura de mai jos are rolul de a rula codul de la ora si ziua exacte de cand a fost rulat initial codul de la calculator 


void AutoSetBuildTime() {

  char input[] = __TIME__ " " __DATE__;  //input

  String H, M, S, D, N, Y;               //output

  const char del[] = " :,";              //delimitarile aferente

  char *token;                          //se creaza un token pentru "ghidare"
  int i = 0;
  char *array[6];                       //numarul de variabile
  int Hi, Mi, Si, Di, Ni, Yi;

  token = strtok(input, del);
  while (token != NULL)                 //aici incep sa le diferentiez ca sa le separ
  {
    array[i++] = token;
    token = strtok(NULL, del);
  }
  H = array[0];
  Hi = H.toInt();
  M = array[1];
  Mi = M.toInt();
  S = array[2];
  Si = S.toInt();
  D = array[4];
  Di = D.toInt();
  N = array[3];       ///de aici incepe conversia de la numere la luni pentru ca interpretorul sa inteleaga luna prezenta
  if (N == "Jan")
    Ni = 1;
  else if (N == "Feb")
    Ni = 2;
  else if (N == "Mar")
    Ni = 3;
  else if (N == "Apr")
    Ni = 4;
  else if (N == "May")
    Ni = 5;
  else if (N == "Jun")
    Ni = 6;
  else if (N == "Jul")
    Ni = 7;
  else if (N == "Aug")
    Ni = 8;
  else if (N == "Sep")
    Ni = 9;
  else if (N == "Oct")
    Ni = 10;
  else if (N == "Nov")
    Ni = 11;
  else if (N = "Dec")
    Ni = 12;
  Y = array[5];
  Yi = Y.toInt();
  setTime(Hi, Mi, Si, Di, Ni, Yi);
  RTC.set(now());
}


            ///ca sa  nu am 10000000 de cifre, am facut secventa asta mica pentru a avea doar doua digituri
void print2digits(int number) {
  if (number >= 0 && number < 10) {
    lcd.write('0');
  }
  lcd.print(number);
}