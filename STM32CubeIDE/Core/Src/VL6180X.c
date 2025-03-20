/*
 * VL6180X.c
 *
 *  Created on: Mar 17, 2025
 *      Author: tmuth
 */

#include "vl6180x.h"

// A small helper for clamping a value between min and max
static inline int16_t clamp_int16(int16_t x, int16_t minVal, int16_t maxVal)
{
  if (x < minVal) return minVal;
  if (x > maxVal) return maxVal;
  return x;
}

// RANGE_SCALER values for 1x, 2x, 3x scaling
static const uint16_t ScalerValues[] = { 0, 253, 127, 84 };

/***********************************************************************
 *  Low-level helper to write single byte
 ***********************************************************************/
void VL6180X_WriteReg(VL6180X_Dev_t *dev, uint16_t reg, uint8_t value)
{
  uint8_t data[1];
  data[0] = value;

  // VL6180X uses a 16-bit register address in big-endian order
  HAL_StatusTypeDef status = HAL_I2C_Mem_Write(dev->hi2c,
                                               (dev->i2c_address << 1),
                                               reg,
                                               I2C_MEMADD_SIZE_16BIT,
                                               data,
                                               1,
                                               100);
  dev->last_status = (uint8_t)status;
}

/***********************************************************************
 *  Low-level helper to write 16 bits
 ***********************************************************************/
void VL6180X_WriteReg16Bit(VL6180X_Dev_t *dev, uint16_t reg, uint16_t value)
{
  uint8_t data[2];
  data[0] = (value >> 8) & 0xFF;
  data[1] = (value >> 0) & 0xFF;

  HAL_StatusTypeDef status = HAL_I2C_Mem_Write(dev->hi2c,
                                               (dev->i2c_address << 1),
                                               reg,
                                               I2C_MEMADD_SIZE_16BIT,
                                               data,
                                               2,
                                               100);
  dev->last_status = (uint8_t)status;
}

/***********************************************************************
 *  Low-level helper to write 32 bits
 ***********************************************************************/
void VL6180X_WriteReg32Bit(VL6180X_Dev_t *dev, uint16_t reg, uint32_t value)
{
  uint8_t data[4];
  data[0] = (value >> 24) & 0xFF;
  data[1] = (value >> 16) & 0xFF;
  data[2] = (value >> 8)  & 0xFF;
  data[3] = (value >> 0)  & 0xFF;

  HAL_StatusTypeDef status = HAL_I2C_Mem_Write(dev->hi2c,
                                               (dev->i2c_address << 1),
                                               reg,
                                               I2C_MEMADD_SIZE_16BIT,
                                               data,
                                               4,
                                               100);
  dev->last_status = (uint8_t)status;
}

/***********************************************************************
 *  Low-level helper to read single byte
 ***********************************************************************/
uint8_t VL6180X_ReadReg(VL6180X_Dev_t *dev, uint16_t reg)
{
  uint8_t value = 0;

  HAL_StatusTypeDef status = HAL_I2C_Mem_Read(dev->hi2c,
                                              (dev->i2c_address << 1),
                                              reg,
                                              I2C_MEMADD_SIZE_16BIT,
                                              &value,
                                              1,
                                              100);
  dev->last_status = (uint8_t)status;
  return value;
}

/***********************************************************************
 *  Low-level helper to read 16 bits
 ***********************************************************************/
uint16_t VL6180X_ReadReg16Bit(VL6180X_Dev_t *dev, uint16_t reg)
{
  uint8_t data[2];
  uint16_t value = 0;

  HAL_StatusTypeDef status = HAL_I2C_Mem_Read(dev->hi2c,
                                              (dev->i2c_address << 1),
                                              reg,
                                              I2C_MEMADD_SIZE_16BIT,
                                              data,
                                              2,
                                              100);
  dev->last_status = (uint8_t)status;

  if (status == HAL_OK)
  {
    value  = (uint16_t)(data[0] << 8);
    value |= (uint16_t)(data[1]);
  }
  return value;
}

/***********************************************************************
 *  Low-level helper to read 32 bits
 ***********************************************************************/
uint32_t VL6180X_ReadReg32Bit(VL6180X_Dev_t *dev, uint16_t reg)
{
  uint8_t data[4];
  uint32_t value = 0;

  HAL_StatusTypeDef status = HAL_I2C_Mem_Read(dev->hi2c,
                                              (dev->i2c_address << 1),
                                              reg,
                                              I2C_MEMADD_SIZE_16BIT,
                                              data,
                                              4,
                                              100);
  dev->last_status = (uint8_t)status;

  if (status == HAL_OK)
  {
    value  = ((uint32_t)data[0] << 24);
    value |= ((uint32_t)data[1] << 16);
    value |= ((uint32_t)data[2] <<  8);
    value |= ((uint32_t)data[3]);
  }
  return value;
}

/***********************************************************************
 *  Set sensor I2C address (7-bit).
 ***********************************************************************/
void VL6180X_SetAddress(VL6180X_Dev_t *dev, uint8_t new_addr)
{
  // Write new address to device
  VL6180X_WriteReg(dev, I2C_SLAVE__DEVICE_ADDRESS, (new_addr & 0x7F));
  dev->i2c_address = new_addr;
}

uint8_t VL6180X_GetAddress(VL6180X_Dev_t *dev)
{
  return dev->i2c_address;
}

/***********************************************************************
 *  Init device as per ST application note: mandatory private registers
 ***********************************************************************/
void VL6180X_Init(VL6180X_Dev_t *dev)
{
  // read part-to-part offset
  dev->ptp_offset = (int8_t)VL6180X_ReadReg(dev, SYSRANGE__PART_TO_PART_RANGE_OFFSET);

  uint8_t fresh = VL6180X_ReadReg(dev, SYSTEM__FRESH_OUT_OF_RESET);
  if (fresh == 1)
  {
    // sensor has just been reset
    dev->scaling = 1;

    // Mandatory private registers (from ST's app note AN4545)
    VL6180X_WriteReg(dev, 0x207, 0x01);
    VL6180X_WriteReg(dev, 0x208, 0x01);
    VL6180X_WriteReg(dev, 0x096, 0x00);
    VL6180X_WriteReg(dev, 0x097, 0xFD);
    VL6180X_WriteReg(dev, 0x0E3, 0x01);
    VL6180X_WriteReg(dev, 0x0E4, 0x03);
    VL6180X_WriteReg(dev, 0x0E5, 0x02);
    VL6180X_WriteReg(dev, 0x0E6, 0x01);
    VL6180X_WriteReg(dev, 0x0E7, 0x03);
    VL6180X_WriteReg(dev, 0x0F5, 0x02);
    VL6180X_WriteReg(dev, 0x0D9, 0x05);
    VL6180X_WriteReg(dev, 0x0DB, 0xCE);
    VL6180X_WriteReg(dev, 0x0DC, 0x03);
    VL6180X_WriteReg(dev, 0x0DD, 0xF8);
    VL6180X_WriteReg(dev, 0x09F, 0x00);
    VL6180X_WriteReg(dev, 0x0A3, 0x3C);
    VL6180X_WriteReg(dev, 0x0B7, 0x00);
    VL6180X_WriteReg(dev, 0x0BB, 0x3C);
    VL6180X_WriteReg(dev, 0x0B2, 0x09);
    VL6180X_WriteReg(dev, 0x0CA, 0x09);
    VL6180X_WriteReg(dev, 0x198, 0x01);
    VL6180X_WriteReg(dev, 0x1B0, 0x17);
    VL6180X_WriteReg(dev, 0x1AD, 0x00);
    VL6180X_WriteReg(dev, 0x0FF, 0x05);
    VL6180X_WriteReg(dev, 0x100, 0x05);
    VL6180X_WriteReg(dev, 0x199, 0x05);
    VL6180X_WriteReg(dev, 0x1A6, 0x1B);
    VL6180X_WriteReg(dev, 0x1AC, 0x3E);
    VL6180X_WriteReg(dev, 0x1A7, 0x1F);
    VL6180X_WriteReg(dev, 0x030, 0x00);

    VL6180X_WriteReg(dev, SYSTEM__FRESH_OUT_OF_RESET, 0);
  }
  else
  {
    // sensor may have been previously initialized
    uint16_t s = VL6180X_ReadReg16Bit(dev, RANGE_SCALER);

    if (s == ScalerValues[3])
      dev->scaling = 3;
    else if (s == ScalerValues[2])
      dev->scaling = 2;
    else
      dev->scaling = 1;

    // Adjust the part-to-part offset for the current scaling
    dev->ptp_offset *= dev->scaling;
  }
}

/***********************************************************************
 *  Configure public registers to recommended defaults
 ***********************************************************************/
void VL6180X_ConfigureDefault(VL6180X_Dev_t *dev)
{
  // "Recommended : Public registers"
  VL6180X_WriteReg(dev, READOUT__AVERAGING_SAMPLE_PERIOD, 0x30); // 48
  VL6180X_WriteReg(dev, SYSALS__ANALOGUE_GAIN, 0x46);            // gain = 1.01
  VL6180X_WriteReg(dev, SYSRANGE__VHV_REPEAT_RATE, 0xFF);
  VL6180X_WriteReg16Bit(dev, SYSALS__INTEGRATION_PERIOD, 0x0063); // 100 ms
  VL6180X_WriteReg(dev, SYSRANGE__VHV_RECALIBRATE, 0x01);

  // Optional
  VL6180X_WriteReg(dev, SYSRANGE__INTERMEASUREMENT_PERIOD, 0x09); // 100 ms
  VL6180X_WriteReg(dev, SYSALS__INTERMEASUREMENT_PERIOD, 0x31);   // 500 ms
  VL6180X_WriteReg(dev, SYSTEM__INTERRUPT_CONFIG_GPIO, 0x24);

  // Reset other settings
  VL6180X_WriteReg(dev, SYSRANGE__MAX_CONVERGENCE_TIME, 0x31);
  VL6180X_WriteReg(dev, INTERLEAVED_MODE__ENABLE, 0);

  // Ensure scaling is set to 1
  VL6180X_SetScaling(dev, 1);
}

/***********************************************************************
 *  Set measurement scaling
 ***********************************************************************/
void VL6180X_SetScaling(VL6180X_Dev_t *dev, uint8_t new_scaling)
{
  if (new_scaling < 1 || new_scaling > 3)
    return;

  dev->scaling = new_scaling;
  VL6180X_WriteReg16Bit(dev, RANGE_SCALER, ScalerValues[new_scaling]);

  // apply scaling to part-to-part offset
  VL6180X_WriteReg(dev, SYSRANGE__PART_TO_PART_RANGE_OFFSET, (uint8_t)(dev->ptp_offset / dev->scaling));

  // apply scaling to crossTalkValidHeight
  VL6180X_WriteReg(dev, SYSRANGE__CROSSTALK_VALID_HEIGHT, (uint8_t)(20 / dev->scaling));

  // enable early convergence estimate only at 1x
  uint8_t rce = VL6180X_ReadReg(dev, SYSRANGE__RANGE_CHECK_ENABLES);
  if (dev->scaling == 1)
    rce |= 0x01;  // set bit0
  else
    rce &= ~0x01; // clear bit0

  VL6180X_WriteReg(dev, SYSRANGE__RANGE_CHECK_ENABLES, rce);
}

uint8_t VL6180X_GetScaling(VL6180X_Dev_t *dev)
{
  return dev->scaling;
}

/***********************************************************************
 *  Single-shot range measurement
 ***********************************************************************/
uint8_t VL6180X_ReadRangeSingle(VL6180X_Dev_t *dev)
{
  VL6180X_WriteReg(dev, SYSRANGE__START, 0x01);
  return VL6180X_ReadRangeContinuous(dev);
}

/***********************************************************************
 *  Single-shot range measurement (mm)
 ***********************************************************************/
uint16_t VL6180X_ReadRangeSingleMillimeters(VL6180X_Dev_t *dev)
{
  uint8_t range = VL6180X_ReadRangeSingle(dev);
  return (uint16_t)(dev->scaling) * range;
}

/***********************************************************************
 *  Single-shot ambient measurement
 ***********************************************************************/
uint16_t VL6180X_ReadAmbientSingle(VL6180X_Dev_t *dev)
{
  VL6180X_WriteReg(dev, SYSALS__START, 0x01);
  return VL6180X_ReadAmbientContinuous(dev);
}

/***********************************************************************
 *  Start continuous range mode
 ***********************************************************************/
void VL6180X_StartRangeContinuous(VL6180X_Dev_t *dev, uint16_t period)
{
  int16_t period_reg = (int16_t)(period / 10) - 1;
  period_reg = clamp_int16(period_reg, 0, 254);

  VL6180X_WriteReg(dev, SYSRANGE__INTERMEASUREMENT_PERIOD, (uint8_t)period_reg);
  VL6180X_WriteReg(dev, SYSRANGE__START, 0x03);
}

/***********************************************************************
 *  Start continuous ambient mode
 ***********************************************************************/
void VL6180X_StartAmbientContinuous(VL6180X_Dev_t *dev, uint16_t period)
{
  int16_t period_reg = (int16_t)(period / 10) - 1;
  period_reg = clamp_int16(period_reg, 0, 254);

  VL6180X_WriteReg(dev, SYSALS__INTERMEASUREMENT_PERIOD, (uint8_t)period_reg);
  VL6180X_WriteReg(dev, SYSALS__START, 0x03);
}

/***********************************************************************
 *  Start continuous interleaved mode (ALS + Range back-to-back)
 ***********************************************************************/
void VL6180X_StartInterleavedContinuous(VL6180X_Dev_t *dev, uint16_t period)
{
  int16_t period_reg = (int16_t)(period / 10) - 1;
  period_reg = clamp_int16(period_reg, 0, 254);

  VL6180X_WriteReg(dev, INTERLEAVED_MODE__ENABLE, 1);
  VL6180X_WriteReg(dev, SYSALS__INTERMEASUREMENT_PERIOD, (uint8_t)period_reg);
  VL6180X_WriteReg(dev, SYSALS__START, 0x03);
}

/***********************************************************************
 *  Stop continuous mode
 ***********************************************************************/
void VL6180X_StopContinuous(VL6180X_Dev_t *dev)
{
  // per ST datasheet, the next single measurement is triggered, so give
  // some delay in your main code if you plan to start continuous again
  VL6180X_WriteReg(dev, SYSRANGE__START, 0x01);
  VL6180X_WriteReg(dev, SYSALS__START, 0x01);

  VL6180X_WriteReg(dev, INTERLEAVED_MODE__ENABLE, 0);
}

/***********************************************************************
 *  Read range in continuous mode
 ***********************************************************************/
uint8_t VL6180X_ReadRangeContinuous(VL6180X_Dev_t *dev)
{
  uint32_t start = HAL_GetTick();

  // wait until new sample ready
  while ((VL6180X_ReadReg(dev, RESULT__INTERRUPT_STATUS_GPIO) & 0x07) != 0x04)
  {
    if ((dev->io_timeout > 0) && ((HAL_GetTick() - start) > dev->io_timeout))
    {
      dev->did_timeout = true;
      return 255;  // indicates timeout
    }
  }

  uint8_t range = VL6180X_ReadReg(dev, RESULT__RANGE_VAL);
  // clear range interrupt
  VL6180X_WriteReg(dev, SYSTEM__INTERRUPT_CLEAR, 0x01);

  return range;
}

/***********************************************************************
 *  Read range in continuous mode (mm)
 ***********************************************************************/
uint16_t VL6180X_ReadRangeContinuousMillimeters(VL6180X_Dev_t *dev)
{
  uint8_t range = VL6180X_ReadRangeContinuous(dev);
  return (uint16_t)(dev->scaling) * range;
}

/***********************************************************************
 *  Read ambient in continuous mode
 ***********************************************************************/
uint16_t VL6180X_ReadAmbientContinuous(VL6180X_Dev_t *dev)
{
  uint32_t start = HAL_GetTick();

  // wait until new ALS sample ready
  while ((VL6180X_ReadReg(dev, RESULT__INTERRUPT_STATUS_GPIO) & 0x38) != 0x20)
  {
    if ((dev->io_timeout > 0) && ((HAL_GetTick() - start) > dev->io_timeout))
    {
      dev->did_timeout = true;
      return 0;
    }
  }

  uint16_t ambient = VL6180X_ReadReg16Bit(dev, RESULT__ALS_VAL);
  // clear ALS interrupt
  VL6180X_WriteReg(dev, SYSTEM__INTERRUPT_CLEAR, 0x02);

  return ambient;
}

/***********************************************************************
 *  Timeout helpers
 ***********************************************************************/
void VL6180X_SetTimeout(VL6180X_Dev_t *dev, uint16_t timeout)
{
  dev->io_timeout = timeout;
}

uint16_t VL6180X_GetTimeout(VL6180X_Dev_t *dev)
{
  return dev->io_timeout;
}

bool VL6180X_TimeoutOccurred(VL6180X_Dev_t *dev)
{
  bool tmp = dev->did_timeout;
  dev->did_timeout = false;
  return tmp;
}

/***********************************************************************
 *  Read range status bits
 ***********************************************************************/
uint8_t VL6180X_ReadRangeStatus(VL6180X_Dev_t *dev)
{
  // upper nibble: error code
  // lower nibble: not used
  uint8_t raw = VL6180X_ReadReg(dev, RESULT__RANGE_STATUS);
  return (raw >> 4);
}

