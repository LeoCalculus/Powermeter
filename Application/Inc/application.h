#ifndef __APPLICATION_H
#define __APPLICATION_H

#include <stdint.h>
#include <usart.h>
#include <string.h>
#include <stdlib.h>
#include <adc.h>
#include <tim.h>

typedef struct __attribute__((packed)) VOFA_REPORT{
    float val[10]; // used + reserved
    unsigned char vofaTail[4];
}VOFA_REPORT;

extern VOFA_REPORT vofa;
extern uint16_t adc1_buffer[3]; // only three ports used for dma

void controller_init(void);
void controller_step(const float dt);

#endif /* __APPLICATION_H */