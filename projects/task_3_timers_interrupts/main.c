#include "main.h"
/*
 * @tasks Flash LEDs with 1 sec pause
 *        By pressing the button change order (use interrupts and remove jitter)
 *
 */
#define NUMBER_OF_LEDS 3

static int8_t leds_counter;
static int8_t leds_direction;

/*
 *  Current led state
 *  1 - led is on
 *  0 - led is off
 */
static int8_t led_state;

/* When button_interapt_state = 1 enable button interrupt */
static int8_t button_interrupt_state;

void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line0);
        leds_direction = -leds_direction;
    }
}

/* Set button_interrupt_state = 1 after 0,2 sec */
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        button_interrupt_state = 1;
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        if (button_interrupt_state) {
            button_interrupt_state = 0;
            if (led_state == 0) {
                GPIO_ResetBits(GPIOA, GPIO_Pin_8 << leds_counter);
            } else {
                GPIO_SetBits(GPIOA, GPIO_Pin_8 << leds_counter);
                leds_counter = (NUMBER_OF_LEDS + (leds_counter - leds_direction) % NUMBER_OF_LEDS) % NUMBER_OF_LEDS;
            }
            led_state = !led_state;
        }
    }
}

int main(void)
{
    leds_counter = 0;
    leds_direction = 1;
    led_state = 0;
    button_interrupt_state = 0;

    configure_leds();
    configure_buttons();
    configure_timers();

    while (1) {
    }
}
