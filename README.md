# USB-MIDI

Refer to sample code of STM32_USB-Host-Device_Lib_V2.1.0. (STM website for details).

Created USB-MIDI example from USB HID Sample.

1. MOdified functions on STM32F2XXX platform.

    STM32_USB-Host-Device_Lib_V2.1.0/Project/USB_Device_Examples/MIDI/src/stm32fxxx_it.c
    STM32_USB-Host-Device_Lib_V2.1.0/Project/USB_Device_Examples/MIDI/src/usbd_desc.c
    STM32_USB-Host-Device_Lib_V2.1.0/Project/USB_Device_Examples/MIDI/src/usbd_usr.c
    STM32_USB-Host-Device_Lib_V2.1.0/Project/USB_Device_Examples/MIDI/inc/usbd_conf.h
    STM32_USB-Host-Device_Lib_V2.1.0/Project/USB_Device_Examples/MIDI/inc/usb_conf.h
    
2. Create USB MIDI Class Libraries
    STM32_USB-Host-Device_Lib_V2.1.0/Libraries/STM32_USB_Device_Library/Class/midi/src/usbd_midi_core.c
    STM32_USB-Host-Device_Lib_V2.1.0/Libraries/STM32_USB_Device_Library/Class/midi/inc/usbd_midi_core.h
    
3. Create one Test App
    STM32_USB-Host-Device_Lib_V2.1.0/Project/USB_Device_Examples/MIDI/src/app.c

4. Verified USB with Test Suite USB30CV

    STM32_USB-Host-Device_Lib_V2.1.0/Libraries/STM32_USB_Device_Library/Core/src/usbd_req.c
    Note: Modified Core Libraries in order to get pass from Diagnostic Test Suite of USB30CV

Reference:
1. Universal Serial Bus Device Class Definition for MIDI Devices
   https://www.usb.org/sites/default/files/midi10.pdf
2. USB Device Class Definition for Human Input Devices
   https://www.usb.org/sites/default/files/documents/hid1_11.pdf
3. USBCV Test Utilities 
   https://www.usb.org/documents?search=&category%5B%5D=50&items_per_page=50
