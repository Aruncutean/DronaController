#ifndef SERIAL_H
#define SERIAL_H

#include <iostream>
#include <pigpio.h>
#include <cstring>

class Serial
{

public:
    Serial(std::string chanel, int baud)
    {
        std::string ch = "/dev/" + chanel;
        char chArray[256];
        std::strcpy(chArray, ch.c_str());
        serialHandle = serOpen(chArray, baud, 0);
        if (serialHandle < 0)
        {
            std::cerr << "Nu s-a putut deschide portul serial" << std::endl;
            gpioTerminate();
        }else{
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

private:
    int baud = 9600;
    int serialHandle;
};

#endif
