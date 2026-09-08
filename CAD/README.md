###  Architecture
This directory contains the custom 3D-printable and CNC cut files for the **Project-Mote** drone payload. 

The chassis is made to house the environmental sensors (Bosch BME688 & BMV080), the ESP32-S3 Sense microcontroller, drone electronics including flight controller, a video system for live fpv camera feed, , all while maintaining a highly aerodynamic profile and looking cool for drone flight.

###  Design Specifications
* Designed from scratch in Onshape. 
* No ARTIFICIAL INTELIGENCE used. 
* Optimized to allow continuous, unimpeded airflow over the particulate(bmv080) and microclimate(bme688) sensors for real-time accuracy.

### ⚙️ Manufacturing & Print Recommendations
If you are replicating this build, follow these print settings for optimal flight performance:

* Materials:
  * Battery Enclosure & Sensor Pod:** I highly recommend Polycarbonate (PC-ABS) for maintaining low weight while providing heat resistance and necessary strength.
  * Camera Mount: Can be printed in normal TPU filament.
  * Main Frame: I highly recommend CNC machining (2mm or 3mm Carbon Fibre Sheet) instead of 3D printing.
* Infill: 20% - 30% Gyroid for the best strength-to-weight ratio.
* Supports: Tree/Organic supports recommended for the sensor mounting overhangs.
* Orientation: Print flat as oriented in the STL files to ensure layer lines align with the main stress axes.

### File Structure
* **3D Printing:** `.stl`, and`.3mf`, both files are located in this `/CAD` folder.
* **CNC & Modification:** The main frame(part 1) `.step` source file is also included for easy CAM toolpath generation or CAD remixing.

###  CAD Onshape Links(if you want to do extra tinkring for your own objectives) 
* [Frame](https://cad.onshape.com/documents/05a52b95227481df21c4d61e/w/404adcbb2aa8a207a427e975/e/822eb19d10cc566edf78f8fc)
* [Pod](https://cad.onshape.com/documents/cd26c9afff5e10d9f9bc6e67/w/c80e0667bd276da188269a66/e/ebaad97bd10442b1b29f2ee8)
* [Battery(1S)](https://cad.onshape.com/documents/b9a957a4efd11c345544f532/w/533336610b812f5b3d86bded/e/02fc169ee8b6c8001d00e674)
* [Battery(2S)](https://cad.onshape.com/documents/b9a957a4efd11c345544f532/w/533336610b812f5b3d86bded/e/d314ea4a2c79227dc41efd6a)
* [Camera Encloser](https://cad.onshape.com/documents/f9ba0b2f4cd3d33b59f0e158/w/f90ac1368fdbbf598d694e47/e/fa5613f01b504bd3c8a06d72)
* [Pod Cover](https://cad.onshape.com/documents/ed6756b1f015da37f33ff3bd/w/7296c9ea6f9250048ba06707/e/54b66b59ead6eeb3fbd70992)
