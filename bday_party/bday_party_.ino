
// Some effects for a small dancefloor

#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            13
#define PIN_MODE_SWITCH 2
#define PIN_OPTION_SWITCH 3
#define PIN_LUMI A0
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      280

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 5;

int currentLumi = 40;

uint32_t red, green, blue, black, white, uv, cyan;

uint32_t tracerColor;
void setup() {
  pixels.begin();

  pinMode(PIN_MODE_SWITCH, INPUT);
  pinMode(PIN_OPTION_SWITCH, INPUT);

  red = pixels.Color(currentLumi, 0, 0);
  blue = pixels.Color(0, 0, currentLumi);
  green = pixels.Color(0, currentLumi, 0);
  black = pixels.Color(0, 0, 0);
  white = pixels.Color(currentLumi, currentLumi, currentLumi);
  uv = pixels.Color(currentLumi, 0, currentLumi);
  cyan = pixels.Color(0, currentLumi, currentLumi);
  tracerColor = red;


  Serial.begin(115200);
}

int nbModes = 6;
String modes[] = {
  "Rain2", "ParallelDown", "RGBTracer", "Rain", "Parallel", "Dual"
};


int currentModeIndex = 0;
String currentMode = modes[currentModeIndex];

int modeSwitch = 0;
bool modeSwitchPressed = false;

int currentOption = 0;
String currentOptionString = "";
int optionSwitch = 0;
bool optionSwitchPressed = false;



void loop() {

  int lumi = analogRead(PIN_LUMI) / 8 + 10;
  if (lumi != currentLumi) {
    Serial.println("Redef colors");
    currentLumi = lumi;
    red = pixels.Color(currentLumi, 0, 0);
    blue = pixels.Color(0, 0, currentLumi);
    green = pixels.Color(0, currentLumi, 0);
    black = pixels.Color(0, 0, 0);
    white = pixels.Color(currentLumi, currentLumi, currentLumi);
    uv = pixels.Color(currentLumi, 0, currentLumi);
    cyan = pixels.Color(0, currentLumi, currentLumi);
  }

  modeSwitch = digitalRead(PIN_MODE_SWITCH);
  if (modeSwitch == HIGH && modeSwitchPressed == false ) {
    modeSwitchPressed = true;
    currentModeIndex = (currentModeIndex + 1) % nbModes;
    currentOption = 0;

  } else if (modeSwitch == LOW) {
    modeSwitchPressed = false;
  }

  optionSwitch = digitalRead(PIN_OPTION_SWITCH);
  if (optionSwitch == HIGH && optionSwitchPressed == false ) {
    optionSwitchPressed = true;
    currentOption++;

    if (modes[currentModeIndex].equals("Rain2") || modes[currentModeIndex].equals("ParallelDown") || modes[currentModeIndex].equals("Parallel") || modes[currentModeIndex].equals("Dual")) {
      if (currentOption == 1) {
        currentOptionString = "uv/blue";
      } else if (currentOption == 2) {
        currentOptionString = "red/green";
      } else if (currentOption == 3) {
        currentOptionString = "colors";
      } else {
        currentOption = 0;
        currentOptionString = "";
      }
    }



  } else if (optionSwitch == LOW) {
    optionSwitchPressed = false;
  }

  if (modes[currentModeIndex].equals("Rain2")) {
    rain2(currentOptionString);
  } else if (modes[currentModeIndex].equals("RGBTracer")) {
    rgbTracer();
  } else if (modes[currentModeIndex].equals("Rain")) {
    rain();
  } else if (modes[currentModeIndex].equals("Parallel")) {
    parallel(currentOptionString, 450);
  } else if (modes[currentModeIndex].equals("Dual")) {
    dual(currentOptionString, 400);
  } else if (modes[currentModeIndex].equals("ParallelDown")) {
    parallelDown(currentOptionString , 250);
  }

 
 
}

int rgbTracerColor = 0;

void rgbTracer () {
  if (rgbTracerColor == 0) {
    tracerColor = blue;
  } else if (rgbTracerColor == 1) {
    tracerColor = green;
  } else if (rgbTracerColor == 2) {
    tracerColor = uv;
  } else if (rgbTracerColor == 3) {
    tracerColor = red;
  }
  tracerColor = blue; 
  rgbTracerColor = (rgbTracerColor + 1 ) % 4;

  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, tracerColor);

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(0); // Delay for a period of time (in milliseconds).
  }
}

void rain() {
  rain(random(0, currentLumi), random(0, currentLumi), random(0, currentLumi));
}

void rain(int r, int g, int b) {

  for (int i = 0; i < 100; i++) {
    // left
    pixels.setPixelColor((i - 7 + 100) % 100, black);
    pixels.setPixelColor((i - 6 + 100) % 100, pixels.Color(r * 0.05, g * 0.05, b * 0.05));
    pixels.setPixelColor((i - 5 + 100) % 100, pixels.Color(r * 0.15, g * 0.15, b * 0.15));
    pixels.setPixelColor((i - 4 + 100) % 100, pixels.Color(r * 0.20, g * 0.20, b * 0.20));
    pixels.setPixelColor((i - 3 + 100) % 100, pixels.Color(r * 0.25, g * 0.25, b * 0.25));
    pixels.setPixelColor((i - 2 + 100) % 100, pixels.Color(r * 0.50, g * 0.50, b * 0.50));
    pixels.setPixelColor((i - 1 + 100) % 100, pixels.Color(r * 0.75, g * 0.75, b * 0.75));
    pixels.setPixelColor(i, pixels.Color(r, g, b));

    // middle
    if (i < 87) {
      pixels.setPixelColor(180 - i + 6, black);
      if (i < 86 && i > 5) {
        pixels.setPixelColor(180 - i + 5, pixels.Color(r * 0.05, g * 0.05, b * 0.05));
      }
      if (i < 85 && i > 4) {
        pixels.setPixelColor(180 - i + 4, pixels.Color(r * 0.15, g * 0.15, b * 0.15));
      }
      if (i < 84 && i > 3) {
        pixels.setPixelColor(180 - i + 3, pixels.Color(r * 0.20, g * 0.20, b * 0.20));
      }
      if (i < 83 && i > 2) {
        pixels.setPixelColor(180 - i + 2, pixels.Color(r * 0.25, g * 0.25, b * 0.25));
      }
      if (i < 82 && i > 1) {
        pixels.setPixelColor(180 - i + 1, pixels.Color(r * 0.50, g * 0.50, b * 0.50));
      }
      if (i < 81 && i > 0) {
        pixels.setPixelColor(180 - i, pixels.Color(r * 0.75, g * 0.75, b * 0.75));
      }
      if (i < 80) {
        pixels.setPixelColor(180 - i - 1, pixels.Color(r, g, b));
      }
    }

    // right
    pixels.setPixelColor((i - 7 + 100) % 100 + 180, black);
    pixels.setPixelColor((i - 6 + 100) % 100 + 180, pixels.Color(r * 0.05, g * 0.05, b * 0.05));
    pixels.setPixelColor((i - 5 + 100) % 100 + 180, pixels.Color(r * 0.15, g * 0.15, b * 0.15));
    pixels.setPixelColor((i - 4 + 100) % 100 + 180, pixels.Color(r * 0.20, g * 0.20, b * 0.20));
    pixels.setPixelColor((i - 3 + 100) % 100 + 180, pixels.Color(r * 0.25, g * 0.25, b * 0.25));
    pixels.setPixelColor((i - 2 + 100) % 100 + 180, pixels.Color(r * 0.50, g * 0.50, b * 0.50));
    pixels.setPixelColor((i - 1 + 100) % 100 + 180, pixels.Color(r * 0.75, g * 0.75, b * 0.75));
    pixels.setPixelColor(i + 180, pixels.Color(r, g, b));

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(5); // Delay for a period of time (in milliseconds).
  }
}

void rain2() {
  rain2("");
}

void rain2(String mode) {
  rain2Up(mode);
  rain2Down(mode);
  delay(1);
}

int rain2UpStart_[] = {0, 0, 0}, rain2UpStop_[] = {0, 0, 0};
int rain2UpCurrent_[] = { -1, -1 , -1} ;
uint32_t rain2UpColor_[3] , rain2UpFade1_[3], rain2UpFade2_[3], rain2UpFade3_[3], rain2UpFade4_[3], rain2UpFade5_[3], rain2UpFade6_[3];

void rain2Up(String mode) {

  for (int j = 0; j < 3; j++) {
    if (rain2UpCurrent_[j] == rain2UpStart_[j] + rain2UpStop_[j] || rain2UpCurrent_[j] == 280) {
      for (int i = 1 ; i < 9; i++) {
        pixels.setPixelColor(rain2UpCurrent_[j] - i, black);
      }
      rain2UpCurrent_[j] = -1;
    }

    if (rain2UpCurrent_[j] == -1) {
      rain2UpStart_[j] = random(0, 180);
      rain2UpStop_[j] = random(40, 280);
      rain2UpCurrent_[j] = rain2UpStart_[j];
      int rain2UpR;
      int rain2UpG;
      int rain2UpB;
      if (mode.equals("")) {
        rain2UpR = random(0, currentLumi);
        rain2UpG = random(0, currentLumi);
        rain2UpB = random(0, currentLumi);
      } else if (mode.equals("colors")) {
        rain2UpR = random(0, 2) * currentLumi;
        rain2UpG = random(0, 2) * currentLumi;
        rain2UpB = random(0, 2) * currentLumi;
      } else if (mode.equals("uv/blue")) {
        if (j == 2) {
          rain2UpR = 0;
          rain2UpG = 0;
          rain2UpB = currentLumi;
        } else {
          rain2UpR = currentLumi;
          rain2UpG = 0;
          rain2UpB = currentLumi;
        }
      } else if (mode.equals("red/green")) {
        if (j != 2) {
          rain2UpR = 0;
          rain2UpG = currentLumi;
          rain2UpB = 0;
        } else {
          rain2UpR = currentLumi;
          rain2UpG = 0;
          rain2UpB = 0;
        }
      }
      rain2UpColor_[j] = pixels.Color(rain2UpR, rain2UpG, rain2UpB);
      rain2UpFade1_[j] = pixels.Color(rain2UpR * 0.75, rain2UpG * 0.75, rain2UpB * 0.75);
      rain2UpFade2_[j] = pixels.Color(rain2UpR * 0.50, rain2UpG * 0.50, rain2UpB * 0.50);
      rain2UpFade3_[j] = pixels.Color(rain2UpR * 0.25, rain2UpG * 0.25, rain2UpB * 0.25);
      rain2UpFade4_[j] = pixels.Color(rain2UpR * 0.20, rain2UpG * 0.20, rain2UpB * 0.20);
      rain2UpFade5_[j] = pixels.Color(rain2UpR * 0.15, rain2UpG * 0.15, rain2UpB * 0.15);
      rain2UpFade6_[j] = pixels.Color(rain2UpR * 0.05, rain2UpG * 0.05, rain2UpB * 0.05);
    }


    pixels.setPixelColor(rain2UpCurrent_[j] - 7, black);
    pixels.setPixelColor(rain2UpCurrent_[j] - 6, rain2UpFade6_[j]);
    pixels.setPixelColor(rain2UpCurrent_[j] - 5, rain2UpFade5_[j]);
    pixels.setPixelColor(rain2UpCurrent_[j] - 4, rain2UpFade4_[j]);
    pixels.setPixelColor(rain2UpCurrent_[j] - 3, rain2UpFade3_[j]);
    pixels.setPixelColor(rain2UpCurrent_[j] - 2, rain2UpFade2_[j]);
    pixels.setPixelColor(rain2UpCurrent_[j] - 1, rain2UpFade1_[j]);
    pixels.setPixelColor(rain2UpCurrent_[j], rain2UpColor_[j]);

    rain2UpCurrent_[j]++;
  }

  pixels.show();
}


int rain2DownStart_[] = {0, 0, 0} , rain2DownStop_[] = {0, 0, 0};
int rain2DownCurrent_[] = { -1, -1 , -1};
uint32_t rain2DownColor_[3] , rain2DownFade1_[3], rain2DownFade2_[3], rain2DownFade3_[3], rain2DownFade4_[3], rain2DownFade5_[3], rain2DownFade6_[3];

void rain2Down(String mode) {

  for (int j = 0; j < 3; j++) {
    if (rain2DownCurrent_[j] == rain2DownStart_[j] + rain2DownStop_[j] || rain2DownCurrent_[j] == 281) {
      for (int i = 1 ; i < 9; i++) {
        pixels.setPixelColor(280 - (rain2DownCurrent_[j] - i), black);
      }
      rain2DownCurrent_[j] = -1;
    }

    if (rain2DownCurrent_[j] == -1) {
      rain2DownStart_[j] = random(0, 180);
      rain2DownStop_[j] = random(40, 280);
      rain2DownCurrent_[j] = rain2DownStart_[j];
      int rain2DownR;
      int rain2DownG;
      int rain2DownB;
      if (mode.equals("")) {
        rain2DownR = random(0, currentLumi);
        rain2DownG = random(0, currentLumi);
        rain2DownB = random(0, currentLumi);
      } else if (mode.equals("colors")) {
        rain2DownR = random(0, 2) * currentLumi;
        rain2DownG = random(0, 2) * currentLumi;
        rain2DownB = random(0, 2) * currentLumi;
      } else if (mode.equals("uv/blue")) {
        if (j != 2) {
          rain2DownR = 0;
          rain2DownG = 0;
          rain2DownB = currentLumi;
        } else {
          rain2DownR = currentLumi;
          rain2DownG = 0;
          rain2DownB = currentLumi;
        }
      } else if (mode.equals("red/green")) {
        if (j != 2) {
          rain2DownR = currentLumi;
          rain2DownG = 0;
          rain2DownB = 0;
        } else {
          rain2DownR = 0;
          rain2DownG = currentLumi;
          rain2DownB = 0;
        }
      }
      rain2DownColor_[j] = pixels.Color(rain2DownR, rain2DownG, rain2DownB);
      rain2DownFade1_[j] = pixels.Color(rain2DownR * 0.75, rain2DownG * 0.75, rain2DownB * 0.75);
      rain2DownFade2_[j] = pixels.Color(rain2DownR * 0.50, rain2DownG * 0.50, rain2DownB * 0.50);
      rain2DownFade3_[j] = pixels.Color(rain2DownR * 0.25, rain2DownG * 0.25, rain2DownB * 0.25);
      rain2DownFade4_[j] = pixels.Color(rain2DownR * 0.20, rain2DownG * 0.20, rain2DownB * 0.20);
      rain2DownFade5_[j] = pixels.Color(rain2DownR * 0.15, rain2DownG * 0.15, rain2DownB * 0.15);
      rain2DownFade6_[j] = pixels.Color(rain2DownR * 0.05, rain2DownG * 0.05, rain2DownB * 0.05);
    }

    pixels.setPixelColor(280 - (rain2DownCurrent_[j] - 7), black);
    pixels.setPixelColor(280 - (rain2DownCurrent_[j] - 6), rain2DownFade6_[j]);
    pixels.setPixelColor(280 - (rain2DownCurrent_[j] - 5), rain2DownFade5_[j]);
    pixels.setPixelColor(280 - (rain2DownCurrent_[j] - 4), rain2DownFade4_[j]);
    pixels.setPixelColor(280 - (rain2DownCurrent_[j] - 3), rain2DownFade3_[j]);
    pixels.setPixelColor(280 - (rain2DownCurrent_[j] - 2), rain2DownFade2_[j]);
    pixels.setPixelColor(280 - (rain2DownCurrent_[j] - 1), rain2DownFade1_[j]);
    pixels.setPixelColor(280 - rain2DownCurrent_[j], rain2DownColor_[j]);

    rain2DownCurrent_[j]++;
  }
  pixels.show(); // This sends the updated pixel color to the hardware.

}

int parallelCount = 0;
uint32_t currentParallelColor = 0;

void parallel(String mode, int period) {
  if (parallelCount == 3) {
    parallelCount = 0;
  }
  if (mode.equals("uv/blue")) {
    if (currentParallelColor == blue) {
      currentParallelColor = uv;
    } else {
      currentParallelColor = blue;
    }
   
  } else if (mode.equals("red/green")) {
    if (currentParallelColor == red) {
      currentParallelColor = green;
    } else {
      currentParallelColor = red;
    }
  } else {
    currentParallelColor = pixels.Color(random(0, currentLumi), random(0, currentLumi), random(0, currentLumi));
  }
   Serial.println(currentParallelColor);
     Serial.println(blue);
  for (int i = 0; i < 100; i++) {

    if (parallelCount == 0) {
      pixels.setPixelColor(i, currentParallelColor);
      pixels.setPixelColor(180 + i, black);
    }

    if (parallelCount == 1) {
      if (i < 80) {
        pixels.setPixelColor(100 + i, currentParallelColor);
      }
      pixels.setPixelColor(i, black);
    }

    if (parallelCount == 2) {
      if (i < 80) {
        pixels.setPixelColor(100 + i, black);
      }
      pixels.setPixelColor(180 + i, currentParallelColor);
    }
  }
  parallelCount++;
  pixels.show();
  delay(period);
}

void strobo(uint32_t color, int period) {
  for (int i = 0; i < 280; i++) {
    pixels.setPixelColor(i, color);
  }
  pixels.show();
  delay(period);
  for (int i = 0; i < 280; i++) {
    pixels.setPixelColor(i, black);
  }
  pixels.show();
  delay(period);
}


void stroboGrad(uint32_t color, int period) {
  if (period < 1) {
    return;
  }
  for (int i = 0; i < 280; i++) {
    pixels.setPixelColor(i, color);
  }
  pixels.show();
  delay(period);
  for (int i = 0; i < 280; i++) {
    pixels.setPixelColor(i, black);
  }
  pixels.show();
  delay(period);
  stroboGrad(color, period - 4);
}

void dual(String mode, int period) {
  dual(mode, period, 0);
}

uint32_t currentDualColor = 0;
void dual(String mode, int period, int offset) {
  if (mode.equals("")) {
    currentDualColor = pixels.Color(random(0, currentLumi), random(0, currentLumi), random(0, currentLumi));
  } else if (mode.equals("colors")) {
    currentDualColor = pixels.Color(random(0, currentLumi), random(0, currentLumi), random(0, currentLumi));
  } else if (mode.equals("uv/blue")) {
    if (currentDualColor == blue) {
      currentDualColor = uv;
    } else {
      currentDualColor = blue;
    }
  } else if (mode.equals("red/green")) {
    if (currentDualColor == red) {
      currentDualColor = green;
    } else {
      currentDualColor = red;
    }
  }

  for (int i = 0; i < 100; i = i + 1 + offset) {
    pixels.setPixelColor(i, currentDualColor);
    pixels.setPixelColor(180 + i, black);
  }
  pixels.show();
  delay(period);

  for (int i = 0; i < 100; i = i + 1 + offset) {
    pixels.setPixelColor(i, black);
    pixels.setPixelColor(180 + i, currentDualColor);
  }
  pixels.show();
  delay(period);
}

int parallelDownCurrentOffset = 0;
int parallelDownStep = 20;

void parallelDown(String mode, int period) {
  uint32_t colora, colorb;
  if (mode.equals("")) {
    colora = pixels.Color(random(0, currentLumi), random(0, currentLumi), random(0, currentLumi));
    colorb = pixels.Color(random(0, currentLumi), random(0, currentLumi), random(0, currentLumi));
  } else if (mode.equals("colors")) {
    colora = pixels.Color(random(0, currentLumi), random(0, currentLumi), random(0, currentLumi));
    colorb = pixels.Color(random(0, currentLumi), random(0, currentLumi), random(0, currentLumi));
  } else if (mode.equals("uv/blue")) {
    if (parallelDownCurrentOffset % 2 == 0) {
      colora = uv;
      colorb = blue;
    } else {
      colora = blue;
      colorb = uv;
    }
  } else if (mode.equals("red/green")) {
    if (parallelDownCurrentOffset % 2 == 0) {
      colora = green;
      colorb = red;
    } else {
      colora = red;
      colorb = green;
    }

  }

  for (int i = 0; i < parallelDownStep; i++) {
    pixels.setPixelColor((i + 100 + parallelDownStep * (parallelDownCurrentOffset - 1)) % 100, black);
    pixels.setPixelColor((i + 100 + parallelDownStep * (parallelDownCurrentOffset - 1)) % 100 + 180, black);
    pixels.setPixelColor((i + 100 + parallelDownStep * parallelDownCurrentOffset) % 100, colora);
    pixels.setPixelColor((i + 100 + parallelDownStep * parallelDownCurrentOffset) % 100 + 180, colora);

    if (parallelDownCurrentOffset * parallelDownStep < 81) {
      if (parallelDownCurrentOffset * parallelDownStep < 80) {
        pixels.setPixelColor(179 - i - parallelDownStep * parallelDownCurrentOffset, colorb);
      }
      pixels.setPixelColor(179 - i - parallelDownStep * (parallelDownCurrentOffset - 1) , black);
    }
  }

  parallelDownCurrentOffset++;
  if (parallelDownCurrentOffset == 100 / parallelDownStep) {
    parallelDownCurrentOffset = 0;
  }
  pixels.show();
  delay(period);
}

