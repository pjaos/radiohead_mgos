#include "mgos.h"
#include "mgos_spi.h"
#include <RHReliableDatagram.h>
#include <RH_Serial.h>
#include <RH_CC110.h>

extern "C" {
    enum mgos_app_init_result mgos_app_init(void);
    static void tx_broadcast_cb(void *arg);
}

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 255 //Broadcast address
#define BROADCAST_PERIOD_MS 1000

uint8_t data[] = "Hello World!";

uint8_t txCount=0;

/**
 * @brief A timer callback to send the broadcast message.
 * @param arg Arguments passed to the callback (Null in this case).
 */
static void tx_broadcast_cb(void *arg) {
    LOG(LL_INFO, ("%s: Client TX",__FUNCTION__) );

    RHReliableDatagram *manager = (RHReliableDatagram *)arg;

    manager->sendtoWait(data, sizeof(data), SERVER_ADDRESS);

    (void) arg;
}

enum mgos_app_init_result mgos_app_init(void) {
    static RH_CC110 driver(SPI.getCSGpio(),
                        mgos_sys_config_get_rh_spi_int_gpio(),
                        false,
                        hardware_spi);
    static RHReliableDatagram manager(driver, CLIENT_ADDRESS);

    if( manager.init() ) {
        driver.setFrequency(435.0);
        driver.setTxPower(RH_CC110::TransmitPowerM30dBm);
        enableInterupt( mgos_sys_config_get_rh_spi_int_gpio() );
        mgos_set_timer(BROADCAST_PERIOD_MS, MGOS_TIMER_REPEAT, tx_broadcast_cb, (void*)&manager);
    }
    else {
        LOG(LL_INFO, ("init failed") );
    }

    return MGOS_APP_INIT_SUCCESS;
}
