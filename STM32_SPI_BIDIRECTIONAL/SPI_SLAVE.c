#include "stm32f10x.h"


void ConfigSpi(void);
void ConfigPins(void);

void transfer(uint8_t transmit);
uint8_t recieve(void);

void ledsOn(void);
void ledsOff(void);

void delay(int rep);
void blink(void);

uint8_t Recive;

int main()
{
	ConfigPins();
	ConfigSpi();
	
	
	while(1)
	{
		//GPIOA->BSRR = GPIO_BSRR_BR4;
		//SPI1 ->CR2 |= SPI_CR2_SSOE ;
		
		if (!(GPIOB -> IDR & GPIO_IDR_IDR1))
		{
			while(!(SPI1 -> SR & SPI_SR_RXNE));
			Recive = SPI1 ->DR;
			delay(10);
			while(!(SPI1 -> SR & SPI_SR_TXE));
			SPI1 -> DR = 1;
			while(!(SPI1 -> SR & SPI_SR_TXE));
			while(SPI1 -> SR & SPI_SR_BSY);
			delay(10);
			//blink();
		}
		else
		{
			SPI1 -> DR = 0;
			while(!(SPI1 -> SR & SPI_SR_TXE));
			delay(10);
			while(!(SPI1 -> SR & SPI_SR_RXNE));
			Recive = SPI1 ->DR;
			while(!(SPI1 -> SR & SPI_SR_TXE));
			while(SPI1 -> SR & SPI_SR_BSY);
			delay(10);
			//blink();
		}
		
		if(Recive == 1)
		{
			ledsOn();
		}
		else 
		{
			ledsOff();
		}
		
	}
}


void ConfigPins(void)
{
	RCC->APB2ENR |= (1 << 0) | (1 << 2) | (1 << 3) | (1 << 4) ; // AFIOEN | IOPAEN | IOPBEN | IOPCEN
	
	
	// B1 -- PUSHBUTTON (INPUT)
	GPIOB->CRL &= ~(0xf << 4); // clear the bits
	GPIOB->CRL |= (8 << 4);	   // (1:0:0:0) -- Input with pull-up / pull-down
	GPIOB->ODR |= (1 << 1);	   // pull up the pin
	// A2 -- LED (OUTPUT)
	GPIOA->CRL &= ~(0xf << 8); // clear the bits
	GPIOA->CRL |= (3 << 8);	// (1:0:1:1) -- General purpose output push-pull
	// C13 -- ONBOARD LED(OUTPUT)
	GPIOC->CRH &= ~(0xf << 20);
	GPIOC->CRH |= (3 << 20);  // (1:0:1:1) -- General purpose output push-pull
	
	//Slave Pins
	
	//PA4
	GPIOA->CRL &= ~(0xf << 16); // clear the bits
	GPIOA-> CRL |=(4<<16);   // CS -- (0:1:0:0) -- Floating input
	//PA5
	GPIOA->CRL &= ~(0xf << 20); // clear the bits
	GPIOA-> CRL |=(4<<20);   // CLOCK -- (0:1:0:0) -- Floating input
	//PA6
	GPIOA->CRL &= ~(0xf << 24); // clear the bits
	GPIOA-> CRL |=(13<<24);   // MISO -- (1:0:1:1) -- Alternate function output Push-pull
	//PA7
	GPIOA->CRL &= ~(0xf << 28); // clear the bits
	GPIOA-> CRL |=(4<<28);   // MOSI -- (0:1:0:0) -- Floating input
	
}

void ConfigSpi()
{
	RCC->APB2ENR |= (1<<12);// ENABLE SPI1 CLOCK
	
	SPI1 ->CR1 &= ~(1<<0); //  CPHA -- Clock phase
	SPI1 ->CR1 &= ~(1<<1); //  CPOL -- Clock polarity

	SPI1 ->CR1 &= ~(1<<11);  // DFF -- Data frame format

	SPI1 ->CR1 &= ~(1<<7); // LSBFIRST -- Frame format
	
	SPI1 ->CR1 &= ~SPI_CR1_SSM;
	SPI1 -> CR1 &= ~SPI_CR1_MSTR; //  MSTR -- Master selection  (SLAVE)
	
	SPI1 ->CR1 &= ~(1<<15); //  BIDIMODE -- Bidirectional data mode enable
	SPI1 ->CR1 &= ~SPI_CR1_RXONLY; // 	RXONLY -- Receive only
	SPI1 ->CR1 &= SPI_CR1_SSI; 
	SPI1 ->CR2 |= SPI_CR2_SSOE ;
	SPI1 -> CR1 |= (1<<6); // ENABLE SPI PERIPHERALS
	
	
}

void transfer(uint8_t transmit)
{
	SPI1 -> CR2 |= SPI_CR2_SSOE;
	
	SPI1 -> DR = transmit;
	while((SPI1 -> SR & SPI_SR_TXE));//--
	while(SPI1 -> SR & SPI_SR_BSY);
	
}

uint8_t recieve(void)
{
	//SPI1 ->CR1 &= ~(1<<14);
	//SPI1->CR1 |= (1<<6); // SPE -- SPI enable
	uint8_t Rx;

	//while(SPI1 -> SR & SPI_SR_BSY);
  while(SPI1 -> SR & SPI_SR_RXNE);
  Rx = SPI1 -> DR;
	//while(SPI1 -> SR & SPI_SR_BSY);
	//SPI1->CR1 &= ~(1<<6); //  SPE -- SPI disable
	
  return Rx;
}

void ledsOff(void)
{
	GPIOC->ODR |= (1 << 13);	// RESETS
  GPIOA->ODR &= ~(1 << 2); // LED OFF

}

void ledsOn(void)
{
	GPIOC->ODR &= ~(1 << 13);	// RESETS
  GPIOA->ODR |= (1 << 2); // LED ON

}

void blink(void)
{
	int i =4;
	while(i--)
	{
		ledsOn();
		delay(10);
		ledsOff();
		delay(10);
	}
}
void delay(int rep) // RANDOM TIME DELAY FUNCTION
{
		
	for(; rep > 0; rep--)
	{
		
		int i;
		for(i = 0; i < 100000; i++)
		{
			__NOP();
		}
	}
}