
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MACRO_DEFINE_H
#define	MACRO_DEFINE_H

#include <xc.h>
#include <pic16lf1824.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    
/************************************************************************/
/*            Mode Setting (Left & Right Board Choice)                  */
/***********************************************************************/
    
#define TX_MODE  // Left Board, Main Precessing(Master) Board
    
//#define RX_MODE // Right Board, Slave Board

/************************************************************************/
    
    
#define LED_CNT         17
#define SCK             LATCbits.LATC0
#define SDO             LATAbits.LATA4
#define RFD             LATCbits.LATC5

#define SPI_H           0
#define SPI_L           1

#define Set_Left        PORTAbits.RA1
#define Set_Right       PORTCbits.RC3
#define Set_Brake       PORTAbits.RA0
#define Set_Power       PORTCbits.RC4

#define AnyKey          (Set_Left & Set_Right & Set_Brake)
#define DirectionKey    (Set_Left & Set_Right)
    
#define SwitchFor5V     LATCbits.LATC1

#define Pressed         0
#define UnPressed       1
#define NullButton      0xFF
    
#define RunTime         50

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* MACRO_DEFINE_H */

