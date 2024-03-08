#ifndef MSP_H
#define MSP_H

#include <iostream>
#include <pigpio.h>
#include <cstring>
#include <bitset>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <vector>
#include "MspType.h"
#include "DataController.h"

struct MSPMessage
{
    uint8_t size;
    uint8_t cmd;
    std::vector<uint8_t> payload;
    uint8_t checksum;
};

struct FlightMode
{
    uint8_t permanentId;
    uint8_t auxChannelIndex;
    uint8_t rangeStartStep;
    uint8_t rangeEndStep;
};

class Msp
{

public:
    Msp(std::string chanel, int baud)
    {
        std::string ch = "/dev/" + chanel;
        char chArray[256];
        std::strcpy(chArray, ch.c_str());
        serialHandle = serOpen(chArray, baud, 0);
        if (serialHandle < 0)
        {
            std::cerr << "Nu s-a putut deschide portul serial" << std::endl;
            gpioTerminate();
        }
        else
        {
            std::cout << "serial open" << std::endl;
        }
    }

    void write(std::string msg)
    {
        char msgArray[256];
        std::strcpy(msgArray, msg.c_str());
        serWrite(serialHandle, msgArray, strlen(msgArray));
    }

    std::string readSerialData()
    {
        char data[256];
        std::string result;

        if (serDataAvailable(serialHandle) > 0)
        {
            int count = serRead(serialHandle, data, sizeof(data) - 1);
            if (count > 0)
            {
                data[count] = '\0';
                result = data;
            }
        }

        return result;
    }

    void serialWrite(char c)
    {
        serWriteByte(serialHandle, c);
    }

    char readChar()
    {
        char c = '\0';
        if (serDataAvailable(serialHandle) > 0)
        {
            c = serReadByte(serialHandle);
        }
        return c;
    }

    void sendMSPRequest(uint8_t mspCommand)
    {
        serialWrite(MSP_START_CHAR);
        serialWrite(MSP_IDENTIFICATOR);
        serialWrite(MSP_SEND_CHAR);
        serialWrite(0);
        serialWrite(mspCommand);

        uint8_t checksum = 0 ^ mspCommand;
        serialWrite(checksum);
    }

    MSPMessage readMSPResponse()
    {
        MSPMessage mspMsg;
        uint8_t dataSize = 0;
        uint8_t command = 0;
        uint8_t checksum = 0;
        uint8_t calculatedChecksum = 0;

        if (readChar() == MSP_START_CHAR &&
            readChar() == MSP_IDENTIFICATOR &&
            readChar() == MSP_RECV_CHAR)
        {
            dataSize = readChar();
            calculatedChecksum ^= dataSize;

            command = readChar();
            calculatedChecksum ^= command;

            for (int i = 0; i < dataSize; i++)
            {
                uint8_t incomingByte = readChar();
                mspMsg.payload.push_back(incomingByte);
                calculatedChecksum ^= incomingByte;
            }
            checksum = readChar();

            if (checksum == calculatedChecksum)
            {
                mspMsg.checksum = checksum;
                mspMsg.size = dataSize;
                mspMsg.cmd = command;
            }
            else
            {
                throw std::runtime_error("Checksum error.");
            }
        }
        else
        {
            throw std::runtime_error("Incorrect header.");
        }
        return mspMsg;
    }

    int mapValue(int x)
    {
        return x * (1000 / 45) + 1000;
    }

    void procesareMessage(MSPMessage mspMessage)
    {

        switch (mspMessage.cmd)
        {
        case 101:
            std::cout << "101" << std::endl;
            if (mspMessage.size >= 11)
            {
                uint16_t cycleTime = mspMessage.payload.at(0) | (mspMessage.payload.at(1) << 8);
                uint16_t i2cErrorCounter = mspMessage.payload.at(2) | (mspMessage.payload.at(3) << 8);
                uint16_t sensor = mspMessage.payload.at(4) | (mspMessage.payload.at(5) << 8);
                uint32_t flightModeFlags = mspMessage.payload.at(6) | (mspMessage.payload.at(7) << 8) | (mspMessage.payload.at(8) << 16) | (mspMessage.payload.at(9) << 24);
                uint8_t configProfileIndex = mspMessage.payload.at(10);

                printf("Cycle Time: %d\n", cycleTime);
                printf("I2C Error Counter: %d\n", i2cErrorCounter);
                printf("Sensor: %d\n", sensor);
                checkSensors(sensor);
                printf("Flight Mode Flags: %u\n", flightModeFlags);
                checkFlightModes(flightModeFlags);
                printf("Config Profile Index: %d\n", configProfileIndex);
            }
            break;
        case 34:
        {
            std::cout << "34" << std::endl;

            std::vector<FlightMode> modes = extractFlightModes(mspMessage.payload);
            for (const auto &mode : modes)
            {

                bool isModeEmpty = (mode.permanentId == 0 && mode.auxChannelIndex == 0 && mode.rangeStartStep == 0 && mode.rangeEndStep == 0);
                if (!isModeEmpty)
                {
                    std::cout << "Mode ID: " << static_cast<int>(mode.permanentId)
                              << ", Aux Channel: " << static_cast<int>(mode.auxChannelIndex) + 5
                              << ", Range: " << mapValue(static_cast<int>(mode.rangeStartStep))
                              << " - " << mapValue(static_cast<int>(mode.rangeEndStep)) << std::endl;

                    mode::Mode modeS;
                    modeS.id = static_cast<int>(mode.permanentId);
                    modeS.auxChannel = static_cast<int>(mode.auxChannelIndex) + 5;
                    modeS.minRange = mapValue(static_cast<int>(mode.rangeStartStep));
                    modeS.maxRange = mapValue(static_cast<int>(mode.rangeEndStep));
                    DataController::getInstance().addMode(modeS);
                }
            }

            break;
        }
        case 105:
        {
            std::cout << "105" << std::endl;
            std::cout << "RX Channels: ";
            std::vector<channel::Channel> channel;
            for (size_t i = 0; i < mspMessage.payload.size() - 1; i += 2)
            {
                uint16_t channelValue = mspMessage.payload.at(i) | (mspMessage.payload.at(i + 1) << 8);

                std::cout << channelValue << " ";
                     for (size_t j = 0; j < DataController::getInstance().getMode().size(); j ++) {
        
     }
            }
            std::cout << std::endl;
            break;
        }
        default:
            break;
        }
    }

    void checkSensors(uint16_t sensorFlags)
    {

        const uint16_t SENSOR_ACC = 0x01;          // 0000 0001
        const uint16_t SENSOR_BARO = 0x02;         // 0000 0010
        const uint16_t SENSOR_MAG = 0x04;          // 0000 0100
        const uint16_t SENSOR_GPS = 0x08;          // 0000 1000
        const uint16_t SENSOR_SONAR = 0x10;        // 0001 0000
        const uint16_t SENSOR_LIDAR = 0x20;        // 0100 0000
        const uint16_t SENSOR_OPTICAL_FLOW = 0x40; // 1000 0000

        std::bitset<16> binaryFlags(sensorFlags);
        DataController::getInstance().setSenzorIsActive(senzor::SenzorType::gyro, true);
        if (sensorFlags & SENSOR_ACC)
        {
            DataController::getInstance().setSenzorIsActive(senzor::SenzorType::accel, true);
            std::cout << "Accelerometru ";
        }
        else
        {
            DataController::getInstance().setSenzorIsActive(senzor::SenzorType::accel, false);
        }
        if (sensorFlags & SENSOR_BARO)
        {
            DataController::getInstance().setSenzorIsActive(senzor::SenzorType::baro, true);
            std::cout << "Barometru ";
        }
        else
        {
            DataController::getInstance().setSenzorIsActive(senzor::SenzorType::baro, false);
        }
        if (sensorFlags & SENSOR_MAG)
        {
            DataController::getInstance().setSenzorIsActive(senzor::SenzorType::mag, true);
            std::cout << "Magnetometru ";
        }
        else
        {
            DataController::getInstance().setSenzorIsActive(senzor::SenzorType::mag, false);
        }
        if (sensorFlags & SENSOR_GPS)
        {
            DataController::getInstance().setSenzorIsActive(senzor::SenzorType::gps, true);
            std::cout << "GPS ";
        }
        else
        {
            DataController::getInstance().setSenzorIsActive(senzor::SenzorType::gps, false);
        }
        if (sensorFlags & SENSOR_SONAR)
        {
            DataController::getInstance().setSenzorIsActive(senzor::SenzorType::sonar, true);
            std::cout << "Sonar ";
        }
        else
        {
            DataController::getInstance().setSenzorIsActive(senzor::SenzorType::sonar, false);
        }
        if (sensorFlags & SENSOR_LIDAR)
        {
            DataController::getInstance().setSenzorIsActive(senzor::SenzorType::speed, true);
            std::cout << "Lidar ";
        }
        else
        {
            DataController::getInstance().setSenzorIsActive(senzor::SenzorType::speed, false);
        }
        if (sensorFlags & SENSOR_OPTICAL_FLOW)
        {
            DataController::getInstance().setSenzorIsActive(senzor::SenzorType::flow, true);
            std::cout << "Flux optic ";
        }
        else
        {
            DataController::getInstance().setSenzorIsActive(senzor::SenzorType::flow, false);
        }
        std::cout << std::endl;
    }

    const uint32_t FLAG_ARMED = 1 << 0;         // 0000 0001
    const uint32_t FLAG_ANGLE_MODE = 1 << 1;    // 0000 0010
    const uint32_t FLAG_HORIZON_MODE = 1 << 2;  // 0000 0100
    const uint32_t FLAG_BARO_MODE = 1 << 3;     // 0000 1000
    const uint32_t FLAG_MAG_MODE = 1 << 4;      // 0001 0000
    const uint32_t FLAG_HEADFREE_MODE = 1 << 5; // 0010 0000
    const uint32_t FLAG_FAILSAFE_MODE = 1 << 6; // 0100 0000
    const uint32_t FLAG_GPS_HOME_MODE = 1 << 7; // 1000 0000
    const uint32_t FLAG_GPS_HOLD_MODE = 1 << 8; // 0001 0000 0000

    void checkFlightModes(uint32_t flags)
    {
        std::cout << "Moduri de zbor active: ";
        std::bitset<32> binaryFlags(flags);

        std::cout << "format binar: " << binaryFlags << std::endl;
        if (flags & FLAG_ARMED)
        {
            std::cout << "Armed ";
        }
        if (flags & FLAG_ANGLE_MODE)
        {
            std::cout << "Angle ";
        }
        if (flags & FLAG_HORIZON_MODE)
        {
            std::cout << "Horizon ";
        }
        if (flags & FLAG_BARO_MODE)
        {
            std::cout << "Baro ";
        }
        if (flags & FLAG_MAG_MODE)
        {
            std::cout << "Mag ";
        }
        if (flags & FLAG_HEADFREE_MODE)
        {
            std::cout << "HeadFree ";
        }
        if (flags & FLAG_FAILSAFE_MODE)
        {
            std::cout << "Failsafe ";
        }
        if (flags & FLAG_GPS_HOME_MODE)
        {
            std::cout << "GPS Home ";
        }
        if (flags & FLAG_GPS_HOLD_MODE)
        {
            std::cout << "GPS Hold ";
        }
        std::cout << std::endl;
    }

    std::vector<FlightMode> extractFlightModes(const std::vector<uint8_t> &payload)
    {
        std::vector<FlightMode> modes;
        size_t numModes = payload.size() / 4;
        for (size_t i = 0; i < numModes; i++)
        {
            FlightMode mode;
            size_t offset = i * 4;
            mode.permanentId = payload[offset];
            mode.auxChannelIndex = payload[offset + 1];
            mode.rangeStartStep = payload[offset + 2];
            mode.rangeEndStep = payload[offset + 3];
            modes.push_back(mode);
        }
        return modes;
    }

private:
    int baud = 9600;
    int serialHandle;
};

#endif
