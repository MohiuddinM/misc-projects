#ifndef RINGBUFF_H_INCLUDED
#define RINGBUFF_H_INCLUDED

#include <stdlib.h>
#include <stdint.h>
typedef struct
{
    uint8_t *startAddr, *endAddr, *currentWriteAddr, *currentReadAddr;
} RingBuffer;

RingBuffer RingInit(short);
void RingPush(RingBuffer *, uint8_t);
uint8_t RingPull(RingBuffer *);
void RingDestroy(RingBuffer*);


#endif // RINGBUFF_H_INCLUDED
