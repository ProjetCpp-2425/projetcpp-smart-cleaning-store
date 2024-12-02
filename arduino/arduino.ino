#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // Adjust pins based on your setup

void setup() {
  Serial.begin(9600); 
  lcd.begin(16, 2);  

  lcd.setCursor(0, 0); 
  lcd.print("Waiting for data"); 
  lcd.setCursor(0, 1); 
  lcd.print("from serial...");
}

void loop() {
  if (Serial.available()) {
    
    String message = Serial.readStringUntil('\n');
   
    if (message.length() > 0) {
      
      int nameStart = message.indexOf("") ;
      int priceStart = message.indexOf("Price:") + 7;

      String name = message.substring(nameStart, message.indexOf(", Price:"));
      String price = message.substring(priceStart);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(name);
      lcd.setCursor(0, 1);
      lcd.print("Price:");
      lcd.print(price);
    }
  }
}
