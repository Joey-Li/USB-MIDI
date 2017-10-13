/**
  ******************************************************************************
  * @file    usbd_midi_core.c
  * @author  Joey
  * @version V0.0.1
  * @date    14-May-2013
  * @brief   This file provides the MIDI core functions.
  *
  * @verbatim
  *      
  *          ===================================================================      
  *                          USB Device Class for MIDI Devices
  *          =================================================================== 
  *          This module manages the MIDI device following the "USB Device Class
  *          Definition for MIDI Devices Release 1.0 Nov 1, 1999".
  *          This driver implements the following aspects of the specification:
  *             - The MIDI Interface without audio control interface
	*             - Two Endpoint for MIDI IN/OUT
  *      
  * @note    In FS mode.
  *           
  *      
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbd_midi_core.h"
#include "usbd_desc.h"
#include "usbd_req.h"


/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_MIDI 
  * @brief usbd core module
  * @{
  */ 

/** @defgroup USBD_MIDI_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBD_MIDI_Private_Defines
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup USBD_MIDI_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 




/** @defgroup USBD_MIDI_Private_FunctionPrototypes
  * @{
  */
static uint8_t  USBD_MIDI_Init (void  *pdev, uint8_t cfgidx);
static uint8_t  USBD_MIDI_DeInit (void  *pdev, uint8_t cfgidx);
static uint8_t  USBD_MIDI_Setup (void  *pdev, USB_SETUP_REQ *req);
static uint8_t  *USBD_MIDI_GetCfgDesc (uint8_t speed, uint16_t *length);
static uint8_t  USBD_MIDI_DataIn (void  *pdev, uint8_t epnum);
static uint8_t  USBD_MIDI_DataOut (void *pdev, uint8_t epnum);
/**
  * @}
  */ 

/** @defgroup USBD_MIDI_Private_Variables
  * @{
  */ 

USBD_Class_cb_TypeDef  USBD_MIDI_cb = 
{
  USBD_MIDI_Init,
  USBD_MIDI_DeInit,
  USBD_MIDI_Setup,
  NULL, /*EP0_TxSent*/  
  NULL, /*EP0_RxReady*/
  USBD_MIDI_DataIn, /*DataIn*/
  USBD_MIDI_DataOut, /*DataOut*/
  NULL, /*SOF */
  NULL,
  NULL,      
  USBD_MIDI_GetCfgDesc, 
};

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */ 

__ALIGN_BEGIN uint8_t USB_Rx_Buffer   [MIDI_DATA_MAX_PACKET_SIZE] __ALIGN_END ;
__ALIGN_BEGIN uint8_t USB_Tx_Buffer   [MIDI_DATA_MAX_PACKET_SIZE] __ALIGN_END ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */        
__ALIGN_BEGIN static uint32_t  USBD_MIDI_AltSet  __ALIGN_END = 0;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */      
//__ALIGN_BEGIN static uint32_t  USBD_MIDI_Protocol  __ALIGN_END = 0;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */  
//__ALIGN_BEGIN static uint32_t  USBD_MIDI_IdleState __ALIGN_END = 0;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */ 
/* USB MIDI device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_MIDI_CfgDesc[USB_MIDI_CONFIG_DESC_SIZ] __ALIGN_END =
{

  /* Configuration 1 */
  0x09,                                 /* bLength */
  USB_CONFIGURATION_DESCRIPTOR_TYPE,    /* bDescriptorType */
  LOBYTE(USB_MIDI_CONFIG_DESC_SIZ),     /* wTotalLength, in bytes */
  HIBYTE(USB_MIDI_CONFIG_DESC_SIZ),      
  0x02,                                 /* bNumInterfaces */
  0x01,                                 /* bConfigurationValue */
  0x00,                                 /* iConfiguration */
  0xC0,                                 /* bmAttributes  BUS Powred */
  0xC8,                                 /* bMaxPower = 100 mA */
  /* 09 byte*/

  // ========= 	
  /* B.3 AudioControl Interface Descriptors */
  /* B.3.1 Standard AudioControl Interface Descriptors */
  /* Table B-3: MIDI Adpter Standard AC interdace Descriptor */
  0x09,                                 /* bLength: Interface Descriptor size */
  0x04,                                 /* bDescriptorType: Interface descriptor type */
  0x00,                                 /* bInterfaceNumber: Number of Interface */
  0x00,                                 /* bAlternateSetting: Alternate setting */
  0x00,                                 /* bNumEndpoints: no endpoints */
  0x01,                                 /* bInterfaceClass: Audio device class */
  0x01,                                 /* bInterfaceSubClass : Audio control */
  0x00,                                 /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
  0x00,                                 /* iInterface: Index of string descriptor */
 /* B.3.2 Class-specific AudioControl Interface Descriptors */
 /* Table B-4: MIDI Adpter Class-specific AC interdace Descriptor */
  0x09,                                 /* bLength */
  0x24,                                 /* bDescriptorType: CS_INTERFACE */
  0x01,                                 /* bDescriptorSubtype, MS_HEADER */
  0x00,
  0x01,                                 /* bcdADC, Revision of Class specification 1.0 */
  0x09,                                  
  0x00,                                 /* wTotalLength */
  0x01,                                 /* bInCollection: Number of streaming Interfaces*/
  0x01,                                 /* baInterfaceNr(1): MIDIStreaming interface 1 belongs to this AC interface*/
  // ========= 

  /* Standard MS Interface Descriptor */
	/* MIDI Adapter Standard MS Interface Descriptor */
	0x09,                                 /* bLength */
  0x04,                                 /* bDescriptorType */
  0x01,                                 /* bInterfaceNumber */
  0x00,                                 /* bAlternateSetting */
  0x02,                                 /* bNumEndpoints */
  USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
  AUDIO_SUBCLASS_MIDISTREAMING,         /* bInterfaceSubClass */
  0x00,                                 /* bInterfaceProtocol */
  0x00,                                 /* iInterface */
  /* 09 byte*/
  
	/* Class-specific MS Interface Descriptor */
  /* MIDI Adapter Class-specific MS Interface Descriptor */ 
  0x07,                                 /* bLength */
  0x24,                                 /* bDescriptorType, CS_INTERFACE_DESCRIPTOR*/
  0x01,                                 /* bDescriptorSubtype, MS_HEADER */
  0x00,                                 /* bcdADC */
  0x01,
  0x41,                                 /* wTotalLength = 0x41*/
  0x00,

  /* MIDI OUT Jack Descriptor */
	
       // External              Embedded   
       // OUT Jack 0x01  -----  IN Jack 0x02  ----+   
       //      bulk OUT endpoint   (to PC)

  /* MIDI Adapter MIDI OUT Jack Descriptor (External) */
  0x09,                                 /* bLength */
  0x24,                                 /* bDescriptorType */
  0x03,                                 /* bDescriptorSubType, MIDI_OUT_JACK */
  0x02,                                 /* bJackType, EXTERNAL */
  0x01,                                 /* bJackID */
  0x01,                                 /* bNrInputPins*/
  0x02,                                 /* BaSourceID */
  0x01,                                 /* BaSourcePin */
  0x00,                                 /* iJack, Unused */
		
  /* MIDI Adapter MIDI IN Jack Descriptor (Embedded) */
	0x06,                                 /* bLength */
  0x24,                                 /* bDescriptorType */
  0x02,                                 /* bDescriptorSubType, MIDI_IN_JACK */
  0x01,                                 /* bJackType, EMBEDDED */
  0x02,                                 /* bJackID */
  0x00,                                 /* iJack, Unused */
	
	     //   External              Embedded
       // IN Jack 0x21  -----  OUT Jack 0x22   
       //
 
  /* MIDI Adapter MIDI IN Jack Descriptor (External) */
	
	0x06,                                 /* bLength */
  0x24,                                 /* bDescriptorType */
  0x02,                                 /* bDescriptorSubType, MIDI_IN_JACK */
  0x02,                                 /* bJackType, External */
  0x21,                                 /* bJackID */
  0x00,                                 /* iJack, Unused */

  /* MIDI Adapter MIDI OUT Jack Descriptor (Embedded) */  
  0x09,                                 /* bLength */
  0x24,                                 /* bDescriptorType */
  0x03,                                 /* bDescriptorSubType, MIDI_OUT_JACK */
  0x01,                                 /* bJackType, Embedded */
  0x22,                                 /* bJackID */
  0x01,                                 /* bNrInputPins*/
  0x21,                                 /* BaSourceID */
  0x01,                                 /* BaSourcePin */
  0x00,                                 /* iJack, Unused */
  
  /* B.5 Bulk OUT Endpoint Descriptor */
  /* B.5.1 Standard Bulk OUT Endpoint Descriptor */
	/* Table B-11: MIDI Adapter Standard Bulk OUT Endpoint Descriptor */
 
    // Emb IN Jack 0x02  ----+----- bulk OUT endpoint 3   
		
  0x09,                                 /* bLength */
  0x05,                                 /* bDescriptorType, ENDPOINT_DESCRIPTOR */
  MAIN_ENDPOINT_OUT,                    /* bEndpointAddress, Host to deivce, Endponit3_out */
  0x02,                                 /* bmAttributes, BULK,not share */
  0x40,                                 /* wMaxPacketSize */
  0x00,                                 
  0x00,                                 /* bInterval , Ignored for Bulk. Set to Zero */
  0x00,                                 /* bRefresh. Unused */
  0x00,                                 /* bSynchAddress. Unused */

  /* B.5.2 Class-specific MS Bulk OUT Endpoint Descriptor */
  /* Table B-12: MIDI Adapter Class-specific Bulk Out Endpoint Descriptor */  
  0x05,                                 /* bLength */
  0x25,                                 /* bDescriptorType, CS_ENDPOINT */
  0x01,                                 /* bDescriptorSubType, MS_GENERAL sub_type */
  0x01,                                 /* bNumEmbMIDIJack, Number of Embedded MIDI Out Jacks */
  0x02,                                 /* BaAssocJackID, ID of the Embedded MIDI In Jack  */

  /* B.6 Bulk IN Endpoint Descriptor */
  /* B.5.1 Standard Bulk In Endpoint Descriptor */
	/* Table B-13: MIDI Adapter Standard Bulk In Endpoint Descriptor */ 

     // Emb OUT Jack 0x22  ----+----- bulk IN endpoint  1 
 
  0x09,                                 /* bLength */
  0x05,                                 /* bDescriptorType, ENDPOINT_DESCRIPTOR */
  MAIN_ENDPOINT_IN,                     /* bEndpointAddress, Device to Host, Endponit1_in */
  0x02,                                 /* bmAttributes, BULK,not share */
  0x40,                                 /* wMaxPacketSize */
  0x00,
  0x00,                                 /* bInterval , Ignored for Bulk. Set to Zero */
  0x00,                                 /* bRefresh. Unused */
  0x00,                                 /* bSynchAddress. Unused */
  /* B.6.2 Class-specific MS Bulk IN Endpoint Descriptor */
  /* Table B-12: MIDI Adapter Class-specific Bulk In Endpoint Descriptor */  
  0x05,                                 /* bLength */
  0x25,                                 /* bDescriptorType, CS_ENDPOINT */
  0x01,                                 /* bDescriptorSubType, MS_GENERAL sub_type */
  0x01,                                 /* bNumEmbMIDIJack, Number of Embedded MIDI Out Jacks */
  0x22                                  /* BaAssocJackID, ID of the Embedded MIDI Out Jack */

} ;

/**
  * @}
  */ 

/** @defgroup USBD_MIDI_Private_Functions
  * @{
  */ 

/**
  * @brief  USBD_MIDI_Init
  *         Initialize the MIDI interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_MIDI_Init (void  *pdev, uint8_t cfgidx)
{

  /* Open EP IN */	
	DCD_EP_Open(pdev,
              MAIN_ENDPOINT_IN,
              MIDI_MAX_IN_PACKET,
              USB_OTG_EP_BULK);
	/* Open EP OUT */
	DCD_EP_Open(pdev,
              MAIN_ENDPOINT_OUT,
              MIDI_MAX_OUT_PACKET,
              USB_OTG_EP_BULK);
	
	DCD_EP_PrepareRx(pdev,
                   MAIN_ENDPOINT_OUT,
                   (uint8_t*)(USB_Rx_Buffer),
                   MIDI_MAX_OUT_PACKET);
  //printf(">>>     USBD_MIDI_Init\r\n" ); 
  return USBD_OK;
}

/**
  * @brief  USBD_MIDI_DeInit
  *         DeInitialize the MIDI layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_MIDI_DeInit (void  *pdev, uint8_t cfgidx)
{
  /* Close EPs */
  DCD_EP_Close (pdev , MAIN_ENDPOINT_IN);
  DCD_EP_Close (pdev , MAIN_ENDPOINT_OUT); 
  //printf(">>>     USBD_MIDI_DeInit\r\n" );
  return USBD_OK;
}

/**
  * @brief  USBD_MIDI_Setup
  *         Handle the MIDI specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  USBD_MIDI_Setup (void  *pdev, USB_SETUP_REQ *req)
{
  uint16_t len = 0;
  uint8_t  *pbuf = NULL;
  //printf(">>>     USBD_MIDI_Setup\r\n" );
  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
//   case USB_REQ_TYPE_CLASS :  
//     switch (req->bRequest)
//     {
//       
//       
//     case HID_REQ_SET_PROTOCOL:
//       USBD_HID_Protocol = (uint8_t)(req->wValue);
//       break;
//       
//     case HID_REQ_GET_PROTOCOL:
//       USBD_CtlSendData (pdev, 
//                         (uint8_t *)&USBD_HID_Protocol,
//                         1);    
//       break;
//       
//     case HID_REQ_SET_IDLE:
//       USBD_HID_IdleState = (uint8_t)(req->wValue >> 8);
//       break;
//       
//     case HID_REQ_GET_IDLE:
//       USBD_CtlSendData (pdev, 
//                         (uint8_t *)&USBD_HID_IdleState,
//                         1);        
//       break;      
//       
//     default:
//       USBD_CtlError (pdev, req);
//       return USBD_FAIL; 
//     }
//     break;
    
  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQ_GET_DESCRIPTOR: 
//       if( req->wValue >> 8 == HID_REPORT_DESC)
//       {
//         len = MIN(HID_MOUSE_REPORT_DESC_SIZE , req->wLength);
//         pbuf = HID_MOUSE_ReportDesc;
//       }
//       else if( req->wValue >> 8 == HID_DESCRIPTOR_TYPE)
//       {
//         
// #ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
//         pbuf = USBD_HID_Desc;   
// #else
//         pbuf = USBD_HID_CfgDesc + 0x12;
// #endif 
//         len = MIN(USB_HID_DESC_SIZ , req->wLength);
//       }
      
      USBD_CtlSendData (pdev, 
                        pbuf,
                        len);
      
      break;
      
    case USB_REQ_GET_INTERFACE :
      USBD_CtlSendData (pdev,
                        (uint8_t *)&USBD_MIDI_AltSet,
                        1);
      break;
      
    case USB_REQ_SET_INTERFACE :
      USBD_MIDI_AltSet = (uint8_t)(req->wValue);
      break;
    }
  }
  return USBD_OK;
}

/**
  * @brief  USBD_MIDI_SendtoHost 
  *         Send MIDI Message
  * @param  pdev: device instance
  * @param  buff: pointer to report
  * @retval status
  */
uint8_t USBD_MIDI_SendtoHost (USB_OTG_CORE_HANDLE  *pdev, uint8_t *message, uint16_t len)
{
  if (pdev->dev.device_status == USB_OTG_CONFIGURED )
  {
    DCD_EP_Tx (pdev, MAIN_ENDPOINT_IN, message, len);
  }
  return USBD_OK;
}

/**
  * @brief  USBD_MIDI_GetCfgDesc 
  *         return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_MIDI_GetCfgDesc (uint8_t speed, uint16_t *length)
{
	//printf(">>>     USBD_MIDI_GetCfgDesc\r\n" );
  *length = sizeof (USBD_MIDI_CfgDesc);
  return USBD_MIDI_CfgDesc;
}

/**
  * @brief  USBD_MIDI_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_MIDI_DataIn (void  *pdev, uint8_t epnum)
{
  /* Ensure that the FIFO is empty before a new transfer, this condition could 
  be caused by  a new transfer before the end of the previous transfer */
	//printf(">>>     USBD_MIDI_DataIn\r\n" );
  DCD_EP_Flush(pdev, MAIN_ENDPOINT_IN);

  return USBD_OK;
}

/**
  * @brief  USBD_MIDI_DataOut
  *         Data received on non-control Out endpoint
  * @param  pdev: device instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t  USBD_MIDI_DataOut (void *pdev, uint8_t epnum)
{      
  uint16_t USB_Rx_Cnt;
  /* Get the received data buffer and update the counter */
  USB_Rx_Cnt = ((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[epnum].xfer_count;
  //printf(">>>     USBD_MIDI_DataOut, USB_Rx_Cnt = %d \r\n" ,USB_Rx_Cnt);
	
  /* USB data will be immediately processed, this allow next USB traffic being 
     NAKed till the end of the application Xfer */
	
  //APP_FOPS.pIf_DataRx(USB_Rx_Buffer, USB_Rx_Cnt);
  // Need to process RX buff

  
  /* Prepare Out endpoint to receive next packet */
  DCD_EP_PrepareRx(pdev,
                   MAIN_ENDPOINT_OUT,
                   (uint8_t*)(USB_Rx_Buffer),
                   MIDI_DATA_MAX_PACKET_SIZE);

  return USBD_OK;
}

/**
  * @}
  */ 


/**
  * @}
  */ 


/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
