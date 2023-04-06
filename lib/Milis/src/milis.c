/**
 * @author [Petr Oblouk]
 * @github [https://github.com/peoblouk]
 * @create date 17-05-2022 - 18:48:38
 * @modify date 17-05-2022 - 18:48:38
 * @desc [Milis function]
 */
#include "milis.h"

// Tato knihovna je pro STM8S103
volatile uint16_t miliseconds = 0; // global variable storing milis value
/* -------------------------------------------------------------------------------------------*/
// vrátí hodnotu v milisekundách
uint16_t get_milis(void)
{
    uint16_t tmp;
    TIM2_ITConfig(TIM2_IT_UPDATE, DISABLE);
    tmp = miliseconds;
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
    return tmp;
}
/* -------------------------------------------------------------------------------------------*/
// init TIM2 as source for milis interrupt (low priority)
void init_milis(void)
{
    TIM2_TimeBaseInit(TIM2_PRESCALER_128, 124); // (16MHz / 128) / 125 = 1000Hz
    TIM2_ClearFlag(TIM2_FLAG_UPDATE);
    TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
    ITC_SetSoftwarePriority(ITC_IRQ_TIM2_OVF, ITC_PRIORITYLEVEL_2); // nÍzká priorita přerušení
    TIM2_Cmd(ENABLE);
    // enableInterrupts();
}

// milis interrupt routine
INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 15)
{
    TIM2_ClearFlag(TIM2_FLAG_UPDATE);
    miliseconds++;
}

/* const Milis_module milis = {
    .init = init_milis,
    .get = get_milis,
};
 */
/* -------------------------------------------------------------------------------------------*/

/*****************************END OF FILE****/
