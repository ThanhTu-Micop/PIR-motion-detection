# PIR-Based Traffic Activity Monitoring System using ESP32 and ThingSpeak

## Overview

This project presents an IoT-based traffic activity monitoring system developed using an ESP32 microcontroller, a PIR (Passive Infrared) motion sensor, and the ThingSpeak cloud platform.

The system detects motion events occurring in the monitored area and transmits real-time data to ThingSpeak through a WiFi connection. The collected data is stored in the cloud and visualized through online dashboards, enabling users to monitor activity levels and analyze traffic patterns over time.

The project was designed for deployment in a university dormitory environment. Since vehicle movement during nighttime hours is relatively low, motion detection events are used as indicators of traffic activity. The primary objective of this project is to demonstrate IoT sensing, cloud communication, data logging, and real-time visualization rather than precise vehicle counting.

The entire system can be simulated using Wokwi or implemented on physical ESP32 hardware.

---

## Project Links

### GitHub Repository

https://github.com/ThanhTu-Micop/PIR-motion-detection

### Wokwi Simulation

https://wokwi.com/projects/460832406883731457

### ThingSpeak Dashboard

https://thingspeak.mathworks.com/channels/3402569

---

## Features

* Real-time motion detection using PIR sensor
* ESP32-based IoT implementation
* Automatic WiFi connection
* Cloud data logging via ThingSpeak
* Real-time dashboard visualization
* Historical data storage and analysis
* Traffic activity monitoring
* Wokwi simulation support
* Serial Monitor debugging output
* Easy deployment for educational and smart campus applications

---

## System Architecture

```text
          +----------------------+
          |      PIR Sensor      |
          +----------+-----------+
                     |
                     v
          +----------------------+
          |        ESP32         |
          +----------+-----------+
                     |
              WiFi Connection
                     |
                     v
          +----------------------+
          |      ThingSpeak      |
          +----------+-----------+
                     |
                     v
          +----------------------+
          | Dashboard & Charts   |
          +----------------------+
```

---

## Hardware Components

| Component         | Description           |
| ----------------- | --------------------- |
| ESP32 Dev Module  | Main microcontroller  |
| PIR Motion Sensor | Detects movement      |
| WiFi Network      | Internet connectivity |
| USB Power Supply  | Power source          |

---

## Software and Platforms

* Arduino IDE
* ESP32 Board Package
* ThingSpeak Cloud Platform
* Wokwi Simulator

---

## Working Principle

1. The PIR sensor continuously monitors movement within its detection range.
2. When motion is detected, the sensor outputs a HIGH signal.
3. The ESP32 reads the sensor value.
4. Motion data is transmitted to ThingSpeak using WiFi.
5. ThingSpeak stores the data in the cloud.
6. Dashboards and charts are automatically updated.
7. Users can monitor traffic activity remotely through the web dashboard.

---

## Data Flow

```text
Movement Activity
        |
        v
   PIR Sensor
        |
        v
      ESP32
        |
        v
   WiFi Network
        |
        v
   ThingSpeak API
        |
        v
 ThingSpeak Cloud
        |
        v
 Dashboard Charts
```

---

## ThingSpeak Dashboard

The project uses ThingSpeak for cloud-based monitoring and data visualization.

Dashboard Link:

https://thingspeak.mathworks.com/channels/3402569

### Functions

* Real-time monitoring
* Historical data storage
* Automatic chart generation
* Remote access through web browser
* Data export for analysis

---

## Data Analysis

The collected data can be exported from ThingSpeak as CSV files for further analysis.

Example dataset:

```text
thingspeak_xe_ra_vao_theo_khung_gio_15-6-2026.csv
```

The exported data can be used to:

* Analyze activity by hour
* Identify peak activity periods
* Compare traffic patterns across different times
* Generate statistical reports
* Evaluate system performance

---

## Applications

### University Dormitories

* Monitor activity around dormitory entrances
* Observe movement trends
* Support campus management

### Smart Campus

* Traffic activity monitoring
* Occupancy estimation
* Infrastructure planning

### Smart Security

* Motion-triggered monitoring
* Activity logging
* Event tracking

### IoT Education

* ESP32 programming
* Sensor integration
* Cloud communication
* Data visualization
* IoT system design

---

## Running the Simulation

### Step 1

Open the Wokwi simulation:

https://wokwi.com/projects/460832406883731457

### Step 2

Click:

```text
Start Simulation
```

### Step 3

Trigger motion events through the PIR sensor.

### Step 4

Open the Serial Monitor to observe sensor readings and ThingSpeak communication.

### Step 5

Check the ThingSpeak dashboard for updated data.

---

## Expected Output

### Motion Detected

```text
Motion Detected
Sending data to ThingSpeak...
Update Successful
```

### No Motion

```text
No Motion
Sending data to ThingSpeak...
Update Successful
```

---

## Results

The system successfully demonstrates:

* Motion sensing using a PIR sensor
* Wireless communication via WiFi
* Cloud-based data storage
* Real-time dashboard visualization
* Historical data analysis through ThingSpeak

The project provides a low-cost and scalable solution for activity monitoring in university dormitories and similar environments.

---

## Future Improvements

* Dual PIR sensors for direction detection
* Vehicle entry and exit classification
* Camera integration
* AI-based object recognition
* MQTT communication
* Mobile application support
* Blynk dashboard integration
* Multi-sensor deployment
* Advanced traffic analytics

---

## Learning Outcomes

Through this project, students can learn:

* ESP32 development
* PIR sensor operation
* IoT architecture design
* Cloud computing concepts
* ThingSpeak API integration
* Data collection and visualization
* Real-time monitoring systems
* Smart campus applications



## License

This project is developed for educational and academic purposes.

Feel free to use, modify, and extend the project for learning and research activities.
