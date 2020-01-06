
#include "gpio_new.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>
char buffer[100] = "Hi in setting the port \n \r";
char buffer1[100] = "Hi in resetting the port\n \r";


 /*Port A Pin 0 is set as input capture for the echo signal - timer 2 channel 1 */

void initGPIOA_Echo(){
    // Enable the clock for GPIO(Port A )

    RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);

    // Configure Pin 0 port A as  Alternate Function mode 0 - input , 1- output, 2 - alternate
    GPIOA->MODER &= ~(GPIO_MODER_MODER0); // MODER) because of pin 0

    GPIOA->MODER |= (2);


    // Alternate function register low due to Pin 0: as we use timer 2 ch1
    GPIOA->AFR[0] &= (~GPIO_AFRL_AFRL1); // AFRL1 is due to selection of AF mode 1 check in table
    GPIOA->AFR[0] |= (GPIO_AFRL_AFRL1 & 1);
    
    
}

/* For the Trigger pin, we use PWM signal in output mode  port B pin 6(PB6) - using Tim 4 ch 1  */


void initGPIOB_Trig(){ //PB6
// GPIOB clock  initialised i

	
	 RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOBEN);
  //init PB6
  // Configure Pin 6 port B as alternate function output mode
  GPIOB->MODER &= ~(GPIO_MODER_MODER6); //MOder4 because of pin 4
// PIN 6 -> Moder 6 , which is bit 8 and 9, thus 2 is left shifted 8 times
  GPIOB->MODER |= (2 << 12); //we use alternate function mode as because we are using  timer3 channel 1 


  // Alternate function register low due to Pin 6:
  GPIOB->AFR[0] &= ~(16<<24);
  GPIOB->AFR[0] |= (2<<24);
	
	// The PWM signal is set as push pull mode which is given in OTYPER :
	GPIOB->OTYPER &= ~(1<<6);
	// No pull up , no pull down 
	GPIOB->PUPDR &= ~(3<<12);
    
}

/* LED is used as GPIO output for triggering in PB2  */

void init_GPIO_LED(){ //PB2
	
	 RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOBEN);
  //init PB2
  // Configure Pin 2 port B as  output mode
  GPIOB->MODER &= ~(GPIO_MODER_MODER2); //MOder4 because of pin 4

   
	GPIOB->MODER |= 1U<<(2*2);      //  Output(01)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOB->OSPEEDR &= ~(3U<<(2*2));
	GPIOB->OSPEEDR |=   3U<<(2*2);  // High speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOB->OTYPER &= ~(1U<<2);       // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOB->PUPDR   &= ~(3U<<(2*2));  // No pull-up, no pull-down
	
	
}

void setPB2(){
		USART_Write(USART2, (uint8_t *)buffer, strlen(buffer));
	GPIOB->ODR |= GPIO_ODR_ODR_2;
}

void resetPB2(){
	USART_Write(USART2, (uint8_t *)buffer1, strlen(buffer1));
	GPIOB-> ODR &= ~(GPIO_ODR_ODR_2);
}









//void initGPIO_Trig(){ //PA1

//    RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOBEN);

//  
//  // Configure Pin 4 port B as alternate function output mode
//  GPIOB->MODER &= ~(GPIO_MODER_MODER4); //MOder1 because of pin 1
//// PIN 1 -> Moder 1 , which is bit 2 and 3, thus 2 is left shifted 2 times so that we get 1000 for AF mode
//  GPIOB->MODER |= (2 << 2); //we use alternate function mode as because we are using  timer2 channel 1 


//  // Alternate function register low due to Pin 1:
//  GPIOB->AFR[2] &= (~GPIO_AFRL_AFRL2);
//  GPIOB->AFR[2] |= (GPIO_AFRL_AFRL2 & 1);
//	
//	// The PWM signal is set as push pull mode which is given in OTYPER :
//	GPIOB->OTYPER &= ~(1<<1);
//	// No pull up , no pull down 
//	GPIOB->PUPDR &= ~(3<<2);
//    


































//#include "gpio_new.h"


//// PA0 is the echo pin used as alternate functionn pin mode.
//void initGPIOA(){
//    // Enable the  clock for GPIO(Port A )

//    RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);

//    // Configure Pin 0 port A as  (Pg:278) : Alternate Function mode
//    GPIOA->MODER &= ~(GPIO_MODER_MODER0);

//    GPIOA->MODER |= (2);


//    // Alternate function register low due to Pin 0:
//    GPIOA->AFR[0] &= (~GPIO_AFRL_AFRL0);
//    GPIOA->AFR[0] |= (GPIO_AFRL_AFRL0 & 1);
//    
//    
//}

////Port B pin 6 for echo pin setting to alternate function time.
////void initGPIOA){
////    // Enable the  clock for GPIO(Port B )

////    RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOBEN);

//////    // Configure Pin 6 port B
////    
////		//Configure PB6 for GPIO (10)
////		GPIOB->MODER &= ~(GPIO_MODER_MODER6);//clear bits
////		GPIOB->MODER |= GPIO_MODER_MODER6_0;//set to 01

////    GPIOB->MODER |= (2);


////    // Alternate function register low due to Pin 6:
////    GPIOB->AFR[0] &= (~GPIO_AFRL_AFRL0);
////    GPIOB->AFR[0] |= (GPIO_AFRL_AFRL0 & 1);
////    
////    
////}

//////void setPB6(){
//////	GPIOB->ODR |= GPIO_ODR_ODR_6;
//////}

//////void resetPB6(){
//////	GPIOB-> ODR &= ~(GPIO_ODR_ODR_6);
//////}

//////////////////////////////////////////////////////////

//// PB2 is trig pin 
//void LED_Init(void){
//	
//	// Enable the peripheral clock of GPIO Port	
//	RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOBEN ;
//	
//	
//	//  PB2 is set  as a trigger pin which is GPIO Output pin 
//	
//	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
//	GPIOB->MODER &= ~(GPIO_MODER_MODER2); 
//	GPIOB->MODER |= GPIO_MODER_MODER2_1;     //  Output(01)
//	
////	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
////	GPIOB->OSPEEDR &= ~(3U<<(2*2));
////	GPIOB->OSPEEDR |=   3U<<(2*2);  // High speed
//	
//	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
//	GPIOB->OTYPER &= ~(1U<<2);       // Push-pull
//	
//	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
//	GPIOB->PUPDR   &= ~(3U<<(2*2));  // No pull-up, no pull-down
//	

//}


//void Trig_On(void){
//	GPIOB->ODR |= GPIO_ODR_ODR_2; 
//	//output data register 
//}

//void Trig_Off(void){
//	GPIOB->ODR &= ~GPIO_ODR_ODR_2;
//}










//#include "GPIOINP.h"

//void initGPIOB(){
//	
//	
//	// GPIO Port B is used as trigger pin which is GPIO Output:
//	 RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;  // enabling the GPIOB clock
//  // Set PB2 as output
//  GPIOB->MODER &= ~(0x03<<(2*2)) ; // resetting the pin
//  GPIOB->MODER |= (1<<4); // set as output
//	GPIOB->OTYPER &= ~(0x4); // push - pull 
//	GPIOB->PUPDR &= ~(0x30); //no pull up- no pull down
//    // Enable the  clock for GPIO(Port B )

//	
//    RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOBEN);

//    // Configure Pin 2 port B as output mode (Pg:278) 01: output mode
//    GPIOB->MODER &= ~(GPIO_MODER_MODER2);

//    GPIOB->MODER |= (1);


//    // Alternate function register low due to Pin 0:
//    GPIOA->AFR[0] &= (~GPIO_AFRL_AFRL0);
//    GPIOA->AFR[0] |= (GPIO_AFRL_AFRL0 & 1);
//    
//    
//}