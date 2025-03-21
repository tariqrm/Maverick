/*
 * QueueArray.c
 *
 *  Created on: Mar 21, 2025
 *      Author: tmuth
 */

#include "QueueArray.h"

// Error handler: you can add your own error reporting or LED blinking here.
static void QueueArray_error(const char *msg) {

    while (1) {
    	printf("QueueArrayError!\r\n");
    	HAL_Delay(500);
    }
}

QueueArray* QueueArray_create(uint16_t capacity) {
    QueueArray* q = (QueueArray*) malloc(sizeof(QueueArray));
    if (q == NULL) {
        QueueArray_error("QUEUE: insufficient memory for queue structure");
    }
    q->contents = (uint8_t*) malloc(sizeof(uint8_t) * capacity);
    if (q->contents == NULL) {
        free(q);
        QueueArray_error("QUEUE: insufficient memory to initialize queue");
    }
    q->capacity = capacity;
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    return q;
}

void QueueArray_destroy(QueueArray* q) {
    if (q) {
        free(q->contents);
        free(q);
    }
}

bool QueueArray_push(QueueArray* q, uint8_t value) {
    if (QueueArray_isFull(q)) {
        // Queue is full; cannot push the new value.
        return false;
    }
    q->contents[q->tail] = value;
    q->tail = (q->tail + 1) % q->capacity;
    q->count++;
    return true;
}

uint8_t QueueArray_pop(QueueArray* q) {
    if (QueueArray_isEmpty(q)) {
        QueueArray_error("QUEUE: can't pop item from empty queue");
    }
    uint8_t item = q->contents[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->count--;
    return item;
}

uint8_t QueueArray_peek(const QueueArray* q) {
    if (QueueArray_isEmpty(q)) {
        QueueArray_error("QUEUE: can't peek item from empty queue");
    }
    return q->contents[q->head];
}

bool QueueArray_isEmpty(const QueueArray* q) {
    return q->count == 0;
}

bool QueueArray_isFull(const QueueArray* q) {
    return q->count == q->capacity;
}

uint16_t QueueArray_count(const QueueArray* q) {
    return q->count;
}


