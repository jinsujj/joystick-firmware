#include "my_lib.h"
#include "option.h"
#include "2450addr.h"

void Main(void)
{
	int i, j;
	/* Memory Management Unit Init */ 
	MMU_Init();

	/* LED GPIO Register Init */
	Led_Port_Init();

	/* Testing LED */
	for(i=0;i<16;i++) {
		Led_Display(i);
		for(j=0;j<0x1ffff;j++);
	}

	/* UART Register Init */
	Uart_Init(115200);
	/* Testing UART */
	Uart_Send_String("\nHello ARM !!!\n\n");

	/* Srart JoyStick Control */
	JoyStick();

	for(;;); // endless loop

}
