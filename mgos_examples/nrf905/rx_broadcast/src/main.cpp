#include "mgos.h"
#include "mgos_spi.h"
#include <RHReliableDatagram.h>
#include <RH_Serial.h>
#include <RH_NRF905.h>

extern "C" {
    enum mgos_app_init_result mgos_app_init(void);
    static void rx_broadcast_poll_timer_cb(void *arg);
}

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 255 //Broadcast address
#define BROADCAST_PERIOD_MS 1000
#define MSG_POLL_PERIOD_MS BROADCAST_PERIOD_MS/2

#define RX_BUFFER_SIZE 32

//!!! Dont put these on the stack:
uint8_t buf[RH_SERIAL_MAX_MESSAGE_LEN];
uint8_t rx_buf[RX_BUFFER_SIZE];

/**
 * @brief A timer callback to receive broadcast messages.
 * @param arg Arguments passed to the callback (Null in this case).
 */
static void rx_broadcast_poll_timer_cb(void *arg) {
    uint8_t from, to, id, flags, rx_byte_count;
    RHReliableDatagram *manager = (RHReliableDatagram *)arg;

    rx_byte_count = RX_BUFFER_SIZE;
    if (manager->recvfrom(rx_buf, &rx_byte_count, &from, &to, &id, &flags)) // Discards the message
    {
        LOG(LL_INFO, ("%s: Client RX",__FUNCTION__) );
        LOG(LL_INFO, ("from:  %02x",from) );
        LOG(LL_INFO, ("to:    %02x",to) );
        LOG(LL_INFO, ("id:    %02x",id) );
        LOG(LL_INFO, ("flags: %02x",flags) );
        LOG(LL_INFO, ("rx_buf:%s",(char*)rx_buf) );
    }

    (void) arg;
}

enum mgos_app_init_result mgos_app_init(void) {
    static RH_NRF905 driver(mgos_sys_config_get_rh_nrf_trx_chip_enable(),
                     mgos_sys_config_get_rh_nrf_tx_enable(),
                     SPI.getCSGpio(),
                     hardware_spi);
    static RHReliableDatagram manager(driver, CLIENT_ADDRESS);

    if( manager.init() ) {
        mgos_set_timer(MSG_POLL_PERIOD_MS, MGOS_TIMER_REPEAT, rx_broadcast_poll_timer_cb, (void*)&manager);
    }
    else {
        Serial.println("init failed");
    }

    return MGOS_APP_INIT_SUCCESS;
}
