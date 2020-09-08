//*****************************************************************************
//
// Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/ 
// 
// 
//  Redistribution and use in source and binary forms, with or without 
//  modification, are permitted provided that the following conditions 
//  are met:
//
//    Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the 
//    documentation and/or other materials provided with the   
//    distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************

//*****************************************************************************
//
// Application Name     - Blinky
// Application Overview - The objective of this application is to showcase the 
//                        GPIO control using Driverlib api calls. The LEDs 
//                        connected to the GPIOs on the LP are used to indicate 
//                        the GPIO output. The GPIOs are driven high-low 
//                        periodically in order to turn on-off the LEDs.
//
//*****************************************************************************

//****************************************************************************
//
//! \addtogroup blinky
//! @{
//
//****************************************************************************

// Standard includes
#include <stdio.h>

// Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "interrupt.h"
#include "hw_apps_rcm.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "gpio.h"
#include "utils.h"

// Common interface includes
#include "gpio_if.h"

#include "pin_mux_config.h"

#include "uart_if.h"

#define APPLICATION_VERSION     "1.4.0"

//*****************************************************************************
//                 GLOBAL VARIABLES -- Start
//*****************************************************************************
#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif
#define DELAY 4000000

static enum state cur_state = none;
//*****************************************************************************
//                 GLOBAL VARIABLES -- End
//*****************************************************************************


//*****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES                           
//*****************************************************************************
static void BoardInit(void);

//*****************************************************************************
//                      LOCAL FUNCTION DEFINITIONS                         
//*****************************************************************************

//*****************************************************************************
//
//! Application startup display on UART
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
static void
DisplayBanner()
{

    Report("\n\n\n\r");
    Report("\t\t *************************************************\n\r");
    Report("\t\t        CC3200 GPIO Application       \n\r");
    Report("\t\t *************************************************\n\r");
    Report("\n\n\n\r");
    Report("\t\t *************************************************\n\r");
    Report("\t\t        Push SW3 to start LED binary counting       \n\r");
    Report("\t\t        Push SW2 to blink LEDs on and off       \n\r");
    Report("\t\t *************************************************\n\r");
}

//*****************************************************************************
//
//! Board Initialization & Configuration
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************
static void
BoardInit(void)
{
/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
    //
    // Set vector table base
    //
#if defined(ccs)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif
    
    //
    // Enable Processor
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}

/* State switching function
 * Switch state to specified state
 * Write GPIO to p18 to high/low
 * Return
 */
static void StateSwitch(enum state into)
{
    // Fail safe. Prompt message if true
    if (cur_state == into) {
        Report("Error at StateSwitch(): Trying to change to same state but was not stopped\r");
        return;
    }

    cur_state = into;
    switch(into) {
        case binary:
            GPIOPinWrite(GPIOA3_BASE, 0x10, 0);
            Message("SW3 pressed\r");
            return;
        case unison:
            GPIOPinWrite(GPIOA3_BASE, 0x10, 0x10);
            Message("SW2 pressed\r");
            return;
        case none:
            // Shouldn't happen
            Report("Error at StateSwitch(): none is being taken in and checked\r");
            return;
        default:
            // Shouldn't happen
            Report("Error at StateSwitch(): nothing is being taken in and checked\r");
            return;
    }
}

/* Unison Blink function
 *      Add delay, then poll if SW3 is high
 *          If high, switch state to binary and return
 *          otherwise, demonstrate next blink stage
 *      Do the same for each stages
 */
static void UnisonBlink(void)
{
    MAP_UtilsDelay(DELAY);
    if (GPIOPinRead(GPIOA1_BASE, 0x20) == 0x20) {
        StateSwitch(binary);
        return;
    }
    GPIO_IF_LedOff(MCU_ALL_LED_IND);

    MAP_UtilsDelay(DELAY);
    if (GPIOPinRead(GPIOA1_BASE, 0x20) == 0x20) {
        StateSwitch(binary);
        return;
    }
    GPIO_IF_LedOn(MCU_ALL_LED_IND);
}

/* Binary Blink function
 *      Add delay, then poll if SW2 is high
 *          If high, switch state to unison, Write GPIO to p18 to high then return
 *          otherwise, demonstrate next blink stage
 *      Do the same for each stages
 */
static void BinaryBlink(void)
{
    // 000
    MAP_UtilsDelay(DELAY);
    if (GPIOPinRead(GPIOA2_BASE, 0x40) == 0x40) {
        StateSwitch(unison);
        return;
    }
    GPIO_IF_LedOff(MCU_ALL_LED_IND);

    // 001
    MAP_UtilsDelay(DELAY);
    if (GPIOPinRead(GPIOA2_BASE, 0x40) == 0x40) {
        StateSwitch(unison);
        return;
    }
    GPIO_IF_LedOn(MCU_RED_LED_GPIO);
    GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);
    GPIO_IF_LedOff(MCU_GREEN_LED_GPIO);

    // 010
    MAP_UtilsDelay(DELAY);
    if (GPIOPinRead(GPIOA2_BASE, 0x40) == 0x40) {
        StateSwitch(unison);
        return;
    }
    GPIO_IF_LedOff(MCU_RED_LED_GPIO);
    GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);
    GPIO_IF_LedOff(MCU_GREEN_LED_GPIO);

    // 011
    MAP_UtilsDelay(DELAY);
    if (GPIOPinRead(GPIOA2_BASE, 0x40) == 0x40) {
        StateSwitch(unison);
        return;
    }
    GPIO_IF_LedOn(MCU_RED_LED_GPIO);
    GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);
    GPIO_IF_LedOff(MCU_GREEN_LED_GPIO);

    // 100
    MAP_UtilsDelay(DELAY);
    if (GPIOPinRead(GPIOA2_BASE, 0x40) == 0x40) {
        StateSwitch(unison);
        return;
    }
    GPIO_IF_LedOff(MCU_RED_LED_GPIO);
    GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);
    GPIO_IF_LedOn(MCU_GREEN_LED_GPIO);

    // 101
    MAP_UtilsDelay(DELAY);
    if (GPIOPinRead(GPIOA2_BASE, 0x40) == 0x40) {
        StateSwitch(unison);
        return;
    }
    GPIO_IF_LedOn(MCU_RED_LED_GPIO);
    GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);
    GPIO_IF_LedOn(MCU_GREEN_LED_GPIO);

    // 110
    MAP_UtilsDelay(DELAY);
    if (GPIOPinRead(GPIOA2_BASE, 0x40) == 0x40) {
        StateSwitch(unison);
        return;
    }
    GPIO_IF_LedOff(MCU_RED_LED_GPIO);
    GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);
    GPIO_IF_LedOn(MCU_GREEN_LED_GPIO);

    // 111
    MAP_UtilsDelay(DELAY);
    if (GPIOPinRead(GPIOA2_BASE, 0x40) == 0x40) {
        StateSwitch(unison);
        return;
    }
    GPIO_IF_LedOn(MCU_RED_LED_GPIO);
    GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);
    GPIO_IF_LedOn(MCU_GREEN_LED_GPIO);
}

//****************************************************************************
//
//! Main function
//!
//! \param none
//! 
//! This function  
//!      1. At Initial state (none), all lights are off
//!
//!      2. In while loop:
//!         If state is none, keep polling SW2 or SW3
//!             If SW3 is received:
//!                 Switch state to binary
//!                 Write GPIO to p18 to low
//!                 Message SW3 was pressed
//!                 Execute BinaryBlink();
//!             If SW2 is received:
//!                 Switch state to unison
//!                 Write GPIO to p18 to high
//!                 Message SW2 was pressed
//!                 Execute UnisonBlink();
//!
//!      If state is binary, execute BinaryBlink();
//!
//!      If state is unison, execute UnisonBlink();
//!
//! \return None.
//
//****************************************************************************
int
main()
{
    //
    // Initialize Board configurations
    //
    BoardInit();
    
    //
    // Power on the corresponding GPIO port B for 9,10,11.
    // Set up the GPIO lines to mode 0 (GPIO)
    //
    PinMuxConfig();
    InitTerm();
    ClearTerm();

    DisplayBanner();

    GPIO_IF_LedConfigure(LED1|LED2|LED3);

    GPIO_IF_LedOff(MCU_ALL_LED_IND);
    
   while (1) {
        if (cur_state == none) {
            MAP_UtilsDelay(DELAY);

            if ((GPIOPinRead(GPIOA1_BASE, 0x20) == 0x20) == (GPIOPinRead(GPIOA2_BASE, 0x40) == 0x40)) {
                continue; // If both buttons are pressed or none are pressed, ignore input
            } else if (GPIOPinRead(GPIOA1_BASE, 0x20) == 0x20) {
                StateSwitch(binary);
            } else if (GPIOPinRead(GPIOA2_BASE, 0x40) == 0x40) {
                StateSwitch(unison);
            } else {
                // This shouldn't happen. Prompt message
                Report("Error at main(): none state have confusing input\r");
                continue;
            }
        } else if (cur_state == binary) {
            BinaryBlink();
        } else if (cur_state == unison) {
            UnisonBlink();
        } else {
            // This shouldn't happen. Prompt message
            Report("Error at main(): Confusing state\r");
            continue; // Shouldn't happen, Prompt message
        }
    }

    return 0;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
