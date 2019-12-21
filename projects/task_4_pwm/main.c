#include "main.h"
#include <math.h>

uint8_t brightness;
uint8_t interrupt_state;
uint8_t led;

/*
*   Сhange brightness from 0 to 100% with 20% step
*/
void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line0);
        if (interrupt_state) {
            interrupt_state = 0;
            brightness = (brightness + 20) % 120;
            switch (led) {
            case 1:
                TIM_SetCompare1(TIM1, (1000000 / PWM_FREQ - 1) * brightness / 100);
                break;
            case 2:
                TIM_SetCompare2(TIM1, (1000000 / PWM_FREQ - 1) * brightness / 100);
                break;
            default:
                TIM_SetCompare3(TIM1, (1000000 / PWM_FREQ - 1) * brightness / 100);
                break;
            }
        }
    }
}

/*
*   Сhange color to one from R G B
*/
void EXTI1_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
        EXTI_ClearITPendingBit(EXTI_Line1);
        if (interrupt_state) {
            interrupt_state = 0;
            led = (led + 1) % 3 + 1;
            TIM_SetCompare1(TIM1, 0);
            TIM_SetCompare2(TIM1, 0);
            TIM_SetCompare3(TIM1, 0);
            switch (led) {
            case 1:
                TIM_SetCompare1(TIM1, (1000000 / PWM_FREQ - 1) * brightness / 100);
                break;
            case 2:
                TIM_SetCompare2(TIM1, (1000000 / PWM_FREQ - 1) * brightness / 100);
                break;
            default:
                TIM_SetCompare3(TIM1, (1000000 / PWM_FREQ - 1) * brightness / 100);
                break;
            }
        }
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        interrupt_state = 1;
    }
}

int main(void)
{
    brightness = 0;
    interrupt_state = 1;
    led = 1;

    configure_timers();
    configure_interrupts();
    configure_pwm();
    configure_buttons();
    configure_leds();
    start_timers();

    TIM_SetCompare1(TIM1, 0);
    for (;;) {
    }
}
