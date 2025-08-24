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

/* [] END OF FILE */

# include "watchdog_timer.h"
# include "stdio.h"
static volatile uint32_t wd_aliveBits = 0;
#define WD_EXPECTED_ALIVE_MASK 0xF  // e.g., 0b00001111 => 4 runnables
/**
* Activate the Watchdog Trigger
* \param WDT_TimeOut_t timeout - [IN] Timeout Period
* @return RC_SUCCESS
*/
RC_t WD_Start(WDT_TimeOut_t timeout)
{
    CyWdtStart(timeout,CYWDT_LPMODE_NOCHANGE);
    return RC_SUCCESS;   
}

/**
* Service the Watchdog Trigger
* @return RC_SUCCESS
*/
RC_t WD_Trigger()
{
    if((wd_aliveBits & WD_EXPECTED_ALIVE_MASK)==0xF)
    {
        CyWdtClear();
        wd_aliveBits=0;
        //UART_LOG_PutString("Watchdog is Feed and Alive..... Resetting bits\r\n");
    }
    return RC_SUCCESS;
}

/**
* Checks the watchdog bit
* @return TRUE if watchdog reset bit was set
*/
boolean_t WD_CheckResetBit()
{
    if(CyResetStatus & CY_RESET_WD)
        return TRUE;
    else
        return FALSE;
}

/**
* Checks the watchdog Alive state
* @return RC_SUCCESS
**/
/*
    Let say joystick runnable sends alive bit i.e. myBitPosition = 0
    Initial value of WD_RunnableAliveBitField = 0000 0000
    left shift 1U by 0 i.e. 0000 0001 <<0 ==> 0000 0001
    Bitwise OR between (1U << myBitPositon) and WD_RunnableAliveBitField ==> 0000 0001
    so for second runnable ==> 0000 0011
    Third runnable ==> 0000 0111
    Fourth runnable ==> 0000 1111
*/
RC_t WD_Alive(uint8_t myBitPosition)
{
    wd_aliveBits |= (1UL << myBitPosition);
    return RC_SUCCESS;
}