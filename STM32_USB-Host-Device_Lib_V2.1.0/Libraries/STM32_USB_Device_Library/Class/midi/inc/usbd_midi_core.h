/**
  ******************************************************************************
  * @file    usbd_midi_core.h
  * @author  Joey
  * @version V0.0.1
  * @date    14-May-2013
  * @brief   header file for the usbd_midi_core.c file.
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/

#ifndef __USB_MIDI_CORE_H_
#define __USB_MIDI_CORE_H_

#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */
  
/** @defgroup USBD_MIDI
  * @brief This file is the Header file for USBD_msc.c
  * @{
  */ 


/** @defgroup USBD_MIDI_Exported_Defines
  * @{
  */


#define AUDIO_SUBCLASS_MIDISTREAMING                  0x03
#define USB_DEVICE_CLASS_AUDIO                        0x01
#define USB_MIDI_CONFIG_DESC_SIZ                       101

/**
  * @}
  */ 


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */


/**
  * @}
  */ 



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */ 

extern USBD_Class_cb_TypeDef  USBD_MIDI_cb;
/**
  * @}
  */ 

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */ 
uint8_t USBD_MIDI_SendtoHost (USB_OTG_CORE_HANDLE  *pdev, 
                                           uint8_t *report,
                                           uint16_t len);
/**
  * @}
  */ 

#endif  // __USB_MIDI_CORE_H_
/**
  * @}
  */ 

/**
  * @}
  */ 
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
