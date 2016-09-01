#include "RingBuff.h"
#include <stdlib.h>
#include <stdint.h>
int loc;
RingBuffer RingInit(short lenght)
{
    uint8_t *values = (uint8_t *)malloc(lenght * sizeof(uint8_t));
    RingBuffer _buff;
    _buff.startAddr = values;
    _buff.endAddr = values + lenght * sizeof(uint8_t);
    _buff.currentWriteAddr = _buff.startAddr;
    _buff.currentReadAddr = _buff.startAddr;
    return _buff;
}
void RingDestroy(RingBuffer *_buff)
{
    free(_buff->startAddr);
    free(_buff);
}
void RingPush(RingBuffer *_buff, uint8_t val)
{
    *(_buff->currentWriteAddr) = val;
    if(_buff->currentWriteAddr == _buff->endAddr) _buff->currentWriteAddr = _buff->startAddr;
    else _buff->currentWriteAddr += sizeof(uint8_t);
    ++loc;
}
uint8_t RingPull(RingBuffer *_buff)
{
    if(0 < loc--)
    {
        int val = *(_buff->currentReadAddr);
        if(_buff->currentReadAddr == _buff->endAddr) _buff->currentReadAddr = _buff->startAddr;
        else    _buff->currentReadAddr += sizeof(uint8_t);
        return val;
    }
    return NULL;

}
