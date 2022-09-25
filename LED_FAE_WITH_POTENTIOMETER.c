#include "stm32f10x.h"
#include <stdint.h> //defines standard integer types like uint32_t

int i, k, value;
uint16_t a[2] = {0, 0};

int main()

{
    //----------------------------------------| RCC & GPIO Config Code |----------------------------------------------------
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN | RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPBEN;
    // Enabling GPIO B & Alternate Function
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN | RCC_APB1ENR_TIM2EN; // Enabling Timer 4 & Timer 2 which is on APB1 Bus

    RCC->AHBENR |= RCC_AHBENR_DMA1EN; // DMA Enable

    // RCC->CFGR |= RCC_ADCPCLK2_DIV6; // Changing Prescaler for ADC to not exceed 14Mhz //  72/6=12Mhz
    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;

    // PA5
    GPIOA->CRL &= ~((1 << 20) | (1 << 21)); // Input  (MODE5)
    GPIOA->CRL &= ~((1 << 22) | (1 << 23)); // Analog Mode  (CNF5)

    // PA7
    GPIOA->CRL &= ~((1 << 28) | (1 << 29)); // Input  (MODE)
    GPIOA->CRL &= ~((1 << 30) | (1 << 31)); // Analog Mode  (CNF)

    // GPIOA->CRL &= ~((1 << 8) | (1 << 9));
    // GPIOA->CRL &= ~((1 << 10) | (1 << 11));
    // Setting Mode of pins PB8

    // Setting Mode of pins PB9
    GPIOB->CRH |= GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1 | GPIO_CRH_CNF9_1; // Output mode, max speed 50 MHz
    GPIOB->CRH &= ~(GPIO_CRH_CNF9_0);                                    // Alternate function output Push-pull
    // Setting Mode of pins PA3
    GPIOA->CRL |= GPIO_CRL_MODE3_0 | GPIO_CRL_MODE3_1 | GPIO_CRL_CNF3_1;
    GPIOA->CRL &= ~(GPIO_CRL_CNF3_0);

    //----------------------------------------| Timer Config Code |----------------------------------------------------

    TIM4->CCER |= TIM_CCER_CC4E; // Enabling Timer ON Channel 4 [capture/compare enable register]
    TIM4->CR1 |= TIM_CR1_ARPE;   // Enabling Auto-Reload Preload

    TIM4->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2; // Enabling PWM Mode 1 in Ouput Compare Mode
    TIM4->CCMR2 |= TIM_CCMR2_OC4PE;                     // Also Used to Enable PWM
    // TIM4->CCER &= ~(TIM_CCER_CC4P);                     // output polarity active high

    TIM2->CCER |= TIM_CCER_CC4E; // Enabling Timer 2 ON Channel 4
    TIM2->CR1 |= TIM_CR1_ARPE;   // Enabling Auto-Reload Preload

    TIM2->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2;
    TIM2->CCMR2 |= TIM_CCMR2_OC4PE;
    // TIM2->CCER &= ~(TIM_CCER_CC4P);

    TIM2->PSC = 72; // 1MHz Frequency
    TIM2->ARR = 1000;
    TIM2->CCR4 = 0; // Duty Cycle 1000*0.25 // 25% Duty Cycle

    TIM4->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
    TIM4->CCMR2 |= TIM_CCMR2_OC3PE;
    TIM4->CCER &= ~(TIM_CCER_CC3P);
    // PWM Freq = Fclk/PSC/ARR
    // PWM Duty = CCR4 / ARR

    TIM4->PSC = 72; // 1MHz Frequency
    TIM4->ARR = 1000;
    TIM4->CCR4 = 0; // Duty Cycle 1000*0.25 // 25% Duty Cycle

    TIM4->EGR |= TIM_EGR_UG;
    TIM4->CR1 |= TIM_CR1_CEN;
    TIM2->EGR |= TIM_EGR_UG;
    TIM2->CR1 |= TIM_CR1_CEN;

    //----------------------------------------| ADC Config Code |----------------------------------------------------

    // Setting the sample rate in channel 5
    ADC1->SMPR2 |= (1 << 15) | (1 << 16) | (1 << 17);
    // Setting the sample rate in channel 7
    ADC1->SMPR2 |= (1 << 21) | (1 << 22) | (1 << 23);

    // Set the channel you want to convert in the Sequence Registers
    ADC1->SQR3 |= ADC_SQR3_SQ1_0 | ADC_SQR3_SQ1_2;                  // Channel 5=00101
    ADC1->SQR3 |= ADC_SQR3_SQ2_0 | ADC_SQR3_SQ2_1 | ADC_SQR3_SQ2_2; // Channel 7=00111

    ADC1->SQR1 |= 1 << 20; // Length Of 2 Sequences

    ADC1->CR1 |= ADC_CR1_SCAN; // Scan mode enabled
    ADC1->CR2 |= ADC_CR2_DMA;  // DMA mode enabled

    //----------------------------------------| DMA Config Code |----------------------------------------------------

    DMA1_Channel1->CPAR = (uint32_t)(&(ADC1->DR)); // channel x peripheral address register
    DMA1_Channel1->CMAR = (uint32_t)a;             // channel x memory address register
    DMA1_Channel1->CNDTR = 2;                      // Number of data to transfer
    // DMA1_Channel1->CCR |= (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9);
    DMA1_Channel1->CCR |= DMA_CCR_CIRC | DMA_CCR_MINC | DMA_CCR_PINC | DMA_CCR_PSIZE_0 | DMA_CCR_MSIZE_0;
    DMA1_Channel1->CCR |= DMA_CCR_PL_1 | DMA_CCR_PL_0;

    // DMA1_Channel1->CCR |= (1 << 0);
    DMA1_Channel1->CCR |= DMA_CCR_EN;
    //----------------------------------------| ------------------------ |----------------------------------------------------
    // How many channels you want to do
    // ADC1->SQR1 &= ~(ADC_SQR1_L_0 | ADC_SQR1_L_1 | ADC_SQR1_L_2 | ADC_SQR1_L_3);
    // ADC1->SQR1 |=ADC_SQR1_L_3;

    // Enable The ADC for the first time and then set it to Continuous Mode
    ADC1->CR2 |= ADC_CR2_ADON | ADC_CR2_CONT;
    delay(1);

    // Turn on ADC for the second time to actually turn it on
    ADC1->CR2 |= ADC_CR2_ADON;
    delay(1);
    // Wait for the Calibration to complete
    while (ADC1->CR2 & (1 << 2))
        ;

    //----------------------------------------| ADC Task Code |----------------------------------------------------

    while (1)
    {

        TIM4->CCR4 = (a[0] * 1000 / 4096);
        TIM2->CCR4 = (a[1] * 1000 / 4096);
    }
}
//----------------------------------------| Mapping Function|----------------------------------------------------

void delay(int m)
{
    for (i = 0; i < m; i++)
    {
        int n = 0;
        for (k = 0; k < 500; k++)
            n++;
    }
}
