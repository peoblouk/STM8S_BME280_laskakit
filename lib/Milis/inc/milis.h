/**
 * @author [Petr Oblouk]
 * @github [https://github.com/peoblouk]
 * @create date 17-05-2022 - 18:48:22
 * @modify date 17-05-2022 - 18:48:22
 * @desc [Milis function]
 */

#ifndef _MILIS_H_
#define _MILIS_H_ 1

#include "stm8s.h"

/* typedef struct
{
    uint16_t (*get)(void);
    void (*init)(void);
} Milis_module;
 */

uint16_t get_milis(void);
void init_milis(void);
// extern const Milis_module milis;

#endif