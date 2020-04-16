#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <wiringPi.h>
#include <lcd.h>

//#define byte uint8_t
#define RX_CLOCK 4
#define RX_DATA 25
#define LCD_RS 6
#define LCD_EN 5
#define LCD_D4 29
#define LCD_D5 28
#define LCD_D6 27
#define LCD_D7 26


char message[16];
uint8_t rx_byte = 0;
volatile int bit_position = 0;
volatile bool update_lcd = true;

void onClockPulse() {
  printf("\nonClockPulse called\n");
  printf("\nbit_position = %d\n", bit_position);
  bool rx_bit = digitalRead(RX_DATA);
  printf("rx_bit = %x\n", rx_bit);

  if (bit_position == 8) {
    printf("if bit_position == 8 set bit_position == 0\n");
    rx_byte = 0;
    bit_position = 0;
  }
  if (rx_bit) {
    printf("rx_byte = %x\n", rx_byte);
    rx_byte |= (0x80 >> bit_position);
    int assign = (0x80 >> bit_position);
    printf("0x80 >> bit_position = %x\n", assign);
    printf("rx_byte = %x\n", rx_byte);
  }
  bit_position += 1;
  if (bit_position == 8) {
    printf("bit_position == 8\n");
    strncat(message, &rx_byte, 1);  
  }
  update_lcd = true;
}

int main(int argc, char** argv) {

  int lcd;
  wiringPiSetup();
  lcd = lcdInit (2, 16, 4, LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);
  //lcdClear(lcd);
  //lcdPosition(lcd, 0, 0);
  strcpy(message, "");
  pinMode(RX_DATA, INPUT);
  wiringPiISR(RX_CLOCK, INT_EDGE_RISING, &onClockPulse);
  
  while(1) {
    if (update_lcd) {
      printf("update_lcd called\n");
      update_lcd = false;
      lcdCursor(lcd, 0);
      lcdPosition(lcd, 0, 0);
      lcdPuts(lcd, message);
      lcdPosition(lcd, 0, 1);
      for (int i = 0; i < 8; i++) {
        if (i < bit_position) {
          lcdPuts(lcd, rx_byte & (0x80 >> i) ? "1" : "0");  
        } else {
          lcdPuts(lcd, " ");  
        }
      }
      lcdPosition(lcd, strlen(message), 0);
      lcdCursor(lcd, 1);
      //delay(500);
    }
  }
}

