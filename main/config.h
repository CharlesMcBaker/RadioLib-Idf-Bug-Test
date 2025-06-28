#ifndef _RADIOLIB_EX_LORAWAN_CONFIG_H
#define _RADIOLIB_EX_LORAWAN_CONFIG_H

#include <RadioLib.h>

#include <string>
#include <stdio.h>

#include <cstdint>
#include <cstdio>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "EspHal.h"

#define RADIO_SCLK_PIN              5   // Check. //5
#define RADIO_MISO_PIN              19  // Check. // 19
#define RADIO_MOSI_PIN              27  // Check
#define RADIO_CS_PIN                18  // Check

#define RADIO_DIO0_PIN              26  // Check
#define RADIO_RST_PIN               23  // Check

#define RADIO_DIO1_PIN              -1  //33
// SX1276/78
#define RADIO_DIO2_PIN              -1  //21

// how often to send an uplink - consider legal & FUP constraints - see notes
const uint32_t uplinkIntervalSeconds = 4UL;    // minutes x seconds




// Replace this with your own implementation
const char* stateDecode(int state) {
    switch(state) {
        case 0: return "OK";
        case 1: return "ERROR";
        // Add more as needed
        default: return "UNKNOWN";
    }
}

void debug(bool failed, const char* message, int state, bool halt) {
    if (failed) {
        printf("%s - %s (%d)\n", message, stateDecode(state), state);
        while (halt) {
            vTaskDelay(1);  // 1 tick delay (tick = 1ms by default)
        }
    }
}

void print_rc(const char* comment, int rc) {
    printf("%s: %d\n", comment, rc);
}

// // helper function to display any issues
// void debug(bool failed, const __FlashStringHelper* message, int state, bool halt) {
//   if(failed) {
//     Serial.print(message);
//     Serial.print(" - ");
//     Serial.print(stateDecode(state));
//     Serial.print(" (");
//     Serial.print(state);
//     Serial.println(")");
//     while(halt) { delay(1); }
//   }
// }

// helper function to display a byte array
void arrayDump(const uint8_t* buffer, uint16_t len) {
    for (uint16_t c = 0; c < len; ++c) {
        if (buffer[c] < 0x10) {
            printf("0");
        }
        printf("%02X", buffer[c]);
    }
    printf("\n");
}
#endif