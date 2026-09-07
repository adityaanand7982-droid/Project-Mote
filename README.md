# Project-Mote 🛰️

![Project Mote Hero Image](./images/hero_render.png)

A high-altitude, open-source data collection payload engineered for drone deployment. It replaces heavy, expensive stationary microclimate rigs with a lightweight, 3D-printable pod designed to capture real-time environmental telemetry.

## ⚙️ Custom Features

* **Venturi Airflow Dynamics:** Custom-engineered internal channeling prevents air stagnation, ensuring sensors sample live ambient air during flight.
* **Li-Ion Flight Platform:** Runs entirely on a standard high-efficiency Li-ion cell, eliminating heavy LiPo packs to maximize airborne observation time and minimize mass.
* **Autonomous Data Logging:** Native MicroSD card integration writes sequential `.csv` telemetry directly to memory for post-flight analysis.
* **Aerospace-Grade Sensors:** Features the Bosch BME688 (microclimate) and SparkFun BMV080 (particulates) operating on a shared I2C bus.

## 🖨️ 3D Printing & Mechanical

The payload chassis was designed entirely from scratch in Onshape. It features aggressive mass-reduction cutouts, hexagonal structural grilles, and a sliding dovetail rail for seamless drone integration. 

* **CAD Source:** [Link to Onshape Workspace]
* **Print Settings:** We recommend PETG for thermal resistance during outdoor deployments. 
* **Production Files:** `.stl` and `.glb` files are located in the `/CAD` folder.

## 🕷️ PCB Design: The "Cyber-Spider"

The brain of the payload is a custom-routed EasyEDA carrier board designed specifically for the **Seeed Studio XIAO ESP32-S3 Sense**. 

![Cyber Spider PCB](./images/pcb_layout.png)

* **Production Files:** Gerber files and the CPL are available in the `/Releases` tab.
* **Schematics:** View the full wiring diagram in the `/Hardware` folder.

## 💻 Firmware

The firmware is written in C++ and handles the pre-flight sensor checks, I2C bus initialization, and the continuous data-logging loop. 

* **Status:** Active Development
* **Source:** Check the `/Firmware` folder for the main `.ino` or PlatformIO `main.cpp` files.

## 📋 BOM (Bill of Materials)

* [Seeed Studio XIAO ESP32-S3 Sense](link)
* [Bosch BME688 Breakout](link)
* [SparkFun BMV080 Breakout](link)
* **General PCB BOM:** [Link to JLCPCB CSV]

## 📜 License

This project is fully open-source and licensed under the MIT License.
