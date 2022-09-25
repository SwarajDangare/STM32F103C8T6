#include "stm32f10x.h"


void configTimer1ch4(void);
void configPins(void);
void dMs(int Ms);
void dms(int ms);
void buttonBlink(int fadeValue);
int lastState = 0;
int mode = 0;


int main()
{
    configPins();

    configTimer1ch4();
	

 while (1)
    {

        for (int i = 1; i <= 1000; i++)
        {
					TIM1->CCR4 = i;
					dMs(10);
					
					if(!(GPIOA->IDR &(1<<9)) == 1 && lastState == 0)
					{
						mode=1;
						
					}
					lastState = !(GPIOA->IDR &(1<<9));
					
					while(mode == 1)
					{
						TIM1->CCR4 = i;
				    dMs(1000);
						TIM1->CCR4 = 1;
				    dMs(1000);
						TIM1->CCR4 = i;
				    dMs(1000);
						TIM1->CCR4 = 1;
				    dMs(1000);
						
						if(!(GPIOA->IDR &(1<<9)) == 1 && lastState == 0)
					  {
						  mode=0;
						
					  }
						lastState = !(GPIOA->IDR &(1<<9));
					}
			  }
				
				for (int i = 1000; i >= 1; i--)
        {
					TIM1->CCR4 = i;
					dMs(10);
					
					if(!(GPIOA->IDR &(1<<9)) == 1 && lastState == 0)
					{
						mode=1;
						
					}
					lastState = !(GPIOA->IDR &(1<<9));
					
					while(mode == 1)
					{
						TIM1->CCR4 = i;
				    dMs(1000);
						TIM1->CCR4 = 1;
				    dMs(1000);
						TIM1->CCR4 = i;
				    dMs(1000);
						TIM1->CCR4 = 1;
				    dMs(1000);
						
						if(!(GPIOA->IDR &(1<<9)) == 1 && lastState == 0)
					  {
						  mode=0;
					  }
						lastState = !(GPIOA->IDR &(1<<9));
					}
			  }
	}
}



void configPins(void)
{
	// INITIALIZE THE CLOCK
    RCC->APB2ENR |= (1 << 2) | (1 << 0);        // INITIALIZE THE CLOCK FOR GPIO A
   

    // STE PIN A11 CONFIGURATION(OUTPUT)
    GPIOA->CRH &= ~(0xf << 12);                 // CLEAR THE BITS
    GPIOA->CRH |= (11 << 12);                   // SET MODE TO 1:0:1:1 i.e OUTPUT AT 50MHZ

    // SET PIN A9 CONFIGURATION (INPUT)
    GPIOA->CRH &= ~(0xf << 4);                   // CLEAR THE BITS
    GPIOA->CRH |= (8 << 4);                      // SET THEB REQUIRED BIT 8 = 1:0:0:0 FROM 4TH BIT
    GPIOA->ODR |= (1 << 9);                      // SET PIN A9 TO  PULL UP
}
void configTimer1ch4(void)
{
	// INITIALIZE THE CLOCK
    RCC->APB2ENR |= (1 << 11);                  // INITIALIZE TIMER-1
	
	// TIMER CONFIGURATION
    TIM1->CCMR2 |= (1<<11);                      // ENABLE COMPARE PRELOAD
   	TIM1->CR1   |= TIM_CR1_ARPE;                 // AUTO-RELOAD PRELOAD ENABLE
    TIM1->CCMR2 |= (3<<13);                      // SET CAPTURE COMPARE PWM MODE TO 1
    TIM1->CCMR2 &= ~(1<<12); 
		TIM1->CCER  &= ~(TIM_CCER_CC4P);                // CAPTURE COMPARE ENABLE
    TIM1->CCER  |= TIM_CCER_CC4E;                // CAPTURE COMPARE ENABLE


   	TIM1->PSC = 7;                              // SET PRESCALER at 1 Mhz
    TIM1->ARR = 1000;                            // SET ARR
    TIM1->CCR4 = 0;                              // START CCR4 AT 0

	    
		TIM1->BDTR |= (1<<15);
    TIM1->EGR |= TIM_EGR_UG;                     // INITIALIZE THE REGISTERS EGR - EVENT GENARATION REGISTER ,UG-UPDATE GENARATION TO 1
    TIM1->CR1 |= TIM_CR1_CEN;                    // START TIMER
}




void buttonBlink(int fade)
{
	while(!(GPIOA->IDR &(1<<9)))        //CHECK IF PIN 9 IS PULLED LOW  !(?&1 = ?)
	{
			TIM4->CCR4 = fade;              //LED ON   
			dMs(1000);
			TIM4->CCR4 = 0;                 //LED ON   
			dMs(1000);
	}
}

void dms(int ms)
{
	TIM1->CNT = 0;
	while(TIM1->CNT < ms);
}


void dMs(int Ms)
{
	
	for(int i=0; i<Ms;i++);
	dms(1000);
}
