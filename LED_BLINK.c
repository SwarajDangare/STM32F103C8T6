
#include "stm32f10x.h" // DEVICE HEADER

void delay(int rep);

int main(void)
{
	RCC -> APB2ENR |= (1 << 4); //  IOPCEN -> INPUT OUTPUT PORT C ENABLE
	
	// C13 -- ONBOARD LED(OUTPUT)
	GPIOC->CRH &= ~(0xf << 20);
	GPIOC->CRH |= (3 << 20);  // (0:0:1:1) -- General purpose output push-pull
	

	
	while(1)
	{
		// BLINKING THE PC13 LED
		
		GPIOC->ODR &= ~(1 << 13);	// SETS -- LED ON
		delay(10);
		GPIOC->ODR |= (1 << 13);	// RESETS -- LED OFF
		delay(10);
		
	}
	

}  

void delay(int rep) // RANDOM TIME DELAY FUNCTION
{
		
	for(; rep > 0; rep--)
	{
		
		int i;
		for(i = 0; i < 500000; i++)
		{
			__NOP();
		}
	}
	
	
}
