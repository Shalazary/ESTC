#include <stm32f4xx.h>

/*
 * @tasks Set SYSCLK 1/2 of maximal
 *        Flash LEDs from 1 to 3 in endless loop
 *        On button press change SYSCLK source from HSE to HSI and back 
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
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  /* Init LEDs */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Init buttons */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Turn all the leds off */
  GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);

  /* Set SYSCLK based on PLL */
  RCC_HSEConfig(RCC_HSE_ON);
  RCC_WaitForHSEStartUp();
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
  RCC_PLLCmd(DISABLE);
  RCC_PLLConfig(RCC_PLLSource_HSI,8, 84, 2,4);
  RCC_PLLCmd(ENABLE);
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

  int8_t oscillator_selection = 0, led_counter = 0;
  while (1)
  {
    /* Select PLL source */
	  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
	  {
      blink_led(GPIOD, GPIO_Pin_12, BLINK_MUL_FAST);
      blink_led(GPIOD, GPIO_Pin_13, BLINK_MUL_FAST);
		  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
		  RCC_PLLCmd(DISABLE);
	  	oscillator_selection  = (oscillator_selection + 1) % 2;
		  if(oscillator_selection)
      {
			  RCC_PLLConfig(RCC_PLLSource_HSE, 8, 168, 2, 4);
      } else
      {
        RCC_PLLConfig(RCC_PLLSource_HSI,8, 168, 2,4);
      }
		  RCC_PLLCmd(ENABLE);
		  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	  }		
    blink_led(GPIOD, GPIO_Pin_12 << led_counter | GPIO_Pin_13 << led_counter | 
                   GPIO_Pin_14 << led_counter, BLINK_MUL_FAST);
    led_counter = (led_counter + 1) % 3;
  }
}

void blink_led(GPIO_TypeDef * port, uint16_t pins, blink_mul_t multiplier)
{
  uint32_t k = 0;

  GPIO_SetBits(port, pins);
  for (k = 0; k < SWITCH_DELAY * multiplier; ++k);
  GPIO_ResetBits(port, pins);
}
