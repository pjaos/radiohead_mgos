#include "mgos.h"
#include <RHReliableDatagram.h>
#include <RH_Serial.h>

extern "C" {
    static void rx_broadcast_poll_timer_cb(void *arg);
    enum mgos_app_init_result mgos_app_init(void);
}

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 255 //Broadcast address
#define BROADCAST_PERIOD_MS 1000
#define MSG_POLL_PERIOD_MS BROADCAST_PERIOD_MS/4

//!!! Dont put these on the stack:
uint8_t rx_buf[RH_SERIAL_MAX_MESSAGE_LEN];

/**
 * @brief A timer callback to receive broadcast messages.
 * @param arg Arguments passed to the callback, a RHReliableDatagram instance.
 */
static void rx_broadcast_poll_timer_cb(void *arg) {
    uint8_t from, to, id, flags, rx_byte_count;
    RHReliableDatagram *manager = (RHReliableDatagram *)arg;

    rx_byte_count = RH_SERIAL_MAX_MESSAGE_LEN;

    if (manager->recvfrom(rx_buf, &rx_byte_count, &from, &to, &id, &flags)) // Discards the message
    {
        LOG(LL_INFO, ("%s: Client RX",__FUNCTION__) );
        LOG(LL_INFO, ("from:  %02x",from) );
        LOG(LL_INFO, ("to:    %02x",to) );
        LOG(LL_INFO, ("id:    %02x",id) );
        LOG(LL_INFO, ("flags: %02x",flags) );
        LOG(LL_INFO, ("rx_buf:%s",(char*)rx_buf) );
    }
}

enum mgos_app_init_result mgos_app_init(void) {
    static RH_Serial driver(Serial);
    static RHReliableDatagram manager(driver, SERVER_ADDRESS);

    Serial.begin( mgos_sys_config_get_rh_serial_baud() );

    if( manager.init() ) {
        mgos_set_timer(MSG_POLL_PERIOD_MS, MGOS_TIMER_REPEAT, rx_broadcast_poll_timer_cb, (void*)&manager);
    }
    else {
        Serial.println("init failed");
    }
    return MGOS_APP_INIT_SUCCESS;
}
