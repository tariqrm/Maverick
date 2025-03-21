/*
 * VL6180X.h
 *
 *  Created on: Mar 17, 2025
 *      Author: tmuth
 */

#ifndef INC_VL6180X_H_
#define INC_VL6180X_H_

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

// Default 7-bit I2C address for VL6180X
#define VL6180X_ADDRESS_DEFAULT        0x29  // 0b0101001

// Error definitions (range status)
#define VL6180X_ERROR_NONE             0
#define VL6180X_ERROR_SYSERR_1         1
#define VL6180X_ERROR_SYSERR_2         2
#define VL6180X_ERROR_SYSERR_3         3
#define VL6180X_ERROR_SYSERR_4         4
#define VL6180X_ERROR_SYSERR_5         5
#define VL6180X_ERROR_ECEFAIL          6
#define VL6180X_ERROR_NOCONVERGE       7
#define VL6180X_ERROR_RANGEIGNORE      8
#define VL6180X_ERROR_SNR             11
#define VL6180X_ERROR_RAWUFLOW        12
#define VL6180X_ERROR_RAWOFLOW        13
#define VL6180X_ERROR_RANGEUFLOW      14
#define VL6180X_ERROR_RANGEOFLOW      15

// VL6180X registers (as in original .h enum)
typedef enum
{
  IDENTIFICATION__MODEL_ID              = 0x000,
  IDENTIFICATION__MODEL_REV_MAJOR       = 0x001,
  IDENTIFICATION__MODEL_REV_MINOR       = 0x002,
  IDENTIFICATION__MODULE_REV_MAJOR      = 0x003,
  IDENTIFICATION__MODULE_REV_MINOR      = 0x004,
  IDENTIFICATION__DATE_HI               = 0x006,
  IDENTIFICATION__DATE_LO               = 0x007,
  IDENTIFICATION__TIME                  = 0x008,

  SYSTEM__MODE_GPIO0                    = 0x010,
  SYSTEM__MODE_GPIO1                    = 0x011,
  SYSTEM__HISTORY_CTRL                  = 0x012,
  SYSTEM__INTERRUPT_CONFIG_GPIO         = 0x014,
  SYSTEM__INTERRUPT_CLEAR               = 0x015,
  SYSTEM__FRESH_OUT_OF_RESET            = 0x016,
  SYSTEM__GROUPED_PARAMETER_HOLD        = 0x017,

  SYSRANGE__START                       = 0x018,
  SYSRANGE__THRESH_HIGH                 = 0x019,
  SYSRANGE__THRESH_LOW                  = 0x01A,
  SYSRANGE__INTERMEASUREMENT_PERIOD     = 0x01B,
  SYSRANGE__MAX_CONVERGENCE_TIME        = 0x01C,
  SYSRANGE__CROSSTALK_COMPENSATION_RATE = 0x01E,
  SYSRANGE__CROSSTALK_VALID_HEIGHT      = 0x021,
  SYSRANGE__EARLY_CONVERGENCE_ESTIMATE  = 0x022,
  SYSRANGE__PART_TO_PART_RANGE_OFFSET   = 0x024,
  SYSRANGE__RANGE_IGNORE_VALID_HEIGHT   = 0x025,
  SYSRANGE__RANGE_IGNORE_THRESHOLD      = 0x026,
  SYSRANGE__MAX_AMBIENT_LEVEL_MULT      = 0x02C,
  SYSRANGE__RANGE_CHECK_ENABLES         = 0x02D,
  SYSRANGE__VHV_RECALIBRATE             = 0x02E,
  SYSRANGE__VHV_REPEAT_RATE             = 0x031,

  SYSALS__START                         = 0x038,
  SYSALS__THRESH_HIGH                   = 0x03A,
  SYSALS__THRESH_LOW                    = 0x03C,
  SYSALS__INTERMEASUREMENT_PERIOD       = 0x03E,
  SYSALS__ANALOGUE_GAIN                 = 0x03F,
  SYSALS__INTEGRATION_PERIOD            = 0x040,

  RESULT__RANGE_STATUS                  = 0x04D,
  RESULT__ALS_STATUS                    = 0x04E,
  RESULT__INTERRUPT_STATUS_GPIO         = 0x04F,
  RESULT__ALS_VAL                       = 0x050,
  RESULT__HISTORY_BUFFER_0              = 0x052,
  RESULT__HISTORY_BUFFER_1              = 0x054,
  RESULT__HISTORY_BUFFER_2              = 0x056,
  RESULT__HISTORY_BUFFER_3              = 0x058,
  RESULT__HISTORY_BUFFER_4              = 0x05A,
  RESULT__HISTORY_BUFFER_5              = 0x05C,
  RESULT__HISTORY_BUFFER_6              = 0x05E,
  RESULT__HISTORY_BUFFER_7              = 0x060,
  RESULT__RANGE_VAL                     = 0x062,
  RESULT__RANGE_RAW                     = 0x064,
  RESULT__RANGE_RETURN_RATE             = 0x066,
  RESULT__RANGE_REFERENCE_RATE          = 0x068,
  RESULT__RANGE_RETURN_SIGNAL_COUNT     = 0x06C,
  RESULT__RANGE_REFERENCE_SIGNAL_COUNT  = 0x070,
  RESULT__RANGE_RETURN_AMB_COUNT        = 0x074,
  RESULT__RANGE_REFERENCE_AMB_COUNT     = 0x078,
  RESULT__RANGE_RETURN_CONV_TIME        = 0x07C,
  RESULT__RANGE_REFERENCE_CONV_TIME     = 0x080,

  RANGE_SCALER                          = 0x096,

  READOUT__AVERAGING_SAMPLE_PERIOD      = 0x10A,
  FIRMWARE__BOOTUP                      = 0x119,
  FIRMWARE__RESULT_SCALER               = 0x120,
  I2C_SLAVE__DEVICE_ADDRESS             = 0x212,
  INTERLEAVED_MODE__ENABLE              = 0x2A3,
} VL6180X_Reg_t;

// Device handle structure
typedef struct
{
  I2C_HandleTypeDef *hi2c;   // STM32 HAL I2C handle
  uint8_t i2c_address;       // 7-bit address (default 0x29)
  uint8_t last_status;       // last HAL status or custom error
  uint8_t scaling;
  int8_t  ptp_offset;
  uint16_t io_timeout;       // timeout in ms (0 means no timeout)
  bool did_timeout;
} VL6180X_Dev_t;

/*==========================================================================
    FUNCTION PROTOTYPES
  ==========================================================================*/
void     VL6180X_Init(VL6180X_Dev_t *dev);
void     VL6180X_ConfigureDefault(VL6180X_Dev_t *dev);
void     VL6180X_SetAddress(VL6180X_Dev_t *dev, uint8_t new_addr);
uint8_t  VL6180X_GetAddress(VL6180X_Dev_t *dev);

void     VL6180X_WriteReg(VL6180X_Dev_t *dev, uint16_t reg, uint8_t value);
void     VL6180X_WriteReg16Bit(VL6180X_Dev_t *dev, uint16_t reg, uint16_t value);
void     VL6180X_WriteReg32Bit(VL6180X_Dev_t *dev, uint16_t reg, uint32_t value);

uint8_t  VL6180X_ReadReg(VL6180X_Dev_t *dev, uint16_t reg);
uint16_t VL6180X_ReadReg16Bit(VL6180X_Dev_t *dev, uint16_t reg);
uint32_t VL6180X_ReadReg32Bit(VL6180X_Dev_t *dev, uint16_t reg);

void     VL6180X_SetScaling(VL6180X_Dev_t *dev, uint8_t new_scaling);
uint8_t  VL6180X_GetScaling(VL6180X_Dev_t *dev);

uint8_t  VL6180X_ReadRangeSingle(VL6180X_Dev_t *dev);
uint16_t VL6180X_ReadRangeSingleMillimeters(VL6180X_Dev_t *dev);

uint16_t VL6180X_ReadAmbientSingle(VL6180X_Dev_t *dev);

void     VL6180X_StartRangeContinuous(VL6180X_Dev_t *dev, uint16_t period);
void     VL6180X_StartAmbientContinuous(VL6180X_Dev_t *dev, uint16_t period);
void     VL6180X_StartInterleavedContinuous(VL6180X_Dev_t *dev, uint16_t period);

void     VL6180X_StopContinuous(VL6180X_Dev_t *dev);

uint8_t  VL6180X_ReadRangeContinuous(VL6180X_Dev_t *dev);
uint16_t VL6180X_ReadRangeContinuousMillimeters(VL6180X_Dev_t *dev);

uint16_t VL6180X_ReadAmbientContinuous(VL6180X_Dev_t *dev);

void     VL6180X_SetTimeout(VL6180X_Dev_t *dev, uint16_t timeout);
uint16_t VL6180X_GetTimeout(VL6180X_Dev_t *dev);
bool     VL6180X_TimeoutOccurred(VL6180X_Dev_t *dev);

uint8_t  VL6180X_ReadRangeStatus(VL6180X_Dev_t *dev);

#endif /* INC_VL6180X_H_ */
