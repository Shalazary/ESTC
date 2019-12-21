#ifndef MAIN_H__
#define MAIN_H__

#include <stm32f4xx.h>

#define PWM_FREQ 60

void TIM2_IRQHandler(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);

void configure_leds(void);
void configure_timers(void);
void configure_interrupts(void);
void configure_pwm(void);
void start_timers(void);
void configure_buttons(void);

#endif