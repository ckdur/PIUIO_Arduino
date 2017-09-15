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
/*                    License is WHAT?                     */
/*  Please consult https://github.com/racerxdl/piuio_clone */
/***********************************************************/

/** \file
 *
 *  USB Device Descriptors, for library use when in USB device mode. Descriptors are special
 *  computer-readable structures which the host requests upon device enumeration, to determine
 *  the device's capabilities and functions.
 */

#include "Descriptors.h"

/** Device descriptor structure. This descriptor, located in FLASH memory, describes the overall
 *  device characteristics, including the supported USB version, control endpoint size and the
 *  number of device configurations. The descriptor is read out by the USB host when the enumeration
 *  process begins.
 */
const USB_Descriptor_Device_t PROGMEM DeviceDescriptor =
{
	// CKDUR: All "Device Description" is here
	.Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

	.USBSpecification       = VERSION_BCD(1,0,0),
	.Class                  = 0xFF,						// RACERXL: 0xFF is a vendor-specifc class. Thats what we want
	.SubClass               = 0x00,
	.Protocol               = 0x00,

	.Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,		// CKDUR: Was FIXED_CONTROL_ENDPOINT_SIZE = 8

	.VendorID               = 0x0547,					// PIUIO Vendor ID. Cypress 0x547
	.ProductID              = 0x1002,					// PIUIO Product ID. FX-USB 0x1002
	.ReleaseNumber          = VERSION_BCD(1,0,0),		// 0.0.1 :(

	.ManufacturerStrIndex   = STRING_ID_Manufacturer,
	.ProductStrIndex        = STRING_ID_Product,
	.SerialNumStrIndex      = NO_DESCRIPTOR,

	.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

/** Configuration descriptor structure. This descriptor, located in FLASH memory, describes the usage
 *  of the device in one of its supported configurations, including information about any device interfaces
 *  and endpoints. The descriptor is read out by the USB host during the enumeration process when selecting
 *  a configuration so that the host may correctly communicate with the USB device.
 */
 // RACERXL:
 /* --------------------------- Functional Range ---------------------------- */
const USB_Descriptor_Configuration_t PROGMEM ConfigurationDescriptor =
{
	.Config =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

			.TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t),
			.TotalInterfaces        = 1,

			.ConfigurationNumber    = 1,
			.ConfigurationStrIndex  = NO_DESCRIPTOR,

			// RACERXL: PIUIO does have own power supply. But I dont like that lol, so mine is just USB powered.
			.ConfigAttributes       = (USB_CONFIG_ATTR_RESERVED | USB_CONFIG_ATTR_SELFPOWERED),

			// RACERXL:  This is the value in mA, it will be divided by two (100 mean 50mA). Its just an info for PC
			.MaxPowerConsumption    = USB_CONFIG_POWER_MA(100)
		},

	.HID_Interface =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber        = INTERFACE_ID_PIUIO,
			.AlternateSetting       = 0x00,

			// RACERXL: We dont need any additional entry points, so this will be 0 CKDUR:1?
			.TotalEndpoints         = 0,

			// CKDUR: The interface didn't have nothing
			.Class                  = 0x00,
			.SubClass               = 0x00,
			.Protocol               = 0x00,

			.InterfaceStrIndex      = NO_DESCRIPTOR
		},

		// CKDUR: We don't need HID! God! D:
};

/** Language descriptor structure. This descriptor, located in FLASH memory, is returned when the host requests
 *  the string descriptor with index 0 (the first index). It is actually an array of 16-bit integers, which indicate
 *  via the language ID table available at USB.org what languages the device supports for its string descriptors.
 */
const USB_Descriptor_String_t PROGMEM LanguageString = USB_STRING_DESCRIPTOR_ARRAY(LANGUAGE_ID_ENG);

/** Manufacturer descriptor string. This is a Unicode string containing the manufacturer's details in human readable
 *  form, and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
const USB_Descriptor_String_t PROGMEM ManufacturerString = USB_STRING_DESCRIPTOR(L"CKDUR");	// CKDUR: By me

/** Product descriptor string. This is a Unicode string containing the product's details in human readable form,
 *  and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
const USB_Descriptor_String_t PROGMEM ProductString = USB_STRING_DESCRIPTOR(L"PIUIO");	// CKDUR: Remember maybe we can name it FX-USB

/** This function is called by the library when in device mode, and must be overridden (see library "USB Descriptors"
 *  documentation) by the application code so that the address and size of a requested descriptor can be given
 *  to the USB library. When the device receives a Get Descriptor request on the control endpoint, this function
 *  is called so that the descriptor details can be passed back and the appropriate descriptor sent back to the
 *  USB host.
 */
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
			Address = &DeviceDescriptor;
			Size    = sizeof(USB_Descriptor_Device_t);
			break;
		case DTYPE_Configuration:
			Address = &ConfigurationDescriptor;
			Size    = sizeof(USB_Descriptor_Configuration_t);
			break;
		case DTYPE_String:
			switch (DescriptorNumber)
			{
				case STRING_ID_Language:
					Address = &LanguageString;
					Size    = pgm_read_byte(&LanguageString.Header.Size);
					break;
				case STRING_ID_Manufacturer:
					Address = &ManufacturerString;
					Size    = pgm_read_byte(&ManufacturerString.Header.Size);
					break;
				case STRING_ID_Product:
					Address = &ProductString;
					Size    = pgm_read_byte(&ProductString.Header.Size);
					break;
			}

			break;
	}

	*DescriptorAddress = Address;
	return Size;
}

