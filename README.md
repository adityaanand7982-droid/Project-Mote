# Project-Mote
A high-altitude, open-source data collection drone engineered for measuring the microclimates across various places. It replaces heavy, expensive stationary microclimate rigs with a lightweight, 3D-printable pod designed to capture real-time environmental telemetry.

 [![Website](https://img.shields.io/badge/MISSION_LOG-ONLINE-000000?style=for-the-badge&logo=github&logoColor=00ffff&borderColor=00ffff)](https://adityaanand7982-droid.github.io/Project-Mote/)
## Custom Features 

* **Venturi Airflow Dynamics:** Custom-engineered internal channeling prevents air stagnation, ensuring sensors sample live ambient air during flight and for proper thermal managment so that sensor and microcontroller heat doesnt interfare with the measurements.
* **Li-Ion Flight Platform:** Runs entirely on a standard high-efficiency Li-ion cell, eliminating heavy LiPo packs to maximize airborne observation time and minimize mass.
* **Autonomous Data Logging:** Native MicroSD card integration writes sequential `.csv` telemetry directly to memory for post-flight analysis.
* **Sensors:** Features the Bosch BME688 (microclimate) and Bosch BMV080 (particulates) operating on a shared I2C bus.
* **Custom Housing:** Designed for weight efficency and max flight duration and to avoid damiging the sensors.
## System Architecture

I made this project with exactly core philosphy: It should be very reiliable, cheap and ready to go at anytime when needed and the hardware and software stacks operate in a continuous, synchronized cycle during flight.

### 1. Mechanical & Aerodynamic Integration
* The pod deliberately lacks internal fans. It relies entirely on the drone’s forward pitch and flight velocity to force ambient air through the custom 3D-printed venturi grilles. The drone's physical movement acts as the system's air pump, ensuring zero air stagnation.

### 2. The Data & Telemetry
* **Microclimate Polling:** At few intervals, the "Cyber-Spider" PCB acts as the mission commander, pinging the onboard BME688 and BMV080 sensors over the I2C bus to capture a real-time snapshot of the immediate atmosphere.
* **The Drone as an Antenna:** To strictly minimize mass, the pod carries no GPS hardware. Instead, the ESP32 is wired directly to the flight controller's serial port. It passively intercepts the drone’s native MAVLink telemetry stream, stripping out the live latitude, longitude, and altitude data.

### 3. Logging
* All drone coordinates and local air quality metrics are merged and written sequentially to `FLIGHT01.CSV` on the onboard MicroSD card on ESP32 board, which gives much better idea of the area along witht the exact visual to see how was the environment at the time when measurements were taken.
  
## 3D Printing & Mechanical

The payload chassis was designed entirely from scratch in Onshape. It features aggressive mass-reduction cutouts, hexagonal structural grilles, and cutouts acting mainly for weight reduction and thermal managment and clip on mechanism for really easy setup. 

<img width="1365" height="641" alt="image" src="https://github.com/user-attachments/assets/381950c1-374d-4b0f-b441-afdeba5d33da" />

* **Print Settings:** I highly recommend Poly Carbonate(PC-ABS) for the battry encloser and the Sensor Pod for maintaining the weight low and provide the heat resistance and the necessery strength.
* While Camera mount can be printed in normal TPU filament.
* And as for frame itself i highly recommend CNC machining(2mm or 3mm Carbon Fibre Sheet) instead of 3D printing.
* **Production Files:** `.stl` and `.glb` files are located in the `/CAD` folder.

## PCB Design

<img width="2160" height="729" alt="PCB_PCB1_2026-09-07" src="https://github.com/user-attachments/assets/0299e733-5b0b-443b-bc24-d2a34fb54bfa" />


* **The brain of the payload(Pod) is a custom-routed EasyEDA carrier board designed specifically for the **Seeed Studio XIAO ESP32-S3 Sense**. 
* **Production Files:** Gerber files and the CPL are available in the `/PCB` tab.
* **Schematics:** View the full wiring diagram in the `/PCB` folder.

## Firmware

The firmware is written in C++ and handles the pre-flight sensor checks, I2C bus initialization, and the continuous data-logging loop for the entire flight duration. 
* It also initialises the esp to take the exact co-ordinates(from the drone's gps) of the measurements and everything is saved in .csv form.
* **Source:** Check the `/Firmware` folder for the main `.ino` or PlatformIO `main.cpp` files.

## BOM (Bill of Materials) For general components

* [Seeed Studio XIAO ESP32-S3 Sense](https://www.seeedstudio.com/XIAO-ESP32S3-Sense-p-5639.html)
* [Bosch BMV080 Breakout Board](https://www.sparkfun.com/sparkfun-air-quality-pm1-pm2-5-pm10-sensor-bmv080-qwiic.html)(I've designed the pod for specifically for SparkFun Air Quality Sensor - BMV080 breakout board but theres one more you can use -[DFRobot Fermion BMV080](https://www.dfrobot.com/product-3069.html), its almost the same size just slightly bigger which also include mounting holes)
* [Bosch BME688 Breakout Board](https://7semi.com/bme688-environmental-sensor-breakout/)
  NOTE: For the pod i've used the $\color{red}{\text{7semi BME688 Nano Breakout}}$ ([this one](https://7semi.com/bme688-environmental-sensor-nano-breakout-2-54mm-pitch-connector/)) its much smaller version(its only 14mm by 12mm) with 2.54mm pitch distance and features only I2C bus connection while the one which i gave you the link have both I2C and SPI and is much larger(31mm by 19mm) which might or might not fit in the pod (i've tried fitting it into the pod in cad assambly and it looks little big in the chamber), and theres also one with much smaller pitch distance [this one](https://7semi.com/bme688-environmental-sensor-nano-breakout-1-27mm-pitch-header/) which has pitch of 1.27mm which is increadbly small and compact.
* **General PCB BOM:**(the pcb bom)
* 
* ** NOTE: The frame, The pod, the camera encloser, the venturi airflow cavity and the Battry Encloser are custom designed from scratch by hand by me. NO AI used.
## License

This project is fully open-source and licensed under the MIT License.
