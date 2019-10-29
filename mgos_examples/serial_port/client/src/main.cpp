#include "mgos.h"
#include <RHReliableDatagram.h>
#include <RH_Serial.h>

extern "C" {
    enum mgos_app_init_result mgos_app_init(void);
    IRAM static void send_msg_cb(void * pvParameters);
}

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

#define BROADCAST_PERIOD_MS 1000

//!!! Dont put these on the stack:
uint8_t data[] = "Hello World!";
uint8_t buf[RH_SERIAL_MAX_MESSAGE_LEN];

/**
 * @brief A timer callback (called every MSG_POLL_PERIOD_MS) to send
 * a message to the server
 * @param arg Arguments passed to the callback, a RHReliableDatagram instance.
 */
IRAM static void send_msg_cb(void * arg) {
    RHReliableDatagram *manager = (RHReliableDatagram *)arg;

    LOG(LL_INFO, ("%s: Client TX",__FUNCTION__) );

    // Send a message to manager_server
    if (manager->sendtoWait(data, sizeof(data), SERVER_ADDRESS))
    {
      // Now wait for a reply from the server
      uint8_t len = sizeof(buf);
      uint8_t from;
      if (manager->recvfromAckTimeout(buf, &len, 2000, &from))
      {
        LOG(LL_INFO, ("%s: got reply from : 0x%02x: %s",__FUNCTION__, from, (char*)buf) );
      }
      else
      {
        LOG(LL_INFO, ("No reply, is serial_reliable_datagram_server running?") );
      }
    }
    else
    {
      LOG(LL_INFO, ("sendtoWait failed") );
    }
}

enum mgos_app_init_result mgos_app_init(void) {
    static RH_Serial driver(Serial);
    static RHReliableDatagram manager(driver, CLIENT_ADDRESS);

    Serial.begin( mgos_sys_config_get_rh_serial_baud() );

    if( manager.init() ) {
        mgos_set_timer(BROADCAST_PERIOD_MS, MGOS_TIMER_REPEAT, send_msg_cb, (void*)&manager);
    }
    else {
        LOG(LL_INFO, ("init failed") );
    }
    return MGOS_APP_INIT_SUCCESS;
}
