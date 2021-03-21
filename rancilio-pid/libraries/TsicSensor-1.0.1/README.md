# TsicSensor
Arduino library for reading TSIC temperature sensors

This library reads temperature values from several TSIC sensor types (TSIC 206/306/506/516/716). It uses interrupts
to decode the ZACwire protocol used by the sensors and supports methods for reading temperature values in several 
scales (Celsius/Fahrenheit/Kelvin)

## Usage

The TsicSensor library uses following classes:
* `TsicScale` selects a temperature scale (Celsius/Fahrenheit/Kelvin) for the `tryGetTempValue()` function	
* `TsicType` defines the type of the used TSIC sensor (TSIC_206,TSIC_306,TSIC_316,TSIC_506,TSIC_516,TSIC_716)
* `TsicSensor` this is the sensor class used to read the temperature values.

To create a sensor instance (the library supports up to 4 instances at the same time) use a call to the
`TsicSensor::create(byte input_pin, byte vcc_pin, TsicType type)` function.


This initializes a TSIC 506 sensor with the signal pin at GPIO 16 and the Vcc pin at GPIO 25:
```
TsicSensor* sensor1 = TsicSensor::create(16, 25, TsicType::TSIC_306);
```
 
Or use `TsicExternalVcc` for the vcc_pin parameter if the sensor is powered with an external Vcc:
```
TsicSensor* sensor1 = TsicSensor::create(16, TsicExternalVcc, TsicType::TSIC_306);
```

The newly created sensor instances can now be used to read temperature values in several scales, e.g.:
```
  float temperature = sensor1->getTempCelsius();
```


## Available TsicSensor functions

```
  // -------------------------------------------------------------------------------------------------------
  // * Creates and initializes a new sensor instance with the given type and input/vcc pins. 
  // * Use "TsicExternalVcc" for the "vcc_pin" parameter if the sensor is connected directly to Vcc.
  // * A maximum of 4 instances can be operated at the same time.
  // * Returns a pointer to a sensor instance (or "nullptr" if the operation failed). 
  // -------------------------------------------------------------------------------------------------------
  static TsicSensor* create(byte input_pin, byte vcc_pin, TsicType type);  
  
  // -------------------------------------------------------------------------------------------------------
  // * Returns the latest temperature value in °C (waits up to 100ms for sensor initialization, if needed).
  // * Returns "-273.15" if the read was not successful.
  // -------------------------------------------------------------------------------------------------------
  float getTempCelsius();

  // -------------------------------------------------------------------------------------------------------
  // * Returns the latest temperature value in °F (waits up to 100ms for sensor initialization, if needed).
  // * Returns "-459.67" if the read was not successful.
  // -------------------------------------------------------------------------------------------------------
  float getTempFahrenheit();
  
  // -------------------------------------------------------------------------------------------------------
  // * Returns the latest temperature value in °K (waits up to 100ms for sensor initialization, if needed).
  // * Returns "0.0" if the read was not successful.
  // -------------------------------------------------------------------------------------------------------
  float getTempKelvin();
  
  // -------------------------------------------------------------------------------------------------------
  // * Gets the latest temperature value in °C/°F/°K. 
  // * (waits up to 100ms for sensor initialization, if needed).
  // * Returns "true" if the read was successful, "false" otherwise.
  // -------------------------------------------------------------------------------------------------------
  bool tryGetTempValue(float* value, TsicScale scale);

  // -------------------------------------------------------------------------------------------------------
  // * If the sensor is connected directly to Vcc, it sends its values 10 times per second.
  // * This function returns "true" if the received temperature value has changed since the last call to
  // * one of the "*getTemp*()" functions.
  // -------------------------------------------------------------------------------------------------------
  bool newValueAvailable();           

```

