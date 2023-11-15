
#include <thermistor.h>

Thermistor::Thermistor(uint8_t _pin_thermistor = A0, int _serial_resistance = 102000, int _thermistor_resistance = 100000)
{
    // GND --- термистор (NTC100 - 100k) --- A0 --- 102к --- 5V

    pin = _pin_thermistor;
    pinMode(pin, INPUT);

    // Сопротивление последовательного резистора
    SERIAL_RESISTANCE = _serial_resistance;
    // Сопротивления термистора, при номинальной температуре
    THERMISTOR_RESISTANCE = _thermistor_resistance;
}

float Thermistor::computeResistance(int v_therm)
{
    // Вычисляем сопротивление термистора
    float resistance = SERIAL_RESISTANCE / ((float)1023 / v_therm - 1);
    resistance /= (float)THERMISTOR_RESISTANCE; // (R/Ro)

    return resistance;
}

float Thermistor::computeTemp(float resistance)
{
    const float NOMINAL_TEMP = 25.0;    // Номинальная температура
    const float B_COEFFICIENT = 3950.0; // Коэффициент уравнения Стайнхарта-Харта

    // Рассчитываем температуру с использованием уравнения Стайнхарта-Харта
    float temp = log(resistance) / B_COEFFICIENT; // 1/B * ln(R/Ro)
    temp += (float)1.0 / (NOMINAL_TEMP + 273.15); // + (1/To)
    temp = (float)1.0 / temp - 273.15;            // Инвертируем и конвертируем в градусы по Цельсию

    return temp;
}

float Thermistor::getTempAverage()
{
    const int SAMPLE_AVERAGE = 20;

    int analogAverage = 0;
    for (byte i = 0; i < SAMPLE_AVERAGE; i++)
        analogAverage += analogRead(pin);
    analogAverage /= SAMPLE_AVERAGE;
    return computeTemp(computeResistance(analogAverage));
}

float Thermistor::getTemp()
{
    return computeTemp(computeResistance(analogRead(pin)));
}
