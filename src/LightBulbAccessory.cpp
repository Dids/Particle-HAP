#include "LightBulbAccessory.h"


#include "homekit/HKConnection.h"


#include <set>

#ifdef PARTICLE_COMPAT
#include "../example/HKTester/HKTester/Particle_Compat/particle_compat.h"
#else
#include <Particle.h>
#endif
#include <string.h>
#include <stdlib.h>
#include "rgb2hsv.h"
#include <cstdio>

RgbColor c;

std::string format(const std::string& format, ...)
{
    va_list args;
    va_start (args, format);
    size_t len = vsnprintf(NULL, 0, format.c_str(), args);
    va_end (args);
    std::vector<char> vec(len + 1);
    va_start (args, format);
    vsnprintf(&vec[0], len + 1, format.c_str(), args);
    va_end (args);
    return &vec[0];
}

void powerTrackable (bool oldValue, bool newValue, HKConnection *sender) {
    RGB.control(true);
    c.r = newValue ? (c.r > 0 ? c.r : 255) : 0;
    c.g = newValue ? (c.g > 0 ? c.g : 255) : 0;
    c.b = newValue ? (c.b > 0 ? c.b : 255) : 0;

    RGB.color(c.r, c.g, c.b);
}

void brightTrackable (int oldValue, int newValue, HKConnection *sender) {
  RGB.control(true);
    c.r = (c.r * newValue) / 255;
    c.g = (c.g * newValue) / 255;
    c.b = (c.b * newValue) / 255;

    RGB.color(c.r, c.g, c.b);
}

std::string getLedHue (HKConnection *sender) {
    return format("%d",((RgbToHsv(c).h) * 360) / 255);
}

void setLedHue (int oldValue, int newValue, HKConnection *sender) {
  RGB.control(true);
    HsvColor hsv = RgbToHsv(c);
    hsv.h = (255 * newValue) / 360;
    c = HsvToRgb(hsv);
    RGB.color(c.r, c.g, c.b);
}

std::string getLedBrightness (HKConnection *sender) {
    return format("%d",((RgbToHsv(c).v) * 100) / 255);
}

void setLedBrightness (int oldValue, int newValue, HKConnection *sender) {
  RGB.control(true);
    HsvColor hsv = RgbToHsv(c);
    hsv.v = (255 * newValue) / 100;
    c = HsvToRgb(hsv);
    RGB.color(c.r, c.g, c.b);
}

std::string getLedSaturation (HKConnection *sender) {
    return format("%d",((RgbToHsv(c).s) * 100) / 255);
}

void setLedSaturation (int oldValue, int newValue, HKConnection *sender) {
    HsvColor hsv = RgbToHsv(c);
    hsv.s = (255 * newValue) / 100;
    c = HsvToRgb(hsv);
    RGB.color(c.r, c.g, c.b);
}

void lightIdentify(bool oldValue, bool newValue, HKConnection *sender) {
    Serial.printf("Start Identify Light\n");
}

void LightBulbAccessory::initAccessorySet() {
    Accessory *lightAcc1 = new Accessory();

    //Add Light
    AccessorySet *accSet = &AccessorySet::getInstance();
    addInfoServiceToAccessory(lightAcc1, "Bulb name", "Vendor name", "Model name", "1", &lightIdentify);
    accSet->addAccessory(lightAcc1);

    Service *lightService1 = new Service(serviceType_lightBulb);
    lightAcc1->addService(lightService1);

    stringCharacteristics *lightServiceName1 = new stringCharacteristics(charType_serviceName, premission_read, 0);
    lightServiceName1->characteristics::setValue("Bulb name");
    lightAcc1->addCharacteristics(lightService1, lightServiceName1);

    boolCharacteristics *powerState1 = new boolCharacteristics(charType_on, premission_read|premission_write|premission_notify);
    powerState1->characteristics::setValue("true");
    powerState1->valueChangeFunctionCall = &powerTrackable;
    lightAcc1->addCharacteristics(lightService1, powerState1);

    intCharacteristics *brightnessState1 = new intCharacteristics(charType_brightness, premission_read|premission_write, 0, 100, 1, unit_percentage);
    brightnessState1->characteristics::setValue("50");
    brightnessState1->valueChangeFunctionCall = &brightTrackable;
    lightAcc1->addCharacteristics(lightService1, brightnessState1);

    intCharacteristics *ledSaturationState = new intCharacteristics(charType_saturation, premission_read|premission_write|premission_notify, 0, 100, 1, unit_percentage);
    ledSaturationState->perUserQuery = &getLedSaturation;
    ledSaturationState->valueChangeFunctionCall = &setLedSaturation;
    lightAcc1->addCharacteristics(lightService1, ledSaturationState);

    intCharacteristics *ledHueState = new intCharacteristics(charType_hue, premission_read|premission_write|premission_notify, 0, 360, 1, unit_arcDegree);
    ledHueState->perUserQuery = &getLedHue;
    ledHueState->valueChangeFunctionCall = &setLedHue;
    lightAcc1->addCharacteristics(lightService1, ledHueState);
};