#ifndef GPTM_H_
#define GPTM_H_
#include <stdint.h>
#include "GPTM_Cfg.h"


typedef enum {TIMERA_SPLIT_MODE, TIMERB_SPLIT_MODE, CASCADED_MODE} TimerConfig;
typedef enum {ONE_SHOT, PERIODIC, CAPTURE_EDGE_COUNT, CAPTURE_EDGE_TIME, RTC_MODE, PWM_MODE} TimerMode;
typedef enum {UP_COUNT, DOWN_COUNT} TimerCountDir;
typedef enum {POSITIVE_EDGE, NEGATIVE_EDGE, BOTH_EDGES} TimerEventMode;

typedef struct
{
    uint8_t TimerID;
    TimerConfig TimerConfig;
    TimerMode TimerMode;
    TimerCountDir TimerCountDir;
    TimerEventMode TimerEventMode;
    uint8_t PrescalerValue;
    void (*CallbackPtr)(void);
} GPTM_CfgType;

typedef enum {GPTM_OK, GPTM_NOK} GPTM_CheckType;

extern GPTM_CfgType GPTM_ConfigParameters[GPTM_GROUPS_NUMBER];


GPTM_CheckType GPTM_Init(void);
GPTM_CheckType GPTM_Delay(uint8_t ModuleID, uint16_t TimeMS);
GPTM_CheckType GPTM_CaptureTime(uint8_t ModuleID, uint32_t *TimeMS);
GPTM_CheckType GPTM_CaptureCount(uint8_t ModuleID, uint16_t Counts);
GPTM_CheckType GPTM_CaptureRTC(uint8_t ModuleID, uint16_t MatchTimeMS);
GPTM_CheckType GPTM_PWM(uint8_t ModuleID, uint16_t TimeMS, uint8_t DutyCycle);


#endif /* GPTM_H_ */
