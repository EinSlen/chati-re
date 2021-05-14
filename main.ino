#include <lcd.h>
#include "LiquidCrystal.h"
#include "RTClib.h"

RTC_DS1307 RTC; //Classe RTC_DS1307



//entrées

int BPdroit = 2;
int BPgauche = 3;
int Securite = 4;
int Detection = 8;// detection de presence pour sortie
int Blocage = 9; // blocage de la sortie par l'utilisateur


//sorties
int Buzzer = 5;
int Relais1 = 6;
int Relais2 = 7;
int Relais12v = 10;


//variables globale

//heure de debut de sortie autorisée
int H1 = 8;
int M1 = 0;
//heure de fin de sortie autorisée
int H2 = 23;
int M2 = 0;
//heure de derniere manoeuvre
int HD = 12;
int MD = 0;

int Autorisation = 0; // autorisation de sortie ou pas !

int Tempo = 20000;


LiquidCrystal_I2C lcd(0x3F,16,2);  
    

void setup() {

  //Serial.begin(57600); //Démarrage de la communication
  Wire.begin(); //Démarrage de la librairie wire.h  

  
//LiquidCrystal_I2C lcd(0x3F,16,2);  


  lcd.init();                      
  lcd.backlight(); // allume le LCD  
  
  
RTC.begin();  



DateTime now = RTC.now();


  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);






//declaration des entrées

pinMode(BPdroit, INPUT);           // declare une entrée de maniere classique dans le Setup
digitalWrite(BPdroit, HIGH);       // Active le pull-up (resistance interne de l'arduino) quand le contact est ouvert l'entrée voit un 1 (logique negative), le contact doit etre branché à la masse et non au 5V !!!

pinMode(BPgauche, INPUT);           // declare une entrée de maniere classique dans le Setup
digitalWrite(BPgauche, HIGH);       // Active le pull-up (resistance interne de l'arduino) quand le contact est ouvert l'entrée voit un 1 (logique negative), le contact doit etre branché à la masse et non au 5V !!!

pinMode(Blocage, INPUT);           // declare une entrée de maniere classique dans le Setup
digitalWrite(Blocage, HIGH);       // Active le pull-up (resistance interne de l'arduino) quand le contact est ouvert l'entrée voit un 1 (logique negative), le contact doit etre branché à la masse et non au 5V !!!

pinMode(Detection, INPUT);           // declare une entrée de maniere classique dans le Setup
digitalWrite(Detection, HIGH);       // Active le pull-up (resistance interne de l'arduino) quand le contact est ouvert l'entrée voit un 1 (logique negative), le contact doit etre branché à la masse et non au 5V !!!

pinMode(Securite, INPUT);           // declare une entrée de maniere classique dans le Setup
digitalWrite(Securite, HIGH);       // Active le pull-up (resistance interne de l'arduino) quand le contact est ouvert l'entrée voit un 1 (logique negative), le contact doit etre branché à la masse et non au 5V !!!



//declaration des sorties

pinMode(Buzzer, OUTPUT);
pinMode(Relais1, OUTPUT);
pinMode(Relais2, OUTPUT);
pinMode(Relais12v, OUTPUT);

      
      digitalWrite(Relais1, HIGH);
      digitalWrite(Relais2, HIGH);
      digitalWrite(Relais12v, LOW);


lcd.clear();
lcd.setCursor(0, 0);// Colone 1 Ligne 0
lcd.print(" Chatiere") ;
lcd.setCursor(0, 1);// Colone 1 Ligne 1
lcd.print(" Automatique") ;
delay(2000); // pause 
  






lcd.clear(); 



int Minute = now.minute();
int Heure = now.hour();


int bpdroit;
int bpgauche;




}// Fin du SETUP





void loop() {
  

DateTime now = RTC.now();

  
//LiquidCrystal_I2C lcd(0x3F,16,2);  
//lcd.init();


                       

  

int Minute = now.minute();
int Heure = now.hour();
int Seconde = now.second();
int bpdroit = digitalRead(BPdroit);
int bpgauche = digitalRead(BPgauche);  
int detection = digitalRead(Detection); 
int blocage = digitalRead(Blocage); 
int securite = digitalRead(Securite); 

if ( ((Heure > H1) && (Heure < H2)) ||  ((Heure == H1) && (Minute >= M1) ) || ((Heure == H2) && (Minute <= M2)) )
{

   lcd.setCursor(0, 0);// Colone 1 Ligne 0
   lcd.print(F("Sortie Autorisee")) ;
   lcd.setCursor(0, 1);
   lcd.print(F("Der.Mvt "));
   if ( HD < 10)// Affiche d'un 0 si les heures sont plus petites que 10
   lcd.print("0") ; 
   lcd.print(HD);
   lcd.print(F(":"));
   if ( MD < 10)// Affiche d'un 0 si les heures sont plus petites que 10
   lcd.print("0") ; 
   lcd.print(MD);
   
   Autorisation = 1;
   lcd.backlight();

   
}// Fin du if d'autorisation de sortie
else{
  
   lcd.setCursor(0, 0);// Colone 1 Ligne 0
   lcd.print(F("Sortie interdite")) ;
   lcd.setCursor(0, 1);
   lcd.print(F("Der.Mvt "));
   if ( HD < 10)// Affiche d'un 0 si les heures sont plus petites que 10
   lcd.print("0") ; 
   lcd.print(HD);
   lcd.print(F(":"));
   if ( MD < 10)// Affiche d'un 0 si les heures sont plus petites que 10
   lcd.print("0") ; 
   lcd.print(MD);
   
   Autorisation = 0;
   lcd.noBacklight();

  
}



// cas du blocage par l'inter
if ( blocage == 1)
{
  lcd.clear();
  do{
   lcd.setCursor(0, 0);// Colone 1 Ligne 0
   lcd.print(F("Sortie interdite")) ;
   lcd.setCursor(0, 1);// Colone 1 Ligne 0
   lcd.print(F("Par utilisateur")) ;
   blocage = digitalRead(Blocage); 
   delay(1000);
  }while( blocage == 1);

  lcd.clear();
   lcd.setCursor(0, 0);// Colone 1 Ligne 0
   lcd.print(F("Sortie debloquee")) ;
   lcd.setCursor(0, 1);// Colone 1 Ligne 0
   lcd.print(F("Par utilisateur")) ;
   delay(2000);
  
  
}


// Cas d'une detection de presence en sortie
if ( (detection == 1) && (Autorisation == 1) && (securite == 0))
{

  // attente de 150 ms pour etre sur qu'il ne s'agit pas d'une detection intempestive
  delay(150);
  detection = digitalRead(Detection); 

  if (detection == 1)
{
   lcd.clear();
   lcd.setCursor(0, 0);// Colone 1 Ligne 0
   lcd.print(F("Ouverture en ")) ;
   lcd.setCursor(0, 1);// Colone 1 Ligne 0
   lcd.print(F("cours")) ;
   
  HD = Heure;
  MD = Minute; 
  Ouverture();
}else{

   lcd.clear();
   lcd.setCursor(0, 0);// Colone 1 Ligne 0
   lcd.print(F("Detection trop")) ;
   lcd.setCursor(0, 1);// Colone 1 Ligne 0
   lcd.print(F("courte !")) ;
   delay(2000);
   lcd.clear();
   
  
}

  
}



// si appui sur le bouton gauche
if( bpgauche == 0)
{
HD = Heure;
MD = Minute; 
   lcd.clear();
   lcd.setCursor(0, 0);// Colone 1 Ligne 0
   lcd.print(F("Ouverture en ")) ;
   lcd.setCursor(0, 1);// Colone 1 Ligne 0
   lcd.print(F("cours")) ; 
   delay(1000);
Ouverture();
}


// si appui sur le bouton droit
if( bpdroit == 0)
Horaire();


}// FIN DU PROGRAMME
