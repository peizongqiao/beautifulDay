#ifndef __BSP_PI_API_H__
#define __BSP_PI_API_H__

#include "n32g43x.h" 
#include "bsp_pi_cfg.h"
#include "app_log.h"

static void bsp_pi_api_init(void);

static bool PI_check(uint8_t channal, float value);

static bool FocusMotorWait(void);
static bool ZoomMotorWait(void);
static bool DoorMotorWait(void);
static bool IrisMotorWait(void);

static void bsp_pi_api_init(void)
{
    //
}

static bool PI_check(uint8_t channal, float value)
{
    float v;
    v = (float)ADC_GetData(channal)*3.3/1023;
		
    if(v > value){
		return false;
	}
	if(v - value < PI_DELTA){
		return true;
	}else{
		return false;
	}
}

static bool FocusMotorWait()
{
    if( PI_check(FOCUS_PI, PI_V))
    {
		return true;
	}else{
		return false;
	}
}

static bool ZoomMotorWait()
{
    if( PI_check(ZOOM_PI, PI_V))
    {
		return true;
	}else{
		return false;
	}
}

static bool DoorMotorWait()
{
    if( PI_check(VALVE_PI, PI_V))
    {
		return true;
	}else{
		return false;
	}
}

static bool IrisMotorWait()
{
    if( PI_check(IRIS_PI, PI_V))
    {
		return true;
	}else{
		return false;
	}
}

#endif /* __BSP_PI_API_H__ */
