#define RADIOLIB_VERBOSE_ASSERT 1
#define RADIOLIB_DEBUG 1

#include <RadioLib.h>
#include "EspHal.h"
#include "config.h"
#include "esp_mac.h"




// RealMVP
#define RADIOLIB_LORAWAN_JOIN_EUI  0x0000000000000000
#define RADIOLIB_LORAWAN_DEV_EUI   0x0000405924EA6BEA
#define RADIOLIB_LORAWAN_APP_KEY   0x42, 0x27, 0x61, 0x43, 0x4e, 0xe8, 0xd1, 0x84, 0x92, 0x85, 0xb3, 0x48, 0x45, 0xc9, 0xe6, 0x48
#define RADIOLIB_LORAWAN_NWK_KEY   0x42, 0x27, 0x61, 0x43, 0x4e, 0xe8, 0xd1, 0x84, 0x92, 0x85, 0xb3, 0x48, 0x45, 0xc9, 0xe6, 0x48 


// // TestCRED
// #define RADIOLIB_LORAWAN_JOIN_EUI  0x0000000000000000   // joinEUI - previous versions of LoRaWAN called this AppEUI
// #define RADIOLIB_LORAWAN_DEV_EUI   0x000040C7B19F3A42
// #define RADIOLIB_LORAWAN_APP_KEY   0x00, 0x6d, 0x80, 0x7c, 0x5e, 0x2a, 0x6b, 0x47, 0x07, 0x40, 0x79, 0x9e, 0xcf, 0xbb, 0x15, 0x5b
// #define RADIOLIB_LORAWAN_NWK_KEY   0x00, 0x6d, 0x80, 0x7c, 0x5e, 0x2a, 0x6b, 0x47, 0x07, 0x40, 0x79, 0x9e, 0xcf, 0xbb, 0x15, 0x5b

uint64_t joinEUI =   RADIOLIB_LORAWAN_JOIN_EUI;
uint64_t devEUI  =   RADIOLIB_LORAWAN_DEV_EUI;
uint8_t appKey[] = { RADIOLIB_LORAWAN_APP_KEY };
uint8_t nwkKey[] = { RADIOLIB_LORAWAN_NWK_KEY };

const LoRaWANBand_t Region = EU433;
const uint8_t subBand = 0;  // For US915, change this to 2, otherwise leave on 0


// create a new instance of the HAL class
EspHal* hal = new EspHal(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);    // why pins again??
SX1276 radio = new Module(hal, RADIO_CS_PIN, RADIO_DIO0_PIN, RADIO_RST_PIN, RADIO_DIO1_PIN);
LoRaWANNode node(&radio, &Region, subBand);

void setup_lora() {

  int16_t rc;
  int16_t state;

    gpio_set_pull_mode(GPIO_NUM_26, GPIO_PULLUP_ONLY);

    state = radio.begin();
    debug(state != RADIOLIB_ERR_NONE, "Initialise radio failed", state, true);
    
    state = node.beginOTAA(joinEUI, devEUI, nwkKey, appKey);
    debug(state != RADIOLIB_ERR_NONE, "Initialise node failed", state, true);

    // hier gaat het mis
    state = node.activateOTAA();
    debug(state != RADIOLIB_LORAWAN_NEW_SESSION, "Join failed", state, true);
    
    node.setADR(true);
    rc = node.setTxPower(20);
    print_rc("setTxPower RC", rc);

    rc = radio.setSpreadingFactor(11);          
    print_rc("setSpreadingFactor RC", rc);
    
    rc = radio.setBandwidth(125.0);
    print_rc("setBandwidth RC", rc);
    
    rc = radio.setCodingRate(8);              
    print_rc("setCodingRate RC", rc);  
}

extern "C" void app_main(void) {

  setup_lora();
  hal->delay(1000);

      // main loop 
      while(1)
      {
        uint8_t payload_bytes[] = { 0xDE, 0xAD, 0xBE, 0xEF };

        // // Perform an uplink
        int16_t state = node.sendReceive(payload_bytes, sizeof(payload_bytes));
        debug(state < RADIOLIB_ERR_NONE, "Error in sendReceive", state, false);

        // Check if a downlink was received
        // (state 0 = no downlink, state 1/2 = downlink in window Rx1/Rx2)
        if (state > 0) {
          printf("Received a downlink\n");
          // digitalWrite(led_pin, HIGH);
          hal->delay(100); // Wait for 1 second
          //digitalWrite(led_pin, LOW);
          hal->delay(100); // Wait for 1 second
        } else {
          printf("No downlink received\n");
        }

        printf("Next uplink in %ld seconds\n", uplinkIntervalSeconds);

        // Wait until next uplink - observing legal & TTN FUP constraints
        hal->delay(uplinkIntervalSeconds * 1000UL);  // delay needs milli-seconds
      }
}