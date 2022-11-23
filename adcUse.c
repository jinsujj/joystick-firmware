#include "2450addr.h"
#include "option.h"
//#include "macro.h"

// for GPH10
void ADC_init(void);

void ADC_init(void)
{

	rADCDLY = (50000); 
	rADCCON =(1<<14)|(39<<6)|(0<<2)|(1<<1)|(1);
	//rADCMUX=0x9; //xm
	//rADCMUX=0x8; //ym  
/*
Analog input channel select.
0000 = AIN 0
0001 = AIN 1

0110 = AIN 6 (YM)
0111 = AIN 7 (YP)
1000 = AIN8 (XM)
1001 = AIN9 (XP)
*/
}