# Honda-Gen-PGM-FI-Interface
A PlatformIO Arduino implementation of data streaming on the Honda Generator DLC Port.

This shows how you can stream various sensors from the Honda EU7000is using the "Data Link Connector"
port.

See [Honda-Gen-PGM-FI-Protocol](https://github.com/blaisejarrett/Honda-Gen-PGM-FI-Protocol) 
for information on the protocol and connector implemented here.

## Test hardware

This project used an ESP32 devkit. But you can use any arduino platform with the provided code.
It makes use of a Hardware Serial port for the DLC connector. The example main
uses another Hardware Serial port for logging.

The DLC port uses 5v logic levels. If you are using a 3.3v device like the ESP32 you must 
incorporate a level shifter in your circuit.

![example hardware](example_hardware.PNG?raw=true)

## Example

Included in main is an example which uses the provided libraries to query sensor data.

The output of the provided main is as follows:


### With generator Off.
```
Start!
--- ECU Info1 ---
Bat: 13.1v
RPM: 0
Runtime: 00:04:08
CylHead: 50 c
Timing: -10 deg
OutputV: 0v

--- ECU Info2 ---
Baro: 100 kpa
Injection Time: 0.0ms
Throttle Pos: 0%
Output Power: 0 va
Starter Count: 22
o2: 3.4v

--- Inverter Master ---
Current: 0.0a
Temp: -40c
Mode: 0
Power: 0w
Volts: 0v

--- Inverter Slave ---
Current: 0.0a
Temp: -40c
Power: 0w

```

### With generator On.
```
--- ECU Info1 ---
Bat: 12.5v
RPM: 2412
Runtime: 00:04:08
CylHead: 49 c
Timing: 33 deg
OutputV: 126v

--- ECU Info2 ---
Baro: 100 kpa
Injection Time: 3.84ms
Throttle Pos: 10%
Output Power: 50 va
Starter Count: 23
o2: 1.0v

--- Inverter Master ---
Current: 0.3a
Temp: 5c
Mode: 2
Power: 40w
Volts: 126v

--- Inverter Slave ---
Current: 0.1a
Temp: 4c
Power: 10w
```

### With Generator On with 500w load in 240v mode.
```
--- ECU Info1 ---
Bat: 12.8v
RPM: 2381
Runtime: 00:04:08
CylHead: 38 c
Timing: 36 deg
OutputV: 126v

--- ECU Info2 ---
Baro: 100 kpa
Injection Time: 4.10ms
Throttle Pos: 13%
Output Power: 550 va
Starter Count: 23
o2: 1.0v

--- Inverter Master ---
Current: 4.3a
Temp: 5c
Mode: 2
Power: 540w
Volts: 126v

--- Inverter Slave ---
Current: 0.1a
Temp: 4c
Power: 10w
```