
#include <xc.h>
#include <pic16lf1824.h>
#include "GlobalVariable.h"
#include "Macro_Define.h"

unsigned char Scan_Button(void)
{
    Button = Key_none;
    
    if(Set_Power == Pressed)
    {
        delay_ms(50);
        
        if(Set_Power == Pressed)
        {
            Button = KeyPower;
        }
    }

    if(Set_Left == Pressed)
    {
        delay_ms(50);
        if(Set_Left == Pressed)
        {
            Button = KeyLeft;
        }
    }
    
    if(Set_Right == Pressed)
    {
        delay_ms(50);
        if(Set_Right == Pressed)
        {
            Button = KeyRight;
        }
    }
    
    if(Set_Brake == Pressed)
    {
        delay_ms(30);
        if(Set_Brake == Pressed)
        {
            Button = KeyBrakeHold;
        }
    }
    
    if((Set_Right == Pressed) && (Set_Left == Pressed))
    {
        delay_ms(50);
        if((Set_Right == Pressed) && (Set_Left == Pressed))
        {
            Button = KeyHazard;
        }
    }

    NewKey = 0;
    
    if (Button != Key_none)
    {
        if ((OldButton != Button)&&(OldButton == Key_none))
            {
                NewKey = 1;
            }
    }    
    
    return NewKey;
}

