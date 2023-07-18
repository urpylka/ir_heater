#include <Arduino.h>
#include <GyverPID.h>
#include <QuadDisplay.h>
#include <termistor.h>
#include <rheostat.h>

#define PIN_TEMP A4
#define PIN_REGUL A3
#define PIN_DISP 9
#define PIN_TRIAC 3
// GyverPID regulator(0.1, 0.05, 0.01, 10); // P, I, D, dt (ms) worked tested

GyverPID regulator(0.2, 0.05, 0.1, 10); // P, I, D, dt (ms)
// QuadDisplay qd(PIN_DISP); // номер пина CS

Termistor termistor(PIN_TEMP);
Rheostat rheostat(PIN_REGUL);

void flash(int N = 10)
{
    DDRB |= (1 << DDB6); // LED PB6

    for (int i = 0; i < N; i++)
    {
        PORTB |= (1 << PORTB6);  // LED PB6
        delay(100);              // Задержка 1 секунда
        PORTB &= ~(1 << PORTB6); // LED PB6
        delay(100);              // Задержка 1 секунда
    }
}

void direct_test_disp()
{
    pinMode(PIN_DISP, OUTPUT);
    // turnOffPWM work here:
    digitalWrite(PIN_DISP, HIGH);

    sendByte(PIN_DISP, 0b10011111);
    sendByte(PIN_DISP, 0b00100101);
    sendByte(PIN_DISP, 0b00001101);
    sendByte(PIN_DISP, 0b10011001);
    latch(PIN_DISP);
}

void setup()
{
    CLKPR = _BV(CLKPCE); // allow to change frequency devider
    CLKPR = 0;           // 1 MHz
    // CLKPR = _BV(CLKPS0); // 8 MHz
    // CLKPR = _BV(CLKPS1) | _BV(CLKPS0); // 16 MHz


    regulator.setMode(ON_ERROR); // ON_RATE
    regulator.setDirection(NORMAL);
    regulator.setLimits(0, 255);
    regulator.setpoint = 20;

    // qd.displayClear();
    // pinMode(PIN_DISP, OUTPUT);

    // flash();

    // qd.begin();
    // qd.displayInt(123);
    // displayInt(PIN_DISP, 123);
    // displayDigits(PIN_DISP, 1, 2, 3, 4);
}

unsigned long last_time = millis();
int value, temp;
bool is_timeout;

void loop()
{
    temp = termistor.get_temp_celsius();
    regulator.input = temp;
    value = rheostat.get();

    if (rheostat.is_updated())
    {
        last_time = millis();

        displayClear(PIN_DISP);
        displayTemperature(PIN_DISP, value);
        regulator.setpoint = value;
        // delay(100);
    }

    is_timeout = millis() - last_time > 4000;

    if (is_timeout)
    {
        displayClear(PIN_DISP);
        displayTemperature(PIN_DISP, temp);
    }

    analogWrite(PIN_TRIAC, regulator.getResultTimer());
    delay(50);
}
