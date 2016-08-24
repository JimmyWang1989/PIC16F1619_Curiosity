/*
 * Copyright (C) 2016 Wang Ge
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "mcc_generated_files/mcc.h"
#include "scheduler.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint16_t duty = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/

void LED_Blink(void)
{
    IO_RA5_Toggle();
}

void LED_Breath(void)
{
    duty = (duty + 128) % 1024;

    PWM3_LoadDutyValue(duty);
}

/** Main application */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    IO_RA1_SetLow();
    IO_RA2_SetLow();
    IO_RA5_SetLow();

    SCH_Init();
    SCH_AddTask(LED_Blink, 0, 125);
    SCH_AddTask(LED_Breath, 1, 125);
    SCH_Start();

    while(1)
    {
        SCH_DispatchTasks();
    }
}

void SCH_Start(void)
{
    TMR0_Initialize();
    TMR0_SetInterruptHandler(SCH_Update);

    INTERRUPT_PeripheralInterruptEnable();
    INTERRUPT_GlobalInterruptEnable();
}

void SCH_GoToSleep(void)
{
    // Sleep();
}

/** End of File */
