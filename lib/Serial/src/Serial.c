#include "stm8s.h"
#include "delay.h"

/**
 * @brief  Configure Serial monitor.
 * @param  BaudRate : Speed of serial monitor.
 * @retval None
 */
void Serial_Begin(uint32_t BaudRate)
{
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);
    UART1_Init(BaudRate, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
    UART1_Cmd(ENABLE);
    delay_ms_2(50); //! TADY MŮŽE NASTAT PROBBLÉM podle logické analýzátoru je delay(50) = 63,17 ms
    // TODO TADY JE TŘEBA VYŘEŠIT tento delay
}

/**
 * @brief  Print string to serial monitor.
 * @param  message : message to write.
 * @retval None
 */
void Serial_Print_String(char message[])
{
    char i = 0;

    while (message[i] != 0x00)
    {
        UART1_SendData8(message[i]);

        while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
            ;
        i++;
    }
}

void Serial_Print_Int(int number)
{
    char count = 0;
    char digit[5] = "";
    while (number != 0)
    {
        digit[count] = number % 10;
        count++;
        number = number / 10;
    }
    {
        UART1_SendData8(digit[count - 1] + 0x30);
        while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
            ;
        count--;
    }
}

char Serial_Read_Char()
{
    while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET)
        ;
    UART1_ClearFlag(UART1_FLAG_RXNE);

    return UART1_ReceiveData8();
}
