// CB PLL
// Jef Collin
// V 1.1 2024

// todo



#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPIFFS.h>
#include "Adafruit_MCP23X17.h"

// I2C
// 0x20 port expander



// Include font files
#include "DSEG7ClassicMini_Regular16pt7b.h"
#include "DSEG7ClassicMini_Regular14pt7b.h"
#include "DIN_1451_Mittelschrift_Regular10pt7b.h"
#include "DIN_1451_Mittelschrift_Regular8pt7b.h"

// stored settings file
#define SettingsFile "/SettingsData2"


// squelch
#define PIN_SQ 8

// mcp pins

#define PIN_LD 1

// Define the pins for PLL0 to PLL8
const uint8_t PIN_PLL[] = {8, 9, 10, 11, 12, 13, 14, 15, 0}; // MCP23017 pins


// use alps or other decoder
#define Use_Alps_Encoder false

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// arrays for 7 segment fixed position display, 19 pixels wide and 31 pixels high
uint32_t segments_0[] = {0x0000fff8, 0x0000fff8, 0x00007ff0, 0x00060003, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00060003, 0x00040001, 0x00000000, 0x00040001, 0x00060003, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00060003, 0x00007ff0, 0x0000fff8, 0x0000fff8};
uint32_t segments_1[] = {0x00000000, 0x00000000, 0x00000000, 0x00000003, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000003, 0x00000001, 0x00000000, 0x00000001, 0x00000003, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000003, 0x00000000, 0x00000000, 0x00000000};
uint32_t segments_2[] = {0x0000fff8, 0x0000fff8, 0x00007ff0, 0x00000003, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000003, 0x0000fff9, 0x0001fffc, 0x0004fff8, 0x00060000, 0x00070000, 0x00070000, 0x00070000, 0x00070000, 0x00070000, 0x00070000, 0x00070000, 0x00070000, 0x00070000, 0x00060000, 0x00007ff0, 0x0000fff8, 0x0000fff8};
uint32_t segments_3[] = {0x0000fff8, 0x0000fff8, 0x00007ff0, 0x00000003, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000003, 0x0000fff9, 0x0001fffc, 0x0000fff9, 0x00000003, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000003, 0x00007ff0, 0x0000fff8, 0x0000fff8};
uint32_t segments_4[] = {0x00000000, 0x00000000, 0x00000000, 0x00060003, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00060003, 0x0004fff9, 0x0001fffc, 0x0000fff9, 0x00000003, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000003, 0x00000000, 0x00000000, 0x00000000};
uint32_t segments_5[] = {0x0000fff8, 0x0000fff8, 0x00007ff0, 0x00060000, 0x00070000, 0x00070000, 0x00070000, 0x00070000, 0x00070000, 0x00070000, 0x00070000, 0x00070000, 0x00070000, 0x00060000, 0x0004fff8, 0x0001fffc, 0x0000fff9, 0x00000003, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000003, 0x00007ff0, 0x0000fff8, 0x0000fff8};
uint32_t segments_6[] = {0x0000fff8, 0x0000fff8, 0x00007ff0, 0x00060000, 0x00070000, 0x00070000, 0x00070000, 0x00070000, 0x00070000, 0x00070000, 0x00070000, 0x00070000, 0x00070000, 0x00060000, 0x0004fff8, 0x0001fffc, 0x0004fff9, 0x00060003, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00060003, 0x00007ff0, 0x0000fff8, 0x0000fff8};
uint32_t segments_7[] = {0x0000fff8, 0x0000fff8, 0x00007ff0, 0x00000003, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000003, 0x00000001, 0x00000000, 0x00000001, 0x00000003, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000003, 0x00000000, 0x00000000, 0x00000000};
uint32_t segments_8[] = {0x0000fff8, 0x0000fff8, 0x00007ff0, 0x00060003, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00060003, 0x0004fff9, 0x0001fffc, 0x0004fff9, 0x00060003, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00060003, 0x00007ff0, 0x0000fff8, 0x0000fff8};
uint32_t segments_9[] = {0x0000fff8, 0x0000fff8, 0x00007ff0, 0x00060003, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00070007, 0x00060003, 0x0004fff9, 0x0001fffc, 0x0000fff9, 0x00000003, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000007, 0x00000003, 0x00007ff0, 0x0000fff8, 0x0000fff8};
uint32_t segments_minus[] = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000fff8, 0x0001fffc, 0x0000fff8, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};


// default freq for channel 18
uint16_t Frequency = 27175;
// default channel 18
int Channel = 18;
// alfa channel or not
boolean ChannelIsAlfa = false;
// pll code
uint16_t PLL = 0;

long unsigned PLLLockTimer;

// keep previous freq to compare
uint16_t PreviousFrequency = 0;
// keep previous channel to compare
int PreviousChannel = 0;

// channel or frequency
boolean ChannelMode = true;

// stepsize khz
uint8_t Stepsize = 10;

// default limits for -120 to 120
uint16_t LowerFrequency = 25615;
uint16_t UpperFrequency = 28305;

int LowerChannel = -120;
int UpperChannel = 120;

int MinChannel = LowerChannel;
int MaxChannel = UpperChannel;
uint16_t MinFrequency = 25615;
uint16_t MaxFrequency = 28305;


// scan parameter defaults
int ScanFrom = 1;
int ScanTo = 40;

boolean ScanActive = false;

uint8_t ScanStatus = 0;

long unsigned ScanTimer;
// delay between channel changes
long unsigned ScanDelay = 70;

long unsigned ScanSQHold = 3000;
long unsigned ScanSQTimer;

boolean ScanForceSkipSquelch = false;

// settings data
typedef struct {
  int DefaultChannel;
  int ChannelLowerLimit;
  int ChannelUpperLimit;
  uint16_t FrequencyLowerLimit;
  uint16_t FrequencyUpperLimit;
} SettingsStructure;

SettingsStructure SettingsData = { 18, -120, 120, 25615, 28305};

boolean SettingsData_OK = false;



// screen 0 = default, 1 = setup, 2 set scan from, 3 set scan to, 4 scan, 5 setup from, 6 setup to
uint8_t ScreenMode = 0;
// flag refresh of screen
boolean ChannelFrequencyRefresh = true;

// Menu variables
const int numMenuItems = 5;


const char* menuItems[] = {"Return", "Scan", "Set scan range", "Store default", "Setup limits"};
int selectedMenu = 0;
int menuOffset = 0;

// rotary encoder
// No complete step yet.
#define DIR_NONE 0x0
// Clockwise step.
#define DIR_CW 0x10
// Anti-clockwise step.
#define DIR_CCW 0x20

#if (Use_Alps_Encoder)
// Alps EC11 encoder requires half step tables, others need full step
#define R_START 0x0
#define R_CCW_BEGIN 0x1
#define R_CW_BEGIN 0x2
#define R_START_M 0x3
#define R_CW_BEGIN_M 0x4
#define R_CCW_BEGIN_M 0x5

const unsigned char ttable[6][4] = {
  // R_START (00)
  {R_START_M,            R_CW_BEGIN,     R_CCW_BEGIN,  R_START},
  // R_CCW_BEGIN
  {R_START_M | DIR_CCW, R_START,        R_CCW_BEGIN,  R_START},
  // R_CW_BEGIN
  {R_START_M | DIR_CW,  R_CW_BEGIN,     R_START,      R_START},
  // R_START_M (11)
  {R_START_M,            R_CCW_BEGIN_M,  R_CW_BEGIN_M, R_START},
  // R_CW_BEGIN_M
  {R_START_M,            R_START_M,      R_CW_BEGIN_M, R_START | DIR_CW},
  // R_CCW_BEGIN_M
  {R_START_M,            R_CCW_BEGIN_M,  R_START_M,    R_START | DIR_CCW},
};

#else
// Use the full-step state table (emits a code at 00 only)
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6
#define R_START 0x0

const unsigned char ttable[7][4] = {
  // R_START
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
  // R_CW_FINAL
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
  // R_CW_BEGIN
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
  // R_CW_NEXT
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
  // R_CCW_BEGIN
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
  // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
  // R_CCW_NEXT
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};
#endif

unsigned char Encoder_State1 = R_START;

// rotary encode io pins
char Encoder_Pin1 = 12;
char Encoder_Pin2 = 13;
char Encoder_Key_Pin1 = 11;

// hold until key depressed
boolean Encoder_Key_Long_Press = false;

// track rotary encoder changes
int EncoderCounter = 0;

// timer for buton push length
long unsigned timer_encoderbutton1;


// interrupt routine for rotary encoder
void IRAM_ATTR EncoderInterrupt() {
  unsigned char pinstate = (digitalRead(Encoder_Pin2) << 1) | digitalRead(Encoder_Pin1);
  Encoder_State1 = ttable[Encoder_State1 & 0xf][pinstate];
  unsigned char result = Encoder_State1 & 0x30;
  if (result == DIR_CW) {
    if (EncoderCounter < 10) {
      EncoderCounter++;
    }
  } else if (result == DIR_CCW) {
    if (EncoderCounter > -10) {
      EncoderCounter--;
    }
  }
}

Adafruit_MCP23X17 mcp;




void setup() {
  Serial.begin(115200);

  Wire.begin(9, 10);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

  pinMode(PIN_SQ, INPUT);

  // setup io expander
  mcp.begin_I2C(0x20);

  for (uint8_t i = 0; i < 9; i++) {
    mcp.pinMode(PIN_PLL[i], OUTPUT);
  }

  mcp.pinMode(PIN_LD, INPUT);
  mcp.pinMode(2, INPUT);
  mcp.pinMode(3, INPUT);
  mcp.pinMode(4, INPUT);
  mcp.pinMode(5, INPUT);
  mcp.pinMode(6, INPUT);
  mcp.pinMode(7, INPUT);

  // calculate PLL code for default frequency and set
  CalculateSetPLL(Frequency);

  DisplaySplashScreen();

  // check file system
  if (!SPIFFS.begin()) {
    SPIFFS.format();
    SPIFFS.begin();
    // save settings
    SaveSettings();
  }

  SettingsData_OK = true;
  // check if calibration file exists
  if (SPIFFS.exists(SettingsFile)) {
    File f = SPIFFS.open(SettingsFile, "r");
    // read settings data
    if (f) {
      f.read((byte *)&SettingsData, sizeof(SettingsData));
      // verify data
      if (SettingsData.DefaultChannel < -120 or SettingsData.DefaultChannel > 120) {
        SettingsData.DefaultChannel = 18;
        SettingsData_OK = false;
      }
      if (SettingsData.ChannelLowerLimit < -120 or SettingsData.ChannelLowerLimit > 120) {
        SettingsData.ChannelLowerLimit = -120;
        SettingsData_OK = false;
      }
      if (SettingsData.ChannelUpperLimit < -120 or SettingsData.ChannelUpperLimit > 120) {
        SettingsData.ChannelUpperLimit = 120;
        SettingsData_OK = false;
      }
      if (SettingsData.ChannelUpperLimit < SettingsData.ChannelLowerLimit) {
        SettingsData.ChannelUpperLimit = SettingsData.ChannelLowerLimit;
        SettingsData_OK = false;
      }
      if (SettingsData.FrequencyLowerLimit < 25615 or SettingsData.FrequencyLowerLimit > 28305) {
        SettingsData.FrequencyLowerLimit = 25615;
        SettingsData_OK = false;
      }
      if (SettingsData.FrequencyUpperLimit < 25615 or SettingsData.FrequencyUpperLimit > 28305) {
        SettingsData.FrequencyUpperLimit = 25615;
        SettingsData_OK = false;
      }
      if (SettingsData.FrequencyUpperLimit < SettingsData.FrequencyLowerLimit) {
        SettingsData.FrequencyUpperLimit = SettingsData.FrequencyLowerLimit;
        SettingsData_OK = false;
      }
      Channel = SettingsData.DefaultChannel;
      LowerFrequency = SettingsData.FrequencyLowerLimit;
      UpperFrequency = SettingsData.FrequencyUpperLimit;
      LowerChannel = SettingsData.ChannelLowerLimit;
      UpperChannel = SettingsData.ChannelUpperLimit;
      f.close();
    }
  }
  else {
    SettingsData_OK = false;
  }
  if (!SettingsData_OK) {
    // save settings
    SaveSettings();
  }

  // setup encoder pins
  pinMode(Encoder_Pin1, INPUT);
  pinMode(Encoder_Pin2, INPUT);
  pinMode(Encoder_Key_Pin1, INPUT);

  // display white screen to align oled
  if (digitalRead(Encoder_Key_Pin1) == 0) {
    display.clearDisplay();
    display.fillRect(0, 0, 128, 64, SSD1306_WHITE);
    display.display();
    while (true) {}
  }

  delay(2000);

  // get current state to start otherwise encoder might not react to first click
  unsigned char temppinstate = (digitalRead(Encoder_Pin2) << 1) | digitalRead(Encoder_Pin1);
  Encoder_State1 = ttable[Encoder_State1 & 0xf][temppinstate];

  // setup encoder interrupts
  attachInterrupt(Encoder_Pin1, EncoderInterrupt, CHANGE);
  attachInterrupt(Encoder_Pin2, EncoderInterrupt, CHANGE);

  timer_encoderbutton1 = millis();
  ScanTimer = millis();

}

void loop() {

  switch (ScreenMode) {
    case 0:
      // running mode
      // key 1 short toggle channel or frequency mode, long menu
      if (digitalRead(Encoder_Key_Pin1) == 0) {
        timer_encoderbutton1 = millis();
        Encoder_Key_Long_Press = false;
        // wait until key is no longer pressed or time expired
        while (digitalRead(Encoder_Key_Pin1) == 0) {
          if (millis() - timer_encoderbutton1 > 1000) {
            Encoder_Key_Long_Press = true;
            break;
          }
        }
        if (Encoder_Key_Long_Press) {
          // long press
          ScreenMode = 1;
          selectedMenu = 0;
          menuOffset = 0;
          EncoderCounter = 0;
          drawMenu();
          while (digitalRead(Encoder_Key_Pin1) == 0) {}
          //little debounce
          delay(200);
        }
        else {
          // short press switch mode
          ChannelMode = !ChannelMode;
          // clear alfa channel but no further action, lower normal channel is used if switched from freq to channel
          ChannelIsAlfa = false;
          ChannelFrequencyRefresh = true;
        }
      }

      if (EncoderCounter != 0) {
        if (ChannelMode) {
          // channel mode
          if (EncoderCounter > 0) {
            if (Channel < UpperChannel) {
              // intercept ch 0
              if (Channel == -1) {
                Channel = 0;
              }
              Channel++;
              EncoderCounter--;
            }
            else {
              EncoderCounter = 0;
            }
          }
          else {
            if (EncoderCounter < 0) {
              if (Channel > LowerChannel) {
                // intercept ch 0
                if (Channel == 1) {
                  Channel = 0;
                }
                Channel--;
                EncoderCounter++;
              }
              else {
                EncoderCounter = 0;
              }
            }
          }
          if (Channel != PreviousChannel) {
            ChannelFrequencyRefresh = true;
          }
        }
        else {
          // frequency mode
          if (EncoderCounter > 0) {
            if (Frequency < UpperFrequency) {
              Frequency = Frequency + Stepsize;

              EncoderCounter--;
            }
            else {
              EncoderCounter = 0;
            }
          }
          else {
            if (EncoderCounter < 0) {
              if (Frequency > LowerFrequency) {
                Frequency = Frequency - Stepsize;
                EncoderCounter++;

              }
              else {
                EncoderCounter = 0;
              }
            }
          }
          if (Frequency != PreviousFrequency) {
            ChannelFrequencyRefresh = true;
          }
        }
      }

      if (ChannelFrequencyRefresh) {
        if (ChannelMode) {
          ConvertChannelToFrequency();
          PreviousChannel = Channel;
          UpdateScreenChannelMode();
        }
        else {
          ConvertFrequencyToChannel();
          PreviousFrequency = Frequency;
          UpdateScreenFrequencyMode();
        }

        // calculate PLL code and set
        CalculateSetPLL(Frequency);

        ChannelFrequencyRefresh = false;
      }
      break;


    case 1:
      // menu mode
      // check for key pressed
      if (digitalRead(Encoder_Key_Pin1) == 0) {
        switch (selectedMenu) {
          case 0:
            // return
            ScreenMode = 0;
            ChannelFrequencyRefresh = true;
            break;

          case 1:
            // scan
            DisplayScanStartScreen();
            delay(1000);
            if (ScanFrom < LowerChannel) {
              ScanFrom = LowerChannel;
            }
            if (ScanTo > UpperChannel) {
              ScanTo = UpperChannel;
            }
            Channel = ScanFrom;
            ConvertChannelToFrequency();
            // calculate PLL code and set
            CalculateSetPLL(Frequency);
            ScreenMode = 4;
            ScanStatus = 1;
            DisplayScanScreen();
            ScanTimer = millis();
            break;

          case 2:
            // set scan range
            ScreenMode = 2;
            UpdateScreenScanFrom();
            break;

          case 3:
            // store current channel as default startup
            SettingsData.DefaultChannel = Channel;
            // save settings
            SaveSettings();
            DisplaySavedMessage();
            delay(500);
            drawMenu();
            break;

          case 4:
            // setup channel limits
            ScreenMode = 5;
            UpdateScreenSetupFrom();
            break;


        }
        // wait until key is no longer pressed
        while (digitalRead(Encoder_Key_Pin1) == 0) {}
        // debounce
        delay(100);
      }
      // scroll menu
      if (EncoderCounter != 0) {
        if (EncoderCounter > 0) {
          if (selectedMenu < numMenuItems - 1) {
            selectedMenu++;
            if (selectedMenu >= menuOffset + 3) {
              menuOffset++;
            } else if (selectedMenu < menuOffset) {
              menuOffset--;
            }

            drawMenu();
          }
          EncoderCounter--;
        }
        if (EncoderCounter < 0) {
          if (selectedMenu > 0) {




            selectedMenu--;
            if (selectedMenu >= menuOffset + 3) {
              menuOffset++;
            } else if (selectedMenu < menuOffset) {
              menuOffset--;
            }

            drawMenu();
          }
          EncoderCounter++;
        }
      }
      break;

    case 2:
      // set scan from
      // go to next screen
      if (digitalRead(Encoder_Key_Pin1) == 0) {
        // wait until key is no longer pressed
        while (digitalRead(Encoder_Key_Pin1) == 0) {
        }
        ScreenMode = 3;
        // set to if current setting is too low
        if (ScanTo < ScanFrom) {
          ScanTo = ScanFrom;
        }
        UpdateScreenScanTo();
      }
      else {
        if (EncoderCounter != 0) {
          if (EncoderCounter > 0) {
            if (ScanFrom < UpperChannel) {
              // intercept ch 0
              if (ScanFrom == -1) {
                ScanFrom = 0;
              }
              ScanFrom++;
              EncoderCounter--;
            }
            else {
              EncoderCounter = 0;
            }
          }
          else {
            if (EncoderCounter < 0) {
              if (ScanFrom > LowerChannel) {
                // intercept ch 0
                if (ScanFrom == 1) {
                  ScanFrom = 0;
                }
                ScanFrom--;
                EncoderCounter++;
              }
              else {
                EncoderCounter = 0;
              }
            }
          }
          UpdateScreenScanFrom();
        }
      }
      break;

    case 3:
      // set scan to
      // done, return
      if (digitalRead(Encoder_Key_Pin1) == 0) {
        // wait until key is no longer pressed
        while (digitalRead(Encoder_Key_Pin1) == 0) {
        }
        ScreenMode = 1;
        EncoderCounter = 0;
        drawMenu();
      }
      else {
        if (EncoderCounter != 0) {
          if (EncoderCounter > 0) {
            if (ScanTo < UpperChannel) {
              // intercept ch 0
              if (ScanTo == -1) {
                ScanTo = 0;
              }
              ScanTo++;
              EncoderCounter--;
            }
            else {
              EncoderCounter = 0;
            }
          }
          else {
            if (EncoderCounter < 0) {
              if (ScanTo > LowerChannel and ScanTo > ScanFrom) {
                // intercept ch 0
                if (ScanTo == 1) {
                  ScanTo = 0;
                }
                ScanTo--;
                EncoderCounter++;
              }
              else {
                EncoderCounter = 0;
              }
            }
          }
          UpdateScreenScanTo();
        }
      }
      break;

    case 4:
      // scanning
      // key 1 short toggle scan on off, long cancel scan
      if (digitalRead(Encoder_Key_Pin1) == 0) {
        timer_encoderbutton1 = millis();
        Encoder_Key_Long_Press = false;
        // wait until key is no longer pressed or time expired
        while (digitalRead(Encoder_Key_Pin1) == 0) {
          if (millis() - timer_encoderbutton1 > 1000) {
            Encoder_Key_Long_Press = true;
            break;
          }
        }
        if (Encoder_Key_Long_Press) {
          // long press
          ScreenMode = 0;
          ChannelMode = true;
          ChannelIsAlfa = false;
          ChannelFrequencyRefresh = true;
          EncoderCounter = 0;
          ScanStatus = 0;
          ConvertChannelToFrequency();
          PreviousChannel = Channel;
          UpdateScreenChannelMode();

          // calculate PLL code and set
          CalculateSetPLL(Frequency);

          while (digitalRead(Encoder_Key_Pin1) == 0) {}
          //little debounce
          delay(200);
        }
        else {
          // short press toggle scanning
          if (ScanStatus == 1 or ScanStatus == 3) {
            // scanning, toggle to hold
            if (ScanStatus == 3) {
              // erase in case of sq hold sign
              display.fillRect(115, 0, 127, 14, SSD1306_BLACK);
            }
            display.setFont(&DIN_1451_Mittelschrift_Regular10pt7b);
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(115, 13);
            display.println("H");
            display.display();
            ScanStatus = 2;
          }
          else {
            // resume scanning
            ScanStatus = 1;
          }
          // restart scan delay timer
          ScanTimer = millis();
        }
      }
      else {
        if (ScanStatus == 1) {
          // scanning in progress

          // check SQ
          if (digitalRead(PIN_SQ) and !ScanForceSkipSquelch) {
            ScanStatus = 3;
            display.setFont(&DIN_1451_Mittelschrift_Regular10pt7b);
            display.setTextSize(1);
            display.setTextColor(SSD1306_WHITE);
            // erase in case of hold sign
            display.fillRect(115, 0, 127, 14, SSD1306_BLACK);
            display.setCursor(115, 13);
            display.println("S");
            display.display();
            ScanSQTimer = millis();
          }
          else {
            // check if delay has passed or next channel is forced after sq hold
            // dont change if not locked
            if ((millis() - ScanTimer >= ScanDelay or ScanForceSkipSquelch) and CheckPLLLock()) {
              // next channel
              if (Channel < ScanTo) {
                Channel++;
                // intercept channel 0
                if (Channel == 0) {
                  Channel = 1;
                }
              }
              else {
                // restart
                Channel = ScanFrom;
              }
              DisplayScanScreen();
              // clear any buffered encoder action, must not occure during scan active
              EncoderCounter = 0;

              ConvertChannelToFrequency();
              // calculate PLL code and set
              CalculateSetPLL(Frequency);
              
              // in case of forced rescan check sq and delay if sq is on since radio could hold squelch a brief moment after channel change
              if (ScanForceSkipSquelch) {
                ScanSQTimer = millis();
                while (digitalRead(PIN_SQ) and millis() - ScanSQTimer < 200) {}
                ScanForceSkipSquelch = false;
              }

              // restart delay timer
              ScanTimer = millis();
            }
          }
        }
        else {
          if (ScanStatus == 3) {
            // squelch hold
            if (millis() - ScanSQTimer >= ScanSQHold) {
              // check squelch status after delay elapsed
              if (digitalRead(PIN_SQ)) {
                // still triggered
                // restart sq timer
                ScanSQTimer = millis();
              }
              else {
                // resume scanning
                ScanStatus = 1;
                // restart delay timer
                ScanTimer = millis();
              }
            }
          }
        }
        // if we have encoder action, resume scanning
        if (EncoderCounter != 0 and (ScanStatus == 2 or ScanStatus == 3)) {
          ScanStatus = 1;
          EncoderCounter = 0;
          // override pending squelch hold
          ScanForceSkipSquelch = true;
          // force update and next channel
          ScanTimer = millis() - ScanDelay - 1000;
        }
      }
      break;

    case 5:
      // setup range from
      // go to next screen
      if (digitalRead(Encoder_Key_Pin1) == 0) {
        // wait until key is no longer pressed
        while (digitalRead(Encoder_Key_Pin1) == 0) {
        }
        ScreenMode = 6;
        // set to if current setting is too low
        if (UpperChannel < LowerChannel) {
          UpperChannel = LowerChannel;
        }
        UpdateScreenSetupTo();
      }
      else {
        if (EncoderCounter != 0) {
          if (EncoderCounter > 0) {
            if (LowerChannel < MaxChannel) {
              // intercept ch 0
              if (LowerChannel == -1) {
                LowerChannel = 0;
              }
              LowerChannel++;
              EncoderCounter--;
            }
            else {
              EncoderCounter = 0;
            }
          }
          else {
            if (EncoderCounter < 0) {
              if (LowerChannel > MinChannel) {
                // intercept ch 0
                if (LowerChannel == 1) {
                  LowerChannel = 0;
                }
                LowerChannel--;
                EncoderCounter++;
              }
              else {
                EncoderCounter = 0;
              }
            }
          }
          UpdateScreenSetupFrom();
        }
      }
      break;


    case 6:
      // setup to
      // done, return
      if (digitalRead(Encoder_Key_Pin1) == 0) {
        // wait until key is no longer pressed
        while (digitalRead(Encoder_Key_Pin1) == 0) {
        }
        ScreenMode = 1;
        EncoderCounter = 0;
        drawMenu();
        // store
        SettingsData.ChannelLowerLimit = LowerChannel;
        SettingsData.ChannelUpperLimit = UpperChannel;
        // convert to freq
        Channel = LowerChannel;
        ConvertChannelToFrequency();
        LowerFrequency = Frequency;
        SettingsData.FrequencyLowerLimit = LowerFrequency;
        Channel = UpperChannel;
        ConvertChannelToFrequency();
        UpperFrequency = Frequency;
        SettingsData.FrequencyUpperLimit = UpperFrequency;
        if (SettingsData.DefaultChannel < SettingsData.ChannelLowerLimit) {
          SettingsData.DefaultChannel = SettingsData.ChannelLowerLimit;
        }
        // save settings
        SaveSettings();
        ChannelMode = true;
        Channel = SettingsData.DefaultChannel;
      }
      else {
        if (EncoderCounter != 0) {
          if (EncoderCounter > 0) {
            if (UpperChannel < MaxChannel) {
              // intercept ch 0
              if (UpperChannel == -1) {
                UpperChannel = 0;
              }
              UpperChannel++;
              EncoderCounter--;
            }
            else {
              EncoderCounter = 0;
            }
          }
          else {
            if (EncoderCounter < 0) {
              if (UpperChannel > MinChannel and UpperChannel > LowerChannel) {
                // intercept ch 0
                if (UpperChannel == 1) {
                  UpperChannel = 0;
                }
                UpperChannel--;
                EncoderCounter++;
              }
              else {
                EncoderCounter = 0;
              }
            }
          }
          UpdateScreenSetupTo();
        }
      }
      break;






  }



  // small delay for stability
  delay(2);
}


// convert frequency to channel including alfa
void ConvertFrequencyToChannel(void) {

  uint8_t FrequencyBand;
  uint8_t Lookup;

  // 45 values per band, 40 plus alpha channels
  // tables based on Jumbo 3
  // channel array positive channels
  int ChannelArrayPos[45] = {1, 2, 3, 3, 4, 5, 6, 7, 7, 8, 9, 10, 11, 11, 12, 13, 14, 15, 15, 16, 17, 18, 19, 19, 20, 21, 22, 24, 25, 23, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40};
  // alfa channel array positive channels
  boolean AlfaArrayPos[45] = {false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
  // channel array negative channels
  int ChannelArrayNeg[45] = { -40, -39, -38, -37, -36, -35, -34, -33, -32, -31, -30, -29, -28, -27, -26, -23, -25, -24, -22, -21, -20, -19, -19, -18, -17, -16, -15, -15, -14, -13, -12, -11, -11, -10, -9, -8, -7, -7, -6, -5, -4, -3, -3, -2, -1};
  // alfa channel array negative channels
  boolean AlfaArrayNeg[45] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true, false, false, false};

  // calculate band, 45 per band 10khz per step
  FrequencyBand = (Frequency - MinFrequency) / 450;
  // calculate lookup parameter
  Lookup = ((Frequency - MinFrequency) - (FrequencyBand * 450)) / 10;

  if (FrequencyBand < 3) {
    Channel = ChannelArrayNeg[Lookup];
    ChannelIsAlfa = AlfaArrayNeg[Lookup];
  }
  else {
    Channel = ChannelArrayPos[Lookup];
    ChannelIsAlfa = AlfaArrayPos[Lookup];
  }

  switch (FrequencyBand) {
    case 0:
      // -81 -120
      Channel = Channel - 80;
      break;

    case 1:
      // -41 -80
      Channel = Channel - 40;
      break;

    case 2:
      // -1 -40
      break;

    case 3:
      // 1 40
      break;

    case 4:
      // 41 80
      Channel = Channel + 40;
      break;

    case 5:
      // 81 120
      Channel = Channel + 80;
      break;
  }
}

// convert channel number to frequency
void ConvertChannelToFrequency(void) {
  // freq multiplier array positive channels
  int ChannelArrayPos[45] = {0, 1, 2, 4, 5, 6, 7, 9, 10, 11, 12, 14, 15, 16, 17, 19, 20, 21, 22, 24, 25, 26, 29, 27, 28, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };
  // freq multiplier array negative channels
  int ChannelArrayNeg[45] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 15, 18, 19, 20, 22, 23, 24, 25, 27, 28, 29, 30, 32, 33, 34, 35, 37, 38, 39, 40, 42, 43, 44};

  uint8_t FrequencyBand = 3;
  uint8_t Lookup = 0;

  if (Channel >= -120 and Channel <= -81) {
    FrequencyBand = 0;
    Lookup = 0;
    Lookup = 120 + Channel;
  }
  else {
    if (Channel >= -80 and Channel <= -41) {
      FrequencyBand = 1;
      Lookup = 80 + Channel;
    }
    else {
      if (Channel >= -40 and Channel <= -1) {
        FrequencyBand = 2;
        Lookup = 40 + Channel;
      }
      else {
        if (Channel >= 1 and Channel <= 40) {
          FrequencyBand = 3;
          Lookup = Channel - 1;
        }
        else {
          if (Channel >= 41 and Channel <= 80) {
            FrequencyBand = 4;
            Lookup = Channel - 41;
          }
          else {
            if (Channel >= 81 and Channel <= 120) {
              FrequencyBand = 5;
              Lookup = Channel - 81;
            }
          }
        }
      }
    }
  }

  if (FrequencyBand < 3) {
    Frequency = 26515 + (ChannelArrayNeg[Lookup] * 10) - ((2 - FrequencyBand) * 450);
  }
  else {
    Frequency = 26965 + (ChannelArrayPos[Lookup] * 10) + ((FrequencyBand - 3) * 450);
  }
}

void DisplaySplashScreen(void) {
  const char* text1 = "CB PLL";
  const char* text2 = "Jef Collin";
  const char* text3 = "V1.1 2024";
  display.clearDisplay();
  display.setFont(&DIN_1451_Mittelschrift_Regular10pt7b);
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  // display.cp437(true);         // Use full 256 char 'Code Page 437' font
  // Get text bounds
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text1, 0, 0, &x1, &y1, &w, &h);
  // Calculate the position to start the text to be centered
  int16_t x = (SCREEN_WIDTH - w) / 2;
  int16_t y = h - 1;
  // Draw the text
  display.setCursor(x, y);
  display.println(text1);
  display.setFont(&DIN_1451_Mittelschrift_Regular10pt7b);
  display.getTextBounds(text2, 0, 0, &x1, &y1, &w, &h);
  x = (SCREEN_WIDTH - w) / 2;
  y = (SCREEN_HEIGHT + h) / 2;
  display.setCursor(x, y);
  display.println(text2);
  display.getTextBounds(text3, 0, 0, &x1, &y1, &w, &h);
  x = (SCREEN_WIDTH - w) / 2;
  y = SCREEN_HEIGHT - 1;
  display.setCursor(x, y);
  display.println(text3);
  display.display();
}

void DisplaySavedMessage(void) {
  display.clearDisplay();
  display.setFont(&DIN_1451_Mittelschrift_Regular10pt7b);
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text

  const char* text1 = "Saved...";

  // Get text bounds
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text1, 0, 0, &x1, &y1, &w, &h);

  // Calculate the position to start the text to be centered
  int16_t x = (SCREEN_WIDTH - w) / 2;
  int16_t y = (SCREEN_HEIGHT - h) / 2;

  // Draw the text
  display.setCursor(x, y);
  display.println(text1);
  display.display();
}

void DisplayScanStartScreen(void) {
  display.clearDisplay();
  display.setFont(&DIN_1451_Mittelschrift_Regular10pt7b);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 13);
  display.println("Scanning...");
  display.setFont(&DIN_1451_Mittelschrift_Regular8pt7b);
  display.setCursor(0, 37);
  display.println("Short: pause/cont");
  display.setCursor(0, 60);
  display.println("Long: cancel");
  display.display();
}

void DisplayScanScreen(void) {
  display.clearDisplay();
  display.setFont(&DIN_1451_Mittelschrift_Regular10pt7b);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 13);
  display.println("Scanning...");
  DrawChannel(Channel, 10, 25, 29);

  // Calculate the width of the progress bar
  int progressBarWidth = map(Channel, ScanFrom, ScanTo, 0, 128);

  // Draw the new progress bar
  display.fillRect(0, 62, progressBarWidth, 2, SSD1306_WHITE);

  display.display();
}


// update screen, primary is frequency
void UpdateScreenFrequencyMode(void) {
  display.clearDisplay();
  display.drawLine(0, 45, 127, 45, SSD1306_WHITE);

  String FrequencyString = String(Frequency);

  // Split the string and insert "."
  String part1 = FrequencyString.substring(0, 2); // Get first 2 characters "27"
  String part2 = FrequencyString.substring(2);    // Get the remaining characters "175"

  FrequencyString = part1 + "." + part2; // Combine parts with "."

  String ChannelString = String(Channel);

  if (ChannelIsAlfa) {
    ChannelString = ChannelString + "A";
  }
  display.setFont(&DSEG7ClassicMini_Regular16pt7b);
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 35);
  display.print(FrequencyString);
  display.setFont(&DIN_1451_Mittelschrift_Regular10pt7b);
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 63);
  display.print("Channel:");
  display.setFont(&DIN_1451_Mittelschrift_Regular10pt7b);
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(83, 63);
  display.print(ChannelString);

  if (!CheckPLLLock()) {
    display.setCursor(122, 63);
    display.print("*");
  }

  display.display();
}

// update screen, primary is channel
void UpdateScreenChannelMode(void) {
  display.clearDisplay();
  display.drawLine(0, 45, 127, 45, SSD1306_WHITE);

  String FrequencyString = String(Frequency);

  // Split the string and insert "."
  String part1 = FrequencyString.substring(0, 2); // Get first 2 characters "27"
  String part2 = FrequencyString.substring(2);    // Get the remaining characters "175"

  FrequencyString = part1 + "." + part2; // Combine parts with "."

  DrawChannel(Channel, 10, 5, 29);

  display.setFont(&DIN_1451_Mittelschrift_Regular10pt7b);
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(17, 63);
  display.print(FrequencyString);
  display.setFont(&DIN_1451_Mittelschrift_Regular10pt7b);
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(83, 63);
  display.print("MHz");

  if (!CheckPLLLock()) {
    display.setCursor(122, 63);
    display.print("*");
  }
  display.display();
}


void UpdateScreenScanFrom(void) {
  display.clearDisplay();
  display.setFont(&DIN_1451_Mittelschrift_Regular10pt7b);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 13);
  display.println("Scan from:");
  display.setCursor(0, 63);
  display.println("Push->");
  DrawChannel(ScanFrom, 10, 17, 29);
  display.display();
}

void UpdateScreenScanTo(void) {
  display.clearDisplay();
  display.setFont(&DIN_1451_Mittelschrift_Regular10pt7b);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 13);
  display.println("Scan to:");
  display.setCursor(0, 63);
  display.println("Push->");
  DrawChannel(ScanTo, 10, 17, 29);
  display.display();
}


void UpdateScreenSetupFrom(void) {
  display.clearDisplay();
  display.setFont(&DIN_1451_Mittelschrift_Regular10pt7b);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 13);
  display.println("Setup from:");
  display.setCursor(0, 63);
  display.println("Push->");
  DrawChannel(LowerChannel, 10, 17, 29);
  display.display();
}

void UpdateScreenSetupTo(void) {
  display.clearDisplay();
  display.setFont(&DIN_1451_Mittelschrift_Regular10pt7b);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 13);
  display.println("Setup to:");
  display.setCursor(0, 63);
  display.println("Push->");
  DrawChannel(UpperChannel, 10, 17, 29);
  display.display();
}


// calculate PLL code based on frequency and set pll pins
void CalculateSetPLL(uint16_t FrequencyIn) {
  // Define the range of Frequency and PLL
  // this is based on cybernet PLL02A platform
  const uint16_t FreqMin = 24405;
  const uint16_t FreqMax = 29505;
  const uint16_t PLLMin = 1;
  const uint16_t PLLMax = 511;

  // Ensure Frequency is within the valid range for channel -120 to 120
  if (FrequencyIn < 25615) {
    FrequencyIn = 25615;
  } else if (FrequencyIn > 28305) {
    FrequencyIn = 28305;
  }

  // Perform inverse linear mapping
  uint16_t PLLOut = map(FrequencyIn, FreqMin, FreqMax, PLLMax, PLLMin);

  // calculate and set pll pins
  for (uint8_t i = 0; i < 9; i++) {
    uint8_t PLLbit = (PLLOut >> i) & 1; // Extract the bit at position i
    mcp.digitalWrite(PIN_PLL[i], PLLbit); // Set the pin according to the bit value
  }

  unsigned LockTimer = millis();

  // wait until PLL is locked but not forever
  while (!CheckPLLLock() and millis() - LockTimer < 5 ) {}

}


// check if pll is locked
boolean CheckPLLLock(void) {
  return mcp.digitalRead(PIN_LD);
}


// draw the channel number like 7 segment display
void DrawChannel(int channel_in, int startX, int startY, int spacebetween) {
  String ChannelString = String(channel_in);
  // pad to 4 characters
  while (ChannelString.length() < 4) {
    ChannelString = " " + ChannelString;
  }
  // Cycle through each character and determine its type
  for (int i = 0; i < ChannelString.length(); i++) {
    char ch = ChannelString.charAt(i);
    switch (ch) {
      case ' ':
        break;
      case '-':
        DrawBitmapLines(segments_minus, sizeof(segments_minus) / sizeof(segments_minus[0]), startX, startY);
        break;
      case '0':
        DrawBitmapLines(segments_0, sizeof(segments_0) / sizeof(segments_0[0]), startX, startY);
        break;
      case '1':
        DrawBitmapLines(segments_1, sizeof(segments_1) / sizeof(segments_1[0]), startX, startY);
        break;
      case '2':
        DrawBitmapLines(segments_2, sizeof(segments_2) / sizeof(segments_2[0]), startX, startY);
        break;
      case '3':
        DrawBitmapLines(segments_3, sizeof(segments_3) / sizeof(segments_3[0]), startX, startY);
        break;
      case '4':
        DrawBitmapLines(segments_4, sizeof(segments_4) / sizeof(segments_4[0]), startX, startY);
        break;
      case '5':
        DrawBitmapLines(segments_5, sizeof(segments_5) / sizeof(segments_5[0]), startX, startY);
        break;
      case '6':
        DrawBitmapLines(segments_6, sizeof(segments_6) / sizeof(segments_6[0]), startX, startY);
        break;
      case '7':
        DrawBitmapLines(segments_7, sizeof(segments_7) / sizeof(segments_7[0]), startX, startY);
        break;
      case '8':
        DrawBitmapLines(segments_8, sizeof(segments_8) / sizeof(segments_8[0]), startX, startY);
        break;
      case '9':
        DrawBitmapLines(segments_9, sizeof(segments_9) / sizeof(segments_9[0]), startX, startY);
        break;
    }
    // advance to next digit
    startX = startX + spacebetween;
  }
}

// Draw bitmap lines from the array
void DrawBitmapLines(uint32_t *array, int arraySize, int startX, int startY) {
  // Loop through each entry in the array
  for (int i = 0; i < arraySize; i++) {
    // Extract the 19 LSBs
    uint32_t value = array[i] & 0x7FFFF;

    // Draw the line corresponding to the 19 LSBs (from MSB to LSB)
    for (int bit = 18; bit >= 0; bit--) {
      int x = startX + (18 - bit); // Adjust x position for MSB to LSB drawing
      int y = startY + i;

      if (value & (1 << bit)) {
        display.drawPixel(x, y, SSD1306_WHITE);
      }
    }
  }
}

void drawMenu() {
  display.clearDisplay();
  display.setFont(&DIN_1451_Mittelschrift_Regular10pt7b);
  display.setTextSize(1);      // Normal 1:1 pixel scale

  for (int i = 0; i < 3; i++) {
    int menuIndex = menuOffset + i;
    if (menuIndex >= numMenuItems) break; // Avoid drawing outside menu array

    if (menuIndex == selectedMenu) {
      display.fillRect(0, 0 + (i * 20), 127, 20, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK); // Draw black text on white background
    } else {
      display.setTextColor(SSD1306_WHITE); // Draw white text on black background
    }

    display.setCursor(3, 16 + (i * 20));
    display.print(menuItems[menuIndex]);
  }

  display.display();
}

void SaveSettings(void) {
  // save calibration data
  File f = SPIFFS.open(SettingsFile, "w");
  if (f) {
    f.write((byte *)&SettingsData, sizeof(SettingsData));
    f.close();
  }
}
