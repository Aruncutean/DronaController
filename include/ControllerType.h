#ifndef CONTROLLERTYPE_H
#define CONTROLLERTYPE_H
namespace senzor
{
    enum SenzorType
    {
        gyro = 1,
        accel = 2,
        mag = 3,
        baro = 4,
        gps = 5,
        flow = 6,
        sonar = 7,
        speed = 8
    };

    struct Senzor
    {
        bool gyro = false;
        bool accel = false;
        bool mag = false;
        bool baro = false;
        bool gps = false;
        bool flow = false;
        bool sonar = false;
        bool speed = false;
    };
}
namespace mode
{

    struct Mode
    {
        int id;
        int auxChannel;
        int minRange;
        int maxRange;
    };

    enum ModeEnum
    {
        arm = 0,
        angle = 1,
        horizon = 2,
        navAlthold = 3,
        mag = 5,
        headFree = 6,
        headADJ = 7,
        camstab = 8,
        navRTH = 10,
        navPosHold = 11,
        passThru = 12,
        beeperon = 13,
        ledLow = 15,
        lLights = 16,
        osb = 19,
        telemetry = 20,
        gTune = 21,
        sonar = 22,
        blackBox = 26,
        failSave = 27,
        navWp = 28,
        airMode = 29,
        HOMERESET = 30,
        GCSNAV = 31,
        HEADINGLOCK = 32,
        SURFACE = 33,
        FLAPERON = 34,
        TURNASSIST = 35,
        NAVLAUNCH = 36,
        AUTOTRIM = 37,
    };

    struct ModeActive
    {
        bool arm = false;
        bool angle = false;
        bool horizon = false;
        bool navAlthold = false;
        bool mag = false;
        bool headFree = false;
        bool headADJ = false;
        bool camstab = false;
        bool navRTH = false;
        bool navPosHold = false;
        bool passThru = false;
        bool beeperon = false;
        bool ledLow = false;
        bool lLights = false;
        bool osb = false;
        bool telemetry = false;
        bool gTune = false;
        bool sonar = false;
        bool blackBox = false;
        bool failSave = false;
        bool navWp = false;
        bool airMode = false;
        bool HOMERESET = false;
        bool GCSNAV = false;
        bool HEADINGLOCK = false;
        bool SURFACE = false;
        bool FLAPERON = false;
        bool TURNASSIST = false;
        bool NAVLAUNCH = false;
        bool AUTOTRIM = false;
    };
}

namespace channel
{
    
struct Channel
{
    int id;
    float value;
};


}

#endif // CONTROLLERTYPE_H