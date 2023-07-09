#include <rheostat.h>

Rheostat::Rheostat(uint8_t _PIN_RESISTOR)
{
    pin = _PIN_RESISTOR;
}
bool Rheostat::is_updated()
{
    int new_val = analogRead(pin);
    new_val = map(new_val, 0, 1023, MIN_TEMP, MAX_TEMP);

    bool res = false;
    if (abs(new_val - val) > 5)
    {
        val = new_val;
        res = true;
    }

    return res;
}
int Rheostat::get()
{
    return val;
}
