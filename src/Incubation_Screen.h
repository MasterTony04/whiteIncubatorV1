class Incubation_Screen {
private:
byte bat18[8]={
   B01110,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111,
  B11111
  };

byte bat32[8]={
    B01110,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111,
  B11111,
  B11111
    };

byte bat50[8]={
    B01110,
  B10001,
  B10001,
  B10001,
  B11111,
  B11111,
  B11111,
  B11111
    };
    
byte bat68[8]={
    B01110,
  B10001,
  B10001,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
    };

byte bat86[8]{
      
      B01110,
  B10001,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
      };

byte bat100[8]{
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
        
        };


public:
//char buf[2];


void IncubationMenuSetup(){

 // lcd.setBacklightPin(3,POSITIVE); // BL, BL_POL
  lcd.setBacklight(HIGH);
  
   lcd.createChar (2, bat18);  
   lcd.createChar (3, bat32); 
   lcd.createChar (4, bat50);
   lcd.createChar (5, bat68);
   lcd.createChar (6, bat86);
   lcd.createChar (7, bat100);
  
  }

void IncubationScreen(float temp, int voltage, float hum, int days, float celcius){
   
   lcd.home();
    
  lcd.print("Temp: ");
    lcd.print(temp);
    lcd.print((char)223);
    lcd.print("C");
    lcd.print(" ");
    lcd.setCursor(16,0);
    if(voltage<=18){
    lcd.write(2);
    }
    if(18<voltage && voltage<=32){
    lcd.write(3);}
if(32<voltage && voltage<=50){
    lcd.write(4);}
     if(50<voltage && voltage<=81){
    lcd.write(5);}
     if(81<voltage && voltage<=96){
    lcd.write(6);}
    if(96<voltage && voltage<=100){
    lcd.write(7);}
    if(voltage<10)
    lcd.print("0");
    lcd.print(voltage);
    lcd.print("%");
    lcd.setCursor(0,1);
    lcd.print("Humid: ");
    lcd.print(hum);
    lcd.print("%");
    
    lcd.setCursor(0,2);
    lcd.print("Time: ");
    lcd.print(days);
    lcd.print("d ");
   // lcd.print(dt.hour);
    lcd.print("h ");
 //   lcd.print(dt.minute);
    lcd.print("m ");
   // lcd.print(dt.second);
    lcd.print("s");
    lcd.print("  ");
 
    lcd.setCursor(0,3);
    lcd.print("Days left:");
    lcd.print(21-days);
    lcd.setCursor(13,3);
    lcd.print(celcius,1);
    lcd.print((char)223);
    lcd.print("C");
  
  }


};

Incubation_Screen IS;
