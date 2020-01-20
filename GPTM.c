#include <GPTM.h>
#include <GPTM_Cfg.h>
#include <stdint.h>
#include "M4MemMap.h"

typedef volatile uint32_t* const GPTM_RegAddType;

#define MODULES_NUMBER          12u

#define TIMER0_BASE_ADDRESS     0x40030000
#define TIMER1_BASE_ADDRESS     0x40031000
#define TIMER2_BASE_ADDRESS     0x40032000
#define TIMER3_BASE_ADDRESS     0x40033000
#define TIMER4_BASE_ADDRESS     0x40034000
#define TIMER5_BASE_ADDRESS     0x40035000
#define WTIMER0_BASE_ADDRESS    0x40036000
#define WTIMER1_BASE_ADDRESS    0x40037000
#define WTIMER2_BASE_ADDRESS    0x4004C000
#define WTIMER3_BASE_ADDRESS    0x4004D000
#define WTIMER4_BASE_ADDRESS    0x4004E000
#define WTIMER5_BASE_ADDRESS    0x4004F000

static const uint32_t TimersBaseAddressLUT[MODULES_NUMBER] =
{
     TIMER0_BASE_ADDRESS,
     TIMER1_BASE_ADDRESS,
     TIMER2_BASE_ADDRESS,
     TIMER3_BASE_ADDRESS,
     TIMER4_BASE_ADDRESS,
     TIMER5_BASE_ADDRESS,
     WTIMER0_BASE_ADDRESS,
     WTIMER1_BASE_ADDRESS,
     WTIMER2_BASE_ADDRESS,
     WTIMER3_BASE_ADDRESS,
     WTIMER4_BASE_ADDRESS,
     WTIMER5_BASE_ADDRESS
};

#define GPTM_REG_ADDRESS(TIMER_ID, REG_OFFSET)   (TimersBaseAddressLUT[TIMER_ID] + REG_OFFSET)

#define GPTMCFG(TIMER_ID)               *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x000))
#define GPTMTAMR(TIMER_ID)              *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x004))
#define GPTMTBMR(TIMER_ID)              *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x008))
#define GPTMCTL(TIMER_ID)               *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x00C))
#define GPTMSYNC(TIMER_ID)              *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x010))
#define GPTMIMR(TIMER_ID)               *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x018))
#define GPTMRIS(TIMER_ID)               *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x01C))
#define GPTMMIS(TIMER_ID)               *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x020))
#define GPTMICR(TIMER_ID)               *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x024))
#define GPTMTAILR(TIMER_ID)             *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x028))
#define GPTMTBILR(TIMER_ID)             *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x02C))
#define GPTMTAMATCHR(TIMER_ID)          *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x030))
#define GPTMTBMATCHR(TIMER_ID)          *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x034))
#define GPTMTAPR(TIMER_ID)              *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x038))
#define GPTMTBPR(TIMER_ID)              *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x03C))
#define GPTMTAPMR(TIMER_ID)             *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x040))
#define GPTMTBPMR(TIMER_ID)             *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x044))
#define GPTMTAR(TIMER_ID)               *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x048))
#define GPTMTBR(TIMER_ID)               *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x04C))
#define GPTMTAV(TIMER_ID)               *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x050))
#define GPTMTBV(TIMER_ID)               *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x054))
#define GPTMRTCPD(TIMER_ID)             *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x058))
#define GPTMTAPS(TIMER_ID)              *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x05C))
#define GPTMTBPS(TIMER_ID)              *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x060))
#define GPTMTAPV(TIMER_ID)              *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x064))
#define GPTMTBPV(TIMER_ID)              *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0x068))
#define GPTMPP(TIMER_ID)                *((GPTM_RegAddType)GPTM_REG_ADDRESS(TIMER_ID, 0xFC0))

static uint8_t GPTM_TimerState[MODULES_NUMBER] = {0};
static uint8_t GPTM_Flag[MODULES_NUMBER] = {0};


GPTM_CheckType GPTM_Init(void)
{
    GPTM_CheckType RetVar;
    GPTM_CfgType* CfgPtr;
    uint8_t ErrorFlag = 0;
    uint8_t LoopIndex;
    for(LoopIndex = 0; ((LoopIndex < GPTM_GROUPS_NUMBER) && (ErrorFlag == 0)); LoopIndex++)
    {
        if(GPTM_ConfigParameters[LoopIndex].TimerID < MODULES_NUMBER)
        {
            CfgPtr = &GPTM_ConfigParameters[LoopIndex];

            /* Enabling the timer clock gating */
            if(CfgPtr->TimerID < 6)
            {
                RCGCTIMER_REG |= (1 << CfgPtr->TimerID);
            }
            else
            {
                RCGCWTIMER_REG |= (1 << (CfgPtr->TimerID - 6));
            }

            /* Disabling Timer A and Timer B before configuration */
            GPTMCTL(CfgPtr->TimerID) &= ~(0x0101);

            /* Selecting the timer mode */
            if(CfgPtr->TimerConfig == CASCADED_MODE)
            {
                GPTMCFG(CfgPtr->TimerID) = 0x00;
                /* Selecting one-shot or periodic modes */
                if(CfgPtr->TimerMode == ONE_SHOT)
                {
                    GPTMTAMR(CfgPtr->TimerID) |= 0x01;
                    /* Selecting up or down count direction */
                    if(CfgPtr->TimerCountDir == UP_COUNT)
                    {
                        GPTMTAMR(CfgPtr->TimerID) |= 0x10;
                    }
                    else
                    {
                        GPTMTAMR(CfgPtr->TimerID) &= ~(0x10);
                    }
                }
                else if(CfgPtr->TimerMode == PERIODIC)
                {
                    GPTMTAMR(CfgPtr->TimerID) |= 0x02;
                    /* Selecting up or down count direction */
                    if(CfgPtr->TimerCountDir == UP_COUNT)
                    {
                        GPTMTAMR(CfgPtr->TimerID) |= 0x10;
                    }
                    else
                    {
                        GPTMTAMR(CfgPtr->TimerID) &= ~(0x10);
                    }
                }
                else
                {
                }
            }
            else if(CfgPtr->TimerMode == RTC_MODE)
            {
                GPTMCFG(CfgPtr->TimerID) = 0x01;
                /* Clearing any configurations in Timer Mode register */
                GPTMTBMR(CfgPtr->TimerID) = 0;
                GPTMTAMR(CfgPtr->TimerID) = 0;
            }
            else if(CfgPtr->TimerConfig == TIMERA_SPLIT_MODE)
            {
                GPTMCFG(CfgPtr->TimerID) = 0x04;
                /* Selecting one-shot, periodic, capture edge time, capture edge count or PWM modes */
                if(CfgPtr->TimerMode == ONE_SHOT)
                {
                    GPTMTAMR(CfgPtr->TimerID) |= 0x01;
                    /* Selecting up or down count direction */
                    if(CfgPtr->TimerCountDir == UP_COUNT)
                    {
                        GPTMTAMR(CfgPtr->TimerID) |= 0x10;
                    }
                    else
                    {
                        GPTMTAMR(CfgPtr->TimerID) &= ~(0x10);
                    }
                }
                else if(CfgPtr->TimerMode == PERIODIC)
                {
                    GPTMTAMR(CfgPtr->TimerID) |= 0x02;
                    /* Selecting up or down count direction */
                    if(CfgPtr->TimerCountDir == UP_COUNT)
                    {
                        GPTMTAMR(CfgPtr->TimerID) |= 0x10;
                    }
                    else
                    {
                        GPTMTAMR(CfgPtr->TimerID) &= ~(0x10);
                    }
                }
                else if(CfgPtr->TimerMode == CAPTURE_EDGE_TIME)
                {
                    /* Selecting capture mode */
                    GPTMTAMR(CfgPtr->TimerID) |= 0x03;
                    /* Selecting edge time mode */
                    GPTMTAMR(CfgPtr->TimerID) |= 0x04;
                    /* Selecting positive, negative or both edges */
                    if(CfgPtr->TimerEventMode == NEGATIVE_EDGE)
                    {
                        GPTMCTL(CfgPtr->TimerID) |= 0x04;
                    }
                    else if(CfgPtr->TimerEventMode == BOTH_EDGES)
                    {
                        GPTMCTL(CfgPtr->TimerID) |= 0x0C;
                    }
                    else
                    {
                    }
                }
                else if(CfgPtr->TimerMode == CAPTURE_EDGE_COUNT)
                {
                    /* Selecting capture mode */
                    GPTMTAMR(CfgPtr->TimerID) |= 0x03;
                    /* Selecting edge count mode */
                    GPTMTAMR(CfgPtr->TimerID) &= ~(0x04);
                    /* Selecting positive, negative or both edges */
                    if(CfgPtr->TimerEventMode == NEGATIVE_EDGE)
                    {
                        GPTMCTL(CfgPtr->TimerID) |= 0x04;
                    }
                    else if(CfgPtr->TimerEventMode == BOTH_EDGES)
                    {
                        GPTMCTL(CfgPtr->TimerID) |= 0x0C;
                    }
                    else
                    {
                    }
                }
                else if(CfgPtr->TimerMode == PWM_MODE)
                {
                    /* Selecting PWM mode from the alternate mode select */
                    GPTMTAMR(CfgPtr->TimerID) |= 0x08;
                    /* Selecting the edge count mode */
                    GPTMTAMR(CfgPtr->TimerID) &= ~(0x04);
                    /* Selecting the periodic mode */
                    // Configuration - One-shot or periodic //
                    GPTMTAMR(CfgPtr->TimerID) |= 0x02;
                    /* Starts with one *///
                    GPTMTAMR(CfgPtr->TimerID) |= 0x0800;
                    /* Selecting whether counting when the timer is enabled or when an input is triggered */
                    // Configuration : when counter is enabled //
                    GPTMCTL(CfgPtr->TimerID) |= 0x40;
                }
                else
                {
                }
                /* Setting the timer prescaler value */
                GPTMTAPR(CfgPtr->TimerID) = CfgPtr->PrescalerValue;
            }
            else if(CfgPtr->TimerConfig == TIMERB_SPLIT_MODE)
            {
                GPTMCFG(CfgPtr->TimerID) = 0x04;
                /* Selecting one-shot, periodic, capture edge time, capture edge count or PWM modes */
                if(CfgPtr->TimerMode == ONE_SHOT)
                {
                    GPTMTBMR(CfgPtr->TimerID) |= 0x01;
                    /* Selecting up or down count direction */
                    if(CfgPtr->TimerCountDir == UP_COUNT)
                    {
                        GPTMTBMR(CfgPtr->TimerID) |= 0x10;
                    }
                    else
                    {
                        GPTMTBMR(CfgPtr->TimerID) &= ~(0x10);
                    }
                }
                else if(CfgPtr->TimerMode == PERIODIC)
                {
                    GPTMTBMR(CfgPtr->TimerID) |= 0x02;
                    /* Selecting up or down count direction */
                    if(CfgPtr->TimerCountDir == UP_COUNT)
                    {
                        GPTMTBMR(CfgPtr->TimerID) |= 0x10;
                    }
                    else
                    {
                        GPTMTBMR(CfgPtr->TimerID) &= ~(0x10);
                    }
                }
                else if(CfgPtr->TimerMode == CAPTURE_EDGE_TIME)
                {
                    /* Selecting capture mode */
                    GPTMTBMR(CfgPtr->TimerID) |= 0x03;
                    /* Selecting edge time mode */
                    GPTMTBMR(CfgPtr->TimerID) |= 0x04;
                    /* Selecting positive, negative or both edges */
                    if(CfgPtr->TimerEventMode == NEGATIVE_EDGE)
                    {
                        GPTMCTL(CfgPtr->TimerID) |= 0x0400;
                    }
                    else if(CfgPtr->TimerEventMode == BOTH_EDGES)
                    {
                        GPTMCTL(CfgPtr->TimerID) |= 0x0C00;
                    }
                    else
                    {
                    }
                }
                else if(CfgPtr->TimerMode == CAPTURE_EDGE_COUNT)
                {
                    /* Selecting capture mode */
                    GPTMTBMR(CfgPtr->TimerID) |= 0x03;
                    /* Selecting edge count mode */
                    GPTMTBMR(CfgPtr->TimerID) &= ~(0x04);
                    /* Selecting positive, negative or both edges */
                    if(CfgPtr->TimerEventMode == NEGATIVE_EDGE)
                    {
                        GPTMCTL(CfgPtr->TimerID) |= 0x0400;
                    }
                    else if(CfgPtr->TimerEventMode == BOTH_EDGES)
                    {
                        GPTMCTL(CfgPtr->TimerID) |= 0x0C00;
                    }
                    else
                    {
                    }
                }
                else if(CfgPtr->TimerMode == PWM_MODE)
                {

                }
                else
                {
                }
                /* Setting the timer prescaler value */
                GPTMTBPR(CfgPtr->TimerID) = CfgPtr->PrescalerValue;
            }
            else
            {
            }
            GPTM_TimerState[LoopIndex] = 1;
            RetVar = GPTM_OK;
        }
        else
        {
            ErrorFlag = 1;
            RetVar = GPTM_NOK;
        }
    }
    return RetVar;
}

GPTM_CheckType GPTM_Delay(uint8_t ModuleID, uint16_t TimeMS)
{
    GPTM_CheckType RetVar;
    GPTM_CfgType *CfgPtr;
    if((ModuleID < GPTM_GROUPS_NUMBER) && (GPTM_TimerState[ModuleID] == 1))
    {
        CfgPtr = &GPTM_ConfigParameters[ModuleID];
        if(CfgPtr->TimerConfig == TIMERB_SPLIT_MODE)
        {
            if(GPTM_Flag[ModuleID] == 0)
            {
                /* Loading the required value into the Interval Load Register */
                GPTMTBILR(CfgPtr->TimerID) = ((TimeMS * 16000) / CfgPtr->PrescalerValue);
                /* Enabling timer to start counting */
                GPTMCTL(CfgPtr->TimerID) |= 0x0100;
                GPTM_Flag[ModuleID] = 1;
            }
            else
            {
            }
            if((GPTMRIS(CfgPtr->TimerID) & 0x0100) != 0)
            {
                GPTMICR(CfgPtr->TimerID) |= 0x0100;
                CfgPtr->CallbackPtr();
            }
            else
            {
            }
        }
        else if(CfgPtr->TimerConfig == TIMERA_SPLIT_MODE)
        {
            if(GPTM_Flag[ModuleID] == 0)
            {
                /* Loading the required value into the Interval Load Register */
                GPTMTAILR(CfgPtr->TimerID) = ((TimeMS * 16000) / CfgPtr->PrescalerValue);
                /* Enabling timer to start counting */
                GPTMCTL(CfgPtr->TimerID) |= 0x0001;
                GPTM_Flag[ModuleID] = 1;
            }
            else
            {
            }
            if((GPTMRIS(CfgPtr->TimerID) & 0x0001) != 0)
            {
                GPTMICR(CfgPtr->TimerID) |= 0x0001;
                CfgPtr->CallbackPtr();
            }
            else
            {
            }
        }
        else if(CfgPtr->TimerConfig == CASCADED_MODE)
        {
            if(GPTM_Flag[ModuleID] == 0)
            {
                /* Loading the required value into the Interval Load Register */
                GPTMTAILR(CfgPtr->TimerID) = (TimeMS * 16000);
                /* Enabling timer to start counting */
                GPTMCTL(CfgPtr->TimerID) |= 0x0101;
                GPTM_Flag[ModuleID] = 1;
            }
            else
            {
            }
            if((GPTMRIS(CfgPtr->TimerID) & 0x0001) != 0)
            {
                GPTMICR(CfgPtr->TimerID) |= 0x0001;
                CfgPtr->CallbackPtr();
            }
            else
            {
            }
        }
        else
        {
        }
        RetVar = GPTM_OK;
    }
    else
    {
        RetVar = GPTM_NOK;
    }
    return RetVar;
}

GPTM_CheckType GPTM_CaptureTime(uint8_t ModuleID, uint32_t *TimeMS)
{
    GPTM_CheckType RetVar;
    GPTM_CfgType *CfgPtr;
    if((ModuleID < GPTM_GROUPS_NUMBER) && (GPTM_TimerState[ModuleID] == 1))
    {
        CfgPtr = &GPTM_ConfigParameters[ModuleID];
        if(CfgPtr->TimerConfig == TIMERB_SPLIT_MODE)
        {
            if(GPTM_Flag[ModuleID] == 0)
            {
                /* Clearing the Interval Load Register */
                GPTMTBILR(CfgPtr->TimerID) = 0x00;
                /* Enabling timer to start counting */
                GPTMCTL(CfgPtr->TimerID) |= 0x0100;
                GPTM_Flag[ModuleID] = 1;
            }
            else
            {
            }
            if((GPTMRIS(CfgPtr->TimerID) & 0x0400) != 0)
            {
                GPTMICR(CfgPtr->TimerID) |= 0x0400;
                *TimeMS = ((((GPTMTBR(CfgPtr->TimerID) & 0xFFFF) * CfgPtr->PrescalerValue)) / 16000);
                CfgPtr->CallbackPtr();
            }
            else
            {
            }
        }
        else if(CfgPtr->TimerConfig == TIMERA_SPLIT_MODE)
        {
            if(GPTM_Flag[ModuleID] == 0)
            {
                /* Clearing the Interval Load Register */
                GPTMTAILR(CfgPtr->TimerID) = 0;
                /* Enabling timer to start counting */
                GPTMCTL(CfgPtr->TimerID) |= 0x0001;
                GPTM_Flag[ModuleID] = 1;
            }
            else
            {
            }
            if((GPTMRIS(CfgPtr->TimerID) & 0x0004) != 0)
            {
                GPTMICR(CfgPtr->TimerID) |= 0x0004;
                *TimeMS = ((((GPTMTAR(CfgPtr->TimerID) & 0xFFFF) * CfgPtr->PrescalerValue)) / 16000);
                CfgPtr->CallbackPtr();
            }
            else
            {
            }
        }
        RetVar = GPTM_OK;
    }
    else
    {
        RetVar = GPTM_NOK;
    }
    return RetVar;
}


GPTM_CheckType GPTM_CaptureCount(uint8_t ModuleID, uint16_t Counts)
{
    GPTM_CheckType RetVar;
    GPTM_CfgType *CfgPtr;
    if((ModuleID < GPTM_GROUPS_NUMBER) && (GPTM_TimerState[ModuleID] == 1))
    {
        CfgPtr = &GPTM_ConfigParameters[ModuleID];
        if(CfgPtr->TimerConfig == TIMERB_SPLIT_MODE)
        {
            if(GPTM_Flag[ModuleID] == 0)
            {
                /* Loading the Counts value into the Interval Load Register */
                GPTMTBILR(CfgPtr->TimerID) = Counts;
                /* Clearing both Match and Prescaler Match Registers */
                GPTMTBMATCHR(CfgPtr->TimerID) = 0;
                GPTMTBPMR(CfgPtr->TimerID) = 0;
                /* Enabling timer to start counting */
                GPTMCTL(CfgPtr->TimerID) |= 0x0100;
                GPTM_Flag[ModuleID] = 1;
            }
            else
            {
            }
            if((GPTMRIS(CfgPtr->TimerID) & 0x0200) != 0)
            {
                GPTMICR(CfgPtr->TimerID) |= 0x0200;
                CfgPtr->CallbackPtr();
                GPTM_Flag[ModuleID] = 0;
            }
            else
            {
            }
        }
        else if(CfgPtr->TimerConfig == TIMERA_SPLIT_MODE)
        {
            if(GPTM_Flag[ModuleID] == 0)
            {
                /* Loading the Counts value into the Interval Load Register */
                GPTMTAILR(CfgPtr->TimerID) = Counts;
                /* Clearing both Match and Prescaler Match Registers */
                GPTMTAMATCHR(CfgPtr->TimerID) = 0;
                GPTMTAPMR(CfgPtr->TimerID) = 0;
                /* Enabling timer to start counting */
                GPTMCTL(CfgPtr->TimerID) |= 0x0001;
                GPTM_Flag[ModuleID] = 1;
            }
            else
            {
            }
            if((GPTMRIS(CfgPtr->TimerID) & 0x0002) != 0)
            {
                GPTMICR(CfgPtr->TimerID) |= 0x0002;
                CfgPtr->CallbackPtr();
                GPTM_Flag[ModuleID] = 0;
            }
            else
            {
            }
        }
        RetVar = GPTM_OK;
    }
    else
    {
        RetVar = GPTM_NOK;
    }
    return RetVar;
}

GPTM_CheckType GPTM_CaptureRTC(uint8_t ModuleID, uint16_t MatchTimeMS)
{
    GPTM_CheckType RetVar;
    GPTM_CfgType *CfgPtr;
    if((ModuleID < GPTM_GROUPS_NUMBER) && (GPTM_TimerState[ModuleID] == 1))
    {
        CfgPtr = &GPTM_ConfigParameters[ModuleID];
        if(CfgPtr->TimerConfig == TIMERB_SPLIT_MODE)
        {
            if(GPTM_Flag[ModuleID] == 0)
            {
                /* Clearing the Interval Load Register */
                GPTMTBILR(CfgPtr->TimerID) = 0x00;
                /* Loading the MatchTimeMS into Match Register */
                GPTMTBMATCHR(CfgPtr->TimerID) = MatchTimeMS;
                /* Clearing the RTC and STALL enable bits in order to halt when reset happens */
                GPTMCTL(CfgPtr->TimerID) &= ~(0x0210);
                /* Enabling timer to start counting */
                GPTMCTL(CfgPtr->TimerID) |= 0x0100;
                GPTM_Flag[ModuleID] = 1;
            }
            else
            {
            }
            if((GPTMRIS(CfgPtr->TimerID) & 0x0008) != 0)
            {
                GPTMICR(CfgPtr->TimerID) |= 0x0008;
                CfgPtr->CallbackPtr();
            }
            else
            {
            }
        }
        else if(CfgPtr->TimerConfig == TIMERA_SPLIT_MODE)
        {
            if(GPTM_Flag[ModuleID] == 0)
            {
                /* Clearing the Interval Load Register */
                //GPTMTAILR(CfgPtr->TimerID) = 0;
                /* Loading the MatchTimeMS into Match Register */
                GPTMTAMATCHR(CfgPtr->TimerID) = MatchTimeMS;
                /* Clearing the RTC and STALL enable bits in order to halt when reset happens */
                GPTMCTL(CfgPtr->TimerID) &= ~(0x0012);
                /* Enabling timer to start counting */
                GPTMCTL(CfgPtr->TimerID) |= 0x0001;
                GPTM_Flag[ModuleID] = 1;
            }
            else
            {
            }
            if((GPTMRIS(CfgPtr->TimerID) & 0x0008) != 0)
            {
                GPTMICR(CfgPtr->TimerID) |= 0x0008;
                CfgPtr->CallbackPtr();
            }
            else
            {
            }
        }
        RetVar = GPTM_OK;
    }
    else
    {
        RetVar = GPTM_NOK;
    }
    return RetVar;
}

GPTM_CheckType GPTM_PWM(uint8_t ModuleID, uint16_t TimeMS, uint8_t DutyCycle)
{
    GPTM_CheckType RetVar;
    GPTM_CfgType *CfgPtr;
    if((ModuleID < GPTM_GROUPS_NUMBER) && (GPTM_TimerState[ModuleID] == 1))
    {
        CfgPtr = &GPTM_ConfigParameters[ModuleID];
        if(CfgPtr->TimerConfig == TIMERB_SPLIT_MODE)
        {
            if(GPTM_Flag[ModuleID] == 0)
            {
                /* Clearing the Interval Load Register */
                GPTMTBILR(CfgPtr->TimerID) = 0x00;
                /* Loading the MatchTimeMS into Match Register */
                //GPTMTBMATCHR(CfgPtr->TimerID) = MatchTimeMS;
                /* Clearing the RTC and STALL enable bits in order to halt when reset happens */
                GPTMCTL(CfgPtr->TimerID) &= ~(0x0210);
                /* Enabling timer to start counting */
                GPTMCTL(CfgPtr->TimerID) |= 0x0100;
                GPTM_Flag[ModuleID] = 1;
            }
            else
            {
            }
            if((GPTMRIS(CfgPtr->TimerID) & 0x0008) != 0)
            {
                GPTMICR(CfgPtr->TimerID) |= 0x0008;
                CfgPtr->CallbackPtr();
            }
            else
            {
            }
        }
        else if(CfgPtr->TimerConfig == TIMERA_SPLIT_MODE)
        {
            if(GPTM_Flag[ModuleID] == 0)
            {
                /* Loading the required value into the Interval Load Register */
                GPTMTAILR(CfgPtr->TimerID) = ((TimeMS * 16000) / CfgPtr->PrescalerValue);
                /* Loading the duty cycle value into Match Register */
                GPTMTAMATCHR(CfgPtr->TimerID) = ((((TimeMS * 16000) / CfgPtr->PrescalerValue) * DutyCycle) / 100);
                /* Enabling timer to start counting */
                GPTMCTL(CfgPtr->TimerID) |= 0x0001;
                GPTM_Flag[ModuleID] = 1;
            }
            else
            {
            }
        }
        RetVar = GPTM_OK;
    }
    else
    {
        RetVar = GPTM_NOK;
    }
    return RetVar;
}
