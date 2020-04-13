
#include <xc.h>
#include "Ask.h"
#include "Macro_Define.h"
#include "GlobalVariable.h"

void delay_ms(unsigned int mscnt)
{
    unsigned int i;
    unsigned int j;
    for (i=0;i<mscnt;i++)
        for(j=0;j<170;j++)
        {
        };
}

void delay_uart(void)
{
    unsigned int i;
    unsigned int j;
        for(j=0;j<125;j++)
        {
        };
}

void InitSystemClock(void)
{
    OSCCONbits.SCS = 0b00;
    OSCCONbits.SPLLEN = 0;
    OSCCONbits.IRCF = 0b1110;
    //OSCCONbits.IRCF = 0x0F;
    while(HFIOFR==0);
}

void InitGPIOS(void)
{
    LATA = 0x00;
    LATC = 0x00;
    TRISA = 0b11101111;
#ifdef TX_MODE
    TRISC = 0b11011100;
    RCSTAbits.SPEN = 0;
#endif
#ifdef RX_MODE
    TRISC = 0b11101100;
    APFCON0bits.RXDTSEL = 0;
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 0;
    RCSTAbits.SPEN = 1;
    BAUDCONbits.BRG16 = 0;
    SPBRGL = 103;
    SPBRGH = 0;
    RCSTAbits.CREN = 1;
#endif    
    OPTION_REGbits.nWPUEN = 0;
    ANSELA = 0x00;
    ANSELC = 0x00;
    LATAbits.LATA0 =  1;
    LATAbits.LATA1 =  1;
    LATCbits.LATC3 =  1;
    LATCbits.LATC4 =  1;
}

void led_init(void)
{
    make_flow_patt(1);
    make_data(0,0,0,0);
    led_display();
}

void interrupt High_isr(void)
{
	unsigned char RXCheck;
//	unsigned char i;
	if (RCIF & RCIE)
	{
		RCIE = 0;
		RCIF = 0;
		RXCheck = RCREG;
        if(OERR){OERR = 0;CREN=0;CREN=1;}
        
        if(NewF==0)
            {
                if(RXCheck == 0xAA)
                    AAcnt++;
                else 
                   {
                        if((RXCheck == 0xA5)&(AAcnt>3)) 
                            NewF = 1;
                        AAcnt = 0;
                   }
            }
        else
        {
            if((RXCheck>0x4F)&(RXCheck<0x5D)) // Because 11-state, 0x50 ~ 0x5C
                {
                    CmdValue = RXCheck-0x50;
                    NewCmdF = 1;
                }
            NewF = 0;
        }
        RCIE = 1;
	}	
}

void APA102Leds_Splash(void)
{
    if(OldState != NowState)
        switch(NowState)
            {
                case stPowerOff:led_init();delay_ms(500); break; // 0
                case stStandBy:DispStandByLight(); break; // 1
#ifdef TX_MODE
                case stStandBy_Left:DispFlowLight(200,200,0);break; // 2
                case stStandBy_Right:DispStandByLight();break;  // 3          
                case stLeft_BrakeHold:DispBrake_FlowLight(200,0,0);break; // 4
                case stLeft_BrakeReleased:DispFlowLight(200,200,0);break; // 5
                case stRight_BrakeHold:DispLEDFix_LightRed();break; // 6
                case stRight_BrakeReleased:DispStandByLight();break; // 7
                case stStandBy_BrakeHold:DispLEDFix_LightRed();break; // 8
                case stStandBy_Hazard:DispFlowLight(200,200,0);break; // 9
                case stHazard_BrakeHold:DispBrake_FlowLight(200,0,0);break; // 10
                case stHazard_BrakeReleased:DispFlowLight(200,200,0);break; // 11
#endif

#ifdef RX_MODE
                case stStandBy_Left:DispStandByLight();break;
                case stStandBy_Right:DispFlowLight(200,200,0);break;
                case stLeft_BrakeHold:DispLEDFix_LightRed();break;
                case stLeft_BrakeReleased:DispStandByLight();break;
                case stRight_BrakeHold:DispFlowLight(200,0,0);break;
                case stRight_BrakeReleased:DispFlowLight(200,200,0);break;
                case stStandBy_BrakeHold:DispLEDFix_LightRed();break;
                case stStandBy_Hazard:DispFlowLight(200,200,0);break;
                case stHazard_BrakeHold:DispFlowLight(200,0,0);break;
                case stHazard_BrakeReleased:DispFlowLight(200,200,0);break;
#endif
                default:break;
            }
}

unsigned char Bike_ButtonProcess(void)
{
    unsigned char ProcessWindF = 0;
    unsigned char RxLED_40sNoti = 0;
    
    OldState = NowState;
    
    switch(NowState)
    {
        case stPowerOff:
#ifdef RX_MODE
        NowState = Rx_RightLED_StateNoti();
        if(OldState != NowState) ProcessWindF = 1;
#endif
                    
#ifdef TX_MODE
                RFD = 0;
                if(NewKey)
                    switch(Button)
                    {
                        case KeyPower:
                            NowState = stStandBy;
                            RFD = 1;
                            delay_ms(100);
                            UART_WRSTR(0x50 + stStandBy);
                            delay_ms(100);
                            RFD = 0;
                            ProcessWindF = 1;
                            break;
                        default:break;
                    }
#endif
            break;
/**********************************************************************/
       case stStandBy: // #############<01>
#ifdef RX_MODE
            NowState = Rx_RightLED_StateNoti();
            if(OldState != NowState) ProcessWindF = 1;
#endif
#ifdef TX_MODE
            RFD = 0;
            Time40S_Complete = 0;
            if(NewKey)
                switch(Button)
                {
                    case KeyLeft: // A
                        NowState = stStandBy_Left; // 2
                        UART_WRSTR(0x50+stStandBy_Left);
                        ProcessWindF = 1;
                        break;
                    case KeyRight: // B
                        NowState = stStandBy_Right; // 3
                        UART_WRSTR(0x50+stStandBy_Right);
                        ProcessWindF = 1;
                        break;
                    case KeyBrakeHold: // C
                        NowState = stStandBy_BrakeHold; // 8
                        UART_WRSTR(0x50+stStandBy_BrakeHold);
                        ProcessWindF = 1;
                        break;
                    case KeyPower: // D
                        if(Set_Power == Pressed)
                        {
                            delay_ms(900);
                            if(Set_Power == Pressed)
                            {
                                NowState = stPowerOff; // 0
                                RFD = 1;
                                delay_ms(100);
                                UART_WRSTR(0x50 + stPowerOff);
                                delay_ms(100);
                                RFD = 0; 
                                ProcessWindF = 1;
                            }
                        }
                        break;
                    case KeyHazard: // E
                        NowState = stStandBy_Hazard; // 9
                        UART_WRSTR(0x50+stStandBy_Hazard);
                        ProcessWindF = 1;
                        break;
                    default:break;
                }
#endif
            if(NowState > stPowerOff)
                {
                    RFD = 1;
                    delay_ms(100);
                }
            break; 
/**********************************************************************/
        case stStandBy_Left: // #############<02>
#ifdef RX_MODE
            NowState = Rx_RightLED_StateNoti();
            if(OldState != NowState) ProcessWindF = 1;
#endif
#ifdef TX_MODE
            if(Time40S_Complete)
                {
                    NowState = stStandBy;
                    UART_WRSTR(0x50+stStandBy);
                    ProcessWindF = 1;
                }
            if(NewKey)
                switch(Button)
                {
                    case KeyLeft:
                        NowState = stStandBy;
                        UART_WRSTR(0x50+stStandBy);
                        ProcessWindF = 1;
                        break;
                    case KeyRight:
                        NowState = stStandBy_Right;
                        UART_WRSTR(0x50+stStandBy_Right);
                        ProcessWindF = 1;
                        break;
                    case KeyBrakeHold:
                        NowState = stLeft_BrakeHold;
                        UART_WRSTR(0x50+stLeft_BrakeHold);
                        ProcessWindF = 1;
                        break;
                    case KeyPower:
                    case KeyHazard:DispFlowLight(200,200,0);break;
                    default:break;
                }
#endif
            break;
/**********************************************************************/
        case stStandBy_Right:  // #############<03>
#ifdef RX_MODE
            NowState = Rx_RightLED_StateNoti();
            if(OldState != NowState) ProcessWindF = 1;
#endif
#ifdef TX_MODE
            RxLED_40sNoti = Wait_RightFor40s();
            if((RxLED_40sNoti == 20)||(RxLED_40sNoti == 90)) // RightKey or 40S end
                {
                    NowState = stStandBy;
                    UART_WRSTR(0x50+stStandBy);
                    ProcessWindF = 1;
                    break;
                }
            else if(RxLED_40sNoti == 10) // LeftKey
                {
                    NowState = stStandBy_Left;
                    UART_WRSTR(0x50+stStandBy_Left);
                    ProcessWindF = 1;
                    break;
                }
            else if(RxLED_40sNoti == 30) // BrakeKey
                {
                    NowState = stRight_BrakeHold;
                    UART_WRSTR(0x50+stRight_BrakeHold);
                    ProcessWindF = 1;
                }
            RxLED_40sNoti = 0;
#endif
            break;
/*****************************************************************/
        case stLeft_BrakeHold:  // #############<04>
#ifdef RX_MODE
            NowState = Rx_RightLED_StateNoti();
            if(OldState != NowState) ProcessWindF = 1;
#endif
#ifdef TX_MODE
            if(Set_Brake == UnPressed)
                {
                    NowState = stLeft_BrakeReleased;
                    UART_WRSTR(0x50+stLeft_BrakeReleased);
                    ProcessWindF = 1;
                }
#endif
            break;
/***************************************************************/
        case stLeft_BrakeReleased:  // #############<05>
#ifdef RX_MODE
            NowState = Rx_RightLED_StateNoti();
            if(OldState != NowState) ProcessWindF = 1;
#endif
#ifdef TX_MODE
            if(Time40S_Complete)
                {
                    NowState = stStandBy;
                    UART_WRSTR(0x50+stStandBy);
                    ProcessWindF = 1;
                }
            if(NewKey)
                switch(Button)
                {
                    case KeyLeft:
                        NowState = stStandBy;
                        UART_WRSTR(0x50+stStandBy);
                        ProcessWindF = 1;
                        break;
                    case KeyRight:
                    case KeyPower:
                    case KeyBrakeHold:
                    case KeyHazard:DispFlowLight(200,200,0);break;
                    default:break;
                }
#endif
            break;
/***************************************************************/
        case stRight_BrakeHold:  // #############<06>
#ifdef RX_MODE
            NowState = Rx_RightLED_StateNoti();
            if(OldState != NowState) ProcessWindF = 1;
#endif
#ifdef TX_MODE
            if(Set_Brake == UnPressed)
                {
                    NowState = stRight_BrakeReleased;
                    UART_WRSTR(0x50+stRight_BrakeReleased);
                    ProcessWindF = 1;
                }
#endif
            break;
/***************************************************************/
        case stRight_BrakeReleased:  // #############<07>
#ifdef RX_MODE
            NowState = Rx_RightLED_StateNoti();
            if(OldState != NowState) ProcessWindF = 1;
#endif
#ifdef TX_MODE
//////////            RxLED_rst = Wait_BrakeRightFor40s();
            if(Wait_BrakeRightFor40s() > 0)
            {
                NowState = stStandBy;
                UART_WRSTR(0x50+stStandBy);
                ProcessWindF = 1;
            }
#endif
            break;
/***************************************************************/
        case stStandBy_BrakeHold:  // #############<08>
#ifdef RX_MODE
            NowState = Rx_RightLED_StateNoti();
            if(OldState != NowState) ProcessWindF = 1;
#endif
#ifdef TX_MODE
            if(Set_Brake == UnPressed)
                {
                    NowState = stStandBy;
                    UART_WRSTR(0x50+stStandBy);
                    ProcessWindF = 1;
                }
#endif
            break;
/***************************************************************/
        case stStandBy_Hazard:  // #############<09>
#ifdef RX_MODE
            NowState = Rx_RightLED_StateNoti();
            if(OldState != NowState) ProcessWindF = 1;
#endif
#ifdef TX_MODE
            if(Time40S_Complete)
                {
                    NowState = stStandBy;
                    UART_WRSTR(0x50+stStandBy);
                    ProcessWindF = 1;
                }
            if(NewKey)
                switch(Button)
                {
                    case KeyLeft:
                    case KeyRight:
                        NowState = stStandBy;
                        UART_WRSTR(0x50+stStandBy);
                        ProcessWindF = 1;
                        break;
                    case KeyBrakeHold:
                        NowState = stHazard_BrakeHold;
                        UART_WRSTR(0x50+stHazard_BrakeHold);
                        ProcessWindF = 1;
                        break;
                    case KeyPower:
                    case KeyHazard:DispFlowLight(200,200,0);break;
                    default:break;
                }
#endif
            break;
/***************************************************************/
        case stHazard_BrakeHold:  // #############<10>
#ifdef RX_MODE
            NowState = Rx_RightLED_StateNoti();
            if(OldState != NowState) ProcessWindF = 1;
#endif
#ifdef TX_MODE
            if(Set_Brake == UnPressed)
                {
                    NowState = stHazard_BrakeReleased;
                    UART_WRSTR(0x50+stHazard_BrakeReleased);
                    ProcessWindF = 1;
                }
#endif
            break;
/***************************************************************/
        case stHazard_BrakeReleased:  // #############<11>
#ifdef RX_MODE
            NowState = Rx_RightLED_StateNoti();
            if(OldState != NowState) ProcessWindF = 1;
#endif
#ifdef TX_MODE
            if(Time40S_Complete)
                {
                    NowState = stStandBy;
                    UART_WRSTR(0x50+stStandBy);
                    ProcessWindF = 1;
                }
            if(NewKey)
                switch(Button)
                {
                    case KeyLeft:
                    case KeyRight:
                        NowState = stStandBy;
                        UART_WRSTR(0x50+stStandBy);
                        ProcessWindF = 1;
                        break;
                    case KeyPower:
                    case KeyBrakeHold:
                    case KeyHazard:DispFlowLight(200,200,0);break;
                    default:break;
                }
#endif
            break;
/***************************************************************/
        default:break;
            
    }
    
    Button = Key_none; // Very Importance
    
    return ProcessWindF;
}

void main(void) 
{
//    unsigned char mni;
    InitSystemClock();
    InitGPIOS();
    delay_ms(200);
    SwitchFor5V = 1;
    SCK = SPI_H;
    SDO = SPI_H;
    DispStandByLight();
    
    NowState = stPowerOff;
    OldState = stNone;
    NewKey = 0;
    Button = Key_none;
    OldButton = Key_none;
    
    Time40S_Complete = 0;

#ifdef RX_MODE
    AAcnt = 0;
    NewF = 0;
    NewCmdF = 0;
    CmdValue = 0;
    RCIE = 1;
    PEIE = 1;
    GIE = 1;
#endif
    delay_ms(1000);
    led_init();
    
    while(1)
    {
        OldButton = Button;
        
        Scan_Button();
        
        if(Bike_ButtonProcess())
            APA102Leds_Splash();
    }        
}
