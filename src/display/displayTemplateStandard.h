/**
 * @file displayTemplateStandard.h
 *
 * @brief Standard display template
 *
 */

#pragma once
#include "displayCommon.h"

/**
 * @brief Send data to display
 */
void printScreen() {

    // Show shot timer:
    if (displayShottimer()) {
        // Display was updated, end here
        return;
    }

    // Print the machine state
    if (displayMachineState()) {
        // Display was updated, end here
        return;
    }

    // If no specific machine state was printed, print default:

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont11_tf); // set font

    displayStatusbar();

    u8g2.setCursor(34, 16);
    u8g2.print(langstring_current_temp);
    u8g2.setCursor(84, 16);
    u8g2.print(temperature, 1);
    u8g2.setCursor(115, 16);
    u8g2.print((char)176);
    u8g2.print("C");
    u8g2.setCursor(34, 26);
    u8g2.print(langstring_set_temp);
    u8g2.setCursor(84, 26);
    u8g2.print(setpoint, 1);
    u8g2.setCursor(115, 26);
    u8g2.print((char)176);
    u8g2.print("C");

    displayThermometerOutline(4, 62);

    // Draw current temp in thermometer
    if (fabs(temperature - setpoint) < 0.3) {
        if (isrCounter < 500) {
            drawTemperaturebar(8, 50, 30);
        }
    }
    else {
        drawTemperaturebar(8, 50, 30);
    }

// Brew time
#if (FEATURE_BREWSWITCH == 1)

    if (featureBrewControl) {
        // Shown brew time while machine is brewing and after the brewing during shotTimerDisplayDelay
        if (machineState == kBrew || (millis() - lastBrewTimeMillis) < (shotTimerDisplayDelay * 1000)) {
            u8g2.setFontMode(1);
            u8g2.setCursor(34, 36);
            u8g2.print(langstring_brew);
            u8g2.setCursor(84, 36);
            u8g2.print(timeBrewed / 1000, 0);
            u8g2.print("/");
            u8g2.print(totalBrewTime / 1000, 0);
            u8g2.print(" s");
        }

        // Flush time

        // Shown flush time while machine is flushing
        if (machineState == kManualFlush) {
            u8g2.setDrawColor(0);
            u8g2.drawBox(34, 36, 100, 10);
            u8g2.setDrawColor(1);
            u8g2.setCursor(34, 36);
            u8g2.print(langstring_manual_flush);
            u8g2.setCursor(84, 36);
            u8g2.print(timeBrewed / 1000, 0);
            u8g2.print(" s");
        }
    }
    else {
        // Brew Timer with optocoupler

        // Shown brew time while machine is brewing and after the brewing during shotTimerDisplayDelay
        if (machineState == kBrew || (millis() - lastBrewTimeMillis) < (shotTimerDisplayDelay * 1000)) {
            u8g2.setCursor(34, 36);
            u8g2.print(langstring_brew);
            u8g2.setCursor(84, 36);
            u8g2.print(timeBrewed / 1000, 0);
            u8g2.print(" s");
        }
    }
#endif

    // PID values over heat bar
    u8g2.setCursor(38, 47);

    u8g2.print(bPID.GetKp(), 0);
    u8g2.print("|");

    if (bPID.GetKi() != 0) {
        u8g2.print(bPID.GetKp() / bPID.GetKi(), 0);
    }
    else {
        u8g2.print("0");
    }

    u8g2.print("|");
    u8g2.print(bPID.GetKd() / bPID.GetKp(), 0);
    u8g2.setCursor(96, 47);

    if (pidOutput < 99) {
        u8g2.print(pidOutput / 10, 1);
    }
    else {
        u8g2.print(pidOutput / 10, 0);
    }

    u8g2.print("%");

    // Show heater output in %
    displayProgressbar(pidOutput / 10, 30, 60, 98);

    u8g2.sendBuffer();
}
