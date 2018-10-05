#include "stm32f4xx.h"
#include "debug.h"
#include "SysTickDelay.h"

int main(void){

	initialiseSysTick();	

	const char s[] = "Noooo waay\n";
	/*stderr*/
	uint32_t m[] = { 2, (uint32_t)s, sizeof(s)/sizeof(char) - 1 };
	/* some interrupt ID */
	send_command(0x05, m);
	
	int counter = 0;
	counter+=10;

	while (1){          
		send_command(0x05, m);
		delayMs(1000);
	}
}

