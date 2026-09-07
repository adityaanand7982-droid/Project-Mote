# Project-Mote
A high-altitude, open-source data collection payload engineered for drone deployment. It replaces heavy, expensive stationary microclimate rigs with a lightweight, 3D-printable pod designed to capture real-time environmental telemetry.

## Custom Features

* **Venturi Airflow Dynamics:** Custom-engineered internal channeling prevents air stagnation, ensuring sensors sample live ambient air during flight and for proper thermal managment so that sensor and microcontroller heat doesnt interfare with the measurements.
* **Li-Ion Flight Platform:** Runs entirely on a standard high-efficiency Li-ion cell, eliminating heavy LiPo packs to maximize airborne observation time and minimize mass.
* **Autonomous Data Logging:** Native MicroSD card integration writes sequential `.csv` telemetry directly to memory for post-flight analysis.
* **Sensors:** Features the Bosch BME688 (microclimate) and Bosch BMV080 (particulates) operating on a shared I2C bus.
* **Custom Housing:** Designed for weight efficency and max flight duration and to avoid damiging the sensors.
## System Architecture: The Symbiotic Flight Loop

Project-Mote is designed with a core engineering philosophy: the drone is not just a delivery vehicle; it is an active, integrated component of the payload. The hardware and software stacks operate in a continuous, synchronized cycle during flight.

Here is the exact step-by-step breakdown of how the system functions in the air:

### 1. Mechanical & Aerodynamic Integration
* **Ram-Air Pumping:** The pod deliberately lacks internal fans. It relies entirely on the drone’s forward pitch and flight velocity to force ambient air through the custom 3D-printed venturi grilles. The drone's physical movement acts as the system's air pump, ensuring zero air stagnation.
* **Parasitic-Free Power:** While the drone and pod share a data link, they do not share a power draw. The payload runs on its own independent, high-efficiency Li-ion cell. This guarantees the environmental sensors never drain the flight controller's battery or compromise maximum flight time.

### 2. The Data & Telemetry Handshake
* **Microclimate Polling:** At exactly 5-second intervals, the "Cyber-Spider" PCB acts as the mission commander, pinging the onboard BME688 and BMV080 sensors over the I2C bus to capture a real-time snapshot of the immediate atmosphere.
* **The Drone as an Antenna:** To strictly minimize mass, the pod carries no GPS hardware. Instead, the ESP32 is wired directly to the flight controller's serial port. It passively intercepts the drone’s native MAVLink telemetry stream, stripping out the live latitude, longitude, and altitude data.

### 3. Command and Logging
* **Reverse Camera Trigger:** The payload is not just a passive listener. The ESP32 sends a command packet *back* up to the flight controller, automatically triggering the drone's primary camera to take a visual record of the exact location being measured along with the time which itself runs on esp32.
* **Synchronized SD Logging:** All drone coordinates and local air quality metrics are merged and written sequentially to `FLIGHT01.CSV` on the onboard MicroSD card on ESP32 board, which gives much better idea of the area along witht the exact visual to see how was the environment at the time when measurements were taken.
  
## 3D Printing & Mechanical

The payload chassis was designed entirely from scratch in Onshape. It features aggressive mass-reduction cutouts, hexagonal structural grilles, and cutouts acting mainly for weight reduction and thermal managment and clip on mechanism for really easy setup. 

<img width="1365" height="641" alt="image" src="https://github.com/user-attachments/assets/381950c1-374d-4b0f-b441-afdeba5d33da" />

* **Print Settings:** I highly recommend Poly Carbonate(PC-ABS) for the battry encloser and the Sensor Pod for maintaining the weight low and provide the heat resistance and the necessery strength.
* While Camera mount can be printed in normal TPU filament.
* And as for frame itself i highly recommend CNC machining instead of 3D printing.
* **Production Files:** `.stl` and `.glb` files are located in the `/CAD` folder.

## PCB Design

<img width="2160" height="729" alt="PCB_PCB1_2026-09-07" src="https://github.com/user-attachments/assets/0299e733-5b0b-443b-bc24-d2a34fb54bfa" />

* **The brain of the payload(Pod) is a custom-routed EasyEDA carrier board designed specifically for the **Seeed Studio XIAO ESP32-S3 Sense**. 
* **Production Files:** Gerber files and the CPL are available in the `/Releases` tab.
* **Schematics:** View the full wiring diagram in the `/Hardware` folder.

## Firmware

The firmware is written in C++ and handles the pre-flight sensor checks, I2C bus initialization, and the continuous data-logging loop for the entire flight duration. 

* **Source:** Check the `/Firmware` folder for the main `.ino` or PlatformIO `main.cpp` files.

## BOM (Bill of Materials) For all the components

* [Seeed Studio XIAO ESP32-S3 Sense](link)
* [Bosch BME688 Breakout](link)
* [Bosch BMV080 Breakout](link)
* **General PCB BOM:** [Link to JLCPCB CSV]
* ** NOTE: The frame, The pod, the camera encloser and the Battry Encloser are custom designed.
## License

This project is fully open-source and licensed under the MIT License.
