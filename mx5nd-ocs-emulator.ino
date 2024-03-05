#include <SPI.h>
#include <mcp_canbus.h>

#define SERIAL_BAUD_RATE 115200

#define PASSENGER_PRESENT_PIN 4 // a switch to select passenger/no passenger can be connected to this pin 

#define CS_PIN 9 // SPI pin to the CAN controller
#define CAN_BAUD_RATE CAN_500KBPS // MX5 ND uses 500k baud rate hor HS CAN

#define LED_PIN 3
#define LED_ON()     digitalWrite(LED_PIN, HIGH)
#define LED_OFF()    digitalWrite(LED_PIN, LOW)
#define LED_TOGGLE() digitalWrite(LED_PIN, 1 - digitalRead(LED_PIN))

#define EMULATOR_STARTUP_TIME_MILLIS 2700 // initial time the OCS sensor sends 0 as occupancy data.

#define CAN_FRAME_LENGTH 8

#define FRAME_x344_ID 0x344
#define FRAME_x344_SEND_INTERVAL_MILLIS 100
#define FRAME_x344_CYCLE_COUNT 16
#define FRAME_x344_BYTE0_STARTUP 0x0
#define FRAME_x344_BYTE0_NO_PASSENGER 0x40
#define FRAME_x344_BYTE0_PASSENGER_PRESENT 0x80
#define FRAME_x344_BYTE1 0x58

#define FRAME_x438_ID 0x438
#define FRAME_x438_SEND_INTERVAL_MILLIS 1000

u8 x438FramePayload[CAN_FRAME_LENGTH] = { 0x42, 0x31, 0x37, 0x38, 0x39, 0x32, 0x54, 0x4C }; // 2019 30AE
//const u8 x438FramePayload[CAN_FRAME_LENGTH] = { 0x44, 0x33, 0x36, 0x34, 0x35, 0x32, 0x51, 0x37 }; // Is it sensor's serial number?

MCP_CAN mcp2515(CS_PIN);

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);

    pinMode(LED_PIN, OUTPUT);
    pinMode(PASSENGER_PRESENT_PIN, INPUT);

    for (int i = 0; i < 20; i++)
    {
        LED_TOGGLE();
        delay(20);
    }

    while (mcp2515.begin(CAN_BAUD_RATE) != CAN_OK)
    {
        delay(1000);
        LED_TOGGLE();
    }

    LED_ON();
    Serial.print("MCP2515 setup successful.");
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

    u8 x344FramePayload[CAN_FRAME_LENGTH] = { byte0, 0, 0, 0, 0, 0, 0, 0 };
    buildFrameX344(cycleNumber, x344FramePayload);
    sendFrame(FRAME_x344_ID, x344FramePayload);

    cycleNumber = (cycleNumber + 1) % FRAME_x344_CYCLE_COUNT;
}

void sendFrame(u16 frameId, u8* payload) {
    mcp2515.sendMsgBuf(frameId, 0, CAN_FRAME_LENGTH, payload);
}

u8* buildFrameX344(u8 cycleNumber, u8* payload) {
    payload[1] = FRAME_x344_BYTE1;
    payload[6] = cycleNumber;
    payload[7] = 0xFF - cycleNumber - payload[0] / 8;

    return payload;
}