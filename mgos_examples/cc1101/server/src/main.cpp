#include "mgos.h"
#include <RHReliableDatagram.h>
#include <RH_Serial.h>
#include <RH_CC110.h>

extern "C" {
    enum mgos_app_init_result mgos_app_init(void);
//    IRAM static void receive_msg_cb(void * pvParameters);
}

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

#define BROADCAST_PERIOD_MS 1000
#define MSG_POLL_PERIOD_MS BROADCAST_PERIOD_MS/4

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
    static int *mgrDriverRefs = (int*)pvParameters;
    RHReliableDatagram *manager = (RHReliableDatagram *)mgrDriverRefs[0];
//    RH_CC110 *driver = (RH_CC110 *)mgrDriverRefs[1];

    if( !manager ) {
        return;
    }

    if( manager->available() > 0 ) {
        uint8_t rx_byte_count = 13; //This is the length of the TX 'Hello World!' message including null terminator character.
        uint8_t from;
        if (manager->recvfromAck(buf, &rx_byte_count, &from)) {
            LOG(LL_INFO, ("%s: got request from : 0x%02x: %s",__FUNCTION__, from, (char*)buf) );
            /*
            if( driver != NULL ) {
                LOG(LL_INFO, ("%s: RX RF Power: %d dBm",__FUNCTION__, driver->lastRssi()) );
            }
            */
            // Send a reply back to the originator client
            if (!manager->sendtoWait(data, sizeof(data), from)) {
                LOG(LL_INFO, ("%s: send reply to id=%d failed",__FUNCTION__, from) );
            }
        }
    }
}

enum mgos_app_init_result mgos_app_init(void) {
    static RH_CC110 driver(SPI.getCSGpio(),
                        mgos_sys_config_get_rh_spi_int_gpio(),
                        false,
                        hardware_spi);
    static RHReliableDatagram manager(driver, SERVER_ADDRESS);
    static int mgrDriverRefs[2];
    mgrDriverRefs[0]=(int)&manager;
    mgrDriverRefs[1]=(int)&driver;

    if( manager.init() ) {
        enableInterupt( mgos_sys_config_get_rh_spi_int_gpio() );
        mgos_set_timer(MSG_POLL_PERIOD_MS, MGOS_TIMER_REPEAT, receive_msg_cb, (void*)mgrDriverRefs);
    }
    else {
        LOG(LL_INFO, ("init failed") );
    }
    return MGOS_APP_INIT_SUCCESS;
}

