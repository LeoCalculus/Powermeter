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
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 200); // start with 10% duty cycle
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // turn on the load
}

// resistor: 9.83k ohm -> 10k ohm;  984 ohm -> 1k ohm 
// ADC1_IN2 = VCC * 984 / (984 + 9830) = VCC * 0.090993 => used to derive VCC 

void controller_step(const float dt){
    // float alpha = 0.5f; // low pass filter coefficient
    // float current_current  = ((float)adc1_buffer[0] / 4095.0f * 3.33f - 1.25f)/(20.0f*0.005f); // the actual volatge now  
    // vofa.val[0]  = current_current * alpha + old_adc_read * (1.0f - alpha); // low pass filter to smooth the current reading
    // vofa.val[1]  = (float)adc1_buffer[1] / 4095.0f* 3.33f * 11.0f;
    // vofa.val[2]  = vofa.val[0] * vofa.val[1]; // current calculated from voltage drop across 5mOhm resistor
    // vofa.val[3]  = (float)adc1_buffer[2] / 4095.0f * 3.33f; // Vref, should be around 1.2V, used for calibration
    
    // define vofa 0 for the voltage reading: (984+9830)/984=10.9898
    vofa.val[0]  = (float)adc1_buffer[1] / 4095.0f * 3.0f * 11.0f; // voltage reading, with a voltage divider of 10k and 1k
    // if we have 1A -> 1A * 0.01 * 20 + 1.24V = ADC1_IN1 so I = (ADC1_IN1-1.24f) / (0.01 * 20) = ADC1_IN1 * 5, so the current reading is:
    vofa.val[1]  = ((float)adc1_buffer[0] / 4095.0f * 3.0f - 1.24f) * 5.0f;
    vofa.val[2]  = vofa.val[0] * vofa.val[1]; // power reading

    if (huart1.gState == HAL_UART_STATE_READY) {
        HAL_UART_Transmit_DMA(&huart1, (uint8_t*)&vofa, sizeof(vofa));
    }
    old_adc_read = vofa.val[0];
}