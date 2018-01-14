#include "Mockduino.h"

unsigned long _micros;

unsigned long micros()
{
  return _micros;
}
