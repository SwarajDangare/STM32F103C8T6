#include "stm32f10x.h"

int main()
{
	/*
	RCC->APB2ENR |= (1<<2);  // initialise the clock for GPIO A
	RCC->APB2ENR |= (1<<4);  // initialise the clock for GPIO C
	*/      
	          //OR
	
	RCC->APB2ENR |= (5<<2);  // initialise the clock for GPIO A AND C 
	
	
	
	// SET PIN A9 CONFIGURATION 
	GPIOA->CRH &= ~(0xf<<4);   //CLEAR THE BITS 
	GPIOA->CRH |=  (8<<4);     //SET THEB REQUIRED BIT 8 = 1:0:0:0 FROM 4TH BIT
	
             //OR
	/*
	GPIOA->CRH &=  ~((1<<4)|(1<<5));  //SET MODE i.e 00 INPUT MODE 
	GPIOA->CRH &=  ~(1<<6);           //SET 0 ON 6TH BIT      // 1:0 IN 6:7 SETS INPUT MODE TO PULL UP OR PULL DOWN
	GPIOA->CRH |=  (1<<7);            //SET 1 ON 7TH BIT
	*/
	
	GPIOA->ODR |= (1<<9);             // SET PIN A9 TO  PULL UP            
	
	
	//STE4 PIN C13 CONFIGURATION
	GPIOC->CRH |=  (3<<20);           //SET MODE TO 1:1 i.e OUTPUT AT 50MHZ
	GPIOC->CRH &=  ~((1<<22)|(1<<23));//SET THE BIT TO GENARAL PURPOSE OUTPUT
	
	
	while(1)
	{	
		if(!(GPIOA->IDR &(1<<9)))       //CHECK IF PIN 9 IS PULLED LOW  !(?&1 = ?)
		{
			GPIOC->BSRR = (1<<(13+16));   //LED ON   //AS PIN 13 IS ACTIVE LOW WE NEED TO PUT THE PIN TO LOW
			
		}
		else
		{
			GPIOC->BSRR = (1<<13);        //LED OFF
		}
	}
}
