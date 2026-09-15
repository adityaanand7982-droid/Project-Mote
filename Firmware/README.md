# Firmware

This directory contains the complete firmware for the Project-Mote payload.
It runs on the XIAO ESP32-S3 Sense and handles sensor sampling, GPS parsing,
image capture, and CSV logging — all autonomously, with no operator
interaction required after power-up.

## Firmware Specifications

- **Target:** Seeed Studio XIAO ESP32-S3 Sense
- **Framework:** Arduino (ESP32 core 3.x)
- **Cycle interval:** 3 seconds (configurable)
- **Flash usage:** 511 KB / 3.3 MB (15%)
- **DRAM usage:** 52 KB / 327 KB (15%)
- **Blocking calls in main loop:** None
- **Boot behavior:** Fully autonomous like once uploaded it'll just after powering up and starts logging immediately after the sensor checkups 

## Design Specifications

- Non-blocking cooperative scheduler built entirely on `millis()`. No
  `delay()` anywhere in the runtime path.
- GPS UART is drained on every loop iteration, so no fix is ever missed at
  115200 baud.
- Every file write follows open → write → close, with no persistent file
  handles. Safe against mid-flight power loss.
- Automatic numbered log rotation on boot — previous flights are never
  overwritten.
- Camera configured for SVGA JPEG at quality 12, double-buffered in PSRAM
  to keep DRAM free for the sensor stack.
- Graceful degradation: any single sensor, the camera, or the SD card can
  fail without stopping the payload.

## What It Does

Every 3 seconds, the firmware:

1. **Reads GPS** — latitude, longitude, altitude, UTC time (NMEA @ 115200).
2. **Samples the BME688** — temperature, humidity, pressure, gas resistance.
3. **Samples the BMV080** — PM1.0, PM2.5, PM10 in continuous mode.
4. **Captures a JPEG** with the onboard OV2640.
5. **Logs one CSV row** and writes the image with a GPS-timestamped filename.

GPS time syncs the ESP32 internal RTC on every valid fix, so all timestamps
are UTC-accurate end-to-end.

## Execution Flow

**Boot:**
```
initI2C → initSD → initCamera → initSensors → initGPS
        → generateCsvFilename → writeCsvHeader
```

**Loop:**
```
drain GPS UART → gps.encode()
if 3 s elapsed → performMeasurementCycle()
if GPS time updated → syncRtcFromGPS()
```

**Measurement cycle:**
```
memset(&mote) → readGPS → readBME688 → readBMV080
              → captureImage → appendCsvRow
```

Typical cycle time is ~240 ms. Worst case is ~1.1 s. Well under the 3 s
interval, so cycles never overlap.

## Output

**`/logs/flight_log_NN.csv`** — one row per cycle:
```
Timestamp,Lat,Lon,Alt_m,Temp_C,Humidity_%,Pressure_hPa,
Gas_Res_Ohm,PM1_0,PM2_5,PM10,Image_Filename
```

**`/images/HHMMSS.jpg`** — GPS-timestamped JPEG. Falls back to
`/images/<millis>.jpg` if there is no fix.

**Placeholder values on failure:**

| Value | Meaning |
|---|---|
| `0.000000` (lat/lon) | No GPS fix |
| `-999.0` | BME688 read failed |
| `-1.0` | BMV080 no new data yet |
| `NO_CAM` / `NO_FRAME` / `WRITE_FAIL` | Camera failure |

## Required Libraries

I used following Libraries and it's avilaible on Aurdino library manager

- **TinyGPSPlus** by Mikal Hart (1.0.3)
- **7Semi BME68x** by 7Semi
- **SparkFun BMV080 Arduino Library** by SparkFun (1.0.4)
- **ESP32Time** by fbiego

> **The SparkFun BMV080 library requires the Bosch BMV080 SDK.** You must download
> the SDK from Bosch Sensortec and copy four files into the SparkFun library
> folder (`bmv080.h`, `bmv080_defs.h`, `lib_bmv080.a`, `lib_postProcessor.a`).
> Download from official Bosch website [Bosch Sensortec](https://www.bosch-sensortec.com/en/products/environmental-sensors/particulate-matter-sensor/bmv080#documents) (requires accepting the license agreement though).
## Board Settings (Arduino IDE)

| Setting | Value |
|---|---|
| Board | `XIAO_ESP32S3` |
| **PSRAM** | **OPI PSRAM** ← its really required for camera |
| Flash Size | 8MB (64Mb) |
| USB CDC On Boot | Enabled |
| CPU Frequency | 240 MHz |

## Configuration

Tunables live at the top of `ProjectMote.ino`:

```cpp
#define GPS_BAUD              115200   // Match your GPS module
#define BMV080_ADDR           0x57
#define I2C_SDA_PIN           5
#define I2C_SCL_PIN           6
#define SD_CS_PIN             3

const unsigned long MEASUREMENT_INTERVAL_MS = 3000UL;
```

**Do not go below `1000UL`.** The BMV080 produces a new reading once per
second. Faster cycles will log `-1.0` on alternating rows.

## Health Check

Open Serial Monitor at **115200 baud**. A healthy boot looks something like this:

```
=== Project-Mote Booting ===
[I2C] Bus initialised on SDA=5, SCL=6
[SD] Card mounted successfully.
[CAM] Camera initialised (SVGA JPEG, PSRAM).
[BME688] Initialised.
[BMV080] Initialised in continuous mode.
[GPS] UART1 started on RX=44, TX=43 @ 115200 baud
[CSV] New log file: /logs/flight_log_01.csv
[CSV] Header written.
=== Boot complete. Starting main loop. ===
```

Every line is prefixed with a `[TAG]` so failures are easy to grep. If a
line is missing or shows an error, that peripheral failed and the firmware
will continue with a placeholder in its place.

## Failure Modes

| Failure | Behavior |
|---|---|
| SD card not inserted | Logs error, keeps reading sensors, writes nothing |
| Camera init fails | CSV logs `NO_CAM`, other data still logged |
| BME688 I²C fail | CSV logs `-999.0`, continues |
| BMV080 no data | CSV logs `-1.0`, continues |
| GPS no fix | Lat/lon `0.000000`, uses `millis()` for filename |
| Entire I²C bus dead | Payload still logs GPS + images |

Single peripheral failure never takes down the rest.

## Known Limitations

- One image per UTC second — collision only possible below a 1 s interval.
- No SD card hot-swap; card must be inserted before boot.
- GPS must output **NMEA 0183** at 115200 baud (see main README).

## File Structure

- `ProjectMote.ino` — main firmware sketch
- `camera_pins.h` — OV2640 pin mappings for the XIAO ESP32S3 Sense
  
## Suggestions & Feedback

Open an issue or PR on the repo. Firmware bugs and hardware compatibility
notes are welcome.
* ** Lead Engineer: Aryabharta Tenjuku
* ** Email:<img width="231" height="19" alt="email" src="https://github.com/user-attachments/assets/14993efc-37d6-41a4-9da8-2d3e5d72ae48" />
>  **RUNTIME WARNING:** Rated for continuous autonomous logging across an
> entire flight with zero operator input, graceful sensor degradation, and
> power-loss-safe file writes.This firmware will run perfectly fine with a dead,
> missing, or exFAT-formatted SD card — you just won't find out until you land.
