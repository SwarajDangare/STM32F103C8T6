#include "stm32f10x.h"

// FUNTION PROTOTYPE
void configPins(void);
void transmit(char send);

int main(void)
{
	configPins();

	// BAUD RATE
	USART1->BRR |= 0x2760; // CLF-72000000 at BUD RATE - 115200

	USART1->CR1 |= (1 << 13) | (1 << 3) | (1 << 2); //	UE: USART enable | TE: Transmitter enabl | RE: Receiver enable

	while (1)
	{

		// FOR TRANSMITING
		if (!(GPIOB->IDR & (1 << 1)))
		{
			// TRANSMIT 1
			transmit('1');
		}
		else
		{
			// TRANSMIT 0
			transmit('0');
		}

		// FOR RECIVING
		 if (USART1->SR & (1 << 5))
		{
			char recive = USART1->DR;

			if (recive == '1')
			{
				GPIOC->ODR &= ~(1 << 13);	// SETS
				GPIOA->ODR = (1 << (5)); // LED ON
			}
			else
			{
				GPIOC->ODR |= (1 << 13);		   // RESETS
				GPIOA->ODR = (1 << (5+13)); // LED OFF
			}
		}

		else
		{
			GPIOC->ODR |= (1 << 13);		   // RESETS
				GPIOA->ODR = (1 << (5+13)); // LED OFF
		}
	}
}

void configPins(void)
{
	RCC->APB2ENR |= (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 14); // AFIOEN | IOPAEN | IOPBEN | IOPCEN | USART1EN

	// UART PINS

	// A9 -- TX
	GPIOA->CRH &= ~(0xf << 4); // clear the bits
	GPIOA->CRH |= (11 << 4);   // (1:0:1:1) -- Alternate function output Push-pull
	// A10->RX
	GPIOA->CRH &= ~(0xf << 8); // clear the bits
	GPIOA->CRH |= (4 << 8);	   // (0:1:0:0) -- Floating input

	// B1 -- PUSHBUTTON (INPUT)
	GPIOB->CRL &= ~(0xf << 4); // clear the bits
	GPIOB->CRL |= (8 << 4);	   // (1:0:0:0) -- Input with pull-up / pull-down
	GPIOB->ODR |= (1 << 1);	   // pull up the pin
	// A5 -- LED (OUTPUT)
	GPIOA->CRL &= ~(0xf << 20); // clear the bits
	GPIOA->CRL |= (3 << 20);	// (0:0:1:1) -- General purpose output push-pull     11
	// C13 -- ONBOARD LED(OUTPUT)
	GPIOC->CRH &= ~(0xf << 20);
	GPIOC->CRH |= (3 << 20);  // (0:0:1:1) -- General purpose output push-pull
}

void transmit(char send)
{
	USART1->DR = send;
	while ((USART1->SR & (1 << 7)) == 0)
		; // check TXE flag --  TXE: Transmit data register empty
}
