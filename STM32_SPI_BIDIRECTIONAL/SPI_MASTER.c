#include "stm32f10x.h"

// FUNCTION PROTOTYPES

void ConfigPins(void);
void ConfigSpi(void);
void transfer(char transmit);
void ledsOn(void);
void ledsOff(void);
char recieve(void);
uint8_t Recive;
void delay(int rep);
void blink(void);


int main()
{
	ConfigPins();
	ConfigSpi();
	
	
	while(1)
	{
		//GPIOA->BSRR = GPIO_BSRR_BR4;
		SPI1 ->CR2 |= SPI_CR2_SSOE ;
		
		if (!(GPIOB -> IDR & GPIO_IDR_IDR1))
		{
			SPI1 -> DR = 1;
			while(!(SPI1 -> SR & SPI_SR_TXE));
			delay(10);
			while(!(SPI1 -> SR & SPI_SR_RXNE));
			Recive = SPI1 ->DR;
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
			//blink();
			delay(10);
		}
		//SPI1 ->CR2 &= ~SPI_CR2_SSOE ;
		//GPIOA->BSRR = GPIO_BSRR_BS4;
		
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

	//Master Pins

	//PA5
	GPIOA->CRL &= ~(0xf << 20); // clear the bits
	GPIOA->CRL |= (11<<20);  //CLOCK  -- (1:0:1:1) --   Alternate function output Push-pull

	//PA7
	GPIOA->CRL &= ~(0xf << 28); // clear the bits
	GPIOA-> CRL |=(11 << 28);   // MOSI -- (1:0:1:1) -- Alternate function output Push-pull

	//PA4
	GPIOA->CRL &= ~(0xf << 16); // clear the bits
	GPIOA->CRL |= (11 << 16);	// NSS -- (1:0:1:1) -- Alternate function output Push-pull


	// B1 -- PUSHBUTTON (INPUT)
	GPIOB->CRL &= ~(0xf << 4); // clear the bits
	GPIOB->CRL |= (8 << 4);	   // (1:0:0:0) -- Input with pull-up / pull-down
	GPIOB->ODR |= (1 << 1);	   // pull up the pin
	// A2 -- LED (OUTPUT)
	GPIOA->CRL &= ~(0xf << 8); // clear the bits
	GPIOA->CRL |= (3 << 8);	// (1:0:1:1) -- Alternate function output Push-pull
	// C13 -- ONBOARD LED(OUTPUT)
	GPIOC->CRH &= ~(0xf << 20);
	GPIOC->CRH |= (3 << 20);  // (1:0:1:1) -- Alternate function output Push-pull

}

void ConfigSpi()
{

	RCC -> APB2ENR |= (1<<12);  // ENABLE SPI1 CLOCK

	SPI1 -> CR1 |= (1<<3); // CLOCK FREQUENCY -- fclk / 16
	SPI1 -> CR1 |= (1<<4);
	SPI1 -> CR1 |= (1<<5);

	SPI1 ->CR1 &= ~(1<<0); //  CPHA -- Clock phase
	SPI1 ->CR1 &= ~(1<<1); //  CPOL -- Clock polarity

	SPI1 ->CR1 &= ~(1<<11);  // DFF -- Data frame format

	SPI1 ->CR1 &= ~(1<<7); // LSBFIRST -- Frame format

	SPI1 -> CR1 &= ~SPI_CR1_SSM;  // SSM -- Software slave management
	SPI1 -> CR2 |= SPI_CR2_SSOE; //  SSOE -- SS output enable
	//SPI1 -> CR1 |= SPI_CR1_SSI; // SSI -- Internal Slave Select
	SPI1 -> CR1 |= SPI_CR1_MSTR; //  MSTR -- Master selection  (MASTER)

	SPI1 ->CR1 &= ~(1<<15); //  BIDIMODE -- Bidirectional data mode enable
  SPI1 ->CR1 &= ~SPI_CR1_RXONLY; // 	RXONLY -- Receive only
	
	///SPI1->CR2 = 0;

	SPI1 -> CR1 |= (1<<6); // ENABLE SPI PERIPHERALS
	
}

void transfer(char transmit)
{
	GPIOA->BSRR = GPIO_BSRR_BR4;        // reset NSS pin
	//SPI1 ->CR2 |= SPI_CR2_SSOE;
	SPI1 ->CR1 &= ~SPI_CR1_RXONLY;
	
  SPI1 -> DR = transmit;
	while(!(SPI1 -> SR & SPI_SR_TXE));
	while(SPI1 -> SR & SPI_SR_BSY);
	
	SPI1 ->CR1 |= SPI_CR1_RXONLY;
	
	GPIOA->BSRR = GPIO_BSRR_BS4;        // set NSS pin

}


char recieve(void)
{
	//SPI1 ->CR1 &= ~(1<<14);
	
	char Rx;
	//int i=8;
	//while(i){
	//while(SPI1 -> SR & SPI_SR_BSY);
  while(!(SPI1 -> SR & SPI_SR_RXNE));
  Rx = SPI1 -> DR;
	//while(!(SPI1 -> SR & SPI_SR_RXNE));
	//while(SPI1 -> SR & SPI_SR_BSY);
	//SPI1->CR1 &= ~(1<<6); //  SPE -- SPI disable
	//i--;
	//}

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