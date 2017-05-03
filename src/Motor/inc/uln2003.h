/*
 * uln2003.h
 *
 *  Created on: 03 mag 2017
 *      Author: raidenfox
 */

#ifndef MOTOR_INC_ULN2003_H_
#define MOTOR_INC_ULN2003_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

#define GPIO_IN_PORT 	GPIOB

#define GPIO_IN1		GPIO_PIN_1
#define GPIO_IN2		GPIO_PIN_15
#define GPIO_IN3		GPIO_PIN_14
#define GPIO_IN4		GPIO_PIN_13

/* Init ULN2003 driver ports */
void ULN2003_Init();

/* Test rotation function */
void ULN2003_TestMotor();

#endif /* MOTOR_INC_ULN2003_H_ */
