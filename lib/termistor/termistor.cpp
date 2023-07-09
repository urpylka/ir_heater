
#include <termistor.h>

Termistor::Termistor(uint8_t _PIN_TERMISTOR)
{
    pin = _PIN_TERMISTOR;
    pinMode(pin, INPUT);
}
int Termistor::get_temp_celsius()
{
    int t = analogRead(pin);
    float tr = 1023.0 / t - 1;
    tr = SERIAL_R / tr;

    float steinhart;
    steinhart = tr / THERMISTOR_R;           // (R/Ro)
    steinhart = log(steinhart);              // ln(R/Ro)
    steinhart /= B;                          // 1/B * ln(R/Ro)
    steinhart += 1.0 / (NOMINAL_T + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;             // Invert
    steinhart -= 273.15;

    return (int)steinhart;
}
