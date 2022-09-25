#include "stm32f10x.h"

// FUNCTION PROTOYPES

void configClock(void);
void configTimer1ch4(void);
void configPins(void);
uint16_t sample[2] = {0, 0};

void dms(int ms);
void dMs(int Ms);

int fade1 = 0;
int fade2 = 0;
int map(int fade);

int main()
{
  configClock();
  configPins();
  configTimer1ch4();

  // ADC SAMPLING RATE
  ADC1->SMPR2 |= (7 << 15); // channel 5
  ADC1->SMPR2 |= (7 << 12); // channel 4

  // SEQUENCE REGISTER FOE ADC CHANNEL
  ADC1->SQR1 |= (1 << 20);
  ADC1->SQR3 |= (5 << 0); // channel 5
  ADC1->SQR3 |= (4 << 5); // channel 4

  // DMA ENABLE
  ADC1->CR1 |= (1 << 8); // SCAN MODE
  ADC1->CR2 |= (1 << 8); // DMA ENABLE

  DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR)); // CHANNEL PERIPHERAL ADDRESS RGISTER
  DMA1_Channel1->CMAR = (uint32_t)sample;        // CHANNEL MEMORY ADDRESS RGISTER
  DMA1_Channel1->CNDTR = 2;                      // CHANNEL NUMBER DATA REGISTER
  DMA1_Channel1->CCR |= (1 << 5) | (1 << 7);     // CIRC-CIRCULAR MODE ENABLE, MINC-MEMORY INCREMENT MODE
  DMA1_Channel1->CCR |= (1 << 8) | (1 << 10);    // PSIZE- PERIPHERAL SIZE, MSIZE-MEMORY SIZE
  DMA1_Channel1->CCR |= (1 << 0);                // ENABLE DMA

  // ADC1 ENABLE FOR SECOND TIME AFTER SOME DELAY

  ADC1->CR2 |= (1 << 0) | (1 << 1);
  dMs(10);
  ADC1->CR2 |= (1 << 0);
  dMs(10);
  ADC1->CR2 |= (1 << 2);

  while (1)
  {
    fade1 = map(sample[0]);
		if(fade1<3)
		{
			TIM1->CCR3 = 0;
			continue;
		}
    TIM1->CCR3 = fade1;

    fade2 = map(sample[1]);
		if(fade2<31)
		{
			TIM1->CCR4 = 0;
			continue;
		}
    TIM1->CCR4 = fade2;
  }
}

void configClock(void)
{
  // INITIALIZE THE CLOCK
  RCC->APB2ENR |= (1 << 11);                 // INITIALIZE TIMER-1

  RCC->APB2ENR |= (1 << 2) | (1 << 0);       // INITIALIZE THE CLOCK FOR GPIO A
  
  // ADC CONFIGURATION
  RCC->CFGR |= RCC_CFGR_ADCPRE_DIV2;         // PRESCALER FOR ADC
  RCC->APB2ENR |= (1 << 9);                  // ENABLE ADC1
  RCC->AHBENR |= (1 << 0);                   // enable DMA-DIRECT MEMORY ACSESS
}
void configTimer1ch4(void)
{

  // TIMER CONFIGURATION
  TIM1->CCMR2 |= (1 << 11);                // ENABLE COMPARE PRELOAD 4
  TIM1->CCMR2 |= (1 << 3);                 // ENABLE COMPARE PRELOAD 3
  TIM1->CR1 |= TIM_CR1_ARPE;               // AUTO-RELOAD PRELOAD ENABLE
  TIM1->CCMR2 |= (3 << 13);                // SET CAPTURE COMPARE4 PWM MODE TO 1
  TIM1->CCMR2 &= ~(1 << 12);
  TIM1->CCMR2 |= (3 << 5);                 // SET CAPTURE COMPARE3 PWM MODE TO 1
  TIM1->CCMR2 &= ~(1 << 4);
  TIM1->CCER &= ~(TIM_CCER_CC4P);          // CAPTURE COMPARE4 OUTPUT POLARITY ACTIVE HIGH
  TIM1->CCER |= TIM_CCER_CC4E;             // CAPTURE COMPARE4 OUTPUT ENABLE
  TIM1->CCER &= ~(TIM_CCER_CC3P);          // CAPTURE COMPARE3 OUTPUT POLARITY ACTIVE HIGH
  TIM1->CCER |= TIM_CCER_CC3E;             // CAPTURE COMPARE3 OUTPUT ENABLE

  TIM1->BDTR |= (1 << 15);
  TIM1->EGR |= TIM_EGR_UG;                 // INITIALIZE THE REGISTERS EGR - EVENT GENARATION REGISTER ,UG-UPDATE GENARATION TO 1
  TIM1->CR1 |= TIM_CR1_CEN;                // START TIMER

  TIM1->PSC = 7;                           // SET PRESCALER at 1 Mhz
  TIM1->ARR = 1000;                        // SET ARR
  dMs(10);
  TIM1->CCR4 = 0;                          // START CCR4

  TIM1->PSC = 7;                           // SET PRESCALER at 1 Mhz
  TIM1->ARR = 1000;                        // SET ARR
  dMs(10);
  TIM1->CCR3 = 0;                          // START CCR4	AT 0
}

void configPins(void)
{

  // STE PIN A11 CONFIGURATION(OUTPUT)
  GPIOA->CRH &= ~(0xf << 12);              // CLEAR THE BITS
  GPIOA->CRH |= (11 << 12);                // SET MODE TO 1:0:1:1 i.e OUTPUT AT 50MHZ

  // SET PIN A10 CONFIGURATION (OUTPUT)
  GPIOA->CRH &= ~(0xf << 8);               // CLEAR THE BITS
  GPIOA->CRH |= (11 << 8);                 // SET MODE TO 1:0:1:1 i.e OUTPUT AT 50MHZ

  GPIOA->CRL &= ~(0xf << 20);
  GPIOA->CRL |= (2 << 22); //(INPUT)       //PIN A5 MODE TO INPUT ALT FUNTION PUSH PULL
  GPIOA->CRL &= ~(0xf << 16);
  GPIOA->CRL |= (2 << 18); //(INPUT)       //PIN A4 MODE TO INPUT ALT FUNTION PUSH PULL
}

void dms(int ms)
{
  TIM1->CNT = 0;
  while (TIM1->CNT < ms)
    ;
}

void dMs(int Ms)
{

  for (int i = 0; i < Ms; i++)
    dms(1000);
}

int map(int fade)
{
  return ((fade * 1000) / 4096);
}
