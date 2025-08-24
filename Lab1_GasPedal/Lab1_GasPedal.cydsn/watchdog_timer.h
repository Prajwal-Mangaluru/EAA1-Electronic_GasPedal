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
# include "project.h"
# include "global.h"
# include "derivate.h"

typedef enum 
{
    ticks_4_6_ms     = CYWDT_2_TICKS,
    ticks_32_48_ms   = CYWDT_16_TICKS,
    ticks_256_384_ms = CYWDT_128_TICKS,
    ticks_2_3_s      = CYWDT_1024_TICKS
}WDT_TimeOut_t;

# define run_joystick_pos 0U
# define run_control_pos  1U
# define run_engine_pos   2U
# define run_brake_pos    3U

/**
* Activate the Watchdog Trigger
* \param WDT_TimeOut_t timeout - [IN] Timeout Period
* @return RC_SUCCESS
*/
RC_t WD_Start(WDT_TimeOut_t timeout);

/**
* Service the Watchdog Trigger
* @return RC_SUCCESS
*/
RC_t WD_Trigger();

/**
* Checks the watchdog bit
* @return TRUE if watchdog reset bit was set
*/
boolean_t WD_CheckResetBit();

/**
* Checks the watchdog Alive state
* @return RC_SUCCESS
**/
RC_t WD_Alive(uint8_t myBitPosition);