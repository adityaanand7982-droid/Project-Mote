#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include "esp_camera.h"

// Camera pins
#define CAMERA_MODEL_XIAO_ESP32S3
#include "camera_pins.h"

#include <TinyGPSPlus.h>
#include <7Semi_BME68x.h>               // 7Semi library
#include <SparkFun_BMV080_Arduino_Library.h> // SparkFun  library
#include <ESP32Time.h>

// XIAO ESP32S3 Sense: SDA = GPIO5, SCL = GPIO6.
#define I2C_SDA_PIN           5
#define I2C_SCL_PIN           6

#define SD_SCK_PIN            7
#define SD_MISO_PIN           8
#define SD_MOSI_PIN           9
#define SD_CS_PIN             3

#define GPS_RX_PIN            44    // ESP32 RX
#define GPS_TX_PIN            43    // ESP32 TX
#define GPS_BAUD              115200 
#define BMV080_ADDR           0x57  // SparkFun BMV080 default I2C

TinyGPSPlus gps;
HardwareSerial gpsSerial(1);   // UART1

ESP32Time rtc(0);              
BME68x_7Semi bme;

SparkFunBMV080 pmSensor;

File logFile;
char csvFilename[32];         

bool cameraReady = false;

struct MoteData {
  double  latitude;
  double  longitude;
  float   altitude;
  bool    gpsValid;
  uint8_t utcHour;
  uint8_t utcMinute;
  uint8_t utcSecond;

  // BME688
  float   temperature;
  float   humidity;
  float   pressure;
  float   gasResistance;

  // BMV080
  float   pm1_0;
  float   pm2_5;
  float   pm10;

  // Image
  char    imageFilename[32];
};

MoteData mote;

const unsigned long MEASUREMENT_INTERVAL_MS = 3000UL;   // 3 seconds, mean every 3 second reading for entire flight time, hehe.
unsigned long lastMeasurementTime = 0;

void   initI2C();
void   initSD();
void   initCamera();
void   initSensors();
void   initGPS();
void   generateCsvFilename();
void   writeCsvHeader();

bool   readGPS(MoteData &d);
bool   readBME688(MoteData &d);
bool   readBMV080(MoteData &d);
bool   captureImage(MoteData &d);
bool   appendCsvRow(const MoteData &d);

void   performMeasurementCycle();
void   syncRtcFromGPS();

void setup() {
  Serial.begin(115200);
  delay(500); 

  Serial.println(F("\n=== Project-Mote Booting ==="));

  // -- I2C bus 
  initI2C();

  // -- SD card 
  initSD();

  // -- Camera 
  initCamera();

  // -- Sensors
  initSensors();

  // -- GPS serial 
  initGPS();

  // -- CSV 
  generateCsvFilename();
  writeCsvHeader();

  Serial.println(F("=== Boot complete. Starting main loop. ===\n"));
  lastMeasurementTime = millis();
}


void loop() {
  // 1. feed GPS parser (never block on this for your mind).
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  // 2. time 
  unsigned long now = millis();
  if (now - lastMeasurementTime >= MEASUREMENT_INTERVAL_MS) {
    lastMeasurementTime = now;
    performMeasurementCycle();
  }

  // 3.RTC 
  if (gps.time.isUpdated()) {
    syncRtcFromGPS();
  }
}


void initI2C() {
  Wire.setPins(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.begin();
  Wire.setClock(400000);   
  Serial.println(F("[I2C] Bus initialised on SDA=5, SCL=6"));
}

void initSD() {
  SPI.begin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println(F("[SD] Mount failed! Check card and wiring."));
    return;
  }

  
  if (!SD.exists("/logs"))  SD.mkdir("/logs");
  if (!SD.exists("/images")) SD.mkdir("/images");

  Serial.println(F("[SD] Card mounted successfully."));
}

void initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;

  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

 
  if (psramFound()) {
    config.frame_size   = FRAMESIZE_SVGA;    
    config.jpeg_quality = 12;               
    config.fb_count     = 2;                 
    config.fb_location  = CAMERA_FB_IN_PSRAM;
    config.grab_mode    = CAMERA_GRAB_LATEST;
  } else {
    config.frame_size   = FRAMESIZE_QVGA;    
    config.jpeg_quality = 20;
    config.fb_count     = 1;
    config.fb_location  = CAMERA_FB_IN_DRAM;
    config.grab_mode    = CAMERA_GRAB_WHEN_EMPTY;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("[CAM] Init failed with error 0x%x\n", err);
    cameraReady = false;
    return;
  }

  cameraReady = true;
  Serial.println(F("[CAM] Camera initialised (SVGA JPEG, PSRAM)."));
}

void initSensors() {

  if (!bme.begin()) {
    Serial.println(F("[BME688] Not found! Check wiring and I2C address (0x77)."));
  } else {
    Serial.println(F("[BME688] Initialised."));
  }

  
  if (pmSensor.begin(BMV080_ADDR, Wire) == false) {
    Serial.println(F("[BMV080] Init failed! Check SDK files and wiring."));
  } else {
    pmSensor.init();
    if (pmSensor.setMode(SF_BMV080_MODE_CONTINUOUS)) {
      Serial.println(F("[BMV080] Initialised in continuous mode."));
    } else {
      Serial.println(F("[BMV080] Failed to set continuous mode."));
    }
  }
}
void initGPS() {
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  Serial.printf("[GPS] UART1 started on RX=%d, TX=%d @ %d baud\n",
                GPS_RX_PIN, GPS_TX_PIN, GPS_BAUD);
}

void generateCsvFilename() {
 
  int flightNum = 1;
  char candidate[32];

  do {
    snprintf(candidate, sizeof(candidate), "/logs/flight_log_%02d.csv", flightNum);
    flightNum++;
  } while (SD.exists(candidate) && flightNum < 100);

  strncpy(csvFilename, candidate, sizeof(csvFilename) - 1);
  csvFilename[sizeof(csvFilename) - 1] = '\0';
  Serial.printf("[CSV] New log file: %s\n", csvFilename);
}

void writeCsvHeader() {
  logFile = SD.open(csvFilename, FILE_WRITE);
  if (!logFile) {
    Serial.println(F("[CSV] Failed to create file for header."));
    return;
  }

  logFile.println(F("Timestamp,Lat,Lon,Alt_m,Temp_C,Humidity_%,Pressure_hPa,"
                    "Gas_Res_Ohm,PM1_0,PM2_5,PM10,Image_Filename"));
  logFile.close();
  Serial.println(F("[CSV] Header written."));
}


void performMeasurementCycle() {
  Serial.println(F("\n--- Measurement Cycle ---"));

 
  memset(&mote, 0, sizeof(mote));
  mote.gpsValid = false;

  
  readGPS(mote);

 
  readBME688(mote);
  readBMV080(mote);

  
  captureImage(mote);

  
  appendCsvRow(mote);

  Serial.println(F("--- Cycle complete ---"));
}


bool readGPS(MoteData &d) {
  if (gps.location.isValid() && gps.location.age() < 2000) {
    d.latitude  = gps.location.lat();
    d.longitude = gps.location.lng();
    d.altitude  = gps.altitude.meters();
    d.gpsValid  = true;

    d.utcHour   = gps.time.hour();
    d.utcMinute = gps.time.minute();
    d.utcSecond = gps.time.second();

    Serial.printf("[GPS] Lat=%.6f Lon=%.6f Alt=%.1f m  Time=%02d:%02d:%02d UTC\n",
                  d.latitude, d.longitude, d.altitude,
                  d.utcHour, d.utcMinute, d.utcSecond);
  } else {
    Serial.println(F("[GPS] No valid fix. Using placeholder values."));
    d.latitude  = 0.0;
    d.longitude = 0.0;
    d.altitude  = 0.0;
    d.gpsValid  = false;
  }
  return d.gpsValid;
}

void syncRtcFromGPS() {
  
  if (gps.date.isValid() && gps.time.isValid()) {
    rtc.setTime(gps.time.second(),
                gps.time.minute(),
                gps.time.hour(),
                gps.date.day(),
                gps.date.month(),
                gps.date.year());
  }
}


bool readBME688(MoteData &d) {
  
  if (!bme.getData(d.temperature, d.humidity, d.pressure, d.gasResistance)) {
    Serial.println(F("[BME688] Read failed."));
    d.temperature    = -999.0f;
    d.humidity       = -999.0f;
    d.pressure       = -999.0f;
    d.gasResistance  = -999.0f;
    return false;
  }

  
  d.pressure = d.pressure / 100.0f;

  Serial.printf("[BME688] T=%.2f C  RH=%.2f %%  P=%.2f hPa  Gas=%.2f kOhm\n",
                d.temperature, d.humidity, d.pressure, d.gasResistance / 1000.0f);
  return true;
}

bool readBMV080(MoteData &d) {
  if (!pmSensor.readSensor()) {
    Serial.println(F("[BMV080] No new data available."));
    d.pm1_0 = -1.0f;
    d.pm2_5 = -1.0f;
    d.pm10  = -1.0f;
    return false;
  }

  d.pm1_0 = pmSensor.PM1();
  d.pm2_5 = pmSensor.PM25();
  d.pm10  = pmSensor.PM10();

  Serial.printf("[BMV080] PM1.0=%.1f  PM2.5=%.1f  PM10=%.1f ug/m3\n",
                d.pm1_0, d.pm2_5, d.pm10);
  return true;
}


bool captureImage(MoteData &d) {
  if (!cameraReady) {
    strncpy(d.imageFilename, "NO_CAM", sizeof(d.imageFilename));
    Serial.println(F("[CAM] Camera not ready. Skipping."));
    return false;
  }

  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    strncpy(d.imageFilename, "NO_FRAME", sizeof(d.imageFilename));
    Serial.println(F("[CAM] Frame capture failed."));
    return false;
  }


  if (d.gpsValid) {
    snprintf(d.imageFilename, sizeof(d.imageFilename),
             "/images/%02d%02d%02d.jpg", d.utcHour, d.utcMinute, d.utcSecond);
  } else {
    
    snprintf(d.imageFilename, sizeof(d.imageFilename),
             "/images/%lu.jpg", millis());
  }

  
  File imgFile = SD.open(d.imageFilename, FILE_WRITE);
  if (!imgFile) {
    Serial.printf("[CAM] Failed to open %s for writing.\n", d.imageFilename);
    esp_camera_fb_return(fb);
    strncpy(d.imageFilename, "WRITE_FAIL", sizeof(d.imageFilename));
    return false;
  }

  size_t written = imgFile.write(fb->buf, fb->len);
  imgFile.close();
  esp_camera_fb_return(fb);  

  if (written != fb->len) {
    Serial.printf("[CAM] Partial write: %u / %u bytes.\n", written, fb->len);
    return false;
  }

  Serial.printf("[CAM] Saved %u bytes -> %s\n", written, d.imageFilename);
  return true;
}


bool appendCsvRow(const MoteData &d) {
  
  logFile = SD.open(csvFilename, FILE_APPEND);
  if (!logFile) {
    Serial.println(F("[CSV] Failed to open for append."));
    return false;
  }

 
  char line[256];
  int n = snprintf(line, sizeof(line),
                   "%02d:%02d:%02d,%.6f,%.6f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%s",
                   d.utcHour, d.utcMinute, d.utcSecond,
                   d.latitude, d.longitude, d.altitude,
                   d.temperature, d.humidity, d.pressure, d.gasResistance,
                   d.pm1_0, d.pm2_5, d.pm10,
                   d.imageFilename);

  if (n <= 0 || n >= (int)sizeof(line)) {
    Serial.println(F("[CSV] Line formatting error."));
    logFile.close();
    return false;
  }

  logFile.println(line);
  logFile.close();   
  Serial.printf("[CSV] Row appended: %s\n", line);
  return true;
}
 // if you reading this, you need to touch grass. 