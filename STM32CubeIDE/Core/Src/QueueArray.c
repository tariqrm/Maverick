/*
 * QueueArray.c
 *
 *  Created on: Mar 18, 2025
 *      Author: tmuth
 */

#include "QueueArray.h"
#include <stdlib.h>  // for malloc, free
#include <string.h>  // for memcpy

QueueArray_t* queue_create(size_t capacity, size_t element_size)
{
    // Basic parameter checks
    if (capacity == 0 || element_size == 0)
        return NULL;

    // Allocate the queue struct
    QueueArray_t *queue = (QueueArray_t*) malloc(sizeof(QueueArray_t));
    if (queue == NULL)
        return NULL; // not enough memory for the struct

    // Allocate the underlying data buffer
    queue->contents = malloc(capacity * element_size);
    if (queue->contents == NULL)
    {
        free(queue);
        return NULL; // not enough memory for the data
    }

    // Initialize fields
    queue->capacity     = capacity;
    queue->element_size = element_size;
    queue->count        = 0;
    queue->head         = 0;
    queue->tail         = 0;

    return queue;
}

void queue_destroy(QueueArray_t *queue)
{
    if (queue == NULL)
        return; // nothing to destroy

    // Free the contents buffer first
    if (queue->contents != NULL)
    {
        free(queue->contents);
        queue->contents = NULL;
    }

    // Now free the queue struct itself
    free(queue);
}

bool queue_push(QueueArray_t *queue, const void *item)
{
    if (queue == NULL || item == NULL)
        return false;

    // Check if full
    if (queue_is_full(queue))
        return false;

    // Compute the position where to store the new item
    uint8_t *base = (uint8_t*)queue->contents;
    size_t offset = queue->tail * queue->element_size;
    void *dest     = base + offset;

    // Copy from *item to the ring buffer
    memcpy(dest, item, queue->element_size);

    // Advance tail
    queue->tail = (queue->tail + 1) % queue->capacity;

    // Increment count
    queue->count++;

    return true;
}

bool queue_pop(QueueArray_t *queue, void *out)
{
    if (queue == NULL)
        return false;

    // Check if empty
    if (queue_is_empty(queue))
        return false;

    // If user wants the data, copy it to *out
    if (out != NULL)
    {
        uint8_t *base = (uint8_t*)queue->contents;
        size_t offset = queue->head * queue->element_size;
        void *src      = base + offset;

        memcpy(out, src, queue->element_size);
    }

    // Advance head
    queue->head = (queue->head + 1) % queue->capacity;
    // Decrement count
    queue->count--;

    return true;
}

bool queue_peek(const QueueArray_t *queue, void *out)
{
    if (queue == NULL || out == NULL)
        return false;

    if (queue_is_empty(queue))
        return false;

    // Copy from the head element to out
    const uint8_t *base = (const uint8_t*)queue->contents;
    size_t offset = queue->head * queue->element_size;
    const void *src = base + offset;

    memcpy(out, src, queue->element_size);

    return true;
}

bool queue_is_empty(const QueueArray_t *queue)
{
    if (queue == NULL)
        return true; // treat invalid as "empty"

    return (queue->count == 0);
}

bool queue_is_full(const QueueArray_t *queue)
{
    if (queue == NULL)
        return false; // treat invalid as "not full"

    return (queue->count == queue->capacity);
}

size_t queue_count(const QueueArray_t *queue)
{
    if (queue == NULL)
        return 0;

    return queue->count;
}

