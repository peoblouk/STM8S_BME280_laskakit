#include "stm8s.h"
#include "delay.h"
#include "LCD_I2C.h"
#include "milis.h"
#include <string.h>
#include "bme280.h"

//! Makra
// Indikační LED
#define BME280_ADDRESS 0x77 // BME280 I2C Address

//! Proměnné
uint16_t mtime_key = 0;                    // Proměnná pro millis
float actual_humidity, actual_temperature; // Proměnné pro teplotu a vlhkost

int teplomer1, teplomer2, teplomer3;

//! Uživatelské funkce
void setup(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    delay_init();                       // Incializace časovače TIM4
    init_milis();                       // Iniciaizace millis TIM2
    LCD_I2C_Init(0x27, 16, 2);          // Inicializace LCD
    LCD_I2C_Print("Ultra teploměr..."); // Úvodní obrazovka na displej
    GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_SLOW);
    delay_ms(1500);
    LCD_I2C_Clear();
}

//! Main program loop
int main(void)
{
    setup();                 // Inicializace všech periferií
    LCD_I2C_SetCursor(0, 0); // Nastavení kurzoru
    while (1)
    {

        delay_ms(1000);
        char buffer1[48];
        char buffer2[48];

        actual_humidity = BM280_ReadHumidity(BME280_ADDRESS);
        actual_temperature = BM280_ReadTemp(BME280_ADDRESS);

        // if ((get_milis() - mtime_key) > 1500) // každých 1500 ms
        // {
        // mtime_key = get_milis(); // milis now
        //}

        GPIO_WriteReverse(GPIOD, GPIO_PIN_4);

        LCD_I2C_SetCursor(0, 1); // Nastavení kurzoru
        sprintf(buffer1, "temp= %02f", actual_temperature);
        LCD_I2C_Print(buffer1);

        LCD_I2C_SetCursor(7, 1); // Nastavení kurzoru
        sprintf(buffer2, "Hum= %02f", actual_humidity);
        LCD_I2C_Print(buffer2);
    }
}
