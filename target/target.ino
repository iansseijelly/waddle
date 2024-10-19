#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_GC9A01A.h"

#define TFT_RST  7
#define TFT_BL   8
#define TFT_DC   9
#define TFT_CS  10

#define RGB565(R, G, B) (((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3))
#define PEACH  0xF673
#define ORANGE 0xBA66
#define BROWN  0x4941
#define BLUSH  0xE48C

Adafruit_GC9A01A tft = Adafruit_GC9A01A(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(9600);
  Serial.println("GC9A01A Test!");
  tft.begin();

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH); // Backlight on

  drawWaddle();
}

void loop() {
  while (true) {
    // generate a random de between 1 and 10
    int delay_amount = random(1, 10) * 1000 + 5000;
    delay(delay_amount);
    blink();
    drawEye(84, 150, 24, 34, BROWN, GC9A01A_WHITE);
    drawEye(156, 150, 24, 34, BROWN, GC9A01A_WHITE);
  }
}

void drawWaddle() {
  tft.fillScreen(ORANGE);

  // Draw the chin 
  tft.fillCircle(120, 220, 200, PEACH); 
  // Draw the outer face
  tft.fillRect(0, 100, 240, 240, ORANGE);
  // // Draw inner face
  tft.fillCircle(120, 120, 100, PEACH);
  tft.fillCircle(45, 75, 38, PEACH);
  tft.fillCircle(195, 75, 38, PEACH);
  // Touch up the cheeks
  tft.fillCircle(6, 103, 14, ORANGE);
  tft.fillCircle(234, 103, 14, ORANGE);  

  drawBlush();
  // Draw the left eye initial pose
  drawEye(84, 150, 24, 34, BROWN, GC9A01A_WHITE);  // Center at (80, 120), radius 20
  // Draw the right eye initial pose
  drawEye(156, 150, 24, 34, BROWN, GC9A01A_WHITE);  // Center at (160, 120), radius 20

  yield();
}

void drawBlush(){
  tft.fillCircle(55, 100, 12, BLUSH);
  tft.fillCircle(185, 100, 12, BLUSH);
}

void drawEye(int x, int y, int w, int h, uint16_t color, uint16_t irisColor) {
  int radius = w / 2;  // Use half of the width as the radius for the corner circles

  // Draw the center rectangle of the eye (straight vertical section)
  tft.fillRect(x-(w/2), y-(h/2), w+1, h, color);  // Center rectangle

  // Draw the top quarter circles (top-left and top-right corners)
  tft.fillCircle(x, y+(h/2), radius, color);

  // Draw the bottom quarter circles (bottom-left and bottom-right corners)
  tft.fillCircle(x, y-(h/2), radius, color);

  // Draw the iris on the top half of the eye
  int irisRadius = radius / 2;
  tft.fillCircle(x, y+(h/2), irisRadius, irisColor);
}

void drawClosedLeftEye(int x, int y, int w, uint16_t color, uint16_t width) {
  int radius = w / 2;
  tft.fillCircle(x, y, radius, color);
  int cutoutRadius = radius + width/2 - 2;
  tft.fillCircle(x-width+3, y-width, cutoutRadius, PEACH);
}

void drawClosedRightEye(int x, int y, int w, uint16_t color, uint16_t width) {
  int radius = w / 2;
  tft.fillCircle(x, y, radius, color);
  int cutoutRadius = radius + width/2 - 2;
  tft.fillCircle(x+width-3, y-width, cutoutRadius, PEACH);

}

void blink() {
  // erase old eyes
  drawEye(84, 150, 24, 34, PEACH, PEACH);
  drawEye(156, 150, 24, 34, PEACH, PEACH);

  // draw new eyes
  drawClosedLeftEye(74, 130, 48, BROWN, 6);
  drawClosedRightEye(166, 130, 48, BROWN, 6);
  drawBlush();
  yield();

  // generate a random de between 2 and 7
  int delay_amount = random(0, 5) * 1000 + 2000;
  delay(delay_amount);
  // restore old eyes
  drawClosedLeftEye(74, 130, 48, PEACH, 6);
  drawClosedRightEye(166, 130, 48, PEACH, 6);
  drawBlush();
  yield();
}

