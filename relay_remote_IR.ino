#include <IRremote.hpp>  // Gunakan IRremote.hpp
#include <LiquidCrystal.h>

// Konfigurasi pin LCD
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Konfigurasi pin LED
const int ledPins[] = {8, 9, 10, 11}; // Pin untuk 4 LED
#define IR_RECEIVE_PIN A0

// Deklarasi kode remote (sesuaikan dengan remote Anda)
const uint32_t REMOTE_CODES[] = {
    0xFFA25D,  // Tombol 1 (contoh)
    0xFF629D,  // Tombol 2
    0xFFE21D,  // Tombol 3
    0xFF22DD   // Tombol 4
};

void setup() {
    Serial.begin(9600);
    
    // Inisialisasi LCD
    lcd.begin(16, 2);
    lcd.print("IR LED Control");
    
    // Inisialisasi LED
    for (int i = 0; i < 4; i++) {
        pinMode(ledPins[i], OUTPUT);
        digitalWrite(ledPins[i], LOW);
    }
    
    // Inisialisasi IR Receiver
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
    if (IrReceiver.decode()) {
        // Tampilkan info di Serial Monitor
        Serial.print("Received: 0x");
        Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
        
        // Update LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Code: 0x");
        lcd.print(IrReceiver.decodedIRData.decodedRawData, HEX);
        
        // Kontrol LED berdasarkan kode
        bool codeMatched = false;
        for (int i = 0; i < 4; i++) {
            if (IrReceiver.decodedIRData.decodedRawData == REMOTE_CODES[i]) {
                digitalWrite(ledPins[i], !digitalRead(ledPins[i])); // Toggle LED
                lcd.setCursor(0, 1);
                lcd.print("LED ");
                lcd.print(i+1);
                lcd.print(": ");
                lcd.print(digitalRead(ledPins[i]) ? "ON " : "OFF");
                codeMatched = true;
            }
        }
        
        if (!codeMatched) {
            lcd.setCursor(0, 1);
            lcd.print("Unknown code");
        }
        
        IrReceiver.resume(); // Lanjutkan penerimaan
        delay(200); // Debounce
    }
}
