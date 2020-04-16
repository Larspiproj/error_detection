#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <wiringPi.h>
#include <lcd.h>

#define TX_RATE 5

const char *message = "B";

#define TX_CLOCK 4
#define TX_DATA 25
#define LCD_RS 6
#define LCD_EN 5
#define LCD_D4 29
#define LCD_D5 28
#define LCD_D6 27
#define LCD_D7 26
#define PARITY 24

int main(int argc, char** argv) {

  int lcd;
  wiringPiSetup();
  pinMode(TX_DATA, OUTPUT);
  pinMode(TX_CLOCK, OUTPUT);
  pinMode(PARITY, INPUT);
  lcd = lcdInit (2, 16, 4, LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);
  lcdClear(lcd);
  //lcdPosition(lcd, 0, 0);
  lcdPuts(lcd, message);

  for (int byte_idx = 0; byte_idx < strlen(message); byte_idx++) {
    char tx_byte = message[byte_idx];
    printf("tx_byte = %c\n", tx_byte);

    lcdCursor(lcd, 0);
    lcdPosition(lcd, 0, 1);
    lcdPuts(lcd, "        ");
    lcdPosition(lcd, byte_idx, 0);
    lcdCursor(lcd, 1);

    for (int bit_idx = 0; bit_idx < 8; bit_idx++) {
      int tx_bit = tx_byte & (0x80 >> bit_idx);

      digitalWrite(TX_DATA, tx_bit);
      delay((1000 / TX_RATE) / 2);

      lcdCursor(lcd, 0);
      lcdPosition(lcd, bit_idx, 1);
      lcdPuts(lcd, tx_bit ? "1" : "0");
      lcdPosition(lcd, byte_idx, 0);
      lcdCursor(lcd, 1);

      digitalWrite(TX_CLOCK, HIGH);
      delay((1000 / TX_RATE) / 2);
      digitalWrite(TX_CLOCK, LOW);
    }
  }
  digitalWrite(TX_DATA, digitalRead(PARITY));
  delay((1000 / TX_RATE) / 2);
  digitalWrite(TX_CLOCK, HIGH);
  delay((1000 / TX_RATE) / 2);
  digitalWrite(TX_CLOCK, LOW);

  digitalWrite(TX_DATA, LOW);
  //delay(5000);
  //lcdClear(lcd);
}

