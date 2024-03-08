#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include <iostream>
#include <string>
#include "ControllerType.h"
#include <vector>

class DataController
{
public:
    static DataController &getInstance()
    {
        static DataController instance; // Garantează o singură instanță
        return instance;
    }

    // Metode pentru a seta/obține datele
    void setData(int data) { this->data = data; }
    int getData() const { return data; }

    bool getSenzorIsActive(senzor::SenzorType senzorType)
    {
        switch (senzorType)
        {
        case senzor::SenzorType::gyro:
            return senzor.gyro;
            break;
        case senzor::SenzorType::accel:
            return senzor.accel;
            break;
        case senzor::SenzorType::mag:
            return senzor.mag;
            break;
        case senzor::SenzorType::baro:
            return senzor.baro;
            break;
        case senzor::SenzorType::gps:
            return senzor.gps;
            break;
        case senzor::SenzorType::flow:
            return senzor.flow;
            break;
        case senzor::SenzorType::sonar:
            return senzor.sonar;
            break;
        case senzor::SenzorType::speed:
            return senzor.speed;
            break;
        default:
            break;
        };

        return false;
    }

    void setSenzorIsActive(senzor::SenzorType senzorType, bool active)
    {
        switch (senzorType)
        {
        case senzor::SenzorType::gyro:
            senzor.gyro = active;
            break;
        case senzor::SenzorType::accel:
            senzor.accel = active;
            break;
        case senzor::SenzorType::mag:
            senzor.mag = active;
            break;
        case senzor::SenzorType::baro:
            senzor.baro = active;
            break;
        case senzor::SenzorType::gps:
            senzor.gps = active;
            break;
        case senzor::SenzorType::flow:
            senzor.flow = active;
            break;
        case senzor::SenzorType::sonar:
            senzor.sonar = active;
            break;
        case senzor::SenzorType::speed:
            senzor.speed = active;
            break;
        default:
            break;
        };
    }

    bool getModeIsActive(mode::ModeEnum mode)
    {
        switch (mode)
        {
        case mode::ModeEnum::arm:
            return modeActive.arm;
            break;
        case mode::ModeEnum::angle:
            return modeActive.angle;
            break;
        case mode::ModeEnum::horizon:
            return modeActive.horizon;
            break;
        case mode::ModeEnum::navAlthold:
            return modeActive.navAlthold;
            break;
        case mode::ModeEnum::mag:
            return modeActive.mag;
            break;
        case mode::ModeEnum::headFree:
            return modeActive.headFree;
            break;
        case mode::ModeEnum::headADJ:
            return modeActive.headADJ;
            break;
        case mode::ModeEnum::camstab:
            return modeActive.camstab;
            break;
        case mode::ModeEnum::navRTH:
            return modeActive.navRTH;
            break;
        case mode::ModeEnum::navPosHold:
            return modeActive.navPosHold;
            break;
        case mode::ModeEnum::passThru:
            return modeActive.passThru;
            break;
        case mode::ModeEnum::beeperon:
            return modeActive.beeperon;
            break;
        case mode::ModeEnum::ledLow:
            return modeActive.ledLow;
            break;
        case mode::ModeEnum::lLights:
            return modeActive.lLights;
            break;
        case mode::ModeEnum::osb:
            return modeActive.osb;
            break;
        case mode::ModeEnum::telemetry:
            return modeActive.telemetry;
            break;
        case mode::ModeEnum::gTune:
            return modeActive.gTune;
            break;
        case mode::ModeEnum::sonar:
            return modeActive.sonar;
            break;
        case mode::ModeEnum::blackBox:
            return modeActive.blackBox;
            break;
        case mode::ModeEnum::failSave:
            return modeActive.failSave;
            break;
        case mode::ModeEnum::navWp:
            return modeActive.navWp;
            break;
        case mode::ModeEnum::airMode:
            return modeActive.airMode;
            break;
        case mode::ModeEnum::HOMERESET:
            return modeActive.HOMERESET;
            break;
        case mode::ModeEnum::GCSNAV:
            return modeActive.GCSNAV;
            break;
        case mode::ModeEnum::HEADINGLOCK:
            return modeActive.HEADINGLOCK;
            break;
        case mode::ModeEnum::SURFACE:
            return modeActive.SURFACE;
            break;

        case mode::ModeEnum::FLAPERON:
            return modeActive.FLAPERON;
            break;
        case mode::ModeEnum::TURNASSIST:
            return modeActive.TURNASSIST;
            break;
        case mode::ModeEnum::NAVLAUNCH:
            return modeActive.NAVLAUNCH;
            break;
        case mode::ModeEnum::AUTOTRIM:
            return modeActive.AUTOTRIM;
            break;
        default:
            break;
        }
        return false;
    }

    void setModeIsActive(mode::ModeEnum mode, bool active)
    {
        switch (mode)
        {
        case mode::ModeEnum::arm:
            modeActive.arm = active;
            break;
        case mode::ModeEnum::angle:
            modeActive.angle = active;
            break;
        case mode::ModeEnum::horizon:
            modeActive.horizon = active;
            break;
        case mode::ModeEnum::navAlthold:
            modeActive.navAlthold = active;
            break;
        case mode::ModeEnum::mag:
            modeActive.mag = active;
            break;
        case mode::ModeEnum::headFree:
            modeActive.headFree = active;
            break;
        case mode::ModeEnum::headADJ:
            modeActive.headADJ = active;
            break;
        case mode::ModeEnum::camstab:
            modeActive.camstab = active;
            break;
        case mode::ModeEnum::navRTH:
            modeActive.navRTH = active;
            break;
        case mode::ModeEnum::navPosHold:
            modeActive.navPosHold = active;
            break;
        case mode::ModeEnum::passThru:
            modeActive.passThru = active;
            break;
        case mode::ModeEnum::beeperon:
            modeActive.beeperon = active;
            break;
        case mode::ModeEnum::ledLow:
            modeActive.ledLow = active;
            break;
        case mode::ModeEnum::lLights:
            modeActive.lLights = active;
            break;
        case mode::ModeEnum::osb:
            modeActive.osb = active;
            break;
        case mode::ModeEnum::telemetry:
            modeActive.telemetry = active;
            break;
        case mode::ModeEnum::gTune:
            modeActive.gTune = active;
            break;
        case mode::ModeEnum::sonar:
            modeActive.sonar = active;
            break;
        case mode::ModeEnum::blackBox:
            modeActive.blackBox = active;
            break;
        case mode::ModeEnum::failSave:
            modeActive.failSave = active;
            break;
        case mode::ModeEnum::navWp:
            modeActive.navWp = active;
            break;
        case mode::ModeEnum::airMode:
            modeActive.airMode = active;
            break;
        case mode::ModeEnum::HOMERESET:
            modeActive.HOMERESET = active;
            break;
        case mode::ModeEnum::GCSNAV:
            modeActive.GCSNAV = active;
            break;
        case mode::ModeEnum::HEADINGLOCK:
            modeActive.HEADINGLOCK = active;
            break;
        case mode::ModeEnum::SURFACE:
            modeActive.SURFACE = active;
            break;

        case mode::ModeEnum::FLAPERON:
            modeActive.FLAPERON = active;
            break;
        case mode::ModeEnum::TURNASSIST:
            modeActive.TURNASSIST = active;
            break;
        case mode::ModeEnum::NAVLAUNCH:
            modeActive.NAVLAUNCH = active;
            break;
        case mode::ModeEnum::AUTOTRIM:
            modeActive.AUTOTRIM = active;
            break;
        default:
            break;
        }
    }
    
    void addMode(mode::Mode mode){
        modes.push_back(mode);
    }

    std::vector<mode::Mode> getMode(){
        return modes;
    }


private:
    int data; // Datele pe care dorești să le păstrezi
    mode::ModeActive modeActive;
    std::vector<mode::Mode> modes;
   
    senzor::Senzor senzor;
    DataController() {} // Constructor privat

    // Prevenirea copierii
    DataController(DataController const &);
    void operator=(DataController const &);
};

#endif // DATACONTROLLER_H