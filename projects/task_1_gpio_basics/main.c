#include <stm32f4xx.h>

/*
 * @brief Simple lab -- blink leds in R->B->G order, on button press
 *        LEDS blink white once for a short period of time and then
 *        blink in a reverse order G->B->R.
 */

#define SWITCH_DELAY    ((uint32_t)2000000)

typedef enum blink_mul_e
{
  BLINK_MUL_FAST = 1,
  BLINK_MUL_LONG = 3,
} blink_mul_t;

void blink_led(GPIO_TypeDef * port, uint16_t pins, blink_mul_t multiplier);

int main(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable peripheral clock for LEDs and buttons port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

  /* Init LEDs */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Turn all the leds off */
  GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);
  RCC_DeInit();
  RCC_PLLConfig(RCC_PLLSource_HSI,8, 168, 2,4);
  RCC_PLLCmd(ENABLE);
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  int8_t i = 0, oscillator_selection = 0;
  while (1)
  {
	  int state = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0);
	  if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1))
	  {
                blink_led(GPIOA, GPIO_Pin_8, BLINK_MUL_FAST);
                blink_led(GPIOA, GPIO_Pin_9, BLINK_MUL_FAST);
	  	oscillator_selection  = (oscillator_selection + 1) % 2;
		RCC_DeInit();
		if(oscillator_selection)
                {
                  RCC_HSEConfig(RCC_HSE_ON);
                  RCC_WaitForHSEStartUp();
                  RCC_PLLConfig(RCC_PLLSource_HSE, 4, 84, 2, 4);

                } else
                {
                  RCC_PLLConfig(RCC_PLLSource_HSI,8, 168, 2,4);
                }
		  RCC_PLLCmd(ENABLE);
		  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	  }		
	  i += !state;
	  if(!state)
	  {
	  blink_led(GPIOA, GPIO_Pin_8, BLINK_MUL_FAST);
	  blink_led(GPIOA, GPIO_Pin_9, BLINK_MUL_FAST);
	  }
	  if(i == 0)
	  {
	  blink_led(GPIOA, GPIO_Pin_8 , BLINK_MUL_FAST);
       	  blink_led(GPIOA, GPIO_Pin_8 | GPIO_Pin_9, BLINK_MUL_FAST);
	  blink_led(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 , BLINK_MUL_FAST);
	  } else if(i == 1)
	  {
	  blink_led(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 , BLINK_MUL_FAST);
	  blink_led(GPIOA, 0, BLINK_MUL_FAST);
	  } else if(i == 2)
	  {
	  blink_led(GPIOA, GPIO_Pin_8, BLINK_MUL_LONG);
	  blink_led(GPIOA, GPIO_Pin_9, BLINK_MUL_LONG);
	  blink_led(GPIOA, GPIO_Pin_10 , BLINK_MUL_LONG);
	  }
	  i %= 2;
  }
}

void blink_led(GPIO_TypeDef * port, uint16_t pins, blink_mul_t multiplier)
{
  uint32_t k = 0;

  GPIO_ResetBits(port, pins);
  for (k = 0; k < SWITCH_DELAY * multiplier; ++k);
  GPIO_SetBits(port, pins);
}
