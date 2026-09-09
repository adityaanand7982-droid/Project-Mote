#  Avionics & Custom PCB

This directory contains the complete board design, schematics, and manufacturing files for the Project-Mote payload. The custom PCB acts as the central nervous system, housing the ESP32 microcontroller and cleanly routing our environmental sensor array (BME688 & BMV080) without the parasitic mass and unreliability of standard jumper wires.

###  Hardware Specifications
* **Microcontroller:** XIAO ESP32-S3
* **Sensor Array:** 7Semi BME688 (Gas/Humidity/Temp/Pressure) & SparkFun Air Quality BMV080(PM1/PM2.5/PM10 Sensor) 
* **Power Delivery:** Power from drone's own battery along with a capacitor to reduce electrical noise cleanly.

###  Design Specifications
* Designed entirely from scratch in EasyEDA Pro(not standard). 
* Absolutely NO ARTIFICIAL INTELLIGENCE was used in the making(except for autorouting of easyeda which was later done manually in few traces). 
* Component placement was carefully optimized to keep the 7Semi BME688 Nano BME688 and SparkFun BMV080 breakouts in its desired place w.r.t the pod design and to thermally isolate it from the ESP32.

###  Manufacturing Files
All PCB files for Project-Mote are located directly in this folder for easy access:

* **Gerber Files:** Contained in the `PCB.zip` archive.
* **BOM & Pick-and-Place:** Reference `PCB design part in EasyEDA.csv` for the Bill of Materials, and `PickAndPlace_PCB1.xlsx` for rapid SMT assembly.
* **Source Project:** The original EasyEDA project file (`ProPrj.epro2`) is included for custom trace routing or component swapping.
* **Schematics & Renders:** Quick-reference PDFs and PNGs (`Schematic.pdf`, `PCB image.png`, etc.) are available if you just want to inspect the traces and layout without opening EDA software. 

##  Suggestions & Feedback
Any Suggestions? or new ideas? or new things to add? I'd love to hear it.

* **Lead Engineer:** Aryabhart Tenjuku
* **Email:** <img width="231" height="19" alt="Screenshot 2026-09-09 111004" src="https://github.com/user-attachments/assets/28e727ef-a86c-48bf-8be5-11573a2be982" />


> **⚠️ ASSEMBLY WARNING**
> Please double-check the polarity of the main battery lead before connecting power. Reversing the polarity will instantly convert the ESP32 into a very brief, expensive, and highly toxic light-emitting diode(not ideal for lungs and vibe tbh).
