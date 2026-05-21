#include <application.h>

VOFA_REPORT vofa;
float old_adc_read = 0.0f;
uint16_t adc1_buffer[3] = {0}; // only three ports used for dma
power_report report;

void controller_init(void){
    memset(&vofa, 0, sizeof(vofa));
    vofa.vofaTail[0] = 0x00;
    vofa.vofaTail[1] = 0x00;
    vofa.vofaTail[2] = 0x80;
    vofa.vofaTail[3] = 0x7f;
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 200); // start with 10% duty cycle
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // turn on the load
    report.current = 0.0f;
    report.voltage = 0.0f;
}

// resistor: 9.83k ohm -> 10k ohm;  984 ohm -> 1k ohm 
// ADC1_IN2 = VCC * 984 / (984 + 9830) = VCC * 0.090993 => used to derive VCC 

void controller_step(const float dt){

    float current = ((float)adc1_buffer[0] / 4095.0f * 3.0f - 1.24f) * 5.0f;
    float voltage = (float)adc1_buffer[1] / 4095.0f * 3.0f * 11.0f;

    report.current = current;
    report.voltage = voltage;

    // send report via CAN
    if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) > 0) {
        CAN_Send_Data(&hcan, 0x391, (uint8_t*)&report, sizeof(report));
    }
}