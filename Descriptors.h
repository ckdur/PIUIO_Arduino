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
/*          Please look at LICENSE for details             */
/*  Please consult https://github.com/racerxdl/piuio_clone */
/***********************************************************/

/** \file
 *
 *  Header file for Descriptors.c.
 */

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

	//#define USE_STATIC_OPTIONS USB_DEVICE_OPT_LOWSPEED
		#include <avr/pgmspace.h>

		#include <LUFA/Drivers/USB/USB.h>

	/* Type Defines: */
		typedef struct
		{
			USB_Descriptor_Configuration_Header_t Config;
			USB_Descriptor_Interface_t            USB_Interface;
		} USB_Descriptor_Configuration_t;

		typedef struct
		{
			USB_Descriptor_Configuration_Header_t Config;
			USB_Descriptor_Interface_t            USB_Interface;
			USB_HID_Descriptor_HID_t              HID_Desc;
			USB_Descriptor_Endpoint_t			  EP0_Desc;
			USB_Descriptor_Endpoint_t			  EP1_Desc;
		} USB_DescriptorLXIO_Configuration_t;

		/** Enum for the device interface descriptor IDs within the device. */
		enum InterfaceDescriptors_t
		{
			INTERFACE_ID_PIUIO = 0, /**< PIUIO interface desciptor ID */
		};

		/** Enum for the device string descriptor IDs within the device.*/
		enum StringDescriptors_t
		{
			STRING_ID_Language     = 0, /**< Supported Languages string descriptor ID (must be zero) */
			STRING_ID_Manufacturer = 1, /**< Manufacturer string ID */
			STRING_ID_Product      = 2, /**< Product string ID */
		};

		#define LXIO_EP0ADDR              (ENDPOINT_DIR_IN | 1)
		#define LXIO_EP1ADDR              (ENDPOINT_DIR_OUT | 2)
		#define LXIO_EPSIZE               16

	/* Function Prototypes: */
		uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
		                                    const uint16_t wIndex,
		                                    const void** const DescriptorAddress)
		                                    ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);
	extern int piuio_which_device;

#endif

