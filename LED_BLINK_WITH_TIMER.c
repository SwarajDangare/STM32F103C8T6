#include "stm32f10x.h"

void TIM4_IRQHandler(void);
void dMicro(int delay);
void dMilli(int delay);
int count;

void dMicro(int delay)
{
	TIM4->CR1 |= TIM_CR1_CEN;
	count = 0;
	while (count < delay)
		;

	TIM4->CR1 &= ~TIM_CR1_CEN;
}

void TIM4_IRQHandler(void)
{
	count++;
	TIM4->SR &= ~TIM_SR_UIF;
}

void dMilli(int delay)
{
	TIM4->CR1 |= TIM_CR1_CEN; // Enable Timer
	count = 0;
	while (count < (delay * 1000))
		;
	TIM4->CR1 &= ~TIM_CR1_CEN; // Disable Timer
}

int main()
{

	RCC->APB2ENR |= (1 << 3); // initialise the clock for GPIO B
	RCC->APB2ENR |= (1 << 4); // initialise the clock for GPIO C
	RCC->APB1ENR |= (1 << 2); // initialise the clock for Timer 4

	// B9 -- LED (OUTPUT)
	GPIOB->CRH &= ~(0xf << 4); // clear the bits
	GPIOB->CRH |= (3 << 4);	   // (0:0:1:1) -- General purpose output push-pull

	// C13 -- ONBOARD LED(OUTPUT)
	GPIOC->CRH &= ~(0xf << 20);
	GPIOC->CRH |= (3 << 20); // (0:0:1:1) -- General purpose output push-pull

	TIM4->PSC = 0;
	TIM4->ARR = 72;
	TIM4->CR1 |= TIM_CR1_URS;	// Only counter overflow/underflow generates an update interrupt
	TIM4->DIER |= TIM_DIER_UIE; // UIE --Update Interrupt Enable
	TIM4->EGR |= TIM_EGR_UG;	// UG -- Update Generation

	NVIC_EnableIRQ(TIM4_IRQn);

	TIM4->CR1 |= TIM_CR1_CEN;

	while (1)
	{
		dMilli(100);
		GPIOC->ODR &= ~(1 << 13); // SETS -- LED ON
		GPIOB->BSRR |= (1 << 9);

		dMilli(100);
		GPIOC->ODR |= (1 << 13); // RESETS -- LED OFF
		GPIOB->BSRR |= (1 << (9 +16));

	}
}
