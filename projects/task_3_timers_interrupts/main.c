#include "main.h"

static int8_t leds_counter;
static int8_t leds_direction;
static int8_t led_state;
void EXTI0_IRQHandler(void)
{
   if (EXTI_GetITStatus(EXTI_Line0) != RESET)
   {
      EXTI_ClearITPendingBit(EXTI_Line0);

      leds_direction = -leds_direction;
   }
}

void TIM2_IRQHandler(void)
{
   if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
   {
      TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
      if(led_state == 1)
      {
         GPIO_ResetBits(GPIOA, GPIO_Pin_8 << leds_counter);
      } else 
      {
         GPIO_SetBits(GPIOA, GPIO_Pin_8 << leds_counter);
         leds_counter = (3 + (leds_counter - leds_direction) % 3) % 3;
      }
      led_state = !led_state;
   }
}

int main(void)
{
   leds_counter = 0;
   leds_direction = 1;
   led_state = 1;

   configure_leds();
   configure_buttons();
   configure_timers();

   while (1)
   {
   }
}
