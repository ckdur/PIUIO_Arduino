#include "PIUIO.h"
#include "PIUIO_ctrl.h"
#include "Descriptors.h"

// Useful commands to test the descriptors
// sudo lsusb -d 0d2f:1020 -vv
// sudo usbhid-dump -d 0d2f:1020 -e descriptor

unsigned char LampData[8];
unsigned char InputData[8];

unsigned char LXLampData[16];
unsigned char LXInputData[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff};

int next_device = 0;

unsigned char EEPROM_read(unsigned int uiAddress);
//unsigned char EEPROM_write(unsigned int uiAddress, unsigned char ucData);

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

    // Get the status of the EEPROM
    unsigned char dat = EEPROM_read(0);
    piuio_which_device = (int)dat;
    if(piuio_which_device < 0 || piuio_which_device > 2) piuio_which_device = 0;
    next_device = piuio_which_device;

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

const USB_Endpoint_Table_t ReportINEndpoint = {
    .Address = LXIO_EP0ADDR,
    .Size    = LXIO_EPSIZE,
    .Type    = EP_TYPE_INTERRUPT,
    .Banks   = 1,
};

const USB_Endpoint_Table_t ReportOUTEndpoint = {
    .Address = LXIO_EP1ADDR,
    .Size    = LXIO_EPSIZE,
    .Type    = EP_TYPE_INTERRUPT,
    .Banks   = 1,
};

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
    //bool ConfigSuccess = true;

    if(piuio_which_device != 0) {
        Endpoint_ConfigureEndpointTable(&ReportINEndpoint, 1);
        Endpoint_ConfigureEndpointTable(&ReportOUTEndpoint, 1);
    }

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

void USB_Loop(void) {
    // Do not handle endpoints if not an LXIO
    if(piuio_which_device == 0) return;

    // Do the conversion from the Lights and Input from regular IO
    // to the LX Lights and Input format
    // NOTE: Note that InputData will carry the 4 sensor anyways
    LXInputData[0] = InputData[0];
    LXInputData[1] = InputData[0];
    LXInputData[2] = InputData[0];
    LXInputData[3] = InputData[0];
    LXInputData[4] = InputData[2];
    LXInputData[5] = InputData[2];
    LXInputData[6] = InputData[2];
    LXInputData[7] = InputData[2];
    LXInputData[8] = InputData[1];
    LXInputData[9] = InputData[3];
    LXInputData[10] = 0xFF; // TODO: We do not have a handler for the front buttons
    LXInputData[11] = 0xFF; // TODO: We do not have a handler for the front buttons
    LXInputData[14] = 0xFF;
    LXInputData[15] = 0xFF;

    // Handle in endpoint
    Endpoint_SelectEndpoint(LXIO_EP0ADDR); // An input

    if (Endpoint_IsINReady())
	{
        nControl++;
        Endpoint_Write_Stream_LE(LXInputData, sizeof(LXInputData), NULL);
        Endpoint_ClearIN();
    }

    // Handle out endpoint
    Endpoint_SelectEndpoint(LXIO_EP1ADDR); // An output
	if (Endpoint_IsOUTReceived())
	{
        if (Endpoint_IsReadWriteAllowed())
		{
            nControl++;
            Endpoint_Read_Stream_LE(&LXLampData, sizeof(LXLampData), NULL);
        }
		Endpoint_ClearOUT();
	}

    memcpy(LampData, LXLampData, 8); // Just copy in the case of lamps
}

// Extracted from the datasheet
unsigned char EEPROM_read(unsigned int uiAddress)
{
    /* Wait for completion of previous write */
    while(EECR & (1<<EEPE));
    /* Set up address register */
    EEAR = uiAddress;
    /* Start eeprom read by writing EERE */
    EECR |= (1<<EERE);
    /* Return data from Data Register */
    return EEDR;
}

void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
    /* Wait for completion of previous write */
    while(EECR & (1<<EEPE));
    /* Set up address and Data Registers */
    EEAR = uiAddress;
    EEDR = ucData;
    /* Write logical one to EEMPE */
    EECR |= (1<<EEMPE);
    /* Start eeprom write by setting EEPE */
    EECR |= (1<<EEPE);
}

void go_next_device() {
    next_device++;
    if(next_device >= 3) next_device = 0;
    EEPROM_write(0, (unsigned char)next_device);
}