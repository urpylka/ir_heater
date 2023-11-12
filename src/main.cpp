#include <Arduino.h>
#include <GyverPID.h>
#include <QuadDisplay.h>
#include <termistor.h>
#include <rheostat.h>

#define PIN_TEMP A4
#define PIN_REGUL A3
#define PIN_DISP 9
#define PIN_TRIAC 3
// GyverPID pid(0.1, 0.05, 0.01, 10); // P, I, D, dt (ms) worked tested

#define PERIOD_REGULATOR 500

GyverPID pid(0.2, 0.05, 0.1, PERIOD_REGULATOR); // P, I, D, dt (ms)
// QuadDisplay qd(PIN_DISP); // номер пина CS

Termistor termistor(PIN_TEMP);
Rheostat rheostat(PIN_REGUL);

// void flash(int N = 10)
// {
//     DDRB |= (1 << DDB6); // LED PB6

//     for (int i = 0; i < N; i++)
//     {
//         PORTB |= (1 << PORTB6);  // LED PB6
//         delay(100);              // Задержка 1 секунда
//         PORTB &= ~(1 << PORTB6); // LED PB6
//         delay(100);              // Задержка 1 секунда
//     }
// }

// void direct_test_disp()
// {
//     pinMode(PIN_DISP, OUTPUT);
//     // turnOffPWM work here:
//     digitalWrite(PIN_DISP, HIGH);

//     sendByte(PIN_DISP, 0b10011111);
//     sendByte(PIN_DISP, 0b00100101);
//     sendByte(PIN_DISP, 0b00001101);
//     sendByte(PIN_DISP, 0b10011001);
//     latch(PIN_DISP);
// }

void setup()
{
    // CLKPR = _BV(CLKPCE); // allow to change frequency devider
    // CLKPR = 0;           // 1 MHz
    // CLKPR = _BV(CLKPS0); // 8 MHz
    // CLKPR = _BV(CLKPS1) | _BV(CLKPS0); // 16 MHz

    Serial.begin(9600);
    Serial.setTimeout(50);

    pid.setMode(ON_ERROR); // ON_RATE
    pid.setDirection(NORMAL);
    pid.setLimits(0, 255);
    pid.setpoint = 20;


    Serial.println("input,output,integral,setpoint");

    // qd.displayClear();
    // pinMode(PIN_DISP, OUTPUT);

    // flash();

    // qd.begin();
    // qd.displayInt(123);
    // displayInt(PIN_DISP, 123);
    // displayDigits(PIN_DISP, 1, 2, 3, 4);
}

// управление через плоттер
void parsing() {
  if (Serial.available() > 1) {
    char incoming = Serial.read();
    float value = Serial.parseFloat();
    switch (incoming) {
      case 'p': pid.Kp = value; break;
      case 'i': pid.Ki = value; break;
      case 'd': pid.Kd = value; break;
      case 's': pid.setpoint = value; break;
    }
  }
}


void loop()
{
    static unsigned long timer_changed = millis();
    static unsigned long timer_pid = millis();
    static int setpoint;

    int temp = termistor.get_temp_celsius();

    // если значение реостата обновлено,
    // то обнуляем таймер отображения значения реостата,
    if (rheostat.is_updated())
    {
        timer_changed = millis();

        setpoint = rheostat.get();

        // Serial.print("setpoint: ");
        // Serial.println(setpoint);

        displayClear(PIN_DISP);
        displayTemperature(PIN_DISP, setpoint);
        // delay(100);
    }


    // если прошло больше 4х секунд с установки нового значения температуры,
    // то отображаем текущую температуру с датчика
    if (millis() - timer_changed > 4000)
    {

        // Serial.print("temp: ");
        // Serial.println(temp);

        displayClear(PIN_DISP);
        displayTemperature(PIN_DISP, temp);
    }


    // производим регуляцию
    if (millis() - timer_pid > PERIOD_REGULATOR)
    {
        timer_pid = millis();
        pid.setpoint = setpoint;
        pid.input = temp;

        pid.getResult();
        analogWrite(PIN_TRIAC, pid.output);

        Serial.print(pid.input); Serial.print(' ');
        Serial.print(pid.output); Serial.print(' ');
        Serial.print(pid.integral); Serial.print(' ');
        Serial.println(pid.setpoint);
    }

    // чтение новых данных из плоттера
    parsing();

    delay(50);
}
