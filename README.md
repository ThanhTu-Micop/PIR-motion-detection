Dưới đây là một **README.md chi tiết** cho project của bạn, tích hợp GitHub, Wokwi và ThingSpeak.

````markdown
# PIR Motion Detection System with ESP32 & ThingSpeak

## Overview

This project implements a real-time motion detection system using an ESP32 and a PIR (Passive Infrared) sensor. When motion is detected, the ESP32 sends data to ThingSpeak for cloud monitoring and visualization.

The system can be simulated entirely on Wokwi without requiring physical hardware.

## Project Links

### GitHub Repository
https://github.com/ThanhTu-Micop/PIR-motion-detection

### Wokwi Simulation
https://wokwi.com/projects/460832406883731457

### ThingSpeak Dashboard
https://thingspeak.mathworks.com/channels/3402569

---

## Features

- Real-time motion detection using PIR sensor
- ESP32-based IoT implementation
- Automatic WiFi connection
- Cloud data logging via ThingSpeak
- Online dashboard visualization
- Wokwi simulation support
- Serial Monitor debugging output

---

## System Architecture

```text
          +----------------+
          |   PIR Sensor   |
          +-------+--------+
                  |
                  v
          +----------------+
          |     ESP32      |
          +-------+--------+
                  |
          WiFi Connection
                  |
                  v
          +----------------+
          |  ThingSpeak    |
          +-------+--------+
                  |
                  v
          Motion Data Chart
````

---

## Components

### Hardware

* ESP32 Dev Module
* PIR Motion Sensor (HC-SR501)
* WiFi Network

### Software

* Arduino IDE
* ThingSpeak
* Wokwi Simulator

---

## PIR Sensor

A PIR (Passive Infrared) sensor detects infrared radiation emitted by humans and animals.

### Output

| Motion Status   | Sensor Output |
| --------------- | ------------- |
| No Motion       | LOW (0)       |
| Motion Detected | HIGH (1)      |

---

## Wiring

### ESP32 ↔ PIR

| PIR Pin | ESP32 Pin                 |
| ------- | ------------------------- |
| VCC     | 5V                        |
| GND     | GND                       |
| OUT     | GPIO (configured in code) |

---

## How It Works

1. ESP32 connects to WiFi.
2. PIR sensor continuously monitors movement.
3. Motion state is read from the PIR output pin.
4. ESP32 sends the sensor value to ThingSpeak.
5. ThingSpeak stores the data.
6. Dashboard graphs are updated automatically.

---

## Data Flow

```text
Human Movement
       |
       v
PIR Sensor
       |
       v
ESP32
       |
       v
ThingSpeak API
       |
       v
ThingSpeak Cloud
       |
       v
Dashboard Graph
```

---

## ThingSpeak Channel

### Public Dashboard

[https://thingspeak.mathworks.com/channels/3402569](https://thingspeak.mathworks.com/channels/3402569)

### Data Format

| Value | Meaning         |
| ----- | --------------- |
| 0     | No Motion       |
| 1     | Motion Detected |

---

## Running the Simulation

### Method 1: Wokwi

Open:

[https://wokwi.com/projects/460832406883731457](https://wokwi.com/projects/460832406883731457)

Click:

```text
Start Simulation
```

Move the PIR sensor state to simulate motion.

Open Serial Monitor to observe logs.

---

### Method 2: Physical ESP32

1. Connect ESP32 and PIR sensor.
2. Open Arduino IDE.
3. Install required libraries.
4. Enter WiFi credentials.
5. Upload code.
6. Open Serial Monitor.
7. Observe ThingSpeak updates.

---

## Expected Output

### Serial Monitor

```text
Connecting to WiFi...
WiFi Connected

Motion Detected
Sending data to ThingSpeak...

Update Successful
```

or

```text
No Motion
Sending data to ThingSpeak...

Update Successful
```

---

## ThingSpeak Visualization

The ThingSpeak dashboard displays:

* Motion events over time
* Historical sensor data
* Real-time monitoring
* Cloud data storage

---

## Applications

### Smart Home

* Intruder detection
* Automatic lighting
* Room occupancy monitoring

### Security Systems

* Motion-triggered alarms
* Surveillance activation
* Restricted area monitoring

### IoT Learning

* ESP32 programming
* Cloud IoT platforms
* Sensor integration
* Data visualization

---

## Project Structure

```text
PIR-motion-detection/
│
├── diagram.json
├── sketch.ino
├── libraries.txt
├── wokwi.toml
├── README.md
│
└── images/
    ├── architecture.png
    ├── wiring.png
    └── dashboard.png
```

---

## Future Improvements

* Email notifications
* Telegram alerts
* Blynk integration
* Camera module support
* MQTT communication
* Occupancy analytics
* Multi-sensor deployment

---

## Learning Outcomes

Through this project, users can learn:

* ESP32 development
* PIR motion sensing
* IoT architecture
* Cloud data logging
* ThingSpeak API usage
* Wokwi simulation
* Real-time monitoring systems

---

## References

### ThingSpeak

[https://thingspeak.mathworks.com](https://thingspeak.mathworks.com)

### ESP32

[https://www.espressif.com](https://www.espressif.com)

### Wokwi

[https://wokwi.com](https://wokwi.com)

---

## Author

**Thanh Tu**

GitHub:
[https://github.com/ThanhTu-Micop](https://github.com/ThanhTu-Micop)

Project:
[https://github.com/ThanhTu-Micop/PIR-motion-detection](https://github.com/ThanhTu-Micop/PIR-motion-detection)

---

## License

This project is provided for educational and learning purposes.

