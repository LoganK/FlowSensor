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
   _pinCounted(true),
   _pin(HIGH),
   _kFactor(kFactor),
   _readPin(readPin)
{
}

void FlowSensor::
tick()
{
  // Only register the event once called past the end of the window. This means
  // the count can be off by one for an extended duration, but it's such a
  // small amount I'm not worried. If you care, call `tick()` manually before
  // reading out the current flow.
  auto eventMicro = micros();
  auto windowClosed = (eventMicro - _lastEventMicro) >= _minEventMicro;
  if (!_pinCounted && windowClosed) {
    _pinCounted = true;

    // Count the rising edge.
    if (_pin == HIGH) {
      _count++;
    }
  }

  auto pin = _readPin();
  if (_pin == pin) { return; }
  _pin = pin;
  _lastEventMicro = eventMicro;

  // If the event came in while the filter window was open, it simply cancels
  // out the last event. We can signal that by saying the most recent
  // transition was already counted.
  _pinCounted = !windowClosed;
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
