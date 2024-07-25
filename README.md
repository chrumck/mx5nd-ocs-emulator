# Mazda MX-5 ND Passenger Occupancy Sensor Emulator

<p align="center">
    <img src="./media/PXL_20240120_113852492.jpg" alt="POS Emulator" style="height: 50%; width:50%;"/>
</p>

## Description

This is an emulator of the MX-5 ND passenger occupancy sensor built with Arduino. It can be used when the original OEM sensor is removed from the car together with the seat.

The code has provisions for a switch which can be connected to one of the analog pins (Look for 'PASSENGER_PRESENT_PIN' in code to get the pin number). When there is a passenger in the car, using the switch to send 5V to the pin will to make the emulator send `Passenger Present` signal to the SRS system to let it know that the passenger airbags should deploy in case of an accident. The pin needs to be connected to ground with a pull down resistor (10kΩ or similar).

Additionally, the unit in the pictures uses a DPDT switch and the other pole is used to toggle the seat belt signal. When the occupancy emulator is set to send `Passenger Present` signal, the `Seat Belt On` signal is sent along with it.

The program is a modification of [Timurrrr's "fake car"](https://github.com/timurrrr/RaceChronoDiyBleDevice#testing-with-a-fake-car) which spoofs Mazda MX-5 ND CAN frames. The emulator is built with an alternative module [from Longan Labs](https://docs.longan-labs.cc/1030017/). It is possible to build the emulator with other components, with only slight modifications to the code.

The emulator needs to be connected [according to this wiring diagram](https://www.mx5manual.com/page.html?p=wiring&s=WR001&docid=SM356447) . The seat belt buckles wiring is [shown here](https://www.mx5manual.com/page.html?p=wiring&s=WR001&docid=SM356446) .

It important that the emulator does not introduce any noise in the car's CAN bus network. The circuit includes a DC-DC 5V inverter and a capacitor to deliver stable power supply. Additionally, it is important that the CAN bus wires are properly twisted and made not too long to avoid interference.

## Pictures

<p align="center">
    <img src="./media/PXL_20240120_110820375.jpg" alt="POS Emulator" style="height: 50%; width:50%;"/>
</p>

<p align="center">
    <img src="./media/ocs-emulator-longan-01.jpg" alt="POS Emulator" style="height: 50%; width:50%;"/>
</p>

<p align="center">
    <img src="./media/ocs-emulator-longan-02.jpg" alt="POS Emulator" style="height: 50%; width:50%;"/>
</p>

<p align="center">
    <img src="./media/ocs-emulator-longan-03.jpg" alt="POS Emulator" style="height: 50%; width:50%;"/>
</p>
