/*
 * Tof.h
 *
 *  Created on: Mar 21, 2025
 *      Author: tmuth
 */

#ifndef INC_TOF_H_
#define INC_TOF_H_

#include "stm32f1xx_hal.h"  // Ensure you include the HAL header for I2C_HandleTypeDef
extern I2C_HandleTypeDef hi2c1;
#include "VL6180X.h"

VL6180X_Dev_t tof1;
VL6180X_Dev_t tof2;
VL6180X_Dev_t tof3;
VL6180X_Dev_t tof4;
VL6180X_Dev_t tof5;

#define SENSOR1_ADDRESS  0x2B
#define SENSOR2_ADDRESS  0x2C
#define SENSOR3_ADDRESS  0x2D
#define SENSOR4_ADDRESS  0x2E
#define SENSOR5_ADDRESS  0x2F


void VL6180X_Setup(){
	// Make sure all sensors are off initially.
	HAL_GPIO_WritePin(GPIO1_GPIO_Port, GPIO1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO2_GPIO_Port, GPIO2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO3_GPIO_Port, GPIO3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO4_GPIO_Port, GPIO4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIO5_GPIO_Port, GPIO5_Pin, GPIO_PIN_RESET);

	// Power up each sensor in turn, assign unique address, init and configure.

	// --------------------- Sensor 1 --------------------

	HAL_GPIO_WritePin(GPIO1_GPIO_Port, GPIO1_Pin, GPIO_PIN_SET);
	tof1.hi2c = &hi2c1;
	tof1.i2c_address = VL6180X_ADDRESS_DEFAULT; // 0x29
	tof1.io_timeout = 10;
	tof1.did_timeout = false;

	VL6180X_Init(&tof1);
	VL6180X_ConfigureDefault(&tof1);
	VL6180X_SetAddress(&tof1, SENSOR1_ADDRESS);

	// Keep sensor 1 at 0x29 or explicitly set it (redundant):
	// VL6180X_SetAddress(&vl6180x_dev1, SENSOR1_ADDRESS);

	// --------------------- Sensor 2 --------------------

	HAL_GPIO_WritePin(GPIO2_GPIO_Port, GPIO2_Pin, GPIO_PIN_SET);
	tof2.hi2c = &hi2c1;
	tof2.i2c_address = VL6180X_ADDRESS_DEFAULT; // 0x29 initially
	tof2.io_timeout = 10;
	tof2.did_timeout = false;

	VL6180X_Init(&tof2);
	VL6180X_ConfigureDefault(&tof2);
	// Set new address to 0x2A
	VL6180X_SetAddress(&tof2, SENSOR2_ADDRESS);

	// --------------------- Sensor 3 --------------------

	HAL_GPIO_WritePin(GPIO3_GPIO_Port, GPIO3_Pin, GPIO_PIN_SET);
	tof3.hi2c = &hi2c1;
	tof3.i2c_address = VL6180X_ADDRESS_DEFAULT;
	tof3.io_timeout = 10;
	tof3.did_timeout = false;

	VL6180X_Init(&tof3);
	VL6180X_ConfigureDefault(&tof3);
	VL6180X_SetAddress(&tof3, SENSOR3_ADDRESS);

	// --------------------- Sensor 4 --------------------

	HAL_GPIO_WritePin(GPIO4_GPIO_Port, GPIO4_Pin, GPIO_PIN_SET);
	tof4.hi2c = &hi2c1;
	tof4.i2c_address = VL6180X_ADDRESS_DEFAULT;
	tof4.io_timeout = 10;
	tof4.did_timeout = false;

	VL6180X_Init(&tof4);
	VL6180X_ConfigureDefault(&tof4);
	VL6180X_SetAddress(&tof4, SENSOR4_ADDRESS);

	// --------------------- Sensor 5 --------------------

	HAL_GPIO_WritePin(GPIO5_GPIO_Port, GPIO5_Pin, GPIO_PIN_SET);
	tof5.hi2c = &hi2c1;
	tof5.i2c_address = VL6180X_ADDRESS_DEFAULT;
	tof5.io_timeout = 10;
	tof5.did_timeout = false;

	VL6180X_Init(&tof5);
	VL6180X_ConfigureDefault(&tof5);
	VL6180X_SetAddress(&tof5, SENSOR5_ADDRESS);

	printf("All 5 VL6180X sensors are initialized.\r\n");
}

void tofPid()
{
    tof[0] = VL6180X_ReadRangeSingle(&tof2);
    tof[2] = VL6180X_ReadRangeSingle(&tof3);
    tof[4] = VL6180X_ReadRangeSingle(&tof4);
}

void tofStart()
{
    tof[1] = VL6180X_ReadRangeSingle(&tof1);
    tof[2] = VL6180X_ReadRangeSingle(&tof3);
    tof[3] = VL6180X_ReadRangeSingle(&tof5);
}

void tofReadAll()
{
    tof[0] = VL6180X_ReadRangeSingle(&tof2);
    tof[1] = VL6180X_ReadRangeSingle(&tof1);
    tof[2] = VL6180X_ReadRangeSingle(&tof3);
    tof[3] = VL6180X_ReadRangeSingle(&tof5);
    tof[4] = VL6180X_ReadRangeSingle(&tof4);
}

void printWallState()
{
    printf("printWallState()\r\n");
    for (int i=0; i<3; i++)
    {
    	printf("%d ", cellWalls[i]);
        printf(" ");
    }
    printf("\r\n");
}

void checkWallsCell()
{
    printf("checkWallsCell()\r\n");
    if (x == 0 && y == 0)
    {
        count = 0;
        while(count <10)
        {
            tofStart();
            if (tof[2] <= 190)
            {
            frontWallAvailable = frontWallAvailable + 1;
            }
            else
            {
            frontWallAvailable = frontWallAvailable - 1;
            }
            if (tof[1] <= 150)
            {
            leftWallAvailable= leftWallAvailable + 1;
            }
            else
            {
            leftWallAvailable= leftWallAvailable - 1;
            }
            if (tof[3] <= 150)
            {
            rightWallAvailable= rightWallAvailable + 1;
            }
            else
            {
            rightWallAvailable= rightWallAvailable - 1;
            }
            count = count + 1;
        }
    }
    if (frontWallAvailable >=0 )
    {
        cellWalls[1] =  1;
        F = true;
        //frontWallAvailable = 0;
    }
    else
    {
        cellWalls[1] =  0;
        F = false;
        //frontWallAvailable = 0;
    }
    if (leftWallAvailable >= 0)
    {
        cellWalls[0] = 1;
        L = true;
        //leftWallAvailable = 0;
    }
    else
    {
        cellWalls[0] = 0;
        L = false;
        //leftWallAvailable = 0;
    }
    if (rightWallAvailable >= 0)
    {
        cellWalls[2] = 1;
        R = true;
        //rightWallAvailable = 0;
    }
    else
    {
        cellWalls[2] = 0;
        R = false;
        //rightWallAvailable = 0;
    }
    printWallState();

}



#endif /* INC_TOF_H_ */
