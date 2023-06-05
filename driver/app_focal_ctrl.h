#ifndef _APP_FOCAL_CTRL_H
#define _APP_FOCAL_CTRL_H

#include "bsp_focal_api_types.h"
#include "n32g43x.h"

typedef struct
{
    u8 focus_run_number;
	u8 zoom_run_number;
    focal_direction_t focus_direction;
    focal_direction_t zoom_direction;
}focal_ctrl_t;

static focal_ctrl_t focal_ctrl;
void foacl();

#endif /* _APP_FOCAL_CTRL_H */