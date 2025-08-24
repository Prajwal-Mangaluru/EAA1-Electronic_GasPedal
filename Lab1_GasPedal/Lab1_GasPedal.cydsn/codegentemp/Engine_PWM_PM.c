/*******************************************************************************
* File Name: Engine_PWM_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Engine_PWM.h"

static Engine_PWM_backupStruct Engine_PWM_backup;


/*******************************************************************************
* Function Name: Engine_PWM_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Engine_PWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Engine_PWM_SaveConfig(void) 
{

    #if(!Engine_PWM_UsingFixedFunction)
        #if(!Engine_PWM_PWMModeIsCenterAligned)
            Engine_PWM_backup.PWMPeriod = Engine_PWM_ReadPeriod();
        #endif /* (!Engine_PWM_PWMModeIsCenterAligned) */
        Engine_PWM_backup.PWMUdb = Engine_PWM_ReadCounter();
        #if (Engine_PWM_UseStatus)
            Engine_PWM_backup.InterruptMaskValue = Engine_PWM_STATUS_MASK;
        #endif /* (Engine_PWM_UseStatus) */

        #if(Engine_PWM_DeadBandMode == Engine_PWM__B_PWM__DBM_256_CLOCKS || \
            Engine_PWM_DeadBandMode == Engine_PWM__B_PWM__DBM_2_4_CLOCKS)
            Engine_PWM_backup.PWMdeadBandValue = Engine_PWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Engine_PWM_KillModeMinTime)
             Engine_PWM_backup.PWMKillCounterPeriod = Engine_PWM_ReadKillTime();
        #endif /* (Engine_PWM_KillModeMinTime) */

        #if(Engine_PWM_UseControl)
            Engine_PWM_backup.PWMControlRegister = Engine_PWM_ReadControlRegister();
        #endif /* (Engine_PWM_UseControl) */
    #endif  /* (!Engine_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Engine_PWM_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Engine_PWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Engine_PWM_RestoreConfig(void) 
{
        #if(!Engine_PWM_UsingFixedFunction)
            #if(!Engine_PWM_PWMModeIsCenterAligned)
                Engine_PWM_WritePeriod(Engine_PWM_backup.PWMPeriod);
            #endif /* (!Engine_PWM_PWMModeIsCenterAligned) */

            Engine_PWM_WriteCounter(Engine_PWM_backup.PWMUdb);

            #if (Engine_PWM_UseStatus)
                Engine_PWM_STATUS_MASK = Engine_PWM_backup.InterruptMaskValue;
            #endif /* (Engine_PWM_UseStatus) */

            #if(Engine_PWM_DeadBandMode == Engine_PWM__B_PWM__DBM_256_CLOCKS || \
                Engine_PWM_DeadBandMode == Engine_PWM__B_PWM__DBM_2_4_CLOCKS)
                Engine_PWM_WriteDeadTime(Engine_PWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Engine_PWM_KillModeMinTime)
                Engine_PWM_WriteKillTime(Engine_PWM_backup.PWMKillCounterPeriod);
            #endif /* (Engine_PWM_KillModeMinTime) */

            #if(Engine_PWM_UseControl)
                Engine_PWM_WriteControlRegister(Engine_PWM_backup.PWMControlRegister);
            #endif /* (Engine_PWM_UseControl) */
        #endif  /* (!Engine_PWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Engine_PWM_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Engine_PWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Engine_PWM_Sleep(void) 
{
    #if(Engine_PWM_UseControl)
        if(Engine_PWM_CTRL_ENABLE == (Engine_PWM_CONTROL & Engine_PWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            Engine_PWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Engine_PWM_backup.PWMEnableState = 0u;
        }
    #endif /* (Engine_PWM_UseControl) */

    /* Stop component */
    Engine_PWM_Stop();

    /* Save registers configuration */
    Engine_PWM_SaveConfig();
}


/*******************************************************************************
* Function Name: Engine_PWM_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Engine_PWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Engine_PWM_Wakeup(void) 
{
     /* Restore registers values */
    Engine_PWM_RestoreConfig();

    if(Engine_PWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Engine_PWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
