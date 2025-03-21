/*
 * Motors.h
 *
 *  Created on: Mar 20, 2025
 *      Author: tmuth
 */

#ifndef INC_MOTORS_H_
#define INC_MOTORS_H_

#include "main.h"
#include <stdint.h>

// If you prefer #defines to label your channels:
#define PWMA_CH TIM_CHANNEL_3
#define PWMB_CH TIM_CHANNEL_4

// The default 8-bit approach (0..255), set ARR=255
extern TIM_HandleTypeDef htim4;  // from main.c or tim.c

void analogWrite(uint32_t channel, uint16_t duty)
{
    // duty is typically 0..255 if ARR=255
    // clamp if needed
    if (duty > 255) duty = 255;

    __HAL_TIM_SET_COMPARE(&htim4, channel, duty);
}

void stbyHigh()
{
    HAL_GPIO_WritePin(STBY_GPIO_Port, STBY_Pin, GPIO_PIN_SET);
}

void stbyLow()
{
    HAL_GPIO_WritePin(STBY_GPIO_Port, STBY_Pin, GPIO_PIN_RESET);
}

void leftForward()
{
    HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
}

void leftReverse()
{
    HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
}

void leftBrake()
{
    HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
}

void leftForwardBase()
{
    stbyHigh();
    leftForward();
    analogWrite(PWMA_CH, leftBase / 2);
}

void leftReverseBase()
{
    stbyHigh();
    leftReverse();
    analogWrite(PWMA_CH, leftBase / 2);
}

void rightForward()
{
    HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
}

void rightReverse()
{
    HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);
}

void rightBrake()
{
    HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);
}

void rightForwardBase()
{
    stbyHigh();
    rightForward();
    analogWrite(PWMB_CH, rightBase / 2);
}

void rightReverseBase()
{
    stbyHigh();
    rightReverse();
    analogWrite(PWMB_CH, rightBase / 2);
}

void writePwm()
{
    analogWrite(PWMA_CH, leftPwm / 2);
    analogWrite(PWMB_CH, rightPwm / 2);
}

void writeBasePwm()
{
    analogWrite(PWMA_CH, leftBase / 2);
    analogWrite(PWMB_CH, rightBase / 2);
}

void forward()
{
    stbyHigh();
    leftForward();
    rightForward();
    writePwm();
}

void reverse()
{
    stbyHigh();
    leftReverse();
    rightReverse();
    writePwm();
}

void brake()
{
    stbyHigh();
    leftBrake();
    rightBrake();
    HAL_Delay(75);
}

void brakeNo()
{
    stbyHigh();
    leftBrake();
    rightBrake();
    // no delay
}

void forwardBase()
{
    stbyHigh();
    leftForward();
    rightForward();
    writeBasePwm();
}

void reverseBase()
{
    stbyHigh();
    leftReverse();
    rightReverse();
    writeBasePwm();
}

void turnRight()
{
    stbyHigh();
    leftForward();
    rightReverse();
    writeBasePwm();
}

void turnLeft()
{
    stbyHigh();
    leftReverse();
    rightForward();
    writeBasePwm();
}

#endif /* INC_MOTORS_H_ */
