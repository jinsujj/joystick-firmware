#include "2450addr.h"
#include "option.h"
//#include "macro.h"


void Start_Init(void);
void motorX(int tone, int duration);
void motorY(int tone, int duration);
void DelayForPlay2(unsigned short time);


#define BASE10	10000

#define TONE_BEEP		1000
#define DURATION_5SEC	5000
#define DURATION_1SEC	1000

#define C1      523     // Do
#define C1_     554
#define D1      587     // Re
#define D1_     622
#define E1      659     // Mi
#define F1      699     // Pa
#define F1_     740
#define G1      784     // Sol
#define G1_     831
#define A1      880     // La
#define A1_     932
#define B1      988     // Si
#define C2      C1*2    // Do
#define C2_     C1_*2
#define D2      D1*2    // Re
#define D2_     D1_*2
#define E2      E1*2    // Mi
#define F2      F1*2    // Pa
#define F2_     F1_*2
#define G2      G1*2    // Sol
#define G2_     G1_*2
#define A2      A1*2    // La
#define A2_     A1_*2
#define B2      B1*2    // Si

void Start_Init(void)
{
	//tout 0  (X Motor)
	rGPBCON &= ~(0x3 << 0);
	rGPBCON |= 0x1<<0;   //output

	//tout2   (Y Motor)
	rGPBCON &= ~(0x3 <<4);
	rGPBCON |= 0x1<<4;   //output

	//tout3   (Y Motor) *예비 
	rGPBCON &= ~(0x3 <<6);
	rGPBCON |= 0x1<<6;   //output

	//tout 1  (Buzzer)
	rGPBCON &= ~(0x3 <<2);
	rGPBCON |= 0x1<<2;   //off



}


void motorX(int tone, int duration)
{
	rTCFG0 = (0xff<<8)|(0); 
	rTCFG1 = (0<<20)|(3<<16); 
	
	
	rTCNTB4 = 16.113*duration; 
	rTCON &=~  (1<<22);   //Timer 4 auto reload off
	rTCON |=  (1<<21);      //Timer 4 manual update on
	rTCON &= ~(1<<21);    //Timer 4 manual update off
	rTCON |=  (1<<20);      //Timer 4 start/stop

	while(rTCNTO4 !=0) 
	{
		rGPBDAT &= ~(0x1<<0);  //Motor_x
		//rGPBDAT &= ~(0x1 <<2);  //Motor_y
		//rGPBDAT &= ~(0x1 <<3);  //pwm_pin
		DelayForPlay2(BASE10/tone);
		rGPBDAT |= (0x1<<0);
		//rGPBDAT |= (0x1 <<2);
		//rGPBDAT |= (0x1 <<3);
		DelayForPlay2(BASE10/tone);
	}
	rTCON &= ~(1<<20);
}

void motorY(int tone, int duration)
{
	rTCFG0 = (0xff<<8)|(0); 
	rTCFG1 = (0<<20)|(3<<16); 

	rTCNTB4 = 16.113*duration; 
	rTCON &=~  (1<<22);   //Timer 4 auto reload off
	rTCON |=  (1<<21);      //Timer 4 manual update on
	rTCON &= ~(1<<21);    //Timer 4 manual update off
	rTCON |=  (1<<20);      //Timer 4 start/stop
	/*
	rTCNTB3 = 16.113*duration;       //타이머 4번과 타이머 3번간의 성능 차이가 존재한다.
	rTCON &=~  (1<<19);   //Timer 3 auto reload off
	rTCON |=  (1<<17);      //Timer 3 manual update on
	rTCON &= ~(1<<17);    //Timer 3 manual update off
	rTCON |=  (1<<16);      //Timer 3 start/stop
	*/
	while(rTCNTO4 !=0) 
	{
		rGPBDAT &= ~(0x1 <<2);  //Motor Y
		DelayForPlay2(BASE10/tone);
		rGPBDAT |= (0x1 <<2);
		DelayForPlay2(BASE10/tone);
	}
	rTCON &= ~(1<<20);
}

void Buzzer(int tone, int duration)
{
	rTCFG0 = (0xff<<8)|(0); 
	rTCFG1 = (0<<20)|(3<<16); 
	
	
	rTCNTB4 = 16.113*duration; 
	rTCON &=~  (1<<22);   //Timer 4 auto reload off
	rTCON |=  (1<<21);      //Timer 4 manual update on
	rTCON &= ~(1<<21);    //Timer 4 manual update off
	rTCON |=  (1<<20);      //Timer 4 start/stop

	while(rTCNTO4 !=0) 
	{
		rGPBDAT &= ~(0x1<<1);  //Buzzer
		DelayForPlay2(BASE10/tone);
		rGPBDAT |= (0x1<<1);
		DelayForPlay2(BASE10/tone);
	}
	rTCON &= ~(1<<20);
}

void DelayForPlay2(unsigned short time)	// resolution=0.1ms
{
	/* Prescaler value : 39  */
	/* Clock Select    : 128 */
	rWTCON=(37<<8)|(3<<3);			// resolution=0.1ms
	rWTDAT=time+10;					// Using WDT
	rWTCNT=time+10;
	rWTCON|=(1<<5);

	while(rWTCNT>10);
	rWTCON = 0;
}

void JoyStick(void) 
{
	int VarRegist=0;
	int realValue=0;
	Start_Init();
	Timer_Init();
	ADC_init();
	
	Buzzer(C1,100);
	Buzzer(D1,100);

	motorX(500,100);
	motorY(500,100);

	while(1)
	{
		rADCMUX=0x9;
		VarRegist = (rADCDAT0&0x3ff);
		realValue = (int)((VarRegist)*(655.0/1024.0)+245);
	
		motorY(realValue,100);   
		//(245~900)
		Uart_Printf("VarRegist_Y =%d\n",realValue);
		

		rADCMUX=0x8;
		VarRegist = (rADCDAT0&0x3ff);  //ADCDAT 레지스터의 data 값만 extract
		realValue = (int)((VarRegist)*(860.0/1024.0)+90);   
		//(90~950)
		motorX(realValue,100);
		Uart_Printf("VarRegist_X =%d\n\n\n",realValue);
		/*
			이론상 0~1024 -> 35~1000  이므로 VarRegist*(965/1024) +35
			realValue = (int)(VarRegist*(float)(965.0/1024.0)+50);
		*/
	}


}


