#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <avr/sleep.h> // Pentru sleep
#define interruptPin 2

//PIN DE DATE
#define ONE_WIRE_BUS 4

//Comunicare cu dispozitive oneWire (am doar unul conectat - senzorul de temperatura)
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//Initializare pini LCD
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  // Afisarea unui mesaj pe LCD
  lcd.print("Temperatura:");
  sensors.begin();
  // Setam modul pentru pinul de intrerupere, este legat
  // la rezistenta de pullup interna a placutei ca sa fie
  // pe valoarea de 1 logic
  pinMode(interruptPin,INPUT_PULLUP);
  pinMode(5,OUTPUT);
  digitalWrite(5, LOW);
}


void Going_To_Sleep(){
    sleep_enable(); // Activare mod de sleep
    attachInterrupt(0, wakeUp, LOW); // Atasam intreruperea pe pinul digital 2
    digitalWrite(5, LOW);
    delay(500);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);// Setam modul de sleep
    sleep_cpu();//activam modul de sleep
    Serial.println("S-a dezactivat modul de sleep"); 
    //Serial.println(currentTime);
    digitalWrite(5, HIGH);
    sensors.requestTemperatures(); // CERERE TEMPERATURA
    lcd.setCursor(0, 12); // Plasare cursor pe LCD
    lcd.print(sensors.getTempCByIndex(0));
    lcd.print(" C");
    delay(5000); // Timpul pana la stingerea backlight-ului LCD-ului
    digitalWrite(5, LOW);
  }

void wakeUp(){
  Serial.println("Intrerupere Inregistrata");// Afisam mesaj ca am inregistrat intreruperea pe seriala
   sleep_disable();// Dezactivam modul de sleep
  detachInterrupt(0); 
}

void loop() {
  Going_To_Sleep();  
  delay(1000);
}
