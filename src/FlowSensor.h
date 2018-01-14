/* FlowSensor.h - A library for reading flow sensor data.
 */
#if !defined(FlowSensor_h)
#define FlowSensor_h

#include <functional>

class FlowSensor {
public:
  using ReadPin = std::function<int()>;
  using Counter = unsigned long;
  
  /* Create a new flow sensor. Example:
   *   #define FLOW_PIN DP1
   *   pinMode(FLOW_PIN, INPUT);
   *   FlowSensor flow([]() { return digitalRead(FLOW_PIN); }, 5500);
   *   attachInterrupt(FLOW_PIN, []() { flow.tick(); }, CHANGE);
   *
   * kFactor defines pulses/liter (check the sensor documentation).
   */
  FlowSensor(
    ReadPin readPin,
	  int     kFactor = 5500,
    int     maxHz = 2000);
	
  /* Call readPin and record the result. Usually attached to an interrupt. */
  void tick();
  
  /* Get the current count. */
  Counter count() const;
    
  /* Set the current total flow in liters. Useful when recovering from a
   * reboot. */
  void setTotalFlowL(float liters);
  
  /* Return the current total flow in liters. */
  float totalFlowL() const;
  
  #if defined(ARDUINO)

  /* Convenience method for constructing a FlowSensor on an Arduino. */
  static ReadPin readFrom(uint8_t pin);
  
  #endif // defined(ARDUINO)

protected:
  volatile Counter _count;
  
  // Used for the noise filtering logic.
  const int _minEventMicro;
  volatile unsigned long _lastEventMicro;
  volatile int _pin;

  const int _kFactor;
  ReadPin _readPin;
};

#if defined(ARDUINO)

/* Convenience method for FlowSensor setup() on an Arduino. */
// Implementation note: defined as a macro because the given FlowSensor must
// be a non-local variable.
#define FlowSensor_setup(fs, pin) \
	pinMode((pin), INPUT_PULLUP); \
	attachInterrupt((pin), []() { (fs).tick(); }, CHANGE);

#endif /* defined(ARDUINO) */

#endif

