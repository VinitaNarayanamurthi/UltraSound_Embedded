#include "gpio.h"

//Port B pin 6 for echo pin setting to alternate function time.
void initGPIOB(){
    // Enable the  clock for GPIO(Port B )

    RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOBEN);

//    // Configure Pin 6 port B
    
		//Configure PB6 for GPIO (10)
		GPIOB->MODER &= ~(GPIO_MODER_MODER6);//clear bits
		GPIOB->MODER |= GPIO_MODER_MODER6_0;//set to 01

    GPIOB->MODER |= (2);


    // Alternate function register low due to Pin 6:
    GPIOB->AFR[0] &= (~GPIO_AFRL_AFRL0);
    GPIOB->AFR[0] |= (GPIO_AFRL_AFRL0 & 1);
    
    
}

void setPB6(){
	GPIOB->ODR |= GPIO_ODR_ODR_6;
}

void resetPB6(){
	GPIOB-> ODR &= ~(GPIO_ODR_ODR_6);
}


void LED_Init(void){
	
	// Enable the peripheral clock of GPIO Port	
	RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOEEN;
	
	
	//  PB2 is setPB6 as a trigger pit which is GPIO Output pin 
	
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOB->MODER = ~(3U<<(2*2));  
	GPIOB->MODER |= 1U<<(2*2);      //  Output(01)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOB->OSPEEDR &= ~(3U<<(2*2));
	GPIOB->OSPEEDR |=   3U<<(2*2);  // High speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOB->OTYPER &= ~(1U<<2);       // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOB->PUPDR   &= ~(3U<<(2*2));  // No pull-up, no pull-down
	

}











