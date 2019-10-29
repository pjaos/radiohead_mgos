#include "mgos.h"
#include <RHReliableDatagram.h>
#include <RH_Serial.h>

extern "C" {
    enum mgos_app_init_result mgos_app_init(void);
}

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 255 //Broadcast address
#define BROADCAST_PERIOD_MS 1000

//!!! Dont put these on the stack:
uint8_t data[] = "Hello World!";
uint8_t buf[RH_SERIAL_MAX_MESSAGE_LEN];

/**
 * @brief A timer callback to send the broadcast messgae.
 * @param arg Arguments passed to the callback (Null in this case).
 */
static void tx_broadcast_poll_timer_cb(void *arg) {

  RHReliableDatagram *manager = (RHReliableDatagram *)arg;

  LOG(LL_INFO, ("%s: Client TX",__FUNCTION__) );
  manager->sendtoWait(data, sizeof(data), SERVER_ADDRESS);

  (void) arg;
}

enum mgos_app_init_result mgos_app_init(void) {
    static RH_Serial driver(Serial);
    static RHReliableDatagram manager(driver, CLIENT_ADDRESS);

    Serial.begin( mgos_sys_config_get_rh_serial_baud() );

    if( manager.init() ) {
        mgos_set_timer(BROADCAST_PERIOD_MS, MGOS_TIMER_REPEAT, tx_broadcast_poll_timer_cb, (void*)&manager);
    }
    else {
        LOG(LL_INFO, ("init failed") );
    }
    return MGOS_APP_INIT_SUCCESS;
}
