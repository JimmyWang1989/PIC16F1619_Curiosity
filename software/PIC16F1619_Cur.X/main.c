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
#include "qpn.h"
#include "blinky.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Local-scope objects -----------------------------------------------------*/
static QEvt l_blinkyQSto[10]; /* Event queue storage for Blinky */

/* QF_active[] array defines all active object control blocks --------------*/
QMActiveCB const Q_ROM QF_active[] = {
    { (QMActive *)0,           (QEvt *)0,        0U                      },
    { (QMActive *)&AO_Blinky,  l_blinkyQSto,     Q_DIM(l_blinkyQSto)     }
};

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Make sure that the QF_active[] array matches QF_MAX_ACTIVE in qpn_conf.h */
Q_ASSERT_COMPILE(QF_MAX_ACTIVE == Q_DIM(QF_active) - 1);

/* QP Framwork Callbacks ******************************************************/
void QF_onStartup(void)
{
    TMR0_Initialize();
}

void QV_onIdle(void)
{
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
}

void Q_onAssert(char_t const Q_ROM * const file, int_t line)
{
    while(1)
    {
        ;
    }
}

/** Main application */
void main(void)
{
    Blinky_ctor();

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

    INTERRUPT_PeripheralInterruptEnable();
    INTERRUPT_GlobalInterruptEnable();
    
    // IO_RA1_SetLow();
    // IO_RA2_SetLow();
    // IO_RA5_SetLow();
    // IO_RC5_SetLow();

    /* Transfer control to QF-nano */
    QF_run();
}
/** End of File */