# Mazda MX-5 ND Passenger Occupancy Sensor Emulator built with Arduino

## Description

This is the emulator of the MX-5 ND passenger occupancy sensor. It is meant to be used when the original OEM sensor is removed together with the seat from the car.

The code has provisions for an occupancy switch which can connected to pin 9. When there is a passenger in the car, the switch can be used to let SRS system know that the passenger airbags should deploy in case of an accident.

The program is a modification of [Timurrrr's "fake car"](https://github.com/timurrrr/RaceChronoDiyBleDevice#testing-with-a-fake-car) which spoofs Mazda MX-5 ND CAN frames. Please see Timurrrr's instructions how to assemble and use the sender.
