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

/*------- Private include files ----------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include "scheduler.h"
#include "sch_config.h"

/*------- Private type declarations ------------------------------*/
typedef struct _taskCB
{
    void (* pTask)(void); /* Pointer to the task (must be a 'void (void)' function). */
    uint16_t delay;       /* Delay (ticks) until the function will (next) be run:
                             - see SCH_Add_Task() for further details. */
    uint16_t period;      /* Interval (ticks) between subsequent runs:
                             - see SCH_Add_Task() for further details. */
    uint16_t runMe;       /* Incremented (by scheduler) when task is due to execute. */
} taskCB_t;

/*------ Public variable definitions -----------------------------*/
static taskCB_t taskCBs[SCH_MAX_TASKS]; /* The array of Task Control Blocks. */
static uint8_t errorCode;               /* Used to display the error code. */

/*------ Start implementation ------------------------------------*/
void SCH_Init(void)
{
    uint8_t i;

    for (i = 0; i < SCH_MAX_TASKS; i++)
    {
        SCH_DeleteTask(i);
    }

    /* Reset the global error variable */
    errorCode = 0;
}

void SCH_Update(void)
{
    uint8_t index;

    /* NOTE: calculations are in *TICKS* (not milliseconds). */
    for (index = 0; index < SCH_MAX_TASKS; index++)
    {
        /* Check if there is a task at this location. */
        if (taskCBs[index].pTask)
        {
            if (taskCBs[index].delay == 0)
            {
                /* The task is due to run. */
                taskCBs[index].runMe += 1; /* Inc. the 'RunMe' flag. */

                if (taskCBs[index].period)
                {
                    /* Schedule regular tasks to run again. */
                    taskCBs[index].delay = taskCBs[index].period;
                }
            }
            else
            {
                    /* Not yet ready to run: just decrement the delay. */
                    taskCBs[index].delay -= 1;
            }
        }
    }
}

uint8_t SCH_AddTask(void (* pFunction)(void),
        const uint16_t delay, const uint16_t peroid)
{
    uint8_t index = 0;

    /* First find a gap in the array (if there is one). */
    while ((taskCBs[index].pTask != 0) && (index < SCH_MAX_TASKS))
    {
        index++;
    }

    /* If we haven't reached the end of the list. */
    if (SCH_MAX_TASKS > index)
    {
        /* If we're here, there is a space in the task array. */
        taskCBs[index].pTask  = pFunction;
        taskCBs[index].delay  = delay;
        taskCBs[index].period = peroid - 1;
        taskCBs[index].runMe  = 0;
    }

    return index; /* return position of task (to allow later deletion). */
}

bool SCH_DeleteTask(const uint8_t taskIndex)
{
    bool result;

    if (0x0 == taskCBs[taskIndex].pTask)
    {
        result = false;
    }
    else
    {
        taskCBs[taskIndex].pTask   = 0x0;
        taskCBs[taskIndex].delay   = 0x0;
        taskCBs[taskIndex].period  = 0x0;
        taskCBs[taskIndex].runMe   = 0x0;

        result = true;
    }

    return result;
}

void SCH_DispatchTasks(void)
{
    uint8_t index;

    /* Dispatches (runs) the next task (if one is ready). */
    for (index = 0; index < SCH_MAX_TASKS; index++)
    {
        if (taskCBs[index].runMe > 0)
        {
            (*taskCBs[index].pTask)(); /* Run the task. */

            taskCBs[index].runMe -= 1; /* Reset / reduce RunMe flag. */

            /* Periodic tasks will automatically run again
               - if this is a 'one shot' task, remove it from the array. */
            if (taskCBs[index].period == 0)
            {
                SCH_DeleteTask(index);
            }
        }
    }

	/* Report system status */
	// SCH_ReportStatus();

	/* The scheduler enters idle mode at this point. */
	SCH_GoToSleep();
}

// void SCH_ReportStatus(void)
// {
// }

/*******************************************************************************
 * EOF
 ******************************************************************************/
