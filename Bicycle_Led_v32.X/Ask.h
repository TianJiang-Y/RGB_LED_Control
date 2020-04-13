#ifndef		__ASK_H_
#define		__ASK_H_

#include "xc.h"

#define max_time_h       60      //宽脉冲最大允许时间 data*TCC_time
#define min_time_h       16      //宽脉冲最小允许时间
#define max_time_l       14      //窄脉冲最大允许时间
#define min_time_l       4      //窄脉冲最小允许时间

//#define max_time_h       60      //宽脉冲最大允许时间 data*TCC_time
//#define min_time_h       38      //宽脉冲最小允许时间
//#define max_time_l       35      //窄脉冲最大允许时间
//#define min_time_l       4     //窄脉冲最小允许时间
   
#define		RECV_BIT_NUMBER		24		 //设定接收的位数
#define 	inport  	    PORTAbits.RA2   
#define		ASK		        PORTAbits.RA2

#define Shock_OUT               LATAbits.LATA0

struct state
{
  unsigned char mode;
  unsigned char dev;
  unsigned char level;
  unsigned char bat_state; 
};

#define     M_LED         1
#define     M_SOUND       2
#define     M_VIBRATION   3
#define     M_ESHOCK      4

void Ask_process();
void Recieve();
void RecieveError();
void ProcessRecv();
void Learn_Sender();
void ReadSelfAddr();
void Dele_Sender();
void ProcessOut();

#endif