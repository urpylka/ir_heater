#ifndef thermistor_H_
#define thermistor_H_

// https://robotclass.ru/tutorials/arduino-thermistor-100k/#:~:text=Подключение%20термистора%20к%20Ардуино,использовался%20в%20уроке%20про%20фоторезистор.

#include <Arduino.h>
#include <inttypes.h>

class Thermistor
{
private:
    uint8_t pin;
    float computeResistance(int v_therm);
    float computeTemp(float resistance);

    int SERIAL_RESISTANCE;
    int THERMISTOR_RESISTANCE;

public:
    Thermistor(uint8_t _pin_thermistor = A0, int _serial_resistance = 102000, int _thermistor_resistance = 100000);
    float getTempAverage();
    float getTemp();
};
#endif
