#include <LiquidCrystal.h>
#include "IRremote.hpp"

// Konfigurasi pin LCD
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define IR_RECEIVE_PIN A0

void setup() {
  // Inisialisasi Serial
  Serial.begin(9600);
  
  // Inisialisasi LCD
  lcd.begin(16, 2); // LCD 16x2
  lcd.print("IR Receiver Ready");
  
  // Inisialisasi IR Receiver
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
  if (IrReceiver.decode()) {
    // Tampilkan di Serial Monitor
    IrReceiver.printIRResultShort(&Serial);
    
    // Tampilkan di LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Protocol:");
    lcd.print(IrReceiver.decodedIRData.protocol);
    
    // Tampilkan raw data pada baris kedua
    lcd.setCursor(0, 1);
    lcd.print("Data: 0x");
    lcd.print(IrReceiver.decodedIRData.decodedRawData, HEX);
    
    // Lanjutkan menerima sinyal berikutnya
    IrReceiver.resume();
    
    // Delay kecil untuk stabilitas
    delay(200);
  }
}
