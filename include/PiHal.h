#ifndef PI_HAL_H
#define PI_HAL_H

#include <RadioLib.h>
#include "pigpio.h"

class PiHal : public RadioLibHal
{
public:
  PiHal(uint8_t spiChannel, uint32_t spiSpeed = 2000000)
      : RadioLibHal(PI_INPUT, PI_OUTPUT, PI_LOW, PI_HIGH, RISING_EDGE, FALLING_EDGE),
        _spiChannel(spiChannel),
        _spiSpeed(spiSpeed)
  {
  }

  void init() override
  {
    if (gpioInitialise() < 0)
    {
      fprintf(stderr, "Nu s-a putut inițializa pigpio\n");
    }
    else
    {
      spiBegin();
      gpioSetMode(18, PI_OUTPUT);
      gpioWrite(18, PI_HIGH);
    }
  }

  void term() override
  {
    spiEnd();
    gpioSetMode(18, PI_OUTPUT);
    gpioWrite(18, PI_LOW);
    gpioTerminate();
  }

  void pinMode(uint32_t pin, uint32_t mode) override
  {
    if (pin == RADIOLIB_NC)
    {
      return;
    }
    gpioSetMode(pin, mode);
  }

  void digitalWrite(uint32_t pin, uint32_t value) override
  {
    if (pin == RADIOLIB_NC)
    {
      return;
    }
    gpioWrite(pin, value);
  }

  uint32_t digitalRead(uint32_t pin) override
  {
    if (pin == RADIOLIB_NC)
    {
      return (0);
    }

    return (gpioRead(pin));
  }

  void attachInterrupt(uint32_t interruptNum, void (*interruptCb)(void), uint32_t mode) override
  {
    if (interruptNum == RADIOLIB_NC)
    {
      return;
    }

    gpioSetISRFunc(interruptNum, mode, 0, (gpioISRFunc_t)interruptCb);
  }

  void detachInterrupt(uint32_t interruptNum) override
  {
    if (interruptNum == RADIOLIB_NC)
    {
      return;
    }

    gpioSetISRFunc(interruptNum, 0, 0, NULL);
  }

  void delay(unsigned long ms) override
  {
    gpioDelay(ms * 1000);
  }

  void delayMicroseconds(unsigned long us) override
  {
    gpioDelay(us);
  }

  unsigned long millis() override
  {
    return (gpioTick() / 1000);
  }

  unsigned long micros() override
  {
    return (gpioTick());
  }

  long pulseIn(uint32_t pin, uint32_t state, unsigned long timeout) override
  {
    if (pin == RADIOLIB_NC)
    {
      return (0);
    }

    this->pinMode(pin, PI_INPUT);
    uint32_t start = this->micros();
    uint32_t curtick = this->micros();

    while (this->digitalRead(pin) == state)
    {
      if ((this->micros() - curtick) > timeout)
      {
        return (0);
      }
    }

    return (this->micros() - start);
  }

  void spiBegin()
  {
    if (_spiHandle < 0)
    {
      _spiHandle = spiOpen(_spiChannel, _spiSpeed, 0);
    }
  }

  void spiBeginTransaction() {}

  void spiTransfer(uint8_t *out, size_t len, uint8_t *in)
  {
    spiXfer(_spiHandle, (char *)out, (char *)in, len);
  }

  void spiEndTransaction() {}

  void spiEnd()
  {
    if (_spiHandle >= 0)
    {
      spiClose(_spiHandle);
      _spiHandle = -1;
    }
  }

private:
  const unsigned int _spiSpeed;
  const uint8_t _spiChannel;
  int _spiHandle = -1;
};

#endif