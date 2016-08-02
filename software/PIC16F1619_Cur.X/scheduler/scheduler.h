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

#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

/*------- Public include files -----------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/*------- Public function prototypes -----------------------------*/

/*------------------------------------------------------------------

  SCH_Init()

  Scheduler initialisation function.  Prepares scheduler
  data structures and sets up timer interrupts at required rate.

  You must call this function before using the scheduler.  

 -----------------------------------------------------------------*/
void SCH_Init(void);

/*------------------------------------------------------------------

  SCH_Start()

  Starts the scheduler, by enabling interrupts.

  NOTE: Usually called after all regular tasks are added,
  to keep the tasks synchronised.

  NOTE: ONLY THE SCHEDULER INTERRUPT SHOULD BE ENABLED!!! 

 -----------------------------------------------------------------*/
void SCH_Start(void);

/*------------------------------------------------------------------

  SCH_Update()

  This is the scheduler ISR.  It is called at a rate 
  determined by the timer settings in the 'init' function.

  This version is triggered by Timer 2 interrupts:
  timer is automatically reloaded.

 -----------------------------------------------------------------*/
void SCH_Update(void);

/*------------------------------------------------------------------

  SCH_Add_Task()

  Causes a task (function) to be executed at regular intervals 
  or after a user-defined delay

  Fn_P   - The name of the function which is to be scheduled.
           NOTE: All scheduled functions must be 'void, void' -
           that is, they must take no parameters, and have 
           a void return type. 
                   
  DELAY  - The interval (TICKS) before the task is first executed

  PERIOD - If 'PERIOD' is 0, the function is only called once,
           at the time determined by 'DELAY'.  If PERIOD is non-zero,
           then the function is called repeatedly at an interval
           determined by the value of PERIOD (see below for examples
           which should help clarify this).


  RETURN VALUE:  

  Returns the position in the task array at which the task has been 
  added.  If the return value is SCH_MAX_TASKS then the task could 
  not be added to the array (there was insufficient space).  If the
  return value is < SCH_MAX_TASKS, then the task was added 
  successfully.  

  Note: this return value may be required, if a task is
  to be subsequently deleted - see SCH_Delete_Task().

  EXAMPLES:

  Task_ID = SCH_Add_Task(Do_X,1000,0);
  Causes the function Do_X() to be executed once after 1000 sch ticks.            

  Task_ID = SCH_Add_Task(Do_X,0,1000);
  Causes the function Do_X() to be executed regularly, every 1000 sch ticks.            

  Task_ID = SCH_Add_Task(Do_X,300,1000);
  Causes the function Do_X() to be executed regularly, every 1000 ticks.
  Task will be first executed at T = 300 ticks, then 1300, 2300, etc.            
 
 -----------------------------------------------------------------*/
uint8_t SCH_AddTask(void (* pFunction)(void),
        const uint16_t delay, const uint16_t period);

/*------------------------------------------------------------------

  SCH_Delete_Task()

  Removes a task from the scheduler.  Note that this does
  *not* delete the associated function from memory: 
  it simply means that it is no longer called by the scheduler. 
 
  TASK_INDEX - The task index.  Provided by SCH_Add_Task(). 

  RETURN VALUE:  RETURN_ERROR or RETURN_NORMAL

 -----------------------------------------------------------------*/
bool SCH_DeleteTask(const uint8_t taskIndex);

/*------------------------------------------------------------------

  SCH_Dispatch_Tasks()

  This is the 'dispatcher' function.  When a task (function)
  is due to run, SCH_Dispatch_Tasks() will run it.
  This function must be called (repeatedly) from the main loop.

 -----------------------------------------------------------------*/
void SCH_DispatchTasks(void);

/*------------------------------------------------------------------

  SCH_GoToSleep()

  This scheduler enters 'idle mode' between clock ticks
  to save power.  The next clock tick will return the processor
  to the normal operating state.

  Note: a slight performance improvement is possible if this
  function is implemented as a macro, or if the code here is simply
  pasted into the 'dispatch' function.

  However, by making this a function call, it becomes easier
  - during development - to assess the performance of the
  scheduler, using the 'performance analyser' in the Keil
  hardware simulator. See Chapter 14 for examples for this.

  *** May wish to disable this if using a watchdog ***

  *** ADAPT AS REQUIRED FOR YOUR HARDWARE ***

 -----------------------------------------------------------------*/
void SCH_GoToSleep(void);

/*------------------------------------------------------------------

  SCH_Report_Status()

  Simple function to display error codes.

  This version displays code on a port with attached LEDs:
  adapt, if required, to report errors over serial link, etc.

  Errors are only displayed for a limited period 
  (60000 ticks = 1 minute at 1ms tick interval).
  After this the the error code is reset to 0. 

  This code may be easily adapted to display the last
  error 'for ever': this may be appropriate in your
  application.

 ------------------------------------------------------------------*/
void SCH_ReportStatus(void);

#endif /* __SCHEDULER_H__ */
