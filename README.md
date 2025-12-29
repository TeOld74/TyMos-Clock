# TyMos-Clock

A unique mechatronic clock featuring 29 servo motors controlled by ESP32, creating an engaging and artistic way to display time.

## ✨ Features

- **29 Servo Motors**: Each motor contributes to the mechanical display of time
- **ESP32 Microcontroller**: Powerful, WiFi-enabled brain of the clock
- **WiFi Connectivity**: Remote configuration and Over-The-Air (OTA) firmware updates
- **Automatic Time Synchronization**: Uses NTP (Network Time Protocol) to keep accurate time
- **RTC Backup**: DS3231 Real-Time Clock maintains time even without WiFi
- **Web Interface** *(Phase 1)*: Browser-based control and configuration
- **Dual PWM Drivers**: Two PCA9685 boards provide precise 16-bit PWM control
- **Standard 5V Power Supply**: Minimum 3A power supply for stable operation

## 🔧 Hardware Requirements

- **ESP32 Development Board** (ESP32-DevKitC or similar)
- **2x PCA9685 16-Channel PWM Driver Boards** (I2C controlled)
- **DS3231 Real-Time Clock Module** (I2C with battery backup)
- **29x Servo Motors** (SG90 or similar 5V micro servos)
- **5V Power Supply** (minimum 3A recommended)
- **Connecting wires and breadboard/PCB**

### Wiring Overview

- **I2C Bus**: ESP32 SDA/SCL connected to both PCA9685 boards and DS3231
- **PWM Drivers**: Each PCA9685 controls up to 16 servos (29 servos total)
- **Power**: Single 5V power supply split into separate rails for servos and ESP32

*Detailed schematics will be available in the `hardware/schematics/` directory*

## 🚀 Getting Started

### For Non-Technical Users

Don't worry if you're not a programmer! Follow these simple steps:

1. **Install Arduino IDE**
   - Download from [arduino.cc](https://www.arduino.cc/en/software)
   - Install ESP32 board support (instructions in Arduino IDE documentation)

2. **Download This Project**
   - Click the green "Code" button above
   - Select "Download ZIP"
   - Extract the ZIP file to your computer

3. **Configure WiFi Credentials**
   - Find the file `secrets.h.template` in the project folder
   - Make a copy and rename it to `secrets.h` (remove `.template`)
   - Open `secrets.h` with a text editor (Notepad, TextEdit, etc.)
   - Replace `your_wifi_network_name` with your WiFi network name
   - Replace `your_wifi_password` with your WiFi password
   - Change `your_ota_password` to a secure password (for future updates)
   - Save the file

4. **Upload Firmware**
   - Open Arduino IDE
   - Go to **File → Open**
   - Navigate to `firmware/TyMos_Phase0/TyMos_Phase0.ino`
   - Connect your ESP32 board via USB
   - Select the correct board and port in **Tools** menu
   - Click the **Upload** button (right arrow icon)

5. **First Boot**
   - After upload completes, open **Serial Monitor** (Tools → Serial Monitor)
   - Set baud rate to **115200**
   - You should see connection messages and the clock starting up

### For Developers

```bash
# Clone the repository
git clone https://github.com/TeOld74/TyMos-Clock.git
cd TyMos-Clock

# Create your secrets.h file
cp secrets.h.template secrets.h

# Edit secrets.h with your credentials
nano secrets.h  # or use your preferred editor

# Open the appropriate firmware in Arduino IDE
# firmware/TyMos_Phase0/TyMos_Phase0.ino
```

## 📋 Development Phases

### Phase 0: Basic Clock Functionality ✅ *Current Phase*

**Goal**: Get the clock working with automatic time display

- [x] Hardware assembly and wiring
- [x] Basic servo control via PCA9685
- [ ] DS3231 RTC integration
- [ ] WiFi connection and NTP time sync
- [ ] Automatic time display algorithm
- [ ] OTA (Over-The-Air) firmware updates
- [ ] Serial debug interface

**Focus**: Reliable, standalone operation with automatic time keeping

### Phase 1: Web Interface and Advanced Controls 🔄 *Planned*

**Goal**: Add remote control and configuration via web browser

- [ ] Web server on ESP32
- [ ] Web-based dashboard
- [ ] Manual servo control interface
- [ ] Settings configuration page
- [ ] System diagnostics and status
- [ ] Time zone configuration via web
- [ ] Calibration tools

**Focus**: User-friendly control without reflashing firmware

### Future Enhancements 💡

- **Web Configurator**: First-time setup wizard accessible via WiFi hotspot (no code editing needed)
- **Animation Modes**: Creative movement patterns and transitions
- **Alarm Function**: Wake-up animations and notifications

## 📁 Project Structure

```
TyMos-Clock/
├── firmware/
│   ├── TyMos_Phase0/       # Basic clock functionality
│   ├── TyMos_Phase1/       # Web interface version
│   └── test-sketches/      # Hardware testing utilities
├── hardware/
│   ├── schematics/         # Electrical schematics
│   └── 3d-models/          # 3D printable parts (STL files)
├── docs/                   # Additional documentation
├── webapp/                 # Web interface source code
├── secrets.h.template      # Configuration template
└── README.md              # This file
```

## 🔐 Security Notes

- **Never commit `secrets.h`** to version control - it contains your passwords
- The `.gitignore` file already protects `secrets.h`
- Change the default `WEB_ACCESS_PIN` in your `secrets.h`
- Use a strong `OTA_PASSWORD` to prevent unauthorized firmware updates

## 📖 Documentation

Additional documentation will be added to the `docs/` directory:

- Assembly instructions
- Calibration guide
- Troubleshooting
- API documentation (Phase 1)

## 🤝 Contributing

Contributions are welcome! Feel free to:

- Report bugs by opening an issue
- Suggest new features
- Submit pull requests
- Share your build photos and videos

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👤 Author

**Matteo Oldani**

- GitHub: [@TeOld74](https://github.com/TeOld74)
- Project Link: [https://github.com/TeOld74/TyMos-Clock](https://github.com/TeOld74/TyMos-Clock)

## 🙏 Acknowledgments

- Thanks to the ESP32 and Arduino communities
- Adafruit for excellent PCA9685 libraries
- Everyone who contributes to open-source hardware projects

---

*Built with ❤️ and 29 servo motors*
