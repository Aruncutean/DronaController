#ifndef MAGNETOMETRU_H
#define MAGNETOMETRU_H
#include <iostream>
#include "pigpio.h"

class Magnetometru
{
public:
    Magnetometru()
    {
        const unsigned I2C_BUS = 1;
        const unsigned I2C_ADDR = 0xd;

         handle = i2cOpen(I2C_BUS, I2C_ADDR, 0);
        if (handle < 0)
        {
            std::cerr << "Could not open I2C communication." << std::endl;
            gpioTerminate();
        }
    }

    void getData()
    {
        i2cWriteByteData(handle, 0x0B, 0x01);
        i2cWriteByteData(handle, 0x09, 0x01);

        time_sleep(0.1);
        char data[6];
        if (i2cReadI2CBlockData(handle, 0x00, data, 6) == 6)
        {
            int16_t x = (data[1] << 8) | data[0];
            int16_t y = (data[3] << 8) | data[2];
            int16_t z = (data[5] << 8) | data[4];
            std::cout << "X: " << x << ", Y: " << y << ", Z: " << z << std::endl;
        }
        else
        {
            std::cerr << "Failed to read data from the device." << std::endl;
        }
    }

private:
int handle;
};

#endif
