#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "post_ultrasonic.h"
#include "UART.h"
#include "Timer.h"
#include "stm32l4xx_hal_tim.h"


char disp_obj[] = " starting POST... Place a large object \n\r";
char post_strt[] = "Press enter key for getting object distance \n\r";
char key_enter[] = "You have pressed enter.. proceeding to calculate object distance \r\n";
char echotime[] = "waiting for the echo time \n\r";

char keyp[100];
uint16_t timecount = 0;
char timedisp[100];
uint16_t start_time = 0;
uint16_t end_time = 0;
char start_end_time[100];

void post_calc_distance();


void Post_init(void){
	
  
	USART_Write(USART2, (uint8_t *)disp_obj, strlen(disp_obj));

	
	while(1){
	
	//Delay function so we don't spam the terminal
		//while(getCNT() < (timecount + 5000));
		//timecount = getCNT();
	  USART_Write(USART2, (uint8_t *)post_strt, strlen(post_strt));

		//Check for User Input While Looping
		
			char c = getChar();
			if(c == 0x0D)
					{
							USART_Write(USART2, (uint8_t *)key_enter, strlen(key_enter));
						  post_calc_distance();
						  break;
					}
			else
					{
			sprintf(keyp, "you have pressed %c, press enter please\n\r",c);
    	putString(keyp);
					}
			
			}

	
	}

	void post_calc_distance()
	{
		
	}