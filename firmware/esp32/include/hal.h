#ifndef HAL_H
#define HAL_H

#include <stdint.h>
#include <stdbool.h>
#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// CartelWorx Hardware Abstraction Layer (HAL) v1.0
// 
// Provides platform-independent interface for:
// - CAN/OBD-II communication
// - Analog-to-Digital Conversion (knock sensor)
// - Engine position sensing (crank angle)
// - Actuator control (ignition, fuel trim)
// - Bluetooth Low Energy communication
// - Timing and system utilities
// ============================================================================

// --- System & Initialization ---

/**
 * @brief Initialize the HAL and all subsystems
 * @return ESP_OK on success, error code otherwise
 */
esp_err_t hal_init_system(void);

/**
 * @brief Deinitialize HAL and cleanup resources
 * @return ESP_OK on success
 */
esp_err_t hal_deinit_system(void);

/**
 * @brief Get current system time in microseconds
 * @return Current time in microseconds (high resolution timer)
 */
uint64_t hal_get_time_us(void);

/**
 * @brief Get current system time in milliseconds
 * @return Current time in milliseconds
 */
uint32_t hal_get_time_ms(void);

/**
 * @brief Delay execution for specified milliseconds
 * @param ms Milliseconds to delay
 */
void hal_delay_ms(uint32_t ms);

// --- CAN/OBD-II Interface ---

#define HAL_CAN_MAX_DATA_LENGTH 8
#define HAL_CAN_RX_BUFFER_SIZE 16

typedef struct {
    uint32_t id;                          // CAN message ID
    uint8_t dlc;                          // Data length code (0-8)
    uint8_t data[HAL_CAN_MAX_DATA_LENGTH];// Data payload
    uint32_t timestamp_us;                // Timestamp in microseconds
    bool is_extended;                     // Extended ID format flag
} hal_can_frame_t;

/**
 * @brief Initialize CAN interface (500kbps for OBD-II)
 * @return ESP_OK on success
 */
esp_err_t hal_can_init(void);

/**
 * @brief Send CAN frame
 * @param frame Pointer to CAN frame structure
 * @return ESP_OK on success
 */
esp_err_t hal_can_send(const hal_can_frame_t *frame);

/**
 * @brief Read available CAN frame from RX buffer
 * @param frame Pointer to receive frame data
 * @return ESP_OK if frame available, ESP_ERR_INVALID_STATE if empty
 */
esp_err_t hal_can_read_frame(hal_can_frame_t *frame);

/**
 * @brief Get number of frames pending in RX buffer
 * @return Number of frames available to read
 */
uint8_t hal_can_get_rx_count(void);

/**
 * @brief Check if CAN interface is initialized and active
 * @return true if CAN is operational
 */
bool hal_can_is_active(void);

// --- ADC Interface (Knock Sensor) ---

#define HAL_ADC_KNOCK_SENSOR_CHANNEL 0  // GPIO34 for knock sensor
#define HAL_ADC_MAX_VALUE 4095           // 12-bit ADC
#define HAL_ADC_VREF_MV 3300            // 3.3V reference

/**
 * @brief Initialize ADC for knock sensor reading
 * @return ESP_OK on success
 */
esp_err_t hal_adc_init(void);

/**
 * @brief Read raw ADC value from knock sensor
 * @return Raw ADC value (0-4095)
 */
uint16_t hal_adc_read_knock_sensor(void);

/**
 * @brief Read and average multiple ADC samples (noise reduction)
 * @param num_samples Number of samples to average (typical: 4-8)
 * @return Averaged ADC value
 */
uint16_t hal_adc_read_knock_sensor_averaged(uint8_t num_samples);

/**
 * @brief Convert raw ADC value to voltage in millivolts
 * @param adc_value Raw ADC value
 * @return Voltage in millivolts
 */
uint32_t hal_adc_to_mv(uint16_t adc_value);

// --- Engine Position Sensing ---

/**
 * @brief Get current crank angle position
 * @return Crank angle in degrees (0-719 for 4-stroke cycle)
 */
uint16_t hal_get_crank_angle(void);

/**
 * @brief Get current engine RPM from crank position sensor
 * @return RPM value
 */
uint16_t hal_get_rpm(void);

/**
 * @brief Check if engine is cranking (starting)
 * @return true if RPM < 250
 */
bool hal_is_engine_cranking(void);

/**
 * @brief Get engine coolant temperature
 * @return Temperature in degrees Celsius
 */
int16_t hal_get_coolant_temp(void);

// --- Actuator Control ---

#define HAL_IGNITION_TIMING_MIN -30  // Degrees BTDC (safety limit)
#define HAL_IGNITION_TIMING_MAX 40   // Degrees BTDC (safety limit)
#define HAL_FUEL_TRIM_MIN -15        // Percent (safety limit)
#define HAL_FUEL_TRIM_MAX 15         // Percent (safety limit)

/**
 * @brief Set ignition timing adjustment
 * @param degrees_btdc Degrees before TDC (-30 to +40)
 * @return ESP_OK on success
 */
esp_err_t hal_set_ignition_timing(int16_t degrees_btdc);

/**
 * @brief Get current ignition timing setting
 * @return Degrees before TDC
 */
int16_t hal_get_ignition_timing(void);

/**
 * @brief Set fuel injector pulse width
 * @param microseconds Pulse width in microseconds
 * @return ESP_OK on success
 */
esp_err_t hal_set_fuel_injector_pulse(uint16_t microseconds);

/**
 * @brief Set fuel trim adjustment
 * @param percent Fuel trim adjustment (-15 to +15)
 * @return ESP_OK on success
 */
esp_err_t hal_set_fuel_trim(int8_t percent);

/**
 * @brief Set boost target (turbo/supercharger control)
 * @param kpa Target boost pressure in kPa
 * @return ESP_OK on success
 */
esp_err_t hal_set_boost_target(uint16_t kpa);

// --- Bluetooth Low Energy Interface ---

#define HAL_BLE_MTU_SIZE 517  // Maximum Transmission Unit

typedef struct {
    uint8_t *data;
    uint16_t length;
    uint16_t handle;  // GATT characteristic handle
} hal_ble_data_t;

/**
 * @brief Initialize Bluetooth LE (GATT Server mode)
 * @return ESP_OK on success
 */
esp_err_t hal_ble_init(void);

/**
 * @brief Start BLE advertising
 * @return ESP_OK on success
 */
esp_err_t hal_ble_start_advertising(void);

/**
 * @brief Stop BLE advertising
 * @return ESP_OK on success
 */
esp_err_t hal_ble_stop_advertising(void);

/**
 * @brief Send notification/indication to connected BLE client
 * @param characteristic_handle GATT characteristic handle
 * @param data Data to send
 * @param length Data length
 * @param is_notification true for notification, false for indication
 * @return ESP_OK on success
 */
esp_err_t hal_ble_send_notify(uint16_t characteristic_handle,
                               const uint8_t *data,
                               uint16_t length,
                               bool is_notification);

/**
 * @brief Check if BLE client is connected
 * @return true if connected
 */
bool hal_ble_is_connected(void);

/**
 * @brief Get current BLE MTU (negotiated with client)
 * @return MTU size in bytes
 */
uint16_t hal_ble_get_mtu(void);

/**
 * @brief Request MTU negotiation with BLE client
 * @param desired_mtu Desired MTU size (up to 517)
 * @return ESP_OK on success (actual MTU negotiated in callback)
 */
esp_err_t hal_ble_request_mtu(uint16_t desired_mtu);

// --- Interrupt & Event Handling ---

typedef void (*hal_interrupt_handler_t)(void);

/**
 * @brief Register CAN RX interrupt handler
 * @param handler Function pointer to call on CAN message
 * @return ESP_OK on success
 */
esp_err_t hal_register_can_interrupt(hal_interrupt_handler_t handler);

/**
 * @brief Register crank position sensor interrupt handler
 * @param handler Function pointer to call on crank sensor edge
 * @return ESP_OK on success
 */
esp_err_t hal_register_crank_interrupt(hal_interrupt_handler_t handler);

// --- Logging & Debug ---

/**
 * @brief Set log level for HAL module
 * @param level Log level (0=silent, 5=verbose)
 */
void hal_set_log_level(uint8_t level);

#ifdef __cplusplus
}
#endif

#endif // HAL_H
