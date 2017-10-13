# USB-MIDI

Modified sample code of STM32_USB-Host-Device_Lib_V2.1.0. Refer to STM32 website for details.
Created USB-MIDI example from USB HID Sample.

Verified functions on STM32F2XXX platform.

STM32_USB-Host-Device_Lib_V2.1.0/Project/USB_Device_Examples/MIDI/src/stm32fxxx_it.c
STM32_USB-Host-Device_Lib_V2.1.0/Project/USB_Device_Examples/MIDI/src/usbd_desc.c
STM32_USB-Host-Device_Lib_V2.1.0/Project/USB_Device_Examples/MIDI/src/usbd_usr.c
STM32_USB-Host-Device_Lib_V2.1.0/Project/USB_Device_Examples/MIDI/inc/usbd_conf.h
STM32_USB-Host-Device_Lib_V2.1.0/Project/USB_Device_Examples/MIDI/inc/usb_conf.h

Note: Modified Core Libraries in order to get pass from Diagnostic Test Suite of USB30CV
STM32_USB-Host-Device_Lib_V2.1.0/Libraries/STM32_USB_Device_Library/Core/src/usbd_req.c