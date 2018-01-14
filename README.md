# FlowSensor

This library simplifies reading from flow sensors on the Arudino platform,
including setup, totalization, and noise filtering. The default values are for
the Swissflow SF800, but it should work with any frequency-based flow sensor.

## Sample

Initialize the sensor for a given pin:

    #define FLOW_1 DP1
    FlowSensor fs(FlowSensor::readFrom(FLOW_1));

And then in `setup()`:

    FlowSensor_setup(fs, FLOW_1);

# Thanks

This library uses the fantastic Catch2 testing library: https://github.com/catchorg/Catch2
