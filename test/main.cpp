// FlowSensor unit tests
//   g++ -o tests -std=c++14 src/*.cpp test/*.cpp && ./tests
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "../src/FlowSensor.h"
#include "Mockduino.h"

struct test_t {
  unsigned long millis;
  pin_mode_t    pin;
};

TEST_CASE( "Simple flow with some filtering." ) {
  std::vector<test_t> data = {
    { 100, HIGH },
    { 100, HIGH },
    { 101, LOW },
    { 102, HIGH }, /* count */
    { 103, LOW },
    { 104, HIGH }, /* count */
    { 105, LOW },
    { 105, HIGH }, /* skip */
    { 107, LOW },
    { 110, HIGH }, /* count */
    { 111, HIGH },};

  auto pinVal = HIGH;
  FlowSensor fs([&] { return pinVal; },
               /* kFactor */ 2,
               /* maxHz */ 500);
  REQUIRE(fs.count() == 0);
  fs.setTotalFlowL(10);
  REQUIRE(fs.count() == 20);

  for (auto &d : data) {
    _micros = d.millis * 1000;
    pinVal = d.pin;
    fs.tick();
  }

  REQUIRE(fs.count() == 23);
  REQUIRE(fs.totalFlowL() == 11.5);
}

