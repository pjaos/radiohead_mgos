#include "mgos.h"
#include <RHReliableDatagram.h>
#include <RH_Serial.h>
#include <RH_NRF905.h>

extern "C" {
    enum mgos_app_init_result mgos_app_init(void);
    IRAM static void receive_msg_cb(void * pvParameters);
}

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

#define BROADCAST_PERIOD_MS 1000
#define MSG_POLL_PERIOD_MS BROADCAST_PERIOD_MS/2

//!!! Don't put these buffers on the stack:
uint8_t data[] = "And hello back to you";
uint8_t buf[RH_SERIAL_MAX_MESSAGE_LEN];

/**
 * @brief A timer callback (called every MSG_POLL_PERIOD_MS) to check for
 * message from a client and send a response back.
 * @param arg Arguments passed to the callback (Null in this case).
 */
IRAM static void receive_msg_cb(void * pvParameters)
{
  RHReliableDatagram *manager = (RHReliableDatagram *)pvParameters;

  if( !manager ) {
      return;
  }

  if( manager->available() > 0 ) {
      uint8_t len = sizeof(buf);
      uint8_t from;
      if (manager->recvfromAck(buf, &len, &from)) {
          Serial.print("got request from : 0x");
          Serial.print(from, HEX);
          Serial.print(": ");
          Serial.println((char*)buf);

        // Send a reply back to the originator client
        if (!manager->sendtoWait(data, sizeof(data), from)) {
            LOG(LL_INFO, ("%s: send reply to id=%d failed",__FUNCTION__, from) );
        }

      }
  }
}

enum mgos_app_init_result mgos_app_init(void) {
    static RH_NRF905 driver(mgos_sys_config_get_rh_nrf_trx_chip_enable(),
                     mgos_sys_config_get_rh_nrf_tx_enable(),
                     SPI.getCSGpio(),
                     hardware_spi);
    static RHReliableDatagram manager(driver, SERVER_ADDRESS);

    if( manager.init() ) {
        mgos_set_timer(MSG_POLL_PERIOD_MS, MGOS_TIMER_REPEAT, receive_msg_cb, (void*)&manager);
    }
    else {
        Serial.println("init failed");
    }

    return MGOS_APP_INIT_SUCCESS;
}

