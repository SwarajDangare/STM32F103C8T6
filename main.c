#include "stm32f10x.h" 



int main()
{

    RCC->APB2ENR |= ((1 << 3) | (1 << 0)); // initialise the clock for GPIO B
    // RCC->APB2ENR |= (1<<4);  // initialise the clock for GPIO c
    RCC->APB1ENR |= (1 << 2); // initialise the clock for 	timer 4

    // STE4 PIN B9 CONFIGURATION
    GPIOB->CRH &= ~(0xf << 4); // CLEAR THE BITS
    GPIOB->CRH |= (11 << 4);   // SET MODE TO 1:0:1:1 i.e OUTPUT AT 50MHZ
    
	  // SET PIN A9 CONFIGURATION (INPUT)
    GPIOA->CRH &= ~(0xf << 4);                 // CLEAR THE BITS
    GPIOA->CRH |= (8 << 4);                    // SET THEB REQUIRED BIT 8 = 1:0:0:0 FROM 4TH BIT
    GPIOA->ODR |= (1 << 9);                    // SET PIN A9 TO  PULL UP

	
    TIM4->PSC = 7;   // PRESCALAR = 1MHz
    TIM4->ARR = 250; // AUTO RELOAD REGISTER
    TIM4->CCR4 = 0;   // CAPTURE/COMPARE REGISTER

    TIM4->CCER |= TIM_CCER_CC4E;                        // OUTPUT POLARITY SET TO ACTIVE HIGH
    TIM4->CR1 |= TIM_CR1_ARPE;                          // CR -> CONTROL REGISTER | ARPE -> AUTO RELOAD PRELOAD REGISTER | ARPE -> SET => TIMER COUNTER (TIM_CNT) IS BUFFERED
    TIM4->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2; // OUPUT COMPARE 4 MODE SET AS PWM MODE 1
    TIM4->CCMR2 |= TIM_CCMR2_OC4PE;                     // ENABLE THE CORRESPONDING PRELOAD REGISTER

    TIM4->EGR |= TIM_EGR_UG;  // BEFORE STARTING TIMER -> INITIALIZE ALL REGISTERS
    TIM4->CR1 |= TIM_CR1_CEN; // START TIMER

    while (1)
    {

        
        for (int i = 0; i < 250; i++)
        {
            TIM4->CCR4 = i;
        }

       
        for (int j = 250; j >= 0; j--)
        {
            TIM4->CCR4 = j;
        }
    }
}
