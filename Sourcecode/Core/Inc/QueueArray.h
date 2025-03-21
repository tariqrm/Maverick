/*
 * QueueArray.h
 *
 *  Created on: Mar 21, 2025
 *      Author: tmuth
 */

#ifndef INC_QUEUEARRAY_H_
#define INC_QUEUEARRAY_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

// Queue structure for storing bytes.
typedef struct {
    uint8_t *contents;   // dynamically allocated array for items.
    uint16_t capacity;   // maximum number of items.
    uint16_t head;       // index of the front item.
    uint16_t tail;       // index where the next pushed item goes.
    uint16_t count;      // current number of items in the queue.
} QueueArray;

// Create and initialize a new queue with given capacity.
// On error, the function calls QueueArray_error (which does not return).
QueueArray* QueueArray_create(uint16_t capacity);

// Destroy a queue and free its memory.
void QueueArray_destroy(QueueArray* q);

// Push a byte into the queue.
// Returns true if successful; if the queue is full, returns false.
bool QueueArray_push(QueueArray* q, uint8_t value);

// Pop a byte from the queue.
// If the queue is empty, an error handler is invoked.
uint8_t QueueArray_pop(QueueArray* q);

// Return (without removing) the byte at the front of the queue.
// If the queue is empty, an error handler is invoked.
uint8_t QueueArray_peek(const QueueArray* q);

// Check if the queue is empty.
bool QueueArray_isEmpty(const QueueArray* q);

// Check if the queue is full.
bool QueueArray_isFull(const QueueArray* q);

// Return the current number of items in the queue.
uint16_t QueueArray_count(const QueueArray* q);


#endif /* INC_QUEUEARRAY_H_ */


