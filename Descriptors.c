/***********************************************************/
/*   ____ ___ _   _ ___ ___     ____ _                     */
/*  |  _ \_ _| | | |_ _/ _ \   / ___| | ___  _ __   ___    */
/*  | |_) | || | | || | | | | | |   | |/ _ \| '_ \ / _ \   */
/*  |  __/| || |_| || | |_| | | |___| | (_) | | | |  __/   */
/*  |_|  |___|\___/|___\___/   \____|_|\___/|_| |_|\___|   */
/*                                                         */
/*  By: Lucas Teske (USB-ON-AT90USBXXX BY CKDUR)           */
/***********************************************************/
/*   Basicly this is an PIUIO Clone with an ATMEGA8U2 and  */
/*    serial from ATMEGA328 interfaced on ARDUINO UNO      */
/***********************************************************/
/*      This is the USB Configuration part                 */
/***********************************************************/
/*          Please look at LICENSE for details             */
/*  Please consult https://github.com/racerxdl/piuio_clone */
/***********************************************************/

/** \file
 *
 *  USB Device Descriptors, for library use when in USB device mode. Descriptors are special
 *  computer-readable structures which the host requests upon device enumeration, to determine
 *  the device's capabilities and functions.
 */

#include "Descriptors.h"


int piuio_which_device = 0; // Default is PIUIO
/** Device descriptor for the regular PIUIO
 */
const USB_Descriptor_Device_t PROGMEM DeviceDescriptorPIUIO =
{
    // All "Device Description" is here
    .Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

    .USBSpecification       = VERSION_BCD(1,0,0),
    .Class                  = 0xFF,
    .SubClass               = 0x00,
    .Protocol               = 0x00,

    .Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,

    .VendorID               = 0x0547,                    // PIUIO Vendor ID. Cypress 0x547
    .ProductID              = 0x1002,                    // PIUIO Product ID. FX-USB 0x1002
    .ReleaseNumber          = VERSION_BCD(1,0,0),

    .ManufacturerStrIndex   = STRING_ID_Manufacturer,
    .ProductStrIndex        = STRING_ID_Product,
    .SerialNumStrIndex      = NO_DESCRIPTOR,

    .NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

/** Device descriptor for the LXIOv1 */
const USB_Descriptor_Device_t PROGMEM DeviceDescriptorLXIO1 =
{
    // All "Device Description" is here
    .Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

    .USBSpecification       = VERSION_BCD(1,1,0),
    .Class                  = 0x00,
    .SubClass               = 0x00,
    .Protocol               = 0x00,

    .Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,

    .VendorID               = 0x0d2f,                    // LXIO v1 Vendor ID. 
    .ProductID              = 0x1020,                    // LXIO v1 Product ID. 
    .ReleaseNumber          = VERSION_BCD(0,0,1),

    .ManufacturerStrIndex   = STRING_ID_Manufacturer,
    .ProductStrIndex        = STRING_ID_Product,
    .SerialNumStrIndex      = NO_DESCRIPTOR,

    .NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

/** Device descriptor for the LXIOv2 */
const USB_Descriptor_Device_t PROGMEM DeviceDescriptorLXIO2 =
{
    // All "Device Description" is here
    .Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

    .USBSpecification       = VERSION_BCD(1,1,0),
    .Class                  = 0x00,
    .SubClass               = 0x00,
    .Protocol               = 0x00,

    .Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,

    .VendorID               = 0x0d2f,                    // LXIO v1 Vendor ID. 
    .ProductID              = 0x1040,                    // LXIO v2 Product ID. 
    .ReleaseNumber          = VERSION_BCD(0,0,1),

    .ManufacturerStrIndex   = STRING_ID_Manufacturer,
    .ProductStrIndex        = STRING_ID_Product,
    .SerialNumStrIndex      = NO_DESCRIPTOR,

    .NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

const USB_Descriptor_Device_t* DeviceDescriptors[] = {
	&DeviceDescriptorPIUIO,
	&DeviceDescriptorLXIO1,
	&DeviceDescriptorLXIO2,
};

/** Configuration descriptor structure for PIUIO*/
const USB_Descriptor_Configuration_t PROGMEM ConfigurationDescriptorPIUIO =
{
    .Config =
        {
            .Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

            .TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t),
            .TotalInterfaces        = 1,

            .ConfigurationNumber    = 1,
            .ConfigurationStrIndex  = NO_DESCRIPTOR,

            // PIUIO does have own power supply. But I dont like that lol, so mine is just USB powered.
            .ConfigAttributes       = (USB_CONFIG_ATTR_RESERVED | USB_CONFIG_ATTR_SELFPOWERED),

            // This is the value in mA, it will be divided by two (100 mean 50mA). Its just an info for PC
            .MaxPowerConsumption    = USB_CONFIG_POWER_MA(100)
        },

    .USB_Interface =
        {
            .Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

            .InterfaceNumber        = INTERFACE_ID_PIUIO,
            .AlternateSetting       = 0x00,

            // We dont need any additional entry points, so this will be 0
            .TotalEndpoints         = 0,

            // The interface didn't have nothing
            .Class                  = 0x00,
            .SubClass               = 0x00,
            .Protocol               = 0x00,

            .InterfaceStrIndex      = NO_DESCRIPTOR
        },
};

const USB_Descriptor_HIDReport_Datatype_t PROGMEM LXIOReport[] =
{
#if 1
	HID_DESCRIPTOR_VENDOR(0, 1, 2, 3, 16)
#else
	0x06, 0x00, 0xFF,  // Usage Page (Vendor Defined 0xFF00)
	0x09, 0x01,        // Usage (0x01)
	0xA1, 0x01,        // Collection (Application)
	0x09, 0x02,        //   Usage (0x02)
	0x15, 0x00,        //   Logical Minimum (0)
	0x25, 0xFF,        //   Logical Maximum (-1)
	0x75, 0x08,        //   Report Size (8)
	0x95, 0x10,        //   Report Count (16)
	0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x09, 0x03,        //   Usage (0x03)
	0x15, 0x00,        //   Logical Minimum (0)
	0x25, 0xFF,        //   Logical Maximum (-1)
	0x75, 0x08,        //   Report Size (8)
	0x95, 0x10,        //   Report Count (16)
	0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0xC0,              // End Collection
#endif
};

const USB_DescriptorLXIO_Configuration_t PROGMEM ConfigurationDescriptorLXIO =
{
    .Config =
        {
            .Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

            .TotalConfigurationSize = sizeof(USB_DescriptorLXIO_Configuration_t),
            .TotalInterfaces        = 1,

            .ConfigurationNumber    = 1,
            .ConfigurationStrIndex  = NO_DESCRIPTOR,

            // PIUIO does have own power supply. But I dont like that lol, so mine is just USB powered.
            .ConfigAttributes       = (USB_CONFIG_ATTR_RESERVED | USB_CONFIG_ATTR_SELFPOWERED),

            // This is the value in mA, it will be divided by two (100 mean 50mA). Its just an info for PC
            .MaxPowerConsumption    = 50
        },

    .USB_Interface =
        {
            .Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

            .InterfaceNumber        = INTERFACE_ID_PIUIO,
            .AlternateSetting       = 0x00,

            // Two interrupt endpoints please
            .TotalEndpoints         = 2,

            // The interface didn't have nothing
            .Class                  = 0x03,
            .SubClass               = 0x00,
            .Protocol               = 0x00,

            .InterfaceStrIndex      = NO_DESCRIPTOR
        },

    .HID_Desc =
        {
            .Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = HID_DTYPE_HID},

			.HIDSpec                = VERSION_BCD(1,1,1),
			.CountryCode            = 0x00,
			.TotalReportDescriptors = 1,
			.HIDReportType          = HID_DTYPE_Report,
			.HIDReportLength        = sizeof(LXIOReport)
        },

    .EP0_Desc =
        {
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = LXIO_EP0ADDR,
			.Attributes             = (EP_TYPE_INTERRUPT),
			.EndpointSize           = LXIO_EPSIZE,
			.PollingIntervalMS      = 1
        },

	.EP1_Desc =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = LXIO_EP1ADDR,
			.Attributes             = (EP_TYPE_INTERRUPT),
			.EndpointSize           = LXIO_EPSIZE,
			.PollingIntervalMS      = 1
		}
};

const void* ConfigurationDescriptors[] = {
	&ConfigurationDescriptorPIUIO,
	&ConfigurationDescriptorLXIO,
	&ConfigurationDescriptorLXIO,
};

const int ConfigurationDescriptorSizes[] = {
	sizeof(USB_Descriptor_Configuration_t),
	sizeof(USB_DescriptorLXIO_Configuration_t),
	sizeof(USB_DescriptorLXIO_Configuration_t),
};

/** Language descriptor structure.*/
const USB_Descriptor_String_t PROGMEM LanguageString = USB_STRING_DESCRIPTOR_ARRAY(LANGUAGE_ID_ENG);

/** Manufacturer descriptor string.*/
const USB_Descriptor_String_t PROGMEM ManufacturerStringPIUIO = USB_STRING_DESCRIPTOR(L"NTDEC");
const USB_Descriptor_String_t PROGMEM ManufacturerStringLXIO = USB_STRING_DESCRIPTOR(L"ANDAMIRO");

const USB_Descriptor_String_t* ManufacturerStrings[] = {
	&ManufacturerStringPIUIO,
	&ManufacturerStringLXIO,
	&ManufacturerStringLXIO,
};

/** Product descriptor string.*/
const USB_Descriptor_String_t PROGMEM ProductStringPIUIO = USB_STRING_DESCRIPTOR(L"PIUIO");
const USB_Descriptor_String_t PROGMEM ProductStringLXIO = USB_STRING_DESCRIPTOR(L"PIU HID V1.00");

const USB_Descriptor_String_t* ProductStrings[] = {
	&ProductStringPIUIO,
	&ProductStringLXIO,
	&ProductStringLXIO,
};

uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint16_t wIndex,
                                    const void** const DescriptorAddress)
{
    const uint8_t  DescriptorType   = (wValue >> 8);
    const uint8_t  DescriptorNumber = (wValue & 0xFF);

    const void* Address = NULL;
    uint16_t    Size    = NO_DESCRIPTOR;

    switch (DescriptorType)
    {
        case DTYPE_Device:
            Address = DeviceDescriptors[piuio_which_device];
            Size    = sizeof(USB_Descriptor_Device_t);
            break;
        case DTYPE_Configuration:
            Address = ConfigurationDescriptors[piuio_which_device];
            Size    = ConfigurationDescriptorSizes[piuio_which_device];
            break;
        case DTYPE_String:
            switch (DescriptorNumber)
            {
                case STRING_ID_Language:
                    Address = &LanguageString;
                    Size    = pgm_read_byte(&LanguageString.Header.Size);
                    break;
                case STRING_ID_Manufacturer:
                    Address = ManufacturerStrings[piuio_which_device];
                    Size    = pgm_read_byte(&ManufacturerStrings[piuio_which_device]->Header.Size);
                    break;
                case STRING_ID_Product:
                    Address = ProductStrings[piuio_which_device];
                    Size    = pgm_read_byte(&ProductStrings[piuio_which_device]->Header.Size);
                    break;
            }

            break;
		// LXIO only dumps these descriptor
		// If by accident come from a PIUIO, well, fat chance
		case HID_DTYPE_HID:
			Address = &ConfigurationDescriptorLXIO.HID_Desc;
			Size    = sizeof(USB_HID_Descriptor_HID_t);
			break;
		case HID_DTYPE_Report:
			Address = &LXIOReport;
			Size    = sizeof(LXIOReport);
			break;
    }

    *DescriptorAddress = Address;
    return Size;
}

