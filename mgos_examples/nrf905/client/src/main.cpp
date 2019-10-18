#include "mgos.h"
#include <RHReliableDatagram.h>
#include <RH_Serial.h>
#include <RH_NRF905.h>

extern "C" {
    enum mgos_app_init_result mgos_app_init(void);
    IRAM static void send_msg_cb(void * pvParameters) ;
}

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

#define BROADCAST_PERIOD_MS 1000

//!!! Don't put these buffers on the stack:
uint8_t buf[RH_SERIAL_MAX_MESSAGE_LEN];
uint8_t data[] = "Hello World!";

/**
 * @brief A timer callback (called every MSG_POLL_PERIOD_MS) to send a message to the server.
 * @param arg Arguments passed to the callback. In this case a RHReliableDatagram instance.
 */
IRAM static void send_msg_cb(void * pvParameters)
{
    RHReliableDatagram *manager = (RHReliableDatagram *)pvParameters;

    LOG(LL_INFO, ("%s: Client TX",__FUNCTION__) );

    // Send a message to manager_server
    if (manager->sendtoWait(data, sizeof(data), SERVER_ADDRESS))
    {
      // Now wait for a reply from the server
      uint8_t len = sizeof(buf);
      uint8_t from;
      if (manager->recvfromAckTimeout(buf, &len, 2000, &from))
      {
        Serial.print("got reply from : 0x");
        Serial.print(from, HEX);
        Serial.print(": ");
        Serial.println((char*)buf);
      }
      else
      {
        Serial.println("No reply, is serial_reliable_datagram_server running?");
      }
    }
    else
    {
      Serial.println("sendtoWait failed");
    }

}

enum mgos_app_init_result mgos_app_init(void) {
    static RH_NRF905 driver(mgos_sys_config_get_rh_nrf_trx_chip_enable(),
                     mgos_sys_config_get_rh_nrf_tx_enable(),
                     SPI.getCSGpio(),
                     hardware_spi);
    static RHReliableDatagram manager(driver, CLIENT_ADDRESS);

    mgos_wdt_disable();

    if( manager.init() ) {
        mgos_set_timer(BROADCAST_PERIOD_MS, MGOS_TIMER_REPEAT, send_msg_cb, (void*)&manager);
    }
    else {
        Serial.println("init failed");
    }
    return MGOS_APP_INIT_SUCCESS;
}
