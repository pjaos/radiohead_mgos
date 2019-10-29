#include "mgos.h"
#include <RHReliableDatagram.h>
#include <RH_Serial.h>
#include <RH_CC110.h>

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
    static int *mgrDriverRefs = (int*)pvParameters;
    RHReliableDatagram *manager = (RHReliableDatagram *)mgrDriverRefs[0];
//    RH_CC110 *driver = (RH_CC110 *)mgrDriverRefs[1];

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
        /*
        if( driver != NULL ) {
            LOG(LL_INFO, ("%s: RX RF Power: %d dBm",__FUNCTION__, driver->lastRssi()) );
        }
        */
      }
      else
      {
        LOG(LL_INFO, ("%s: No reply, is serial_reliable_datagram_server running?",__FUNCTION__) );
      }
    }
    else
    {
      LOG(LL_INFO, ("%s: sendtoWait failed",__FUNCTION__) );
    }
}

enum mgos_app_init_result mgos_app_init(void) {
    static RH_CC110 driver(SPI.getCSGpio(),
                        mgos_sys_config_get_rh_spi_int_gpio(),
                        false,
                        hardware_spi);
    static RHReliableDatagram manager(driver, CLIENT_ADDRESS);
    static int mgrDriverRefs[2];
    mgrDriverRefs[0]=(int)&manager;
    mgrDriverRefs[1]=(int)&driver;

    if( manager.init() ) {
        driver.setTxPower(RH_CC110::TransmitPowerM30dBm);
        enableInterupt( mgos_sys_config_get_rh_spi_int_gpio() );
        mgos_set_timer(BROADCAST_PERIOD_MS, MGOS_TIMER_REPEAT, send_msg_cb, (void*)mgrDriverRefs);
    }
    else {
        LOG(LL_INFO, ("%s: manager.init() failed",__FUNCTION__) );
    }
    return MGOS_APP_INIT_SUCCESS;
}
