#include <application.h>

VOFA_REPORT vofa;
float old_adc_read = 0.0f;
uint16_t adc1_buffer[3] = {0}; // only three ports used for dma

void controller_init(void){
    memset(&vofa, 0, sizeof(vofa));
    vofa.vofaTail[0] = 0x00;
    vofa.vofaTail[1] = 0x00;
    vofa.vofaTail[2] = 0x80;
    vofa.vofaTail[3] = 0x7f;
}


void controller_step(const float dt){
    // vofa.val[0]  = adc1_buffer[0] * 1.25f / 4095.0f / 20.0f; // Simulate some changing data
    // vofa.val[1]  = adc1_buffer[1] * 1.25f / 4095.0f / 20.0f; // Simulate some changing data
    float alpha = 0.5f; // low pass filter coefficient
    float current_current  = ((float)adc1_buffer[0] / 4095.0f * 3.33f - 1.25f)/(20.0f*0.005f); // the actual volatge now  
    vofa.val[0]  = current_current * alpha + old_adc_read * (1.0f - alpha); // low pass filter to smooth the current reading
    vofa.val[1]  = (float)adc1_buffer[1] / 4095.0f* 3.33f * 11.0f;
    vofa.val[2]  = vofa.val[0] * vofa.val[1]; // current calculated from voltage drop across 5mOhm resistor
    vofa.val[3]  = (float)adc1_buffer[2] / 4095.0f * 3.33f; // Vref, should be around 1.2V, used for calibration
    if (huart1.gState == HAL_UART_STATE_READY) {
        HAL_UART_Transmit_DMA(&huart1, (uint8_t*)&vofa, sizeof(vofa));
    }
    old_adc_read = vofa.val[0];
}