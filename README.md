# Mazda MX-5 ND Passenger Occupancy Sensor Emulator

## Description

This is the emulator of the MX-5 ND passenger occupancy sensor built with Arduino. It is meant to be used when the original OEM sensor is removed together with the seat from the car.

The code has provisions for a switch which can be connected to one of the analog pins (Look for 'PASSENGER_PRESENT_PIN' in code to get the pin number). When there is a passenger in the car, using the switch to send 5V to the pin will to make the emulator send `Passenger Present` signal to the SRS system to let it know that the passenger airbags should deploy in case of an accident.

The program is a modification of [Timurrrr's "fake car"](https://github.com/timurrrr/RaceChronoDiyBleDevice#testing-with-a-fake-car) which spoofs Mazda MX-5 ND CAN frames. Please see Timurrrr's instructions how to assemble and use the sender.
