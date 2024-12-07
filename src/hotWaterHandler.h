/**
 * @file hotWaterHandler.h
 *
 * @brief Handler for digital hot water switch
 */

enum hotWaterState {
    kHotWaterIdle = 10,
    kHotWaterRunning = 20
};

hotWaterState currHotWaterState = kHotWaterIdle;

uint8_t currStateHotWaterSwitch;
int8_t hotWaterOn = 0;

void checkHotWaterSwitch() {
    if (FEATURE_HOTWATERSWITCH) {
        uint8_t hotWaterSwitchReading = hotWaterSwitch->isPressed();

        if (HOTWATERSWITCH_TYPE == Switch::TOGGLE) {
            // Set HotWaterState to kHotWaterRunning when waterswitch is HIGH and no allowed maschinestate is active
            if (hotWaterSwitchReading == HIGH && machineState != kBrew && !(machineState > kSteam)) {
                currHotWaterState = kHotWaterRunning;
                LOG(DEBUG, "HotWater pressed currWaterState = kHotWaterRunning");
            }

            if (hotWaterSwitchReading == LOW && hotWaterOn == 1) {
                currHotWaterState = kHotWaterIdle;
                LOG(DEBUG, "HotWater released currWaterState = kHotWaterIdle");
            }
        }
        else if (HOTWATERSWITCH_TYPE == Switch::MOMENTARY) {
            if (hotWaterSwitchReading != currStateHotWaterSwitch) {
                currStateHotWaterSwitch = hotWaterSwitchReading;

                if (currStateHotWaterSwitch == HIGH) {
                    if (hotWaterOn == 0 && machineState != kBrew && !(machineState > kSteam)) {
                        currHotWaterState = kHotWaterRunning;
                        LOG(DEBUG, "HotWater pressed currWaterState = kHotWaterRunning");
                    }
                    else {
                        currHotWaterState = kHotWaterIdle;
                        LOG(DEBUG, "HotWater released currWaterState = kHotWaterIdle");
                    }
                }
            }
        }
    }
}

bool hotWater() {
    unsigned long currentMillisTemp = millis();
    checkHotWaterSwitch();
    LOG(TRACE, "currHotWaterState = "+currHotWaterState);
    if (currHotWaterState == kHotWaterIdle) {
        hotWaterOn = 0;
        pumpRelay.off();
        LOG(TRACE, "hotWater() turning off pump");
    }

    if (currHotWaterState == kHotWaterRunning) {
            hotWaterOn = 1;
            pumpRelay.on();
            LOG(TRACE, "hotWater() turning on pump");
    }

    return (hotWaterOn == 1);
}