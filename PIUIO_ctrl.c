#include "PIUIO.h"
#include "PIUIO_ctrl.h"
#include "Descriptors.h"

unsigned char LampData[8];
unsigned char InputData[8];

unsigned char LXLampData[16];
unsigned char LXInputData[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff};

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
#if (ARCH == ARCH_AVR8)
    /* Disable watchdog if enabled by bootloader/fuses */
    wdt_reset(); 
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    /* Disable clock division */
    clock_prescale_set(clock_div_1);

#elif (ARCH == ARCH_XMEGA)
    /* Start the PLL to multiply the 2MHz RC oscillator to 32MHz and switch the CPU core to run from it */
    XMEGACLK_StartPLL(CLOCK_SRC_INT_RC2MHZ, 2000000, F_CPU);
    XMEGACLK_SetCPUClockSource(CLOCK_SRC_PLL);

    /* Start the 32MHz internal RC oscillator and start the DFLL to increase it to 48MHz using the USB SOF as a reference */
    XMEGACLK_StartInternalOscillator(CLOCK_SRC_INT_RC32MHZ);
    XMEGACLK_StartDFLL(CLOCK_SRC_INT_RC32MHZ, DFLL_REF_INT_USBSOF, F_USB);

    PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
#endif

    /* Hardware Initialization */
    USB_Init();
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
    //bool ConfigSuccess = true;
    // Do not enable SOFs
    USB_Device_EnableSOFEvents();
}

int nControl = 0;

void EVENT_USB_Device_ControlRequestPIUIO(void) {
    if(USB_ControlRequest.bRequest == 0xAE && piuio_which_device == 0) {   // Access Game IO as PIUIO
        nControl++;
        if(!(USB_ControlRequest.bmRequestType & 0x80))    {

            Endpoint_SelectEndpoint(ENDPOINT_CONTROLEP);
            while (!(Endpoint_IsINReady()));

            // Read the lamp values
            Endpoint_ClearSETUP();
            Endpoint_Read_Control_Stream_LE(LampData, 8);
            Endpoint_ClearIN();

            /* mark the whole request as successful: */
            //Endpoint_ClearStatusStage();
            //LEDoff(RXLED);
        }                                       // Reading input data
        else
        {
            //LEDon(TXLED);

            Endpoint_SelectEndpoint(ENDPOINT_CONTROLEP);

            // Set the switch state
            Endpoint_ClearSETUP();
            Endpoint_Write_Control_Stream_LE(InputData, 8);
            Endpoint_ClearOUT();

            /* mark the whole request as successful: */
            //Endpoint_ClearStatusStage();
            //LEDoff(TXLED);
        }
    }
}

bool using_report_protocol = true;
uint16_t IdleCount = 0;
void EVENT_USB_Device_ControlRequestLXIO(void) {
    // Extracted from LUFA/Drivers/USB/Class/Device/HIDClassDevice.c
    // Modified to match the LXIO
    switch (USB_ControlRequest.bRequest)
    {
        case HID_REQ_GetReport:
            if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE)) // 0xA1
            {
                uint16_t ReportSize = 16;
                uint8_t  ReportID   = (USB_ControlRequest.wValue & 0xFF);
                //uint8_t  ReportType = (USB_ControlRequest.wValue >> 8) - 1;
                uint8_t  ReportData[16];

                memcpy(ReportData, LXInputData, 16);

                Endpoint_SelectEndpoint(ENDPOINT_CONTROLEP);

                Endpoint_ClearSETUP();

                if (ReportID)
                    Endpoint_Write_8(ReportID);

                Endpoint_Write_Control_Stream_LE(ReportData, ReportSize);
                Endpoint_ClearOUT();
            }

            break;
        case HID_REQ_SetReport:
            if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE)) // 0x21
            {
                uint16_t ReportSize = 16;
                //uint8_t  ReportID   = (USB_ControlRequest.wValue & 0xFF);
                //uint8_t  ReportType = (USB_ControlRequest.wValue >> 8) - 1;
                uint8_t  ReportData[ReportSize];

                Endpoint_ClearSETUP();
                Endpoint_Read_Control_Stream_LE(ReportData, ReportSize);
                Endpoint_ClearIN();

                memcpy(LXLampData, ReportData, 16);
            }

            break;
        case HID_REQ_GetProtocol:
            if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                Endpoint_ClearSETUP();
                while (!(Endpoint_IsINReady()));
                Endpoint_Write_8(using_report_protocol);
                Endpoint_ClearIN();
                Endpoint_ClearStatusStage();
            }

            break;
        case HID_REQ_SetProtocol:
            if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                Endpoint_ClearSETUP();
                Endpoint_ClearStatusStage();

                using_report_protocol = ((USB_ControlRequest.wValue & 0xFF) != 0x00);
            }

            break;
        case HID_REQ_SetIdle:
            if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                Endpoint_ClearSETUP();
                Endpoint_ClearStatusStage();

                IdleCount = ((USB_ControlRequest.wValue & 0xFF00) >> 6);
            }

            break;
        case HID_REQ_GetIdle:
            if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
            {
                Endpoint_ClearSETUP();
                while (!(Endpoint_IsINReady()));
                Endpoint_Write_8(IdleCount >> 2);
                Endpoint_ClearIN();
                Endpoint_ClearStatusStage();
            }

            break;
    }
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
    if (!(Endpoint_IsSETUPReceived()))
        return;

    if (USB_ControlRequest.wIndex != INTERFACE_ID_PIUIO)
        return;
    
    if(piuio_which_device == 0) {
        EVENT_USB_Device_ControlRequestPIUIO();
        return;
    }
    
    if(piuio_which_device > 0 && piuio_which_device <= 2) {
        EVENT_USB_Device_ControlRequestLXIO();
        return;
    }
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
}
