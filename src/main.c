#include "stm32f4xx.h"
#include "SysTickDelay.h"

#ifdef DEBUG_FLAG
	#include "stdio.h"
#endif

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


#ifdef DEBUG_FLAG
	extern void initialise_monitor_handles(void);
#endif 	

int main(void){

	initialiseSysTick();	

	#ifdef DEBUG_FLAG
		printf("Program started!!!\r\n");
	#endif

	while (1){         

		//printf("Program started!!!\r\n");
		delayMs(1000);
	}
}

