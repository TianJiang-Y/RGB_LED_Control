
#include <xc.h>
#include <pic16lf1824.h>
#include "Macro_Define.h"
#include "GlobalVariable.h"

void output_spi(unsigned char vl)
{
//    unsigned char spii;
    unsigned char vl_tmp,bitv;
    vl_tmp = vl;
    bitv = 0x80;
    SCK = SPI_L;
    if((vl_tmp & 0x80) == 0x80) SDO = SPI_H;
    else SDO = SPI_L;
    SCK = SPI_H;
    NOP();NOP();

    SCK = SPI_L;
    if((vl_tmp & 0x40) == 0x40) SDO = SPI_H;
    else SDO = SPI_L;
    SCK = SPI_H;
    NOP();NOP();
    
    SCK = SPI_L;
    if((vl_tmp & 0x20) == 0x20) SDO = SPI_H;
        else SDO = SPI_L;
    SCK = SPI_H;
    NOP();NOP();
    
    SCK = SPI_L;
    if((vl_tmp & 0x10) == 0x10) SDO = SPI_H;
    else SDO = SPI_L;
    SCK = SPI_H;
    NOP();NOP();
    
    SCK = SPI_L;
    if((vl_tmp & 0x08) == 0x08) SDO = SPI_H;
    else SDO = SPI_L;
    SCK = SPI_H;
    NOP();NOP();
    
    SCK = SPI_L;
    if((vl_tmp & 0x04) == 0x04) SDO = SPI_H;
    else SDO = SPI_L;
    SCK = SPI_H;
    NOP();NOP();
    
    SCK = SPI_L;
    if((vl_tmp & 0x02) == 0x02) SDO = SPI_H;
    else SDO = SPI_L;
    SCK = SPI_H;
    NOP();NOP();
    
    SCK = SPI_L;
    if((vl_tmp & 0x01) == 0x01) SDO = SPI_H;
    else SDO = SPI_L;
    SCK = SPI_H;
}

void make_flow_patt(signed char endn)
{
    signed char brti;
    if(endn<6)
        {
            for(brti=0;brti<(LED_CNT);brti++)
                {
                    if(brti<(endn*3)) 
                        brt_patt[brti] = 25;
                    else 
                        brt_patt[brti] = 0;
                }
            brt_patt[endn*3-1] = 15;
        }
    else
    {
        for(brti=0;brti<(LED_CNT);brti++)
                {
                    if(brti>((endn-2)*3-1)) 
                        brt_patt[brti] = 30;
                    else 
                        brt_patt[brti] = 25;
                 }            
    }
}

void make_fix_patt(char fbrt)
{
    char brti;
//    char delt;
    for(brti=0;brti<(LED_CNT);brti++)
    {
        brt_patt[brti] = fbrt;
    }
}

void make_data(unsigned char brt,unsigned char rcl,unsigned char gcl, unsigned char bcl)
{
    unsigned char mki;
    
    led_data[0] = 0;
    led_data[1] = 0;
    led_data[2] = 0;
    led_data[3] = 0;
    
    for(mki = 0; mki < LED_CNT ; mki++)
        {
            if(mki>0) 
                led_data[4 + mki*4] = 0xE0 + (0x1f & brt_patt[mki]);
            else 
                led_data[4 + mki*4] = 0xE0 + (0x1f & brt);

            if(brt_patt[mki]==25)
                {
                    led_data[5 + mki*4] = bcl/3*2;
                    led_data[6 + mki*4] = gcl/3*2;
                    led_data[7 + mki*4] = rcl/3*2;
                }
            else if(brt_patt[mki]==15)
                {
                    led_data[5 + mki*4] = bcl/3;
                    led_data[6 + mki*4] = gcl/3;
                    led_data[7 + mki*4] = rcl/3;
                }
            else
                {
                    led_data[5 + mki*4] = bcl;
                    led_data[6 + mki*4] = gcl;
                    led_data[7 + mki*4] = rcl;
                }
        }    
}

void led_display(void)
{
    unsigned char dsi;
    for(dsi = 0; dsi < (4*(LED_CNT+1)) ; dsi++) // because of start frame, added by one.
    {
        output_spi(led_data[dsi]);
    }    
}

void UART_WRCH(char TxCh)
{
//    unsigned char uti;
    RFD = 0;
    delay_uart();
    if((TxCh & 0x01) == 0x01) RFD = 1;
            else RFD = 0;
    delay_uart();

    if((TxCh & 0x02) == 0x02) RFD = 1;
            else RFD = 0;
    delay_uart();
    
    if((TxCh & 0x04) == 0x04) RFD = 1;
            else RFD = 0;
    delay_uart();
    
    if((TxCh & 0x08) == 0x08) RFD = 1;
            else RFD = 0;
    delay_uart();
    
    if((TxCh & 0x10) == 0x10) RFD = 1;
            else RFD = 0;
    delay_uart();
    
    if((TxCh & 0x20) == 0x20) RFD = 1;
            else RFD = 0;
    delay_uart();
    
    if((TxCh & 0x40) == 0x40) RFD = 1;
            else RFD = 0;
   delay_uart();
    
    if((TxCh & 0x80) == 0x80) RFD = 1;
            else RFD = 0;
    delay_uart();
    RFD = 1;
    delay_uart();
    delay_uart();
}

void UART_WRSTR(unsigned char cmd)
{
    UART_WRCH(0xAA);delay_ms(2);
    UART_WRCH(0xAA);delay_ms(2);
    UART_WRCH(0xAA);delay_ms(2);
    UART_WRCH(0xAA);delay_ms(2);
    UART_WRCH(0xAA);delay_ms(2);
    UART_WRCH(0xAA);delay_ms(2);
    UART_WRCH(0xAA);delay_ms(2);
    UART_WRCH(0xAA);delay_ms(2);
    UART_WRCH(0xAA);delay_ms(2);
    UART_WRCH(0xAA);delay_ms(2);
    UART_WRCH(0xAA);delay_ms(2);
    UART_WRCH(0xAA);delay_ms(2);
    UART_WRCH(0xAA);delay_ms(2);
    UART_WRCH(0xA5);delay_ms(2);
    UART_WRCH(cmd);delay_ms(20);
}


#ifdef TX_MODE
unsigned char DispFlowLight(unsigned char colrR, unsigned char colrG, unsigned char colrB)
{
    unsigned char dsptj, dspti;
    unsigned char dsptKeyPress;
    dspti = 0;    
    dsptKeyPress = 0;
    Time40S_Complete = 0;
    
    while((dspti < RunTime) && (dsptKeyPress == 0))
        {
            dsptj = 1;
            while((dsptj<20) && (dsptKeyPress == 0))
                {
                    if(dsptj<6)
                    {    
                        make_flow_patt(dsptj);
                        make_data(30,colrR,colrG,colrB);
                        led_display();
                    }
                    else if(dsptj<9)
                    {
                        make_flow_patt(6);
                        make_data(30,colrR,colrG,colrB);
                        led_display();
                    }
                    else if(dsptj==9)
                    {
                        make_flow_patt(5);
                        make_data(30,colrR,colrG,colrB);
                        led_display();
                    }
                    else if(dsptj > 9)
                    {
                        make_flow_patt(5);
                        make_data(0,1,1,1);
                        led_display();
                    }
                    delay_ms(26);
//////                    delay_uart();
                    dsptj++;

                    if( dspti > 1 )
                        {
                            if(AnyKey == Pressed)
                                {
                                    delay_ms(40);
                                    if(AnyKey == Pressed)
                                        dsptKeyPress = 1;
                                }
                        }
                };

            dspti++;
        }
   Time40S_Complete = 1;
   return dsptKeyPress;
}

unsigned char DispBrake_FlowLight(unsigned char colrR, unsigned char colrG, unsigned char colrB)
{
    unsigned char dspBrakej, dspBrakei;
    unsigned char dspBrakeKeyPress;
    dspBrakei = 0;    
    dspBrakeKeyPress = 0;
    
    while((dspBrakei < RunTime) && (dspBrakeKeyPress == 0))
        {
            dspBrakej = 1;
            while((dspBrakej<20) && (dspBrakeKeyPress == 0))
                {
                    if(dspBrakej<6)
                    {    
                        make_flow_patt(dspBrakej);
                        make_data(30,colrR,colrG,colrB);
                        led_display();
                    }
                    else if(dspBrakej<9)
                    {
                        make_flow_patt(6);
                        make_data(30,colrR,colrG,colrB);
                        led_display();
                    }
                    else if(dspBrakej==9)
                    {
                        make_flow_patt(5);
                        make_data(30,colrR,colrG,colrB);
                        led_display();
                    }
                    else if(dspBrakej > 9)
                    {
                        make_flow_patt(5);
                        make_data(0,1,1,1);
                        led_display();
                    }
                    delay_ms(25);
//////                    delay_uart();
                    dspBrakej++;

                    if( dspBrakei > 1 )
                        {
                            if(Set_Brake == UnPressed)
                                {
                                    delay_ms(60);
                                    if(Set_Brake == UnPressed)
                                        dspBrakeKeyPress = 1;
                                }
                        }
                };

            dspBrakei++;
        }
   return dspBrakeKeyPress;
}
#endif

#ifdef RX_MODE
unsigned char DispFlowLight(unsigned char colrR, unsigned char colrG, unsigned char colrB)
{
    unsigned char dspti,dsptj;
    unsigned char dsptk;
    dspti = 0;    
    dsptk = 0;
    
    while((dspti<RunTime)&(NewCmdF==0))
    {
        dsptj = 1;
        while((dsptj<20) & (dsptk == 0))
        {
            if(dsptj<6)
            {    
                make_flow_patt(dsptj);
                make_data(30,colrR,colrG,colrB);
                led_display();
            }
            else if(dsptj<9)
            {
                make_flow_patt(6);
                make_data(30,colrR,colrG,colrB);
                led_display();
            }
            else if(dsptj==9)
            {
                make_flow_patt(5);
                make_data(30,colrR,colrG,colrB);
                led_display();
            }
            else if(dsptj > 9)
            {
                make_flow_patt(5);
                make_data(0,1,1,1);
                led_display();
            }
            delay_ms(25);
            dsptj++;     
        }
        
        dspti++;
    }
    
   return NewCmdF;
}
#endif

unsigned char Wait_RightFor40s(void)
{
    unsigned char dspti,dsptj;
    unsigned char dsptk;    
    dspti = 0;    
    dsptk = 0;    
    while((dspti < 10)&(dsptk==0))
    {
        dsptj = 0;
        while((dsptj<160) & (dsptk == 0))
        {
            delay_ms(25);
            dsptj++;            
            if( dsptj > 0 )
                {
                    if(AnyKey == Pressed)
                        {
                            delay_ms(10);
                            if(AnyKey == Pressed)
                                {
                                    delay_ms(10);                                    
                                    if(Set_Left == Pressed) {dsptk = 10;return dsptk;}
                                    else if(Set_Right == Pressed) {dsptk = 20;return dsptk;}
                                    else {dsptk = 30;return dsptk;}
                                }
                        }
                }
        };
        dspti++;
    }   
   dsptk = 90;
   return dsptk;
}

unsigned char Wait_BrakeRightFor40s(void)
{
    unsigned char dspti,dsptj;
    unsigned char dsptk;
    
    dspti = 0;    
    dsptk = 0;
    Time40S_Complete = 0;
    
    while((dspti < 10)&(dsptk==0))
    {
        dsptj = 0;
        while((dsptj<160) & (dsptk == 0))
        {
            delay_ms(25);
            dsptj++;
            
            if( dsptj > 20 )
                {
                    if(DirectionKey == Pressed)
                        {
                            delay_ms(40);
                            
                            if(DirectionKey == Pressed)
                                {
                                    dsptk = 1;
                                }
                        }
                }
        };
        dspti++;
    }
    
   Time40S_Complete = 1;
   dsptk = 2;
   return dsptk;
}

void DispRainBowLight(void)
{
    unsigned char dspti,dsptj;
    unsigned char dspr,dspg,dspb;
    dspti = 0;
    
    make_fix_patt(20);
    while(dspti<5)
    {
        dsptj = 0;
        while(dsptj<LED_CNT)
        {
            dspr = (dsptj+5) % 7;
            dspg = (dsptj+10) % 7;
            dspb = dsptj % 7;
            make_data(30,dspr*30,dspg*30,dspb*30);
            led_display();
            delay_ms(80);
            dsptj++;
        } ;       
        dspti++;
    }
}

void DispStandByLight(void)
{
   unsigned char sti;
    led_data[0] = 0;
    led_data[1] = 0;
    led_data[2] = 0;
    led_data[3] = 0;
    
    for(sti = 0; sti < LED_CNT ; sti++)
    {
        if(sti>0) 
        {   
            led_data[4 + sti*4] = 0xE0 + (0x1f & 10);
            led_data[5 + sti*4] = 0;
            led_data[6 + sti*4] = 50;
            led_data[7 + sti*4] = 50;
        }
        else 
        {
            led_data[4 + sti*4] = 0xE0 + (0x1f & 30);
            led_data[5 + sti*4] = 0;
            led_data[6 + sti*4] = 250;
            led_data[7 + sti*4] = 220;
        }
    }    
    led_display();
}

void DispSingleLED_On(void)
{
   unsigned char ssti;
    led_data[0] = 0;
    led_data[1] = 0;
    led_data[2] = 0;
    led_data[3] = 0;
    
    for(ssti = 0; ssti < LED_CNT ; ssti++)
    {
        if(ssti>0) 
        {   
            led_data[4 + ssti*4] = 0xE0;
            led_data[5 + ssti*4] = 0;
            led_data[6 + ssti*4] = 0;
            led_data[7 + ssti*4] = 0;
        }
        else 
        {
            led_data[4 + ssti*4] = 0xE0 + (0x1f & 30);
            led_data[5 + ssti*4] = 0;
            led_data[6 + ssti*4] = 250;
            led_data[7 + ssti*4] = 220;
        }
    }    
    led_display();
}

void DispLEDFix_LightRed(void)
{
    make_fix_patt(30);
    make_data(30,200,0,0);
    led_display();
}

void DispLEDFix_LightGreen(void)
{
    make_fix_patt(30);
    make_data(30,200,200,0);
    led_display();
}

void DispLEDFix_LightBlue(void)
{
    make_fix_patt(30);
    make_data(30,200,200,0);
    led_display();
}

void DispHazardLight(void)
{
    make_fix_patt(30);
    make_data(30,200,200,0);
    led_display();
}

unsigned char Rx_RightLED_StateNoti(void)
{
    unsigned char StateValue;
    
    while(NewCmdF == 0)
        {
            if(FERR == 1)
                {
                    SPEN = 0;
                    SPEN = 1;
                }
        };
    if(NewCmdF == 1)
        {
            StateValue = CmdValue;
            NewCmdF = 0;
        }
        
    return StateValue;
}


