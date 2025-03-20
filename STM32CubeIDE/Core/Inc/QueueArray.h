/*
 * QueueArray.h
 *
 *  Created on: Mar 18, 2025
 *      Author: tmuth
 */

#ifndef INC_QUEUEARRAY_H_
#define INC_QUEUEARRAY_H_

#include <stdint.h>   // for uint16_t, etc.
#include <stdbool.h>  // for bool
#include <stddef.h>   // for size_t

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A ring-buffer queue holding generic data elements.
 */
typedef struct
{
    void    *contents;     // pointer to allocated array buffer
    size_t   capacity;     // number of elements that fit in 'contents'
    size_t   element_size; // size in bytes of one element
    size_t   count;        // how many elements are in the queue
    size_t   head;         // index of the front element
    size_t   tail;         // index of the next free slot
} QueueArray_t;

/**
 * Create a new queue.
 *
 * \param capacity     The maximum number of elements the queue can hold
 * \param element_size The size (in bytes) of each element stored in the queue
 * \return             Pointer to a newly allocated QueueArray_t, or NULL on error
 */
QueueArray_t* queue_create(size_t capacity, size_t element_size);

/**
 * Destroy a queue created by queue_create().
 * Frees all memory. The pointer becomes invalid after this call.
 *
 * \param queue Pointer to an existing QueueArray_t
 */
void queue_destroy(QueueArray_t *queue);

/**
 * Push (enqueue) an element to the tail of the queue.
 *
 * \param queue Pointer to an existing QueueArray_t
 * \param item  Pointer to the data to be stored. Must be at least element_size bytes.
 * \return      true on success, false if the queue is full or queue is invalid
 */
bool queue_push(QueueArray_t *queue, const void *item);

/**
 * Pop (dequeue) the front element from the queue.
 *
 * \param queue Pointer to an existing QueueArray_t
 * \param out   Pointer to a buffer for storing the popped element (element_size bytes).
 *              If NULL, the item is removed but not copied anywhere.
 * \return      true on success, false if the queue is empty or invalid
 */
bool queue_pop(QueueArray_t *queue, void *out);

/**
 * Peek the front element from the queue without removing it.
 *
 * \param queue Pointer to an existing QueueArray_t
 * \param out   Pointer to a buffer where the front element should be copied
 * \return      true on success, false if the queue is empty or invalid
 */
bool queue_peek(const QueueArray_t *queue, void *out);

/**
 * Check if the queue is empty.
 *
 * \param queue Pointer to an existing QueueArray_t
 * \return      true if empty or invalid, false otherwise
 */
bool queue_is_empty(const QueueArray_t *queue);

/**
 * Check if the queue is full.
 *
 * \param queue Pointer to an existing QueueArray_t
 * \return      true if full or invalid, false otherwise
 */
bool queue_is_full(const QueueArray_t *queue);

/**
 * Get the current number of items in the queue.
 *
 * \param queue Pointer to an existing QueueArray_t
 * \return      Number of items, or 0 if queue is invalid
 */
size_t queue_count(const QueueArray_t *queue);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* INC_QUEUEARRAY_H_ */



