#include "main.h"
#include "app_focal_ctrl.h"
#include "bsp_focal_dz24.h"
#include "app_softtimer.h"
#include "app_log.h"
#include "bsp_pi_api.h"
#include "dlp_manager.h"
#include "i2c_slave_N32G43X.h"

uint8_t reture_date[10];
uint16_t log_step,log_pps; 
uint16_t absolutely_step,focal_mode = 0;
//uint16_t flag;
uint16_t send[1];
extern uint8_t RxBuffer[15];
extern uint8_t log_number,log_flag;
//uint8_t cont;
uint8_t cont_1;
bool flag_pps = 0;
bool TSP_mode;
bool TSP_mode_1;
uint8_t flag_df;
uint8_t d[2];
uint8_t temp_t;
uint8_t pwn_count;
uint16_t 	user_buf1[128];
uint8_t 	user_buf2[256];
uint32_t	addr;
extern int8_t tepm_DMD;

void foacl()
{
    if(log_flag==1)
    {
//		    log_info("f\t");
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x02 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x01 && RxBuffer[4] ==0x55)// aa 01 00 01 55 开光机
		{	
			
			d[0] = 0x01;	
			d[1] = 0x00;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x02 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x02 && RxBuffer[4] ==0x55)//关光机
		{
			
			d[0] = 0x01;	
			d[1] = 0x01;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x02 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x03 && RxBuffer[4] ==0x55)//aa 03 00 01 55LOOK0
		{
			
			d[0] = 0x01;	
			d[1] = 0x02;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x02 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x04 && RxBuffer[4] ==0x55)//aa 03 00 02 55LOOK11
		{
			
			d[0] = 0x01;	
			d[1] = 0x03;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x02 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x05 && RxBuffer[4] ==0x55)//aa 03 00 03 55//LOOK2
		{
			
			d[0] = 0x01;	
			d[1] = 0x04;
			i2c_write_block(0x02,2,0,d);
		}  
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x02 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x06 && RxBuffer[4] ==0x55)//aa 03 00 04 55LOOK3
		{
			
			d[0] = 0x01;	
			d[1] = 0x05;
			i2c_write_block(0x02,2,0,d);
		}  
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x02 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x07 && RxBuffer[4] ==0x55)//aa 03 00 05 55LOOK4
		{
			
			d[0] = 0x01;	
			d[1] = 0x06;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x02 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x08 && RxBuffer[4] ==0x55)//aa 03 00 06 55//LOOK5
		{
			
			d[0] = 0x01;	
			d[1] = 0x07;
			i2c_write_block(0x02,2,0,d);
		}  
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x02 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x09 && RxBuffer[4] ==0x55)//aa 03 00 07 55//LOOK6
		{
			
			d[0] = 0x01;	
			d[1] = 0x08;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x02 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x0A && RxBuffer[4] ==0x55)//aa 03 00 08 55//LOOK7
		{
			
			d[0] = 0x01;	
			d[1] = 0x09;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x02 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x0B && RxBuffer[4] ==0x55)//aa 03 00 08 55//LOOK7
		{
			
			d[0] = 0x01;	
			d[1] = 0x2A;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x03 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x01 && RxBuffer[4] ==0x55)//aa 03 00 09 55LOOK8
		{
			d[0] = 0x01;	
			d[1] = 0x0A;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x03 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x02 && RxBuffer[4] ==0x55)//aa 03 00 0a 55//LOOK9
		{
			
			d[0] = 0x01;	
			d[1] = 0x0B;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x03 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x03 && RxBuffer[4] ==0x55)//aa 08 00 01 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x0C;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x03 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x06 && RxBuffer[4] ==0x55)//aa 08 00 02 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x0D;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x03 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x07 && RxBuffer[4] ==0x55)//aa 08 00 03 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x0E;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x03 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x08 && RxBuffer[4] ==0x55)//aa 08 00 04 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x0F;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x04 && RxBuffer[2] == 0x01 && RxBuffer[3] ==0x01 && RxBuffer[4] ==0x55)//aa 08 00 06 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x10;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x04 && RxBuffer[2] == 0x01 && RxBuffer[3] ==0x02 && RxBuffer[4] ==0x55)//aa 08 00 07 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x11;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x04 && RxBuffer[2] == 0x01 && RxBuffer[3] ==0x03 && RxBuffer[4] ==0x55)//aa 08 00 08 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x12;
			i2c_write_block(0x02,2,0,d);           
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x04 && RxBuffer[2] == 0x01 && RxBuffer[3] ==0x04 && RxBuffer[4] ==0x55)//aa 08 00 09 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x13;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x04 && RxBuffer[2] == 0x01 && RxBuffer[3] ==0x05 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x14;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x04 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x01 && RxBuffer[4] ==0x55)//aa 08 00 06 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x15;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x04 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x02 && RxBuffer[4] ==0x55)//aa 08 00 07 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x16;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x04 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x03 && RxBuffer[4] ==0x55)//aa 08 00 08 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x17;
			i2c_write_block(0x02,2,0,d);            }
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x04 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x04 && RxBuffer[4] ==0x55)//aa 08 00 09 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x18;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x04 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x05 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x19;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x04 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x06 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x02;	
			d[1] = 0x01;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x04 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x07 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x02;	
			d[1] = 0x02;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x04 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x08 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x02;	
			d[1] = 0x03;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x04 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x09 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x02;	
			d[1] = 0x04;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x04 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x0A && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x02;	
			d[1] = 0x05;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x05 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x00 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x1A;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x05 && RxBuffer[2] == 0x01 && RxBuffer[3] ==0x00 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x1B;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x06 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x01 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			TSP_mode = 0;
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x06 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x00 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			TSP_mode = 1;
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x06 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x02 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			TSP_mode_1 = 1;
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x06 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x03 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			TSP_mode_1 = 0;
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x01 && RxBuffer[3] ==0x05 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x1C;
//				log_info("a\t");
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x01 && RxBuffer[3] ==0x06 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x1D;
//				log_info("b\t");
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x05 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x1E;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x06 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x1F;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x03 && RxBuffer[3] ==0x05 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x20;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x03 && RxBuffer[3] ==0x06 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x21;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x08 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x01 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x22;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x08 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x02 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x23;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x08 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x03 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x24;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x08 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x04 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x25;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x08 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x05 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x40;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x08 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x06 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x41;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x08 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x07 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x26;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x08 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x08 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x37;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x09 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x00 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			flag_df = 1;
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x09 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x01 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			flag_df = 2;
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x09 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x02 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			flag_df = 3;
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x01 && RxBuffer[3] ==0x07 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			d[0] = 0x01;	
			d[1] = 0x2B;
//				log_info("c\t");
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x01 && RxBuffer[3] ==0x08 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			d[0] = 0x01;	
			d[1] = 0x2C;
//				log_info("d\t");
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x07 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x2D;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x08 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x2E;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x03 && RxBuffer[3] ==0x07 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x2F;
			i2c_write_block(0x02,2,0,d);

		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x03 && RxBuffer[3] ==0x08 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x30;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x01 && RxBuffer[3] ==0x09 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x31;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x01 && RxBuffer[3] ==0x0A && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x32;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x09 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x33;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x0A && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x34;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x03 && RxBuffer[3] ==0x09 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x35;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x03 && RxBuffer[3] ==0x0A && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x01;	
			d[1] = 0x36;
			i2c_write_block(0x02,2,0,d);
		}	
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x09 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x03 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			flag_df=4;
		}	
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x09 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x04 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			flag_df=5;
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x0A && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x00 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			pwn_count = Flash_Read(0x08017000);
			TIM4->CCDAT1=++pwn_count;
			if(pwn_count>125)
			{
				pwn_count = 125;
			}
			addr = FLASH_WRITE_START_ADDR;
			memset(user_buf2, 0xa1, 256);
			user_buf2[0] = pwn_count;
			FLASH_Program_User(addr, user_buf2, 256);	//在0x08010000起始地址写256字节
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x0A && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x01 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			pwn_count = Flash_Read(0x08017000);
			TIM4->CCDAT1=--pwn_count;
			addr = FLASH_WRITE_START_ADDR;
			memset(user_buf2, 0xa1, 256);
			user_buf2[0] = pwn_count;
			FLASH_Program_User(addr, user_buf2, 256);	//在0x08010000起始地址写256字节
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x0A && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x00 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			addr = 0x08017000;
			memset(user_buf2, 0xa1, 256);
			user_buf2[0] = 0x64;
			FLASH_Program_User(addr, user_buf2, 256);	//在0x08010000起始地址写256字节	
		}
		
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x03 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x0B && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			addr = 0x08017000;
			memset(user_buf2, 0xa1, 256);
			user_buf2[0] = 0x3B;
			FLASH_Program_User(addr, user_buf2, 256);	//在0x08010000起始地址写256字节	
			
			d[0] = 0x02;	
			d[1] = 0x20;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x03 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x0C && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			addr = 0x08017000;
			memset(user_buf2, 0xa1, 256);
			user_buf2[0] = 0x75;
			FLASH_Program_User(addr, user_buf2, 256);	//在0x08010000起始地址写256字节	
			
			d[0] = 0x02;	
			d[1] = 0x21;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x05 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x00 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x02;	
			d[1] = 0x13;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x05 && RxBuffer[2] == 0x03 && RxBuffer[3] ==0x00 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x02;	
			d[1] = 0x14;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x03 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x09 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			d[0] = 0x02;	
			d[1] = 0x10;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x03 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x0A && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x02;	
			d[1] = 0x11;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x03 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x0B && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			addr = 0x08017000;
			memset(user_buf2, 0xa1, 256);
			user_buf2[0] = 0x3B;
			FLASH_Program_User(addr, user_buf2, 256);	//在0x08010000起始地址写256字节	
			
			d[0] = 0x02;	
			d[1] = 0x20;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x03 && RxBuffer[2] == 0x00 && RxBuffer[3] ==0x0C && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			addr = 0x08017000;
			memset(user_buf2, 0xa1, 256);
			user_buf2[0] = 0x75;
			FLASH_Program_User(addr, user_buf2, 256);	//在0x08010000起始地址写256字节	
			
			d[0] = 0x02;	
			d[1] = 0x21;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x01 && RxBuffer[3] ==0x0B && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			d[0] = 0x03;	
			d[1] = 0x01;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x01 && RxBuffer[3] ==0x0C && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x03;	
			d[1] = 0x05;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x01 && RxBuffer[3] ==0x0D && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x03;	
			d[1] = 0x02;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x0B && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x03;	
			d[1] = 0x06;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x0C && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			
			d[0] = 0x03;	
			d[1] = 0x03;
			i2c_write_block(0x02,2,0,d);
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x07 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x0D && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			d[0] = 0x03;	
			d[1] = 0x04;
			i2c_write_block(0x02,2,0,d);
		}
		
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x04 && RxBuffer[2] == 0x01 && RxBuffer[3] ==0x06 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			tepm_DMD = Flash_Read(0x080179FF);
			tepm_DMD = tepm_DMD+1;
			addr = 0x080179FF;
			memset(user_buf2, 0xa1, 256);
			user_buf2[0] = tepm_DMD;
			FLASH_Program_User(addr, user_buf2, 256);	//在0x08010000起始地址写256字节
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x04 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x06 && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			tepm_DMD = Flash_Read(0x080179FF);
			tepm_DMD = tepm_DMD-1;
			addr = 0x080179FF;
			memset(user_buf2, 0xa1, 256);
			user_buf2[0] = tepm_DMD;
			FLASH_Program_User(addr, user_buf2, 256);	//在0x08010000起始地址写256字节
		}
		if(RxBuffer[0] ==0xaa && RxBuffer[1] == 0x04 && RxBuffer[2] == 0x02 && RxBuffer[3] ==0x0B && RxBuffer[4] ==0x55)//aa 08 00 0a 55
		{
			addr = 0x080179FF;
			memset(user_buf2, 0xa1, 256);
			user_buf2[0] = 0;
			FLASH_Program_User(addr, user_buf2, 256);	//在0x08010000起始地址写256字节
		}
		 I2C2_GPIO_init();
         USART_ConfigInt(UART4, USART_INT_RXDNE, ENABLE);
        }
		log_flag = 0;
}