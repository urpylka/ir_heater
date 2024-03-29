#ifndef rheostat_H_
#define rheostat_H_

#define MIN_TEMP 20
#define MAX_TEMP 400
#define MIN_DIF 1

#include <Arduino.h>
#include <inttypes.h>

class Rheostat
{
private:
    uint8_t pin;
    int val = 0;
    int read();

public:
    Rheostat(uint8_t _PIN_RESISTOR);
    bool is_updated();
    int get();
};
#endif
