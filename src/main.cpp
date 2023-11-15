#include <Arduino.h>
#include <GyverPID.h>
#include <QuadDisplay.h>
#include <thermistor.h>
#include <rheostat.h>

#define PIN_THERM A4
#define PIN_REGUL A3
#define PIN_DISP 9
#define PIN_TRIAC 3

#define PERIOD_REGULATOR 500

GyverPID pid(0.5, 0, 0, PERIOD_REGULATOR); // P, I, D, dt (ms)

Thermistor thermistor(PIN_THERM);
Rheostat rheostat(PIN_REGUL);

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(50);

  pid.setMode(ON_ERROR); // ON_RATE
  pid.setDirection(NORMAL);
  pid.setLimits(0, 255);
  pid.setpoint = 20;
}

// управление через плоттер
void parsing()
{
  if (Serial.available() > 1)
  {
    char incoming = Serial.read();
    float value = Serial.parseFloat();
    switch (incoming)
    {
    case 'p':
      pid.Kp = value;
      break;
    case 'i':
      pid.Ki = value;
      break;
    case 'd':
      pid.Kd = value;
      break;
    case 's':
      pid.setpoint = value;
      break;
    }
  }
}

void loop()
{
  static unsigned long timer_changed = millis();
  static unsigned long timer_pid = millis();
  static int setpoint;

  float temp = thermistor.getTempAverage();

  // если значение реостата обновлено,
  // то обнуляем таймер отображения значения реостата,
  if (rheostat.is_updated())
  {
    timer_changed = millis();

    setpoint = rheostat.get();

    displayClear(PIN_DISP);
    displayTemperature(PIN_DISP, setpoint);
  }

  // если прошло больше 4х секунд с установки нового значения температуры,
  // то отображаем текущую температуру с датчика
  if (millis() - timer_changed > 4000)
  {
    displayClear(PIN_DISP);
    displayTemperature(PIN_DISP, (int)temp);

    pid.setpoint = setpoint;
  }

  // производим регуляцию
  if (millis() - timer_pid > PERIOD_REGULATOR)
  {
    timer_pid = millis();
    pid.input = temp;

    pid.getResult();
    analogWrite(PIN_TRIAC, pid.output);

    Serial.print("input:");
    Serial.print(pid.input);
    Serial.print(',');
    Serial.print("output:");
    Serial.print(pid.output);
    Serial.print(',');
    Serial.print("integral:");
    Serial.print(pid.integral);
    Serial.print(',');
    Serial.print("setpoint:");
    Serial.println(pid.setpoint);
  }

  // чтение новых данных из плоттера
  parsing();

  delay(50);
}
