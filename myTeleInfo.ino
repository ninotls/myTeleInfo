#include <LiquidCrystal_I2C.h>
#include <TeleInfo.h>
#include <SoftwareSerial.h>

const unsigned int   rxPin = 4;           // Pin for RX TIC
const unsigned int screenButtonPin = 9;  // Numéro de la pin du bouton poussoir qui sert à changer d'écran

const char  *adco;       // Adresse du compteur
const char  *opTarif;    // Option tarifaire
long        iSousc;      // Puissance souscrite
long        indexHC;     // Index HC
long        indexHP;     // Index HP
long        indexBase;   // Index Base
const char  *ptec;       // Période tarifaire en cours
long        iInst;       // Intensité instannée
long        iMax;        // Intensité max appelée
long        pApp;        // Puissance apparente
bool        modeBase = false;

unsigned int impulsion    = 0;
unsigned int impulsionCpt = 0;

// Tele-Info
SoftwareSerial serial(rxPin, rxPin);
TeleInfo teleinfo(&serial);

// LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  // Debug
  Serial.begin(9600);

  // Init LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Conso:");

  // Init button
  pinMode(screenButtonPin, INPUT_PULLUP);

  // Init TIC
  serial.begin(1200);
  teleinfo.begin();

  // Get static TIC info
  teleinfo.process();
  if (teleinfo.available()) {
    // Debug
    Serial.println("--- tele info available --- ");
    teleinfo.printAllToSerial();
    Serial.println("------ ");

    adco      = teleinfo.getStringVal("ADCO");
    opTarif   = teleinfo.getStringVal("OPTARIF");
    iSousc    = teleinfo.getLongVal("ISOUSC");
    ptec      = teleinfo.getStringVal("PTEC");
    indexBase = teleinfo.getLongVal("BASE");

    if (indexBase != 0) {
      modeBase = true;

      // Debug
      Serial.print("Index Base = ");
      indexBase == NULL ? Serial.println("unknown") : Serial.println(indexBase);
    }
    else {
      indexHC = teleinfo.getLongVal("HCHC");
      indexHP = teleinfo.getLongVal("HCHP");

      // Debug
      Serial.print("Index HC = ");
      indexHC == NULL ? Serial.println("unknown") : Serial.println(indexHC);
      Serial.print("Index HP = ");
      indexHP == NULL ? Serial.println("unknown") : Serial.println(indexHP);
    }

    // Debug
    Serial.print("Adresse Compteur = ");
    adco == NULL ? Serial.println("unknown") : Serial.println(adco);
    Serial.print("Option Tarifaire = ");
    opTarif == NULL ? Serial.println("unknown") : Serial.println(opTarif);
    Serial.print("Puissance Souscrite = ");
    iSousc == NULL ? Serial.println("unknown") : Serial.println(opTarif);

  }
  teleinfo.resetAvailable();
}

void loop() {
  teleinfo.process();

  if (teleinfo.available()) {
    // Debug
    Serial.println("--- tele info available --- ");
    teleinfo.printAllToSerial();
    Serial.println("------ ");

    // Get TIC data
    pApp  = teleinfo.getLongVal("PAPP");
    iInst = teleinfo.getLongVal("IINST");
    iMax  = teleinfo.getLongVal("IMAX");

    // Debug
    Serial.print("Puissance Apparente = ");
    pApp == NULL ? Serial.println("unknown") : Serial.println(pApp);
    Serial.print("Intensité Instannée = ");
    iInst == NULL ? Serial.println("unknown") : Serial.println(iInst);
    Serial.print("Intensité Max = ");
    iMax == NULL ? Serial.println("unknown") : Serial.println(iMax);


    if (modeBase) {
      indexBase = teleinfo.getLongVal("BASE");

      // Debug
      Serial.print("Index Base = ");
      indexBase == NULL ? Serial.println("unknown") : Serial.println(indexBase);
    }
    else {
      indexHC = teleinfo.getLongVal("HCHC");
      indexHP = teleinfo.getLongVal("HCHP");

      // Debug
      Serial.print("Index HC = ");
      indexHC == NULL ? Serial.println("unknown") : Serial.println(indexHC);
      Serial.print("Index HP = ");
      indexHP == NULL ? Serial.println("unknown") : Serial.println(indexHP);
    }

    teleinfo.resetAvailable();
  }

  impulsion = digitalRead(screenButtonPin);
  if (impulsion == 0) impulsionCpt += 1 % 3;

  if (impulsionCpt == 0)
  {
    //-----Ecran n°1:-------------------------------
    lcd.setCursor(0, 0);
    lcd.print("Ecran 1");
  }
  else if (impulsionCpt == 1)
  {
    // -----ECRAN N°2:-------------------------------
    lcd.setCursor(0, 0);
    lcd.print("Ecran 2");
  }
  else if (impulsionCpt == 2)
  {
    // -----ECRAN N°3:-------------------------------
    lcd.setCursor(0, 0);
    lcd.print("Ecran 3");
  }
}
