#include "FlowSensor.h"

#if defined(ARDUINO)
#include "Arduino.h"
#else
#include "../test/Mockduino.h"
#endif

using ReadPin = FlowSensor::ReadPin;
using Counter = FlowSensor::Counter;

FlowSensor::
FlowSensor(
  ReadPin readPin,
  int     kFactor,
  int     maxHz) :
   _count(0),
   // micros / s / #transitions-at-maxHz
   _minEventMicro(1000000 / (maxHz * 2)),
   // Always register the first tick().
   _lastEventMicro(0),
   _pin(HIGH),
   _kFactor(kFactor),
   _readPin(readPin)
{
}
void FlowSensor::
tick()
{
  // Read the pin before the time filter to avoid inadvertantly ignoring the
  // start of a transition.
  int pin = _readPin();
  if (_pin == pin) { return; }
  _pin = pin;
  
  unsigned long eventMicro = micros();
  if (eventMicro - _lastEventMicro < _minEventMicro) { return; }
  _lastEventMicro = eventMicro;

  if (pin == HIGH) {
    _count++;
  }
}
  
Counter FlowSensor::
count() const
{
  return _count;
}
  
void FlowSensor::
setTotalFlowL(float liters)
{
  _count = static_cast<int>((liters * _kFactor) + 0.5);
}
  
float FlowSensor::
totalFlowL() const
{
  float count = _count;
  return count / _kFactor; 
}

#if defined(ARDUINO)
#include "Arduino.h"

/* Convenience method for constructing a FlowSensor on an Arduino. */
ReadPin FlowSensor::
readFrom(uint8_t pin)
{
	return [=]() { return digitalRead(pin); };
}
  
#endif // defined(ARDUINO)
