# 🚗 DC Motor Control & Monitoring System (STM32 + Qt Dashboard)

Welcome to our DC Motor Control and Monitoring System! This project uses an STM32 microcontroller running FreeRTOS to control a DC motor, with a Qt-based GUI that acts as a car dashboard to monitor and adjust motor speed in real-time. 🛠️

## 📋 Project Overview

### What's Inside:
- **Embedded Firmware** (STM32 + FreeRTOS): Controls the motor using PWM, processes commands from a USB serial interface, and manages LEDs for status feedback.
- **Qt Dashboard**: A car-inspired GUI that communicates with the STM32 to display motor speed and control acceleration and braking.

### Features:
- Real-time motor control through USB virtual COM port.
- Dynamic speed adjustment via a sleek Qt GUI.
- Emergency braking system.
- LED feedback for motor status and alerts (using built-in STM32 LEDs due to the absence of a real DC motor).

---

## 🛠️ How It Works

### Firmware (STM32 + FreeRTOS):
- **Communication Task**: Listens for speed commands sent from the Qt dashboard.
- **Control Task**: Adjusts PWM duty cycle to set the motor speed.
- **Alert Task**: Monitors the speed and activates LEDs (red for high-speed alerts, green for normal operation).

### Qt Dashboard:
- Designed to mimic a car's instrument cluster.
- Allows control of motor speed through accelerator and brake buttons.
- Visual feedback for current speed.

---

## 📂 Project Structure
```
.
├── Firmware (STM32)
│   ├── Core
│   │   ├── Src
│   │   ├── Inc
│   │   └── FreeRTOS
│   └── Drivers
└── Dashboard (Qt GUI)
    ├── src
    └── resources
```

---

## 🚀 Getting Started

### Requirements:
- **STM32 Development Board** (STM32F4 series recommended)
- **Qt** (for GUI development)
- **STM32CubeIDE**

### Steps:
1. Flash the STM32 firmware.
2. Build and run the Qt dashboard.
3. Connect the STM32 board via USB.
4. Control motor speed through the Qt interface and observe LED feedback.

---

## 🔧 How to Build

### Firmware:
- Open STM32CubeIDE.
- Import the project and build.
- Flash the firmware to the STM32 board.

### Qt Dashboard:
- Open the Qt project.
- Build and run the application.

---

## 🏁 Demo
- Simulated car acceleration and braking.
- Real-time feedback loop between STM32 and Qt GUI.

---

## 🤝 Contributing
Feel free to fork this project and add your own features!

---

## 📬 Contact
For questions or feedback, reach out via GitHub issues. Let's build something amazing together! 🚀

