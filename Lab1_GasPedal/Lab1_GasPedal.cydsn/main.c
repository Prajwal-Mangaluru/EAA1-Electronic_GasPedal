/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "global.h"

#include "tsk_io.h"
#include "tsk_control.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "joystick.h"
#include "watchdog_timer.h"
//ISR which will increment the systick counter every ms
ISR(systick_handler)
{
    CounterTick(cnt_systick);
}

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
   
    //Set systick period to 1 ms. Enable the INT and start it.
	EE_systick_set_period(MILLISECONDS_TO_TICKS(1, BCLK__BUS_CLK__HZ));
	EE_systick_enable_int();
   
    // Start Operating System
    for(;;)	    
    	StartOS(OSDEFAULTAPPMODE);
}

void unhandledException()
{
    //Ooops, something terrible happened....check the call stack to see how we got here...
    __asm("bkpt");
}


/********************************************************************************
 * Task Definitions
 ********************************************************************************/

TASK(tsk_init)
{
    
    //Init MCAL Drivers
    JOYSTICK_Init();
    Engine_PWM_Start();
    UART_LOG_Init();
    UART_LOG_Start();
    
    WD_Start(ticks_2_3_s);   
    if(WD_CheckResetBit())
    {
        UART_LOG_PutString("Reset by WatchDog Timer\r\n");
    }
    else
    {
        UART_LOG_PutString("Reset by PowerOn Reset\r\n");
    }
    
    //Reconfigure ISRs with OS parameters.
    //This line MUST be called after the hardware driver initialisation!
    EE_system_init();
    //Start SysTick
	//Must be done here, because otherwise the isr vector is not overwritten yet
    EE_systick_start();  
	
    //Start the alarm with 100ms cycle time
    SetRelAlarm(alrm_1ms,1,1);
    
    ActivateTask(tsk_io);
    ActivateTask(tsk_control);
    ActivateTask(tsk_background);
    
    TerminateTask();
    
}

TASK(tsk_background)
{
    while(1)
    {
        //do something with low prioroty
        WD_Trigger();    
    }
}

/********************************************************************************
 * ISR Definitions
 ********************************************************************************/
ISR(isr_WD_Button)
{
    if(WD_Button_Read() == 1)
    {
        ShutdownOS(0);   
    }
}
/* [] END OF FILE */
