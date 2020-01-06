#include "stm32l476xx.h"
#include "SysClock.h"

#include "UART.h"
#include "timer.h"
#include "gpio_new.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern int  flag;
extern uint32_t risetime;
extern uint32_t falltime;
int i = 0;
char keyp[100];
uint16_t timecount = 0;
char timedisp[100];
uint16_t start_time = 0;
uint16_t end_time = 0;
char start_end_time[100];
uint32_t time_diff  = 0;
int post_pass = 0;
uint32_t distance_measurement1 = 0;

int key_press_flag = 0;

uint32_t distance_check = 0;
uint32_t time_dis;
uint32_t current_captured1 =0;
uint32_t current_captured2 =0;

// To get the values from timer.c file:
extern uint32_t startTime;
extern uint32_t endTime;



char disp_obj[] = " starting POST... Place a large object \n\r";
char post_strt[] = "Press enter key for getting object distance \n\r";
char key_enter[] = "You have pressed enter.. proceeding to calculate object distance \r\n";
char echotime[] = "waiting for the echo time \n\r";
char check[] = "Led is on  \n\r";
char falledge[] = "Falling edge is found";
char objcl[] = "object not placed properly";
char single_measurement[100];

uint32_t distance_measurement[100];
char buff[100];

void Post_check();
void  display_readings();
void initialize_arrays();
int comparator(const void * p,const void * q)  
{   
    return (*((uint32_t *) p) - *((uint32_t *) q)); 
} 


int main(void){

System_Clock_Init(); // Switch System Clock = 80 MHz
UART2_Init();
initGPIOA_Echo();
initGPIOB_Trig();	
initTIM4_trig();
initTIM2_echo();
init_delaytim(); 
uint32_t mean_calculate();
	
//init_GPIO_LED();
//  initGPIOA();
//  initTIM4(); //1MHz clock frequency
	
uint16_t counter;
int i = 0;
	uint32_t lastMeasurement = 0;

	while(1) {
	
	/*** Starting the main code***/
		
	/*Step 1: Check Post */
	Post_check();
	
	/*Step 2: Single measurement */  
	 if(start_time && end_time)   // Obtained from timer.c file
	 {
		 distance_measurement1 = ((end_time - start_time)*0.343)/2;
		sprintf(single_measurement, "Distance measured in mm is   = %d\r\n", distance_measurement1);
		
		 	USART_Write(USART2, (uint8_t *)single_measurement, strlen(single_measurement));
		 
	 }
	 
	 /* Step 3: Initialize the arrays for starage*/
	 initialize_arrays();

	 
	 /* Step 4: Measure 100 readings  */
	 uint32_t readings = 0;
	 while(!key_press_flag && readings < 101)
	 { 
		 // if user presses any key, exit the measurement
		 if(USART2->ISR & USART_ISR_RXNE)
		 { 
		 key_press_flag = 1;
			 break;
		 }
		 else
		 {
			 if(  TIM2->SR &(TIM_SR_CC1IF))
					{
					startTime =TIM2->CCR1;
					}
			 if(TIM2->SR &(TIM_SR_CC1IF))
					{
					endTime = TIM2->CCR1;
					}
			 distance_check = ((end_time - start_time)*0.343)/2;
			 readings ++;
			 
			if((distance_check <= 50) || (distance_check>= 1000))// Check for outliers
			{
		    
				
				while(distance_check > 50 && distance_check <1000)  // To correct the outliers
				{
					  
				  distance_check =0;
					start_time = 0;
				 end_time = 0;
				 if(  TIM2->SR &(TIM_SR_CC1IF))
					{
					startTime =TIM2->CCR1;
					}
			   if(TIM2->SR &(TIM_SR_CC1IF))
					{
					endTime = TIM2->CCR1;
					}
					distance_check = ((end_time - start_time)*0.343)/2;
				
				
				}
				
			}
			(distance_measurement[readings] = ((end_time - start_time)*0.343)/2);
			
		   
			 start_time = 0;
			 end_time = 0;
		 	 distance_check =0;
		 }
	 }
	 
	
	 /* Step 5: Display the readings */
	 display_readings(readings);
	 
	  /*Step 6 : Sort and calulate mean */
	 qsort(distance_measurement, readings, sizeof(uint32_t), comparator);
	uint32_t mean_val = mean_calculate();
	
 }
	
}



void Post_check(void){
	
  
	USART_Write(USART2, (uint8_t *)disp_obj, strlen(disp_obj));

	
	while(!post_pass){
		
	
	 // millisec_delay(1000);
	  USART_Write(USART2, (uint8_t *)post_strt, strlen(post_strt));

		//Check for User Input While Looping
		
			char c = getChar();
			if(c == 0x0D)
					{
							USART_Write(USART2, (uint8_t *)key_enter, strlen(key_enter));
						  if(start_time && end_time) // since echo is set in t_clk_cnt having 1 micro second, distance 
								// is measured in micro seconds.
							{
								start_time = 0;
							 end_time = 0;
							 if(  TIM2->SR &(TIM_SR_CC1IF))
								{
								startTime =TIM2->CCR1;
								}
							 if(TIM2->SR &(TIM_SR_CC1IF))
								{
								endTime = TIM2->CCR1;
								}
								time_diff = end_time - start_time;
							}
							if((time_diff > 300) && (time_diff < 1000))
							{
								post_pass = 1;
							}
							
							
							else 
							{
							post_pass = 0;
							USART_Write(USART2, (uint8_t *)objcl, strlen(objcl));
							
							}								
						  
						
					}
			
			}

	
	}

	
	
	void display_readings(uint32_t count)
	{
	for (i =0; i<count; i++)
		{
		sprintf(buff, "Distance measured  = %d \r\n", distance_measurement[i]);
		
		 	USART_Write(USART2, (uint8_t *)buff, strlen(buff));
		}
	
	}

void initialize_arrays()
{
   for(int i = 0; i < 100; i++){
        distance_measurement[i] = 0;
    }
}

uint32_t mean_calculate()
{uint32_t mean =0;
for(int i=0; i<100; i++)
	{
	mean += distance_measurement[i];
	}
	mean = mean/100;
	return mean;
}