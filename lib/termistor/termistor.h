#ifndef termistor_H_
#define termistor_H_

#define B 3950              // B-коэффициент
#define SERIAL_R 102000     // сопротивление последовательного резистора, 102 кОм
#define THERMISTOR_R 100000 // номинальное сопротивления термистора, 100 кОм
#define NOMINAL_T 25        // номинальная температура (при которой TR = 100 кОм)

#include <Arduino.h>
#include <inttypes.h>

class Termistor
{
private:
    uint8_t pin;

public:
    Termistor(uint8_t _PIN_TERMISTOR);
    int get_temp_celsius();
};

#endif
