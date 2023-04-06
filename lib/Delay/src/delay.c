/**
 * @author [Petr Oblouk]
 * @github [https://github.com/peoblouk]
 * @create date 31-05-2022 - 17:05:15
 * @modify date 31-05-2022 - 17:05:15
 * @desc [Delay Library]
 */

#include "delay.h"

void delay_init(void) // inicialization of counter
{
	TIM4_DeInit();
	TIM4_TimeBaseInit(TIM4_PRESCALER_64, 249); // 16 000 000 / (1000 * 64) = 1 ms
	TIM4_Cmd(ENABLE);
}

/* void delay_cycles(uint16_t value) // function for delay created by loop
{
	uint16_t i;
	for (i = 0; i < value; i++)
		;
}
 */

void delay_ms(uint16_t time_ms) // function for delay created by timer
{
	TIM4_SetCounter(0);
	for (uint16_t loop_count = 0; loop_count < time_ms; loop_count++)
	{
		while (TIM4_GetFlagStatus(TIM4_FLAG_UPDATE) != SET)
			; // wait until flag change
		TIM4_ClearFlag(TIM4_FLAG_UPDATE);
	}
}

void delay_us(uint16_t time_us) // function for delay created by timer
{
	TIM4_SetCounter(0);
	for (uint16_t loop_count = 0; loop_count < (time_us / 1000); loop_count++)
	{
		while (TIM4_GetFlagStatus(TIM4_FLAG_UPDATE) != SET)
			;
		TIM4_ClearFlag(TIM4_FLAG_UPDATE);
	}
}

void delay_ms_2(uint32_t time_ms_2)
{
	uint32_t clock_freq = CLK_GetClockFreq();

	for (uint32_t i = 0; i < ((clock_freq / 18 / 1000UL) * time_ms_2); i++)
	{
		nop();
	}
}

/* const Delay_Module delay = {
	.init = delay_init,
	.cycles = delay_cycles,
	.ms = delay_ms,
	.us = delay_us,
}; */