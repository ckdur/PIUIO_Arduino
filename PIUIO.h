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
/*      This is main code from PIUIO Clone (HEADER)        */
/***********************************************************/
/*                    License is WHAT?                     */
/*  Please consult https://github.com/racerxdl/piuio_clone */
/***********************************************************/

/** \file
 *
 *  Header file for PIUIO.c.
 */

#ifndef _PIUIO_H_
#define _PIUIO_H_

	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/power.h>
		#include <avr/interrupt.h>
		#include <string.h>

		#include "Descriptors.h"

		//#include <LUFA/Drivers/Board/Joystick.h>
		//#include <LUFA/Drivers/Board/LEDs.h>
		//#include <LUFA/Drivers/Board/Buttons.h>
		#include <LUFA/Drivers/USB/USB.h>
		#include <LUFA/Platform/Platform.h>
		#include <LUFA/Drivers/Peripheral/Serial.h>

	/* Function Prototypes: */
		void SetupHardware(void);

		void EVENT_USB_Device_Connect(void);
		void EVENT_USB_Device_Disconnect(void);
		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_ControlRequest(void);
		void EVENT_USB_Device_StartOfFrame(void);

#endif

