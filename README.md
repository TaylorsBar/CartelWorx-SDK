# CartelWorx SDK - AI-Powered Automotive Diagnostics & Performance Tuning

## Project Overview

CartelWorx SDK is a comprehensive, modular platform combining:
- **Vehicle Edge Firmware** (ESP32 FreeRTOS + BLE 5.3)
- **Mobile Client** (Kotlin/Android with real-time diagnostics)
- **Cloud Intelligence** (ML-powered adaptive tuning)
- **Custom Protocol** (CartelWorx Protocol v1.2 over BLE/CAN)

### Key Features
1. **Real-Time Knock Detection** - Crank-angle resolved processing
2. **Adaptive Tuning** - RL-based self-learning tuning maps
3. **Per-Cylinder Diagnostics** - Imbalance detection & health scoring
4. **Live Parameter Adjustment** - Safety-bounded tuning interface
5. **OTA Updates** - Staged rollout for firmware & tuning maps

## Architecture Layers

### Part 1: Vehicle Edge Layer
- **MCU**: ESP32 (Cortex-M4F) with FreeRTOS
- **Communication**: BLE 5.3 + CAN (OBD-II)
- **Real-Time**: Interrupt-driven processing, multi-PID polling
- **Security**: CRC16 validation, frame authentication

### Part 2: Diagnostics Engine
- **Knock Detection**: Adaptive threshold learning
- **Data Buffers**: Ring buffers for 512-sample windows
- **Actuator Control**: Ignition timing, fuel trim adjustments
- **Safety**: Hard bounds (-30° to +40° BTDC)

### Part 6: Mobile Application
- **Real-Time Gauges**: 60+ FPS needle animation
- **Cylinder Visualization**: Per-cylinder knock scores
- **Live Tuning**: Slider-based parameter adjustment
- **BLE Integration**: MTU negotiation, high-priority connection

### Part 4-5: Cloud Intelligence
- **Data Ingestion**: Google Cloud Pub/Sub streams
- **ML Training**: TRPO-based RL for tuning optimization
- **Telemetry**: Fleet analytics & predictive maintenance
- **Model Deployment**: Vertex AI integration

## Project Structure

```
CartelWorx-SDK/
├── firmware/
│   ├── esp32/
│   │   ├── src/
│   │   │   ├── main.cpp                    # FreeRTOS entry point
│   │   │   ├── hal.h                       # Hardware abstraction layer
│   │   │   ├── CartelWorxProtocol.cpp      # Custom protocol stack
│   │   │   ├── KnockDetection.cpp          # Real-time knock detection
│   │   │   ├── CanInterface.cpp            # OBD-II/CAN interface
│   │   │   └── BluetoothLE.cpp             # BLE 5.3 interface
│   │   ├── platformio.ini                  # PlatformIO configuration
│   │   └── CMakeLists.txt                  # Build configuration
│   └── include/
│       ├── ring_buffer.h
│       ├── cartelworx_types.h
│       └── diagnostic_codes.h
├── mobile/
│   ├── app/src/main/
│   │   ├── kotlin/
│   │   │   ├── DashboardViewModel.kt       # BLE connection logic
│   │   │   ├── BluetoothService.kt         # Protocol parsing
│   │   │   └── ui/
│   │   │       ├── DashboardScreen.kt      # Gauge rendering
│   │   │       └── EngineHealthScreen.kt   # Cylinder diagnostics
│   │   └── AndroidManifest.xml
│   └── build.gradle.kts
├── cloud/
│   ├── services/
│   │   ├── telemetry_ingestion.py          # Pub/Sub data handler
│   │   ├── adaptive_tuning_engine.py       # RL agent & training
│   │   └── predictive_maintenance.py       # Fleet analytics
│   ├── models/
│   │   └── tuning_policy_model.h5          # Trained RL policy
│   └── requirements.txt
├── docs/
│   ├── PROTOCOL_SPEC.md                    # CartelWorx Protocol v1.2
│   ├── API_REFERENCE.md                    # Cloud API endpoints
│   └── ARCHITECTURE.md                     # System design
├── tests/
│   ├── firmware_tests.cpp
│   ├── mobile_tests.kt
│   └── integration_tests.py
├── platformio.ini                          # Global PlatformIO config
├── docker-compose.yml                      # Local development stack
└── .github/workflows/                      # CI/CD pipelines
```

## Development Roadmap

### Phase 1: Architectural Foundation (Current)
- [x] System specification & architecture design
- [x] Hardware abstraction layer (HAL) definition
- [x] Interrupt-driven CAN receiver implementation
- [x] Multi-PID polling strategy (20-50Hz per param)
- [ ] FreeRTOS task scheduling & priorities
- [ ] BLE MTU negotiation & high-speed data streaming

### Phase 2: Real-Time Diagnostics
- [ ] Knock sensor signal processing
- [ ] Adaptive noise floor learning
- [ ] Per-cylinder score calculation
- [ ] Ignition timing control logic
- [ ] Ring buffer data structure & optimization

### Phase 3: Mobile Client
- [ ] BLE connection management
- [ ] Real-time gauge rendering (Jetpack Compose)
- [ ] Per-cylinder diagnostic visualization
- [ ] Live tuning parameter interface
- [ ] Data logging & session management

### Phase 4: Cloud Intelligence
- [ ] Telemetry data ingestion (Pub/Sub)
- [ ] BigQuery analytics pipeline
- [ ] TRPO/PPO RL training loop
- [ ] Tuning map generation & OTA deployment
- [ ] Fleet management dashboard

### Phase 5: Integration & Testing
- [ ] End-to-end firmware compilation
- [ ] Mobile app integration testing
- [ ] Cloud pipeline validation
- [ ] Hardware-in-loop simulation
- [ ] Security audit & hardening

## Technology Stack

**Firmware:**
- ESP-IDF 5.x + FreeRTOS 10.x
- C/C++17 with ARM Cortex-M4F optimizations
- MCP2515 CAN controller
- Espressif BLE Stack (NimBLE)

**Mobile:**
- Kotlin + Jetpack Compose
- Android 12+ (API 31+)
- Kotlin Coroutines + Flow
- Android Bluetooth LE APIs

**Cloud:**
- Google Cloud Platform (GCP)
- Cloud Pub/Sub + Cloud Functions
- BigQuery for analytics
- TensorFlow + Keras for ML
- Vertex AI for model deployment

**DevOps:**
- GitHub Actions CI/CD
- Docker + Kubernetes
- PlatformIO for firmware builds
- Firebase for real-time database

## Getting Started

### Prerequisites
- ESP-IDF 5.x installed & configured
- PlatformIO IDE or VS Code + PlatformIO extension
- Android Studio 2023+ for mobile development
- Python 3.9+ with pip
- Docker & Docker Compose (optional, for local cloud dev)

### Quick Start

1. **Clone the repository**
   ```bash
   git clone https://github.com/TaylorsBar/CartelWorx-SDK.git
   cd CartelWorx-SDK
   ```

2. **Set up firmware environment**
   ```bash
   cd firmware/esp32
   platformio run --target build
   ```

3. **Set up mobile development**
   ```bash
   cd ../../mobile
   ./gradlew build
   ```

4. **Set up cloud services**
   ```bash
   cd ../cloud
   pip install -r requirements.txt
   ```

## Documentation

- [Protocol Specification](docs/PROTOCOL_SPEC.md) - CartelWorx Protocol v1.2 details
- [API Reference](docs/API_REFERENCE.md) - Cloud service endpoints
- [Architecture Guide](docs/ARCHITECTURE.md) - System design & data flow
- [Contributing Guide](CONTRIBUTING.md) - Development guidelines

## Performance Targets

- **Knock Detection**: <2ms latency (DSP chain)
- **BLE Data Rate**: 100Hz critical stream (knock), 50Hz secondary (diagnostics)
- **CAN Polling**: 20-50Hz per PID, round-robin strategy
- **Mobile UI**: 60+ FPS needle animation, <100ms touch response
- **Cloud Training**: Weekly model updates with 10k+ vehicle telemetry events
- **OTA Deployment**: <5min firmware flash, staged rollout (10%/50%/100%)

## Safety & Compliance

- All tuning adjustments are bounded by safety limits
- Ignition timing: -30° to +40° BTDC (configurable per engine)
- Fuel trim: ±15% maximum adjustment
- CRC16 validation on all protocol frames
- Fallback to stock ECU parameters on communication loss

## Contributors

- **Taylor Berger** - Founder/CTO, Full-Stack Development

## License

MIT License - See LICENSE file for details

## Acknowledgments

- Espressif Systems for ESP-IDF & ESP32 ecosystem
- Google Cloud for ML/AI infrastructure
- Open-source FreeRTOS & NimBLE communities

---

**Status**: 🚧 Active Development - Phase 1 (Architecture Foundation)
**Last Updated**: December 14, 2025
**Current Version**: v0.1.0-alpha
