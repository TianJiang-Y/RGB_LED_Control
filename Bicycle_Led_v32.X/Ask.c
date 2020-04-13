
#include <xc.h>
#include "Ask.h"

#define LED_DRV                 LATAbits.LATA5
struct state Action_state;
extern unsigned char Cnten_flag;
extern unsigned int act_period_cnt = 0;

extern unsigned char timer_4_count;
extern unsigned char timer_4_countover;

unsigned char in_bit = 0;       
unsigned char rx_start = 0;     
extern unsigned char rx_data_ok;

extern unsigned char recvbit[4];
extern unsigned char recvbitcount;
extern unsigned char intcnt;

extern unsigned char recvbyte[25];
extern unsigned char recvbytecount;
unsigned char Recv_data[5];

unsigned char in_bit_n = 0;       

unsigned char Host_id = 120;
unsigned char Dev_id = 1;

extern void delay_ms(unsigned int cnt);
extern void short_delay(unsigned char t);


///////////////////Shock Drive Function///////////////////////
void EShock_Control(unsigned int actiontime,unsigned char level)
{
    Cnten_flag = 1;
//    do{
    if(level > 0)
    {
     if(level == 100)
     {
       Shock_OUT = 1;
       delay_ms(100);
       Shock_OUT = 0;  
     }
     else
     {
       Shock_OUT = 1;
       delay_ms(level);
//       for(unsigned char i = 0;i<(100-level);i++)
//       {
//          delay_ms(1); 
//       }  
       Shock_OUT = 0;
     }
    }
//  }while(!act_period_cnt);
  act_period_cnt = 0;
  Cnten_flag = 0;
}

void led_toggle(void)
{
    LED_DRV = 0;
    delay_ms(500);
    LED_DRV = 1;
}

///////////////////ASK Process Function///////////////////////
void Ask_process()
{
  while(1)
  {
     CLRWDT();
    ProcessRecv();      //Receive Process Function  
  }
}
///////////////////Receive Function///////////////////////
void Recieve()
{
  
  in_bit_n = inport;	//inport is ASK module's data input pin
  if(in_bit == in_bit_n)	
  {
    return;
  }
  in_bit = in_bit_n;

  if(timer_4_countover)
  {//timeout error
	RecieveError();
	return;	
  }
  // capture 4 times High level , this is 1 bit data
  if((timer_4_count > min_time_l)&&(timer_4_count < max_time_l)) 
  {	//short pulse ,4-14(min_time_l and max_time_l)),is 200-700us
    if(in_bit) //high level,If current level is high,actually preview pulse is low level
    {
        recvbit[recvbitcount] = 0x00;	//short low level 
    }
    else	//low level
    {
        recvbit[recvbitcount] = 0x01;	//short high level
    }
  }
  else if((timer_4_count > min_time_h)&&(timer_4_count < max_time_h))
  {	//long pulse,16-60(min_time_h and max_time_h), is 800-3000us
    if(in_bit)
    {
	recvbit[recvbitcount] = 0x02;	 //long low level 
    }
    else
    {
	recvbit[recvbitcount] = 0x03;	//long high level
    }
  }
  else
  {//otherwise Error
	RecieveError();
	return;
  }
  timer_4_count = 0;
  timer_4_countover = 0;

 // 1527	
  recvbitcount++;
  if(recvbitcount < 2) 
  {
	return;
  }
  else
  {
    //Here,judge 0 and 1 data Bit
    if((recvbit[0] == 1)&&(recvbit[1] == 2))   //high short and low long
    {
	recvbyte[recvbytecount] = 0;
    }
    else if((recvbit[0] == 3)&&(recvbit[1] == 0))  //high long and low short
    {
	recvbyte[recvbytecount] = 1;
    }
    else
    {
	RecieveError();
	return;
    }		
  }
  recvbytecount++;	 	//Received byte increase 1
  recvbitcount = 0;		//
  if(recvbytecount < RECV_BIT_NUMBER)
  {// incomplete Receiving
    return;
  }
  recvbytecount = 0;
  timer_4_count = 0;
  rx_data_ok = 1;
}
///////////////////Receive Error Process Function///////////////////////
void RecieveError()
{
  rx_start = 0;
  rx_data_ok = 0;

  recvbitcount = 0;
  recvbytecount = 0;
	
  timer_4_count = 0;
  timer_4_countover = 0;
  
  intcnt = 0;

}
///////////////////Process Receive Function///////////////////////
void ProcessRecv()
{
  unsigned char i,j;
  unsigned char p=0;
  unsigned char temp;
  Recieve();

  if(rx_data_ok)
  {
    rx_data_ok = 0;
    for(i=0;i<RECV_BIT_NUMBER;i=i+8)
    {// combine receive byte
      temp=0;
      for(j=i;j<(i+8);j++)
      {
	   temp += (recvbyte[j]<<(7-(j-i)));
      }
      Recv_data[p++]=temp;
    }

    ProcessOut();
  }
  else 
  {	
			
  }
}


//////////////////Process RF Command////////////////////////
void ProcessOut()
{
  unsigned char Recv_Host_id;
  Recv_Host_id = Recv_data[0];
  if(((Recv_data[0] == Host_id) && ((Recv_data[1]>>4)&0x0F) == Dev_id))
  {
    Action_state.mode = (Recv_data[1]&0x0F);
    Action_state.level = Recv_data[2];    
  }
  
  switch(Action_state.mode)
    {
	case M_LED:
//        led_toggle();
          

                  break;
	case M_SOUND:
//             Beep_Control(10);
                  break;
	case M_VIBRATION:
//             Motor_Control(15,Action_state.level);
                  break;
	case M_ESHOCK:
             EShock_Control(2,Action_state.level);
             led_toggle();
                   break;
        default:
		break;
    }
}



