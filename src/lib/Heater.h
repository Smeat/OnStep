// -----------------------------------------------------------------------------------
// Control a single dew Heater

#pragma once

#ifndef DEW_HEATER_PULSE_WIDTH_MS
  #define DEW_HEATER_PULSE_WIDTH_MS 2000
#endif

class dewHeaterControl {
  public:
    void init(int pin, int nvAddress) {
      _pin = pin;
      if (_pin >= 0 && _pin <= 255) pinMode(_pin, OUTPUT);
      _nvAddress = nvAddress;
      zero = nv.read(_nvAddress)/10.0 - 5.0;
      span = nv.read(_nvAddress + 1)/10.0 - 5.0;
    }

    void poll(float deltaAboveDewPointC) {
      if (!enabled) return;

      int switchTimeMs = 0;
      switchTimeMs = map(lround(deltaAboveDewPointC*10.0), lround(zero*10.0), lround(span*10.0), DEW_HEATER_PULSE_WIDTH_MS, 0);
      switchTimeMs = constrain(switchTimeMs, 0, DEW_HEATER_PULSE_WIDTH_MS);
#ifdef DEW_HEATER_MAX_POWER
      switchTimeMs = lround(switchTimeMs*(DEW_HEATER_MAX_POWER/100.0))
#endif
      currentTime = millis();

      if ((long)(currentTime - (lastHeaterCycle + DEW_HEATER_PULSE_WIDTH_MS)) > 0) {
        lastHeaterCycle = currentTime;
      } else
      if (!heaterOn && (long)(currentTime - (lastHeaterCycle + switchTimeMs)) <= 0)
      {
        if (_pin >= 0 && _pin <= 255) digitalWrite(_pin, HIGH);
        heaterOn = true;
      }
      else if (heaterOn && (long)(currentTime - (lastHeaterCycle + switchTimeMs)) > 0)
      {
        if (_pin >= 0 && _pin <= 255) digitalWrite(_pin, LOW);
        heaterOn = false;
      }
    }

    float getZero() {
      return zero;
    }
    void setZero(float t) {
      if (t >= -5.0 && t <= 20.0) {
        zero = t;
        nv.write(_nvAddress, round((zero + 5.0)*10.0));
      }
    }

    float getSpan() {
      return span;
    }
    void setSpan(float t) {
      if (t >= -5.0 && t <= 20.0) {
        span = t;
        nv.write(_nvAddress + 1, round((span + 5.0)*10.0));
      }
    }

    bool isEnabled() {
      return enabled;
    }
    void enable(bool state) {
      if (_pin >= 0  && _pin <= 255) digitalWrite(_pin, LOW);
      heaterOn = false;
      enabled = state;
    }

    bool isOn() {
      return heaterOn;
    }

  private:
    unsigned long lastHeaterCycle = 0;
    unsigned long currentTime = 0;

    bool heaterOn = false;
    bool enabled = false;

    int zero = -5;
    int span = 15;

    int _pin = 0;
    int _nvAddress = 0;
};
