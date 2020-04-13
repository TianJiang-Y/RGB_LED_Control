#ifndef		__ASK_H_
#define		__ASK_H_

#include "xc.h"

#define max_time_h       60      //�������������ʱ�� data*TCC_time
#define min_time_h       16      //��������С����ʱ��
#define max_time_l       14      //խ�����������ʱ��
#define min_time_l       4      //խ������С����ʱ��

//#define max_time_h       60      //�������������ʱ�� data*TCC_time
//#define min_time_h       38      //��������С����ʱ��
//#define max_time_l       35      //խ�����������ʱ��
//#define min_time_l       4     //խ������С����ʱ��
   
#define		RECV_BIT_NUMBER		24		 //�趨���յ�λ��
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