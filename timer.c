#include "Timer.h"
#include "UART.h"

#include <string.h>
#include <stdio.h>
char buf[100];
char checkirq[100] = "in irq \n\r";
uint32_t myticks = 0;
volatile uint32_t startTime = 0;
volatile  uint32_t endTime = 0;

//uint32_t timegap = 0; 
//volatile uint32_t last_captured = 0; 
//volatile uint32_t signal_polarity = 0; 

/*** Timer 2 channel 1 is used as input capture - echo signal
			Timer 4 channel 1 is used as output compare PWM signal ***/



//void TIM2_IRQHandler(void){
//	USART_Write(USART2, (uint8_t *)checkirq, strlen(checkirq));
//	if(!pulsing){
//    if(startTime){
//			endTime = TIM2->CCR1;
//		}
//		else{
//			startTime = TIM2->CCR1;
//		}
//		
//		//clear interrupt
//    TIM2->SR &= ~(TIM_SR_CC1IF);
//		TIM2->SR &= ~(TIM_SR_UIF);
//		TIM2->SR &= ~(TIM_SR_CC1OF);
//		
//	}
//}
	
	
	
//	uint32_t current_captured;
//	if((TIM2->SR & TIM_SR_CC1IF) != 0)
//	{
//		current_captured = TIM2->CCR1;
//		signal_polarity = 1- signal_polarity;
//		if(signal_polarity == 0 )
//		{
//			timegap = current_captured - last_captured;
//}
//		last_captured = current_captured;
//}
//	if((TIM2->SR & TIM_SR_UIF) != 0)
//	{
//	TIM2->SR &= ~TIM_SR_UIF;
//	}
	




/* For the echo signal we use timer 2 channel 1 in  input capture mode - PA0 */

void initTIM2_echo(){ // input capture 
    //TODO Enable TIM2 Clock
    RCC->APB1ENR1 |= (RCC_APB1ENR1_TIM2EN);
    //Load Prescaler
    TIM2->PSC = 79; // we want the echo pulse for every micro seconds only, thus we keep PSC as 79
	
//    //Generate event
 TIM2->EGR |= TIM_EGR_UG;
	
	// No filters
	//TIM2->CCMR1 &= ~(TIM_CCMR1_IC1F);
    
    //Disable Timer
    TIM2->CCER &= (~TIM_CCER_CC1E);
    
    //Enable input capture
	
    TIM2->CCMR1 |= (TIM_CCMR1_CC1S & 1);
    
    //Select edge of active transition both edges , rising and falling
   
	TIM2->CCER |= TIM_CCER_CC1P | TIM_CCER_CC1NP;
		// input prescaler 
	TIM2->CCMR1 &= (~TIM_CCMR1_IC1PSC);
	
// ENABLE CAPTURE FOR CHANNEL1
	TIM2->CCER |= TIM_CCER_CC1E;
	//TIM2->DIER |= TIM_DIER_CC1IE;
	TIM2->CR1 |= TIM_CR1_CEN;
	//NVIC_SetPriority(TIM2_IRQn, 0);
	//NVIC_EnableIRQ(TIM2_IRQn);
	
	// Checking the SR and CC1IF flag, getting the rising edge as well as falling edga values 
	
	
	  
	
    
}

uint32_t getCCR1(){
    return TIM2->CCR1;
}

uint32_t getCNT(){
    //TIM2->SR &= (~TIM_SR_CC1IF);
    return TIM2->CNT;
}



/* For the trigger, we use PWM signal in output compare mode with timer 4 channel 1   - PB6*/

		
void initTIM4_trig(){
    //TODO Enable TIM3 Clock
    RCC->APB1ENR1 |= (RCC_APB1ENR1_TIM4EN);
	   TIM4->CR1 &= ~(TIM_CR1_DIR);
      
    //Disable Timer
    TIM4->CCER &= (~TIM_CCER_CC1E);
  
    //Load Prescaler
    TIM4->PSC = 79;
    //Set PWM Period
    //TIM4->ARR = 99;
    //Set duty cycle for both channels
    
    TIM4->CCMR1 &= ~(TIM_CCMR1_OC1M);
	TIM4->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	
  
    TIM4->CR1 |= TIM_CR1_ARPE;
    //Generate event
    TIM4->EGR |= TIM_EGR_UG;
    
    //Setup CC1
    //Enable output on CC1
    TIM4->CCMR1 &= (~TIM_CCMR1_CC1S);
    TIM4->CCMR1 &= (~TIM_CCMR1_OC1M);
    TIM4->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
    //Select edge of active transition
    TIM4->CCER &= (~TIM_CCER_CC1NP);
    TIM4->CCER &= (~TIM_CCER_CC1P);
       
    TIM4->CCMR1 |= TIM_CCMR1_OC1PE;  
    //enable timer
    TIM4->CCER |= (TIM_CCER_CC1E);

		TIM4->CCR1 = 1;
    
    TIM4->CR1 |= TIM_CR1_CEN;
 
}


/* Timer 3 is used to create milli and micro second delay */

void TIM3_IRQHandler(void){
		//USART_Write(USART2, (uint8_t *)enabling_interr, strlen(enabling_interr));
	   myticks ++;
	   TIM3->SR &= ~TIM_SR_UIF;
	
}

void microsec_delay(uint32_t microsec)
	{
	TIM3->CR1 |= TIM_CR1_CEN;
	myticks = 0;
	while(myticks< microsec );
	TIM3->CR1 &=  ~TIM_CR1_CEN;
}

void millisec_delay(uint32_t millisec)
	{
	TIM3->CR1 |= TIM_CR1_CEN;
	myticks = 0;
	while(myticks< (millisec * 1000) );
	TIM3->CR1 &=  ~TIM_CR1_CEN;
}

   // Delay timer is timer 2 channel 1 input capture 
// Enable the timer 2 clock 

void init_delaytim()
{
RCC->APB1ENR1 |= (RCC_APB1ENR1_TIM3EN);
	

 // Prescalar value 
	TIM3->PSC = 0;
	TIM3->ARR = 80; // SUCH THAT the time period is equal  to 1MHZ
 
    TIM3->CR1 |= TIM_CR1_URS; // THE  counter underflow and overflow only causes interrupt
	TIM3->DIER |= TIM_DIER_UIE; // enable the timer interrupt 
	 TIM3->EGR |= TIM_EGR_UG;
	
	NVIC_EnableIRQ(TIM3_IRQn);
	
	
    
    //Select edge of active transition
    TIM3->CCER |= (TIM_CCER_CC1NP);
    TIM3->CCER |= (TIM_CCER_CC1P);
    
    //Clear input prescaler
    TIM3->CCMR1 &= (~TIM_CCMR1_IC1PSC);
	
    
    //enable timer
    TIM3->CCER |= (TIM_CCER_CC1E);
    // Counter enable
    TIM3->CR1 |= TIM_CR1_CEN;
		
}




































//#include "timer.h"
//#include "UART.h"
//#include <string.h>
//#include <stdio.h>

//uint32_t risetime = 0;
//uint32_t falltime = 0;
//uint32_t myticks = 0;
//int flag = 0;
//#define MAX_POS (10.0)
//#define MIN_POS (2.0)
//char buf[100];
////extern uint8_t pulsing;
//char starttime[] = "rising edge";
//char insint[] = "it is inside interrupt";
//char fintime[] = "falling edge";


//void TIM4_IRQHandler(void)
//	{
//		
//		if(TIM4->SR && TIM_SR_CC1IF !=0)   // when  a capture occurs, CC1IF is set 
//		{
//		if(flag == 0)
//		{
//		USART_Write(USART2, (uint8_t *)insint, strlen(insint));
//		risetime = TIM4->CCR1;
//		flag = 1;
//		}
//		
//		if(flag == 1)
//		{
//		//if(TIM_SR_CC1OF !=0) // if CC1IF is already set, then CC1OF is set
//		{
//		falltime = TIM4->CCR1;
//		flag = 0;
//		}
//	}
//		
//		
//		TIM4->SR &= ~(TIM_SR_CC1IF);
//		TIM4->SR &= ~(TIM_SR_CC1OF);
//		
//	}
//}


//void initTIM4(){
//    //TODO Enable TIM2 Clock
//    RCC->APB1ENR1 |= (RCC_APB1ENR1_TIM4EN);
//    //Load Prescaler
//    TIM4->PSC = 79;  // echo clock is set in 1 microseconds as t_clk_cnt
//    //Generate event
//    TIM4->EGR |= TIM_EGR_UG;
//    
//    //Disable Timer
//    TIM4->CCER &= (~TIM_CCER_CC1E);
//    
//    //Enable input capture
//    TIM4->CCMR1 &=  ~(TIM_CCMR1_CC1S);
//		TIM4->CCMR1 |= 0x1;
//	
//    
//    //Select edge of active transition
//    TIM4->CCER |= (TIM_CCER_CC1NP); //falling edge
//    TIM4->CCER |= (TIM_CCER_CC1P);  // rising edge
//    
//	// To capture each valid transition, set the input  prescaler to be zero
//		TIM4->CCMR1 &= ~TIM_CCMR1_IC1PSC;
//		
//    //
//		TIM2->CCER |= (TIM_CCER_CC1E); // capture of the counter enabled
//		TIM4->DIER |= TIM_DIER_CC1IE; //capture of the interrupts enabled
//		
//		
//		TIM4->CCER &= (~TIM_CCER_CC1E); // enable the counter
//		TIM2->CR1 |= TIM_CR1_CEN;
//		
//		NVIC_EnableIRQ(TIM4_IRQn);
//	
//    

//}

//uint32_t getCCR1(){
//    return TIM4->CCR1;
//}

//uint32_t getCNT(){
//    TIM2->SR &= (~TIM_SR_CC1IF);
//    return TIM4->CNT;
//}


//////////////////////////////////////////////////////////

/* Timer 3 is used to create milli and micro second delay */

//void TIM3_IRQHandler(void){
//		//USART_Write(USART2, (uint8_t *)enabling_interr, strlen(enabling_interr));
//	   myticks ++;
//	   TIM3->SR &= ~TIM_SR_UIF;
//	
//}

//void microsec_delay(uint32_t microsec)
//	{
//	TIM3->CR1 |= TIM_CR1_CEN;
//	myticks = 0;
//	while(myticks< microsec );
//	TIM3->CR1 &=  ~TIM_CR1_CEN;
//}

//void millisec_delay(uint32_t millisec)
//	{
//	TIM3->CR1 |= TIM_CR1_CEN;
//	myticks = 0;
//	while(myticks< (millisec * 1000) );
//	TIM3->CR1 &=  ~TIM_CR1_CEN;
//}

//   // Delay timer is timer 2 channel 1 input capture 
//// Enable the timer 2 clock 

//void init_delaytim()
//{
//RCC->APB1ENR1 |= (RCC_APB1ENR1_TIM3EN);
//	

// // Prescalar value 
//	TIM3->PSC = 0;
//	TIM3->ARR = 80; // SUCH THAT the time period is equal  to 1MHZ
// 
//    TIM3->CR1 |= TIM_CR1_URS; // THE  counter underflow and overflow only causes interrupt
//	TIM3->DIER |= TIM_DIER_UIE; // enable the timer interrupt 
//	 TIM3->EGR |= TIM_EGR_UG;
//	
//	NVIC_EnableIRQ(TIM3_IRQn);
//	
//	
//    
//    //Select edge of active transition
//    TIM3->CCER |= (TIM_CCER_CC1NP);
//    TIM3->CCER |= (TIM_CCER_CC1P);
//    
//    //Clear input prescaler
//    TIM3->CCMR1 &= (~TIM_CCMR1_IC1PSC);
//	
//    
//    //enable timer
//    TIM3->CCER |= (TIM_CCER_CC1E);
//    // Counter enable
//    TIM3->CR1 |= TIM_CR1_CEN;
//		
//}
//////////////////////////////////////////////////////////////////////////

//// Trigger timer 2 is output compare: Ton is 10microseconds, Ton + Toff is 0.1 seconds as in the assignment
//// we link timer 2 channel 1 to pin PB2 - trigger mode !!


//	
//	
//	

//void initTIM2(){
//    //TODO Enable TIM2 Clock
//    RCC->APB1ENR1 |= (RCC_APB1ENR1_TIM2EN);
//      
//    //Disable Timer
//    TIM2->CCER &= (~TIM_CCER_CC1E);
//  
//    //Load Prescaler
//    TIM2->PSC = 7999;
//    //Set PWM Period
//    TIM2->ARR = 999;
//		TIM2->CCR1 = 1;
// 
//    TIM2->RCR = 0;
//  
//    TIM2->CR1 |= TIM_CR1_ARPE;
//    //Generate event
//    TIM2->EGR |= TIM_EGR_UG;
//    
//    //Setup CC1
//    //Enable output on CC1
//    TIM2->CCMR1 &= (~TIM_CCMR1_CC1S);
//    TIM2->CCMR1 &= (~TIM_CCMR1_OC1M);
//    TIM2->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
//    //Select edge of active transition
//    TIM2->CCER &= (~TIM_CCER_CC1NP);
//    TIM2->CCER &= (~TIM_CCER_CC1P);
//    
//    //enable timer
//    TIM2->CCER |= (TIM_CCER_CC1E);
//    TIM2->CCER |= (TIM_CCER_CC2E);   
//    TIM2->CR1 |= TIM_CR1_CEN;
// 
//}




