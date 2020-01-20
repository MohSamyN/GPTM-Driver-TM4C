#include <GPTM.h>
#include <GPTM_Cfg.h>

void Fn1(void);

extern GPTM_CfgType GPTM_ConfigParameters[GPTM_GROUPS_NUMBER] =
{
    {
         1,
         TIMERA_SPLIT_MODE,
         PWM_MODE,
         DOWN_COUNT,
         POSITIVE_EDGE,
         1,
         &Fn1
    },
    {
         2,
         TIMERA_SPLIT_MODE,
         CAPTURE_EDGE_TIME,
         UP_COUNT,
         NEGATIVE_EDGE,
         250,
         &Fn1
    },
    {
         0,
         TIMERA_SPLIT_MODE,
         CAPTURE_EDGE_COUNT,
         UP_COUNT,
         NEGATIVE_EDGE,
         0,
         &Fn1
    }
};
