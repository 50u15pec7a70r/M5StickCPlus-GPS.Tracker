#include <M5StickCPlus.h>
#include <TinyGPSPlus.h>

static const uint32_t GPSBaud = 9600;

int tcount = 0;
int LCD = 2;

double homeLat = 0;
double homeLng = 0;
double homeAlt = 0;

bool homeIsSet = false ;

TinyGPSPlus gps;

TFT_eSprite Disbuff = TFT_eSprite(&M5.Lcd);

HardwareSerial GPSRaw(2);

void buttons_code() {
  // Button A control the LCD (ON/OFF) and brightness
  if (M5.BtnA.pressedFor(2000)) {
    LCD = (LCD + 1) % 3;
    if (LCD == 2) {
      turn_on_lcd();
      set_maximum_brightness();
    } else if (LCD == 1) {
      turn_on_lcd();
      set_minimum_brightness();
    } else {
      turn_off_lcd();
      set_minimum_brightness();
    }
  }
  /*
  // Button B doing a time resync if pressed for 2 sec
  if (M5.BtnA.wasPressed()) {
    infoShow();
  }
  */
  if (M5.BtnB.pressedFor(2000)) {
    setHome();
  }
}

void setHome() {
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(65, 60);
  M5.Lcd.fillRect(19, 49, 202, 32,  M5.Lcd.color565(255, 255, 255));
  M5.Lcd.fillRect(20, 50, 200, 30,  M5.Lcd.color565(0, 0, 0));
  if (gps.location.isValid())
  {
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.fillRect(20, 50, 200, 30,  M5.Lcd.color565(60, 100, 60));
    M5.Lcd.setCursor(70, 60);
    M5.Lcd.printf("home point saved");
    homeLat = gps.location.lat();
    homeLng = gps.location.lng();
    homeAlt = gps.altitude.meters();
    homeIsSet = true ;
  }
  else
  {
    M5.Lcd.fillRect(20, 50, 200, 30,  M5.Lcd.color565(100, 60, 60));
    M5.Lcd.setTextColor(RED);
    M5.Lcd.setCursor(70, 60);
    M5.Lcd.printf("LOCATION INVALID");
  }
  delay(2500);
}

void infoShow() {
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(65, 55);
  M5.Lcd.fillRect(19, 49, 202, 32,  M5.Lcd.color565(255, 255, 255));
  M5.Lcd.fillRect(20, 50, 200, 30,  M5.Lcd.color565(0, 0, 0));
  M5.Lcd.printf("simple GPS tracker");
  M5.Lcd.setCursor(70, 67);
  M5.Lcd.printf("by 50u15pec7a70r");
  delay(2500);
}

void turn_off_lcd() {
  M5.Axp.SetLDO2(false);
}

void turn_on_lcd() {
  M5.Axp.SetLDO2(true);
}

void set_maximum_brightness() {
  M5.Axp.ScreenBreath(12);
}

void set_minimum_brightness() {
  M5.Axp.ScreenBreath(9);
}

void doScreen() {
  //if (timeToDo(1000)) {
  Disbuff.fillRect(0, 0, 240, 135, Disbuff.color565(0, 0, 0));
  Disbuff.setTextColor(WHITE, BLACK);
  Disbuff.setTextSize(1);
  Disbuff.fillScreen (BLACK);
  Disbuff.setCursor(0,10);

  Disbuff.print(F("Satellites: "));
  if (gps.satellites.isValid())
  {
    if (gps.satellites.value() == 0)
    {
    Disbuff.setTextColor(RED, BLACK);  
    }
    else
    {
    Disbuff.setTextColor(GREEN, BLACK);
    }
    Disbuff.print(gps.satellites.value(), 5);
    Disbuff.setTextColor(WHITE, BLACK);
  }
  else
  {
    Disbuff.setTextColor(RED, BLACK);
    Disbuff.print(F("INVALID"));
    Disbuff.setTextColor(WHITE, BLACK);
  }
  //===============
  Disbuff.print(F("\n\nLocation: "));
  if (gps.location.isValid())
  {
    Disbuff.setTextColor(GREEN, BLACK);
    Disbuff.print(gps.location.lat(), 6);
    Disbuff.setTextColor(WHITE, BLACK);
    Disbuff.print(F(" , " ));
    Disbuff.setTextColor(GREEN, BLACK);
    Disbuff.print(gps.location.lng(), 6);
    Disbuff.setTextColor(WHITE, BLACK);
  }
  else
  {
    Disbuff.setTextColor(RED, BLACK);
    Disbuff.print(F("INVALID"));
    Disbuff.setTextColor(WHITE, BLACK);
  }
  //===============
  Disbuff.print(F("\n\nLocation age (ms): "));
  if (gps.location.isValid())
  {
    Disbuff.setTextColor(GREEN, BLACK);
    Disbuff.print( (double) gps.location.age(), 0);
    Disbuff.setTextColor(WHITE, BLACK);
  }
  else
  {
    Disbuff.setTextColor(RED, BLACK);
    Disbuff.print(F("INVALID"));
    Disbuff.setTextColor(WHITE, BLACK);
  }
  //===============
  Disbuff.print(F("\n\nGPS Time: "));
  if (gps.time.isValid())
  {
    Disbuff.setTextColor(GREEN, BLACK);
    Disbuff.printf("%02d",gps.time.hour()); //UTC+3
    Disbuff.setTextColor(WHITE, BLACK);
    Disbuff.print(F(":"));
    Disbuff.setTextColor(GREEN, BLACK);
    Disbuff.printf("%02d",gps.time.minute());
    Disbuff.setTextColor(WHITE, BLACK);
    Disbuff.print(F(":"));
    Disbuff.setTextColor(GREEN, BLACK);
    Disbuff.printf("%02d",gps.time.second());
    Disbuff.setTextColor(WHITE, BLACK);
    Disbuff.print(F(" | UTC+3: "));
    Disbuff.setTextColor(GREEN, BLACK);
    Disbuff.printf("%02d",gps.time.hour()+3); //UTC+3
    Disbuff.setTextColor(WHITE, BLACK);
    Disbuff.print(F(":"));
    Disbuff.setTextColor(GREEN, BLACK);
    Disbuff.printf("%02d",gps.time.minute());
    Disbuff.setTextColor(WHITE, BLACK);
    Disbuff.print(F(":"));
    Disbuff.setTextColor(GREEN, BLACK);
    Disbuff.printf("%02d",gps.time.second());
    Disbuff.setTextColor(WHITE, BLACK);
  }
  else
  {
    Disbuff.setTextColor(RED, BLACK);
    Disbuff.print(F("INVALID"));
    Disbuff.setTextColor(WHITE, BLACK);
  }
  //===============
  Disbuff.print(F("\n\nGPS Date: "));
  if (gps.date.isValid())
  {
    Disbuff.setTextColor(GREEN, BLACK);
    Disbuff.print(gps.date.day());
    Disbuff.setTextColor(WHITE, BLACK);
    Disbuff.print(F("/"));
    Disbuff.setTextColor(GREEN, BLACK);
    Disbuff.print(gps.date.month());
    Disbuff.setTextColor(WHITE, BLACK);
    Disbuff.print(F("/"));
    Disbuff.setTextColor(GREEN, BLACK);
    Disbuff.print(gps.date.year());
    Disbuff.setTextColor(WHITE, BLACK);
  }
  else
  {
    Disbuff.setTextColor(RED, BLACK);
    Disbuff.print(F("INVALID"));
    Disbuff.setTextColor(WHITE, BLACK);
  }
  //===============
  Disbuff.print(F("\n\nAltitude: "));
  if (gps.altitude.isValid())
  {
    Disbuff.setTextColor(GREEN, BLACK);
    Disbuff.print(gps.altitude.meters(),2);
    Disbuff.setTextColor(WHITE, BLACK);
  }
  else
  {
    Disbuff.setTextColor(RED, BLACK);
    Disbuff.print(F("INVALID"));
    Disbuff.setTextColor(WHITE, BLACK);
  }
  //===============
  Disbuff.print(F("\n\nCourse: "));
  if (gps.course.isValid())
  {
    Disbuff.setTextColor(GREEN, BLACK);
    Disbuff.print(gps.course.deg(),2);
    Disbuff.setTextColor(WHITE, BLACK);
  }
  else
  {
    Disbuff.setTextColor(RED, BLACK);
    Disbuff.print(F("INVALID"));
    Disbuff.setTextColor(WHITE, BLACK);
  }
  //===============
  Disbuff.print(F("\n\nSpeed: "));
  if (gps.speed.isValid())
  {
    Disbuff.setTextColor(GREEN, BLACK);
    Disbuff.print(gps.speed.kmph(),2);
    Disbuff.setTextColor(WHITE, BLACK);
  }
  else
  {
    Disbuff.setTextColor(RED, BLACK);
    Disbuff.print(F("INVALID"));
    Disbuff.setTextColor(WHITE, BLACK);
  }
  //===============

  //printInt(gps.charsProcessed(), true, 6);
  //printInt(gps.sentencesWithFix(), true, 10);
  //printInt(gps.failedChecksum(), true, 9);

  Disbuff.setCursor(100,105);
  Disbuff.print(F("Alt/diff: "));
  if ( gps.satellites.value()!=0 && homeIsSet)
  {
    /*
    unsigned long distanceKmToHome =
        (unsigned long)TinyGPSPlus::distanceBetween(
            gps.location.lat(), gps.location.lng(), homeLat, homeLng) /
        1000;
    */
    double altitureDiffToHome = homeAlt - gps.altitude.meters();
    Disbuff.setTextColor(GREEN, BLACK);
    Disbuff.print(altitureDiffToHome,2);
    Disbuff.setTextColor(WHITE, BLACK);
  }
  else
  {
    Disbuff.setTextColor(RED, BLACK);
    Disbuff.print(F("INVALID"));
    Disbuff.setTextColor(WHITE, BLACK);
  }

  Disbuff.setCursor(100,121);
  Disbuff.print(F("Distance: "));
  if ( gps.satellites.value()!=0 and homeIsSet)
  {
    /*
    unsigned long distanceKmToHome =
        (unsigned long)TinyGPSPlus::distanceBetween(
            gps.location.lat(), gps.location.lng(), homeLat, homeLng) /
        1000;
    */
    double distanceMToHome =
        TinyGPSPlus::distanceBetween(
            gps.location.lat(), gps.location.lng(), homeLat, homeLng);
    Disbuff.setTextColor(GREEN, BLACK);
    Disbuff.print(distanceMToHome,2);
    Disbuff.setTextColor(WHITE, BLACK);
  }
  else
  {
    Disbuff.setTextColor(RED, BLACK);
    Disbuff.print(F("INVALID"));
    Disbuff.setTextColor(WHITE, BLACK);
  }

  //double courseToLondon = TinyGPSPlus::courseTo(
  //    gps.location.lat(), gps.location.lng(), LONDON_LAT, LONDON_LON);
  //printFloat(courseToLondon, gps.location.isValid(), 7, 2);
  //const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToLondon);
  //printStr(gps.location.isValid() ? cardinalToLondon : "*** ", 6);

  //===============
  Disbuff.setTextSize(1);
  Disbuff.setTextColor(WHITE);
  Disbuff.setCursor(140, 10);
  Disbuff.printf("battery: ");
  Disbuff.setTextColor(GREEN);
  Disbuff.printf("%.1f%%", getBatteryLevel());

  if (millis() > 5000 && gps.charsProcessed() < 10) 
  {
    Disbuff.setTextColor(RED, BLACK);
    Disbuff.setTextSize(1);
    Disbuff.fillScreen (BLACK);
    Disbuff.setCursor(0,10);
    Disbuff.println(F("No GPS data received: check wiring!"));
  }
  Disbuff.pushSprite(0,0);

  smartDelay(1000); //Update every 1 second(s)
}

bool timeToDo(int tbase) {
  tcount++;
  if (tcount == tbase) {
    tcount = 0;
    return true;
  } else {
    return false;
  }
}

static void smartDelay(unsigned long ms) {
    unsigned long start = millis();
    do {
        while (GPSRaw.available()) gps.encode(GPSRaw.read());
    } while (millis() - start < ms);
}

double getBatteryLevel(void)
{
  uint16_t vbatData = M5.Axp.GetVbatData();
  double vbat = vbatData * 1.1 / 1000;
  double dBatLvl = 100.0 * ((vbat - 3.0) / (4.07 - 3.0));
  if ( dBatLvl > 100.0 ) {
    return 100.0;
  }
  else {
    return dBatLvl;
  }
}

void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(0,10);
    M5.Lcd.println("Initializing GPS Port...");
    GPSRaw.begin(GPSBaud,SERIAL_8N1, 33, 32 ); //33, 32
    M5.Lcd.setTextColor(GREEN, BLACK);
    M5.Lcd.println("\nGPS Port is initialized!");
    delay (2000);
    Disbuff.createSprite(240, 135);
    Disbuff.fillRect(0, 0, 240, 135, Disbuff.color565(0, 0, 0));
    Disbuff.pushSprite(0, 0);
}

void loop() {
  M5.update();
  buttons_code();
  doScreen();
}


