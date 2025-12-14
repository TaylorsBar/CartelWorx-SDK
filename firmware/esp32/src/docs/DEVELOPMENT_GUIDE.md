# CartelWorx SDK Development Guide

## Current Status: Phase 1 - Architectural Foundation (In Progress)

This document outlines the development progress and next steps for the CartelWorx SDK project.

## Completed Tasks ✅

### 1. Project Structure & Documentation
- [x] Comprehensive README.md with project overview
- [x] Multi-layer architecture design (Edge, Mobile, Cloud)
- [x] Development roadmap with 5 phases
- [x] Technology stack specification

### 2. Firmware Build Configuration
- [x] PlatformIO configuration with multiple environments
- [x] ESP-IDF 5.x + FreeRTOS 10.x setup
- [x] BLE 5.3 configuration flags
- [x] Build optimizations for real-time processing
- [x] Debug and test environment configurations

### 3. Core Firmware Implementation
- [x] FreeRTOS entry point (main.cpp)
- [x] Multi-core task scheduling (Core 0: Knock detection, Core 1: CAN/BLE)
- [x] GATT server initialization for BLE connectivity
- [x] Task structure for:
  - Real-time knock monitoring
  - CAN PID request polling (20Hz per parameter)
  - CAN response processing
  - BLE data transmission queue

## In Progress Tasks 🚧

### 1. Hardware Abstraction Layer (HAL)
- [ ] CAN/OBD-II interface functions
- [ ] ADC reading for knock sensor
- [ ] Crank angle position acquisition
- [ ] Ignition timing control
- [ ] Fuel injector pulse width modulation
- [ ] BLE GATT data sending

### 2. Real-Time Diagnostics Engine
- [ ] Knock detection algorithm
  - Signal processing (FFT/band-pass filtering)
  - Adaptive noise floor learning
  - Per-cylinder knock score calculation
  - Immediate knock response (ignition retard)
- [ ] Ring buffer implementation for 512-sample windows
- [ ] Engine state tracking (RPM, load, temp, Lambda)

### 3. Custom Protocol Stack
- [ ] CartelWorx Protocol v1.2 frame structure
- [ ] Frame header, service codes, CRC16 validation
- [ ] Service implementations:
  - SERVICE_REALTIME_SENSOR_BURST (0xC0)
  - SERVICE_KNOCK_STREAM (0xC1)
  - SERVICE_ADAPTIVE_TUNING_STATE (0xC4)
  - SERVICE_TUNING_ADJUSTMENT (0xCE)
- [ ] Command parsing and response generation

### 4. CAN Interface
- [ ] Interrupt-driven CAN receiver (ISR + flag mechanism)
- [ ] Multi-PID polling round-robin strategy
- [ ] OBD-II PID parsing (RPM, IAT, MAP, Timing, Lambda, TPS)
- [ ] UDS Mode 2E support for parameter writes
- [ ] Safety bounds checking before ECU commands

### 5. BLE 5.3 Optimization
- [ ] MTU negotiation (target 517 bytes)
- [ ] High-priority connection requests
- [ ] Data buffering and notification batching
- [ ] Connection state management
- [ ] Error handling and reconnection logic

## Compilation & Build Strategy

### Current Development Environment
```bash
# Build for ESP32 with default configuration
cd firmware/esp32
platformio run

# Build for debug (with logging and symbols)
platformio run -e cartelworx-esp32-debug

# Run unit tests
platformio test -e cartelworx-esp32-test

# Upload to connected ESP32
platformio run -t upload

# Monitor serial output
platformio device monitor
```

### Build Outputs
- Binary: `.pio/build/cartelworx-esp32/firmware.bin`
- ELF: `.pio/build/cartelworx-esp32/firmware.elf`
- Map file (symbols): `.pio/build/cartelworx-esp32/firmware.map`

### Hardware Requirements
- **MCU**: ESP32-WROOM-32D (4MB Flash, 520KB SRAM)
- **CAN Controller**: MCP2515 SPI interface
- **Knock Sensor**: Piezoelectric transducer → 12-bit ADC
- **Bluetooth Antenna**: PCB trace or external (5dBi typical)
- **Power Supply**: 5V @ 500mA minimum

## Next Immediate Steps

### Week 1: HAL Implementation
1. Implement CAN interface functions
2. Add ADC reading with calibration
3. Test CAN communication with vehicle simulator
4. Verify OBD-II PID parsing

### Week 2: Real-Time Signal Processing
1. Implement knock detection algorithm
2. Add ring buffer data structure
3. Adaptive threshold learning
4. Per-cylinder knock tracking

### Week 3: Protocol & Communication
1. Implement CartelWorx Protocol frame handling
2. Add GATT characteristics for data exchange
3. Test BLE data streaming at 100Hz
4. Implement command parsing from mobile app

### Week 4: Integration & Testing
1. End-to-end firmware compilation test
2. Hardware-in-loop simulation
3. Performance profiling and optimization
4. Security audit (CRC validation, bounds checking)

## Key Performance Metrics

| Metric | Target | Status |
|--------|--------|--------|
| Knock Detection Latency | <2ms | 🔄 In progress |
| BLE Critical Stream | 100Hz | ✅ Queued |
| BLE Diagnostic Stream | 50Hz | ✅ Queued |
| CAN PID Polling | 20-50Hz per param | ✅ Configured |
| Mobile UI Response | <100ms | 📋 Planned |
| Code Size | <2MB | ✅ On track |
| Memory Usage | <400KB | ✅ On track |

## Testing Strategy

### Unit Tests
- Knock detection algorithm correctness
- Ring buffer operations
- Protocol frame parsing
- CAN message handling

### Integration Tests
- FreeRTOS task scheduling
- Inter-task communication (queues, semaphores)
- BLE GATT read/write operations
- CAN frame transmission/reception

### System Tests
- Full firmware compilation on clean environment
- Hardware upload and boot verification
- Real-time performance under load
- Error handling and recovery

## Code Organization Best Practices

1. **Header Files** (firmware/esp32/include/)
   - HAL interface definitions
   - Protocol definitions
   - Data structure types

2. **Source Files** (firmware/esp32/src/)
   - Modular implementation per feature
   - Clear separation of concerns
   - Inline documentation for complex logic

3. **Build System** (platformio.ini)
   - Multiple environments for different use cases
   - Consistent compiler flags and optimization levels
   - Platform-specific configurations

## IDE Setup

### Recommended IDEs
1. **VS Code + PlatformIO Extension** (Recommended)
   - Install PlatformIO IDE extension
   - Auto-detects connected ESP32
   - Built-in terminal and debugging

2. **ESP-IDF with ESP-IDF Tools**
   - Full ESP-IDF toolkit
   - Advanced debugging with JTAG
   - Menuconfig for fine-tuning

3. **Visual Studio Community + VisualGDB**
   - Full IDE experience
   - Advanced debugging capabilities
   - IntelliSense for ESP-IDF

## Debugging Tips

### Serial Output
```bash
# Monitor with filtering
platformio device monitor --filter=esp32_exception_decoder

# Save logs to file
platformio device monitor > firmware.log
```

### Performance Profiling
```c
// Use esp_timer for high-resolution timing
uint64_t start = esp_timer_get_time();
// ... operation ...
uint64_t elapsed_us = esp_timer_get_time() - start;
ESP_LOGI(TAG, "Operation took %lld us", elapsed_us);
```

### Task Monitoring
```c
// Check task stack usage
UBaseType_t stack_high_water = uxTaskGetStackHighWaterMark(knock_task_handle);
ESP_LOGI(TAG, "Knock task stack high water: %u bytes", stack_high_water * 4);
```

## Resource Links

- [ESP-IDF Documentation](https://docs.espressif.com/projects/esp-idf/)
- [FreeRTOS Documentation](https://www.freertos.org/)
- [NimBLE (Bluetooth LE Stack)](https://github.com/apache/mynewt-nimble)
- [CAN Protocol Overview](https://en.wikipedia.org/wiki/CAN_bus)
- [OBD-II PID Reference](https://en.wikipedia.org/wiki/OBD-II_PID)

## Contributing

When contributing code:
1. Follow the existing code style (snake_case for variables/functions)
2. Add detailed comments for complex logic
3. Test on hardware before submitting
4. Update this guide if adding new components
5. Ensure no compiler warnings with current flags

---

**Last Updated**: December 14, 2025
**Version**: 0.1.0-alpha
**Next Review**: January 18, 2026
