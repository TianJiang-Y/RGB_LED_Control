/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef GLOBALVARIABLE_H
#define	GLOBALVARIABLE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "Macro_Define.h"

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

unsigned char led_data[70];
unsigned char brt_patt[20];
unsigned long Timer4Count = 0;
unsigned char timer_4_count = 0;
unsigned char timer_4_countover = 0;
unsigned char Cnten_flag;
unsigned int act_period_cnt;

unsigned char NewCmdF, AAcnt,NewF,CmdValue;
unsigned char FlowStateF;

unsigned char Time40S_Complete;

unsigned char NewKey;
enum tKeyCodes
{
    KeyPower,
    KeyLeft,
    KeyRight,
    KeyBrakeHold,
    KeyBrakeRelease,
    KeyHazard,
    Key_none = NullButton      
};
enum tKeyCodes    Button,OldButton;

enum BikeState
{
        stPowerOff = 0,
        stStandBy = 1,
        stStandBy_Left = 2,
        stStandBy_Right = 3,
        stLeft_BrakeHold = 4,
        stLeft_BrakeReleased = 5,
        stRight_BrakeHold = 6,
        stRight_BrakeReleased = 7,
        stStandBy_BrakeHold = 8,
        stStandBy_Hazard = 9,
        stHazard_BrakeHold = 10,
        stHazard_BrakeReleased = 11,
        stNone = 12
};
enum BikeState   NowState, OldState;

void output_spi(unsigned char vl);
void make_flow_patt(signed char endn);
void make_fix_patt(char fbrt);
void make_data(unsigned char brt,unsigned char rcl,unsigned char gcl, unsigned char bcl);
void led_display(void);
void UART_WRCH(char TxCh);
void UART_WRSTR(unsigned char cmd);

#ifdef TX_MODE
unsigned char DispFlowLight(unsigned char colrR, unsigned char colrG, unsigned char colrB);
unsigned char DispBrake_FlowLight(unsigned char colrR, unsigned char colrG, unsigned char colrB);
#endif

#ifdef RX_MODE
unsigned char DispFlowLight(unsigned char colrR, unsigned char colrG, unsigned char colrB);
#endif

unsigned char Wait_BrakeRightFor40s(void);
unsigned char Wait_RightFor40s(void);

void DispRainBowLight(void);
void DispSingleLED_On(void);

void DispStandByLight(void);
void DispLEDFix_LightRed(void);
void DispLEDFix_LightGreen(void);
void DispLEDFix_LightBlue(void);

void DispHazardLight(void);

unsigned char Rx_RightLED_StateNoti(void);

void delay_ms(unsigned int mscnt);
void delay_uart(void);

unsigned char Scan_Button(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* GLOBALVARIABLE_H */

