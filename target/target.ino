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

#define LONG_ANIMATION_DELAY 5000

#define eye_x_offset 34
#define eye_y 150
#define eye_w 24
#define eye_h 34

#define l_eye_x (120 - eye_x_offset)
#define l_eye_y eye_y
#define l_eye_w eye_w
#define l_eye_h eye_h
#define r_eye_x (120 + eye_x_offset)
#define r_eye_y eye_y
#define r_eye_w eye_w
#define r_eye_h eye_h

Adafruit_GC9A01A tft = Adafruit_GC9A01A(TFT_CS, TFT_DC, TFT_RST);

enum Action {
  BLINK,
  SMILE,
  LOOK_SIDEWAYS,
};

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
    int delay_amount = random(5, 10) * 1000;
    delay(delay_amount);
    int action = random(0, 3);
    if (action == BLINK) {
      blink(0);
    } else if (action == SMILE) {
      delay(LONG_ANIMATION_DELAY);
      blink(1);
    } else if (action == LOOK_SIDEWAYS) {
      lookSideWays();
    }
    drawEye(l_eye_x, l_eye_y, l_eye_w, l_eye_h, BROWN, GC9A01A_WHITE);
    drawEye(r_eye_x, r_eye_y, r_eye_w, r_eye_h, BROWN, GC9A01A_WHITE);
    yield();
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
  drawEye(l_eye_x, l_eye_y, l_eye_w, l_eye_h, BROWN, GC9A01A_WHITE);  // Center at (80, 120), radius 20
  // Draw the right eye initial pose
  drawEye(r_eye_x, r_eye_y, r_eye_w, r_eye_h, BROWN, GC9A01A_WHITE);  // Center at (160, 120), radius 20

  yield();
}

// this may be called elsewhere when blushes are accidentally erased by other 
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
  int cutoutRadius = radius + width/2;
  tft.fillCircle(x-width+3, y-width-5, cutoutRadius, PEACH);
}

void drawClosedRightEye(int x, int y, int w, uint16_t color, uint16_t width) {
  int radius = w / 2;
  tft.fillCircle(x, y, radius, color);
  int cutoutRadius = radius + width/2;
  tft.fillCircle(x+width-3, y-width-5, cutoutRadius, PEACH);
}

void blink(int last) {
  // erase old eyes
  drawEye(l_eye_x, l_eye_y, l_eye_w, l_eye_h, PEACH, PEACH);
  drawEye(r_eye_x, r_eye_y, r_eye_w, r_eye_h, PEACH, PEACH);

  // draw new eyes
  drawClosedLeftEye(74, 130, 48, BROWN, 6);
  drawClosedRightEye(166, 130, 48, BROWN, 6);
  drawBlush();
  yield();
  int delay_amount;
  if (last == 1) {
    // generate a random de between 2 and 7
    delay_amount = random(0, 5) * 1000 + 2000;
    Serial.println("[Animation]: Smiling with delay: " + String(delay_amount));
  } else {
    delay_amount = 500;
    Serial.println("[Animation]: Blinking with delay: " + String(delay_amount));
  }
  delay(delay_amount);
  // restore old eyes
  drawClosedLeftEye(74, 130, 48, PEACH, 6);
  drawClosedRightEye(166, 130, 48, PEACH, 6);
  drawBlush();
  yield();
}

void drawSidewayEyes(int x, int y, int w, int h, uint16_t color, uint16_t irisColor, int offset) {
  // fill the old iris with brown
  int radius = w / 2;
  int irisRadius = radius / 2;
  // fill the old iris with brown
  tft.fillCircle(x, y+(h/2), irisRadius, BROWN);
  // draw the new iris
  tft.fillCircle(x+offset, y+(h/2), irisRadius, irisColor);
}

void lookSideWays() {
  int rand_offset = random(-5, 5);
  drawSidewayEyes(l_eye_x, l_eye_y, l_eye_w, l_eye_h, BROWN, GC9A01A_WHITE, rand_offset);
  drawSidewayEyes(r_eye_x, r_eye_y, r_eye_w, r_eye_h, BROWN, GC9A01A_WHITE, rand_offset);
  // generate a random de between 2 and 5
  int delay_amount = random(2, 5) * 1000;
  Serial.println("[Animation]: Looking sideways with offset: " + String(rand_offset) + " and delay: " + String(delay_amount));
  delay(delay_amount);
}