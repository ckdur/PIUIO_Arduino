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
/*      This is the USB Configuration part (HEADER)        */
/***********************************************************/
/*                    License is WHAT?                     */
/*  Please consult https://github.com/racerxdl/piuio_clone */
/***********************************************************/

/** \file
 *
 *  Header file for Descriptors.c.
 */

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_a

	#define USE_STATIC_OPTIONS USB_DEVICE_OPT_LOWSPEED
		#include <avr/pgmspace.h>

		#include <LUFA/Drivers/USB/USB.h>

	/* Type Defines: */
		/** Type define for the device configuration descriptor structure. This must be defined in the
		 *  application code, as the configuration descriptor contains several sub-descriptors which
		 *  vary between devices, and which describe the device's usage to the host.
		 */
		typedef struct
		{
			USB_Descriptor_Configuration_Header_t Config;

			// Joystick HID Interface
			USB_Descriptor_Interface_t            HID_Interface;

			// CKDUR: We don't need HID! God! D:
			// RACERXL: We dont need any additional entry points, so this will be 0
			// CKDUR: But we need still one
		} USB_Descriptor_Configuration_t;

		/** Enum for the device interface descriptor IDs within the device. Each interface descriptor
		 *  should have a unique ID index associated with it, which can be used to refer to the
		 *  interface from other descriptors.
		 */
		enum InterfaceDescriptors_t
		{
			INTERFACE_ID_PIUIO = 0, /**< PIUIO interface desciptor ID */
		};

		/** Enum for the device string descriptor IDs within the device. Each string descriptor should
		 *  have a unique ID index associated with it, which can be used to refer to the string from
		 *  other descriptors.
		 */
		enum StringDescriptors_t
		{
			STRING_ID_Language     = 0, /**< Supported Languages string descriptor ID (must be zero) */
			STRING_ID_Manufacturer = 1, /**< Manufacturer string ID */
			STRING_ID_Product      = 2, /**< Product string ID */
		};

	/* Function Prototypes: */
		uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
		                                    const uint16_t wIndex,
		                                    const void** const DescriptorAddress)
		                                    ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif

