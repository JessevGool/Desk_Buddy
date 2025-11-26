# DeskBuddy

**DeskBuddy** is a small hobby project designed to bring useful information directly to your desktop.  
It runs on an **ESP32** equipped with a **TFT display** and a **joystick** for easy navigation between different information screens.

## Overview

DeskBuddy displays various useful bits of information right on your desk — such as:
- GitHub activity
- 3D printer status
- And much more (fully extendable with custom data sources)

The goal is to keep relevant information visible at a glance while maintaining a fun, interactive hardware project.

## Features
- 🧭 Joystick-based navigation  
- 📺 TFT display interface  
- 🌐 Expandable info panels (GitHub, 3D printer, etc.)  
- ⚙️ Built with **PlatformIO**  
- 🧩 Simulated and tested using **Wokwi**

## Development

DeskBuddy was created as a **hobby project**, combining both hardware and software to make a simple, modular, and extendable desktop companion.

## Pinout
| ESP | HW504 |
|-----|-------|
| GND |  GND  |
| 3V3 | +5V   |
| D33 | VRx   |
| D32 | VRy   |
| D27 | SW    |

| ESP | TFT  |
|-----|------|
| GND |  GND |
| 3V3 | VCC  |
| D18 | CLK  |
| D23 | MOSI |
| D4  | RES  |
| TX2 | DC   |
| D19 | MISO |
| D5  | CS1  |
### Tools Used
- [PlatformIO](https://platformio.org/) – for development and flashing  
- [Wokwi](https://wokwi.com/) – for simulation and testing  

---

💡 *DeskBuddy is still evolving — feel free to expand it with new integrations and ideas!*
