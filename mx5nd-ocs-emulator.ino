#include <CAN.h>

#define SERIAL_BAUD_RATE 115200

#define PASSENGER_PRESENT_PIN 9 // a switch to select passenger/no passenger can be connected to this pin 

#define CS_PIN 10 // SPI pin to the CAN controller
#define IRQ_PIN 9 // SPI pin to the CAN controller
#define MCP2515_QUARTZ_MHZ 8  // Some MCP2515 boards have 8 MHz quartz.
#define SPI_MHZ 8 // SPI frequency
#define CAN_BAUD_RATE 500E3 // MX5 ND uses 500k baud rate hor HS CAN
#define CAN_FRAME_LENGTH 8 // can frame payload size

#define EMULATOR_STARTUP_TIME_MILLIS 2700 // initial time the OCS sensor sends 0 as occupancy data.

#define FRAME_x344_ID 0x344
#define FRAME_x344_SEND_INTERVAL_MILLIS 100
#define FRAME_x344_CYCLE_COUNT 16
#define FRAME_x344_BYTE0_STARTUP 0x0
#define FRAME_x344_BYTE0_NO_PASSENGER 0x40
#define FRAME_x344_BYTE0_PASSENGER_PRESENT 0x80
#define FRAME_x344_BYTE1 0x58

#define FRAME_x438_ID 0x438
#define FRAME_x438_SEND_INTERVAL_MILLIS 1000
const u8 x438FramePayload[8] = { 0x42, 0x31, 0x37, 0x38, 0x39, 0x32, 0x54, 0x4C };

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    pinMode(PASSENGER_PRESENT_PIN, INPUT);

    CAN.setClockFrequency(MCP2515_QUARTZ_MHZ * 1E6);
    CAN.setSPIFrequency(SPI_MHZ * 1E6);
    CAN.setPins(CS_PIN, IRQ_PIN);

    while (!CAN.begin(CAN_BAUD_RATE)) {
        Serial.println("Failed to connect to the CAN controller!");
        delay(1000);
    }

    Serial.println("CAN controller connected");
}

void loop() {
    static u8 cycleNumber = 0;
    static u32 lastCycleTime = millis();

    u32 currentTime = millis();
    if (currentTime - lastCycleTime < FRAME_x344_SEND_INTERVAL_MILLIS) return;
    lastCycleTime = currentTime;

    if (currentTime % FRAME_x438_SEND_INTERVAL_MILLIS < FRAME_x344_SEND_INTERVAL_MILLIS) {
        sendFrame(FRAME_x438_ID, x438FramePayload);
    }

    int passengerPresent = digitalRead(PASSENGER_PRESENT_PIN);
    u8 byte0 = currentTime < EMULATOR_STARTUP_TIME_MILLIS ?
        FRAME_x344_BYTE0_STARTUP : passengerPresent ?
        FRAME_x344_BYTE0_PASSENGER_PRESENT :
        FRAME_x344_BYTE0_NO_PASSENGER;

    u8 frameX344Payload[CAN_FRAME_LENGTH] = { byte0, 0, 0, 0, 0, 0, 0, 0 };
    buildFrameX344(cycleNumber, frameX344Payload);
    sendFrame(FRAME_x344_ID, frameX344Payload);

    cycleNumber = (cycleNumber + 1) % FRAME_x344_CYCLE_COUNT;
}

void sendFrame(u16 frameId, const u8* payload) {
    if (!CAN.beginPacket(frameId)) {
        Serial.println("CAN.beginPacket() failed.");
        return;
    }

    if (CAN.write(payload, CAN_FRAME_LENGTH) != CAN_FRAME_LENGTH) {
        Serial.println("CAN.write() failed.");
    }

    if (!CAN.endPacket()) {
        Serial.println("CAN.endPacket() failed.");
        return;
    }

    return;
}

u8* buildFrameX344(u8 cycleNumber, u8* payload) {
    payload[1] = FRAME_x344_BYTE1;
    payload[6] = cycleNumber;
    payload[7] = 0xFF - cycleNumber - payload[0] / 8;

    return payload;
}