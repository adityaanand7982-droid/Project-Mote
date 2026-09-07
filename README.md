# Project-Mote

![Project Mote Hero Image](./<a href='https://postimg.cc/TKBKftph' target='_blank'><img src='https://i.postimg.cc/TKBKftph/PCB-PCB1-2026-09-07.png' border='0' alt='PCB-PCB1-2026-09-07'></a>)

A high-altitude, open-source data collection payload engineered for drone deployment. It replaces heavy, expensive stationary microclimate rigs with a lightweight, 3D-printable pod designed to capture real-time environmental telemetry.

## Custom Features

* **Venturi Airflow Dynamics:** Custom-engineered internal channeling prevents air stagnation, ensuring sensors sample live ambient air during flight and for proper thermal managment so that sensor and microcontroller heat doesnt interfare with the measurements.
* **Li-Ion Flight Platform:** Runs entirely on a standard high-efficiency Li-ion cell, eliminating heavy LiPo packs to maximize airborne observation time and minimize mass.
* **Autonomous Data Logging:** Native MicroSD card integration writes sequential `.csv` telemetry directly to memory for post-flight analysis.
* **Sensors:** Features the Bosch BME688 (microclimate) and Bosch BMV080 (particulates) operating on a shared I2C bus.
* **Custom Housing:** Designed for weight efficency and max flight duration and avoiding damiging the sensors.

## 3D Printing & Mechanical

The payload chassis was designed entirely from scratch in Onshape. It features aggressive mass-reduction cutouts, hexagonal structural grilles, and cutouts acting mainly for weight reduction and thermal managment and clip on mechanism for really easy setup. 

* **CAD Source:** [Link to Onshape Workspace]
* **Print Settings:** I recommend Poly Carbonate(PC-ABS) for the battry encloser and the Sensor Pod for maintaining the weight low and provide the heat resistance. 
* **Production Files:** `.stl` and `.glb` files are located in the `/CAD` folder.

## PCB Design

The brain of the payload(Pod) is a custom-routed EasyEDA carrier board designed specifically for the **Seeed Studio XIAO ESP32-S3 Sense**. 

![PCB Design](./images/pcb_layout.png)

* **Production Files:** Gerber files and the CPL are available in the `/Releases` tab.
* **Schematics:** View the full wiring diagram in the `/Hardware` folder.

## Firmware

The firmware is written in C++ and handles the pre-flight sensor checks, I2C bus initialization, and the continuous data-logging loop for the entire flight duration. 

* **Status:** Active Development
* **Source:** Check the `/Firmware` folder for the main `.ino` or PlatformIO `main.cpp` files.

## BOM (Bill of Materials) For all the components

* [Seeed Studio XIAO ESP32-S3 Sense](link)
* [Bosch BME688 Breakout](link)
* [Bosch BMV080 Breakout](link)
* **General PCB BOM:** [Link to JLCPCB CSV]
* ** NOTE: The frame and the Battry Encloser are custom designed.
## License

This project is fully open-source and licensed under the MIT License.
