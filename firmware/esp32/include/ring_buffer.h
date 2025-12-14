#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief Ring Buffer (Circular Buffer) Template-like Implementation for C
 * 
 * Fixed-size ring buffer optimized for real-time embedded systems.
 * O(1) insertion and removal. No dynamic allocation after initialization.
 * Thread-safe when used with proper synchronization (mutexes/semaphores).
 */

/**
 * @brief Generic ring buffer structure
 * 
 * Use RING_BUFFER_CREATE and RING_BUFFER_DECLARE to create instances.
 */
typedef struct {
    uint8_t *buffer;      // Data buffer
    uint32_t size;        // Total buffer size (must be power of 2 for efficiency)
    uint32_t head;        // Write pointer
    uint32_t tail;        // Read pointer
    uint32_t count;       // Number of valid items
    uint32_t element_size;// Size of each element in bytes
} ring_buffer_t;

/**
 * @brief Initialize a ring buffer
 * 
 * @param rb Ring buffer structure
 * @param buffer Pre-allocated buffer memory
 * @param buffer_size Total size in bytes (should be divisible by element_size)
 * @param element_size Size of each element stored
 * @return true on success, false if parameters invalid
 */
bool ring_buffer_init(ring_buffer_t *rb,
                      uint8_t *buffer,
                      uint32_t buffer_size,
                      uint32_t element_size);

/**
 * @brief Push an element to the back of the ring buffer
 * 
 * Overwrites oldest element if buffer is full (FIFO wrap-around).
 * 
 * @param rb Ring buffer
 * @param element Pointer to element data
 * @return true if successful (or overwrote), false on error
 */
bool ring_buffer_push(ring_buffer_t *rb, const void *element);

/**
 * @brief Push multiple elements
 * 
 * @param rb Ring buffer
 * @param elements Pointer to array of elements
 * @param count Number of elements to push
 * @return Number of elements actually pushed
 */
uint32_t ring_buffer_push_multiple(ring_buffer_t *rb,
                                     const void *elements,
                                     uint32_t count);

/**
 * @brief Pop an element from the front of the ring buffer
 * 
 * @param rb Ring buffer
 * @param element Pointer to receive popped element
 * @return true if element was popped, false if buffer empty
 */
bool ring_buffer_pop(ring_buffer_t *rb, void *element);

/**
 * @brief Pop multiple elements
 * 
 * @param rb Ring buffer
 * @param elements Pointer to receive array of elements
 * @param count Number of elements to pop
 * @return Number of elements actually popped
 */
uint32_t ring_buffer_pop_multiple(ring_buffer_t *rb,
                                    void *elements,
                                    uint32_t count);

/**
 * @brief Peek at front element without removing it
 * 
 * @param rb Ring buffer
 * @param element Pointer to receive peeked element
 * @return true if element exists, false if buffer empty
 */
bool ring_buffer_peek(const ring_buffer_t *rb, void *element);

/**
 * @brief Peek at arbitrary index
 * 
 * @param rb Ring buffer
 * @param index Index from front (0 = front, 1 = second, etc.)
 * @param element Pointer to receive peeked element
 * @return true if index valid, false otherwise
 */
bool ring_buffer_peek_at(const ring_buffer_t *rb,
                         uint32_t index,
                         void *element);

/**
 * @brief Get number of valid elements in buffer
 * 
 * @param rb Ring buffer
 * @return Number of elements
 */
static inline uint32_t ring_buffer_count(const ring_buffer_t *rb) {
    return rb->count;
}

/**
 * @brief Get total capacity of buffer
 * 
 * @param rb Ring buffer
 * @return Maximum number of elements that can be stored
 */
static inline uint32_t ring_buffer_capacity(const ring_buffer_t *rb) {
    return rb->size / rb->element_size;
}

/**
 * @brief Check if buffer is empty
 * 
 * @param rb Ring buffer
 * @return true if empty
 */
static inline bool ring_buffer_is_empty(const ring_buffer_t *rb) {
    return rb->count == 0;
}

/**
 * @brief Check if buffer is full
 * 
 * @param rb Ring buffer
 * @return true if full
 */
static inline bool ring_buffer_is_full(const ring_buffer_t *rb) {
    return rb->count == ring_buffer_capacity(rb);
}

/**
 * @brief Clear all elements from buffer
 * 
 * @param rb Ring buffer
 */
static inline void ring_buffer_clear(ring_buffer_t *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}

/**
 * @brief Get space available for writing
 * 
 * @param rb Ring buffer
 * @return Number of free slots
 */
static inline uint32_t ring_buffer_available(const ring_buffer_t *rb) {
    return ring_buffer_capacity(rb) - rb->count;
}

/**
 * @brief Get pointer to linear write region (for zero-copy operations)
 * 
 * Returns contiguous region available for writing without wrap-around.
 * Use with care - ensure synchronization with other operations.
 * 
 * @param rb Ring buffer
 * @param out_size Receives size of available region in bytes
 * @return Pointer to write region, NULL if buffer full or error
 */
void *ring_buffer_get_write_region(ring_buffer_t *rb, uint32_t *out_size);

/**
 * @brief Advance write pointer after zero-copy write
 * 
 * @param rb Ring buffer
 * @param bytes_written Number of bytes written to write region
 * @return true if successful
 */
bool ring_buffer_advance_write(ring_buffer_t *rb, uint32_t bytes_written);

/**
 * @brief Get pointer to linear read region (for zero-copy operations)
 * 
 * @param rb Ring buffer
 * @param out_size Receives size of available region in bytes
 * @return Pointer to read region, NULL if buffer empty or error
 */
const void *ring_buffer_get_read_region(const ring_buffer_t *rb, uint32_t *out_size);

/**
 * @brief Advance read pointer after zero-copy read
 * 
 * @param rb Ring buffer
 * @param bytes_read Number of bytes read from read region
 * @return true if successful
 */
bool ring_buffer_advance_read(ring_buffer_t *rb, uint32_t bytes_read);

#endif // RING_BUFFER_H
