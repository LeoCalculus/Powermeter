#ifndef __USER_CAN_H
#define __USER_CAN_H

#include <can.h>
#include <stm32f1xx_hal_can.h>
#include <stdint.h>

void CAN_Init(CAN_HandleTypeDef *hcan);
void CAN_Filter_Mask_Config(CAN_HandleTypeDef *hcan, uint8_t Object_Para, uint32_t ID, uint32_t Mask_ID);
uint8_t CAN_Send_Data(CAN_HandleTypeDef *hcan, uint16_t ID, uint8_t *Data, uint16_t Length);
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);




#endif /* __USER_CAN_H */