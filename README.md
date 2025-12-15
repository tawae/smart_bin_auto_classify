# Smart AI-Powered Waste Classification System

![IoT](https://img.shields.io/badge/IoT-ESP32-blue)
![AI](https://img.shields.io/badge/AI-MobileNetV2-orange)
![Platform](https://img.shields.io/badge/Platform-Node--RED-red)
![Cloud](https://img.shields.io/badge/Cloud-Google%20Colab-yellow)

An end-to-end IoT solution that automates waste sorting at the source. This system utilizes Computer Vision and Deep Learning to classify trash into 6 categories and automatically controls a sorting mechanism using an ESP32-based hardware setup.

---

## Table of Contents
- [Overview](#-overview)
- [System Architecture](#-system-architecture)
- [Features](#-features)
- [Hardware & Tech Stack](#-hardware--tech-stack)
- [Project Structure](#-project-structure)
- [Setup & Installation](#-setup--installation)
- [Database & Logging](#-database--logging)
- [Results](#-results)
- [Future Improvements](#-future-improvements)
- [Contributors](#-contributors)

---

## Overview
Improper waste disposal is a significant environmental challenge. This project aims to solve the "sorting at source" problem by creating a **Smart Trash Bin**. 

The system detects when trash is present, captures an image, processes it via a Cloud-hosted AI model (Transfer Learning with MobileNetV2), and mechanically sorts the waste into the correct compartment (Recyclable vs. Non-recyclable/Others) using a Servo motor.

---

## System Architecture

The system operates on a **Distributed Architecture** combining Edge devices and Cloud computing:

1.  **Detection:** An IR Sensor detects an object and signals the Master ESP32.
2.  **Capture:** The Master ESP32 sends a TCP command to the Slave ESP32-CAM.
3.  **Transmission:** ESP32-CAM captures an image and POSTs it to the Node-RED server (Middleware).
4.  **Processing:** Node-RED forwards the image via **Ngrok** to a **Flask API** running on **Google Colab**.
5.  **Inference:** The AI Model (MobileNetV2) classifies the image (Glass, Paper, Cardboard, Plastic, Metal, Trash).
6.  **Action:** The result is returned to Node-RED, which sends an HTTP command back to the Master ESP32 to rotate the Servo Motor to the correct bin.

---

## Features
* **Automatic Detection:** Uses IR sensors to trigger the system only when waste is present.
* **Deep Learning Classification:** Classifies waste into 6 specific categories using a custom-trained MobileNetV2 model.
* **Real-time Dashboard:** Node-RED Dashboard displays the live camera feed, classification result, and confidence score.
* **Cloud Data Logging:** Automatically saves classification logs (Timestamp, Label, Confidence) to **SQLite** and stores captured images to **Google Drive**.
* **Mechanical Sorting:** Servo-driven mechanism to divert waste to the appropriate compartment.

---

## Hardware & Tech Stack

### Hardware
* **Master Node:** ESP32 DevKit V1 (Controls Servo & IR Sensor).
* **Vision Node:** ESP32-CAM (AI-Thinker) + FTDI Programmer.
* **Sensors:** IR Obstacle Sensor (LM393).
* **Actuator:** SG90 Micro Servo Motor.
* **Power Supply:** 5V DC.

### Software & Tools
* **Firmware:** Arduino IDE (C++).
* **Middleware:** Node-RED (Flow-based programming).
* **AI/Backend:** Python, TensorFlow/Keras, Flask (hosted on Google Colab).
* **Tunneling:** Ngrok (exposes local Flask server to the internet).
* **Database:** SQLite (Metadata) & Google Drive (Image Storage).

---

## Project Structure

```text
├── firmware/
│   ├── Main_Controller/
│   │   └── thuchanhiot.ino       # Code for ESP32 (IR Sensor + Servo Control)
│   └── Camera_Node/
│       └── CameraWebServer.ino   # Code for ESP32-CAM (Image Capture + HTTP POST)
├── ai_model/
│   ├── predict_server.ipynb      # Google Colab Notebook (Flask API + SQLite + Ngrok)
│   └── nhan_dien_rac.ipynb       # Notebook for training MobileNetV2 model
├── backend/
│   └── flows.json                # Node-RED flows configuration
└── docs/
    └── report.pdf                # Full project report (Vietnamese)
