#include "bme280.h"

static int data_bme[2];

void BM280_Init(uint8_t address)
{
    I2C_DeInit();
    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
    GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);
    I2C_Init(100000, address << 1, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, CLK_GetClockFreq() / 1000000);
    I2C_Cmd(ENABLE);

    delay_ms(500);
}

// Read temperature data from LM75A
int *BM280_ReadRegister(uint8_t adress_of_slave, uint8_t command)
{
    uint8_t _address_of_slave = adress_of_slave << 1; // Přepočet na 7bit

    //! 1.
    I2C_GenerateSTART(ENABLE); // Vygeneruj start
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
        ;                                                     // Počkej na vlajku
    I2C_Send7bitAddress(_address_of_slave, I2C_DIRECTION_TX); // Pošli adresu slave
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        ;
    //! 2.
    I2C_SendData(command); // Žádost o čtení
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;
    I2C_GenerateSTOP(ENABLE); // Vygeneruj stop
    ///////////////////////////////////////////////////////////////////
    //! 3.
    I2C_GenerateSTART(ENABLE); // Vygeneruj start
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
        ;
    I2C_Send7bitAddress(_address_of_slave, I2C_DIRECTION_RX);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
        ;
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
        ;
    //! MSB
    data_bme[0] = I2C_ReceiveData(); // Ulož MSB data do proměnné
    I2C_AcknowledgeConfig(DISABLE);  // Vypni zpětnou odezvu
    I2C_GenerateSTOP(ENABLE);        // Vygeneruj stop
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
        ;
    //! LSB
    data_bme[1] = I2C_ReceiveData(); // Ulož LSB data do proměnné

    return data_bme; // TODO Musí se tu zpracovat výstupní data
}

float BM280_ReadTemp(uint8_t address)
{
    // char bme_temperature[2];
    uint32_t temp;
    float temperature;

    BM280_ReadRegister(address, BME280_REGISTER_TEMPDATA);

    temp = (data_bme[0] << 12) | (data_bme[1] << 4);

    temp >>= 4;
    temperature = (float)temp * 0.001;

    return temperature;
}

float BM280_ReadHumidity(uint8_t address)
{

    // char bme_humidity[2];
    uint32_t humi;
    float humidity;

    BM280_ReadRegister(address, BME280_REGISTER_HUMIDDATA);

    humi = (data_bme[0] << 8) | data_bme[1];

    humidity = (float)humi * 0.001;

    return humidity;
}