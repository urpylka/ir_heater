#include <Arduino.h>
// #include <GyverPID.h>
#include <QuadDisplay.h>
// #include <termistor.h>
// #include <rheostat.h>

// #define PIN_TEMP A4
// #define PIN_REGUL A3
#define PIN_DISP 9
// #define PIN_TRIAC 3

// GyverPID regulator(0.1, 0.05, 0.01, 100); // P, I, D, dt (ms)
// QuadDisplay qd(PIN_DISP); // номер пина CS

// Termistor termistor(PIN_TEMP);
// Rheostat rheostat(PIN_REGUL);

void flash(int N=10)
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

void setup()
{
    CLKPR = _BV(CLKPCE); // allow to change frequency devider
    CLKPR = 0; // 1 MHz
    // CLKPR = _BV(CLKPS0); // 8 MHz
    // CLKPR = _BV(CLKPS1) | _BV(CLKPS0); // 16 MHz

    // regulator.setDirection(NORMAL);
    // regulator.setLimits(0, 255);
    // regulator.setpoint = 50;

    // qd.displayClear();
    // pinMode(PIN_DISP, OUTPUT);

    flash();


    pinMode(PIN_DISP, OUTPUT);
    // turnOffPWM work here:
    digitalWrite(PIN_DISP, HIGH);

    sendByte(PIN_DISP, 0b10011111);
    sendByte(PIN_DISP, 0b00100101);
    sendByte(PIN_DISP, 0b00001101);
    sendByte(PIN_DISP, 0b10011001);
    latch(PIN_DISP);

    // qd.begin();
    // qd.displayInt(123);
    // displayInt(PIN_DISP, 123);
    // displayDigits(PIN_DISP, 1, 2, 3, 4);
}

// unsigned int last_time = millis();
// int value, temp;
// bool is_timeout;

void loop()
{

    // digitalWrite(PIN_DISP, HIGH);
    // delay(500);
    // digitalWrite(PIN_DISP, LOW);
    // delay(500);

    // qd.displayInt(1);

    // qd.displayClear();
    // qd.displayInt(123);
    // displayInt(PIN_DISP, 123);
    // delay(1000);

    // qd.displayClear();

    // value = rheostat.get();
    // temp = termistor.get_temp_celsius();
    // is_timeout = millis() - last_time > 2000;

    // if (rheostat.is_updated() || !is_timeout)
    // {
    //     if (is_timeout)
    //     {
    //         last_time = millis();
    //     }

    //     qd.displayTemperatureC(value);
    //     regulator.setpoint = value;
    // }
    // else
    // {
    //     qd.displayTemperatureC(temp);
    // }

    // regulator.input = temp;

    // analogWrite(PIN_TRIAC, regulator.getResultTimer());

    // delay(100);
}
