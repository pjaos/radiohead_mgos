#include "mgos.h"
#include <RHReliableDatagram.h>
#include <RH_Serial.h>

extern "C" {
    enum mgos_app_init_result mgos_app_init(void);
}

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 255 //Broadcast address
#define BROADCAST_PERIOD_MS 100
#define RX_BUFFER_SIZE 80
uint8_t rx_buf[RX_BUFFER_SIZE];

/**
 * @brief A timer callback to receive broadcast messages.
 * @param arg Arguments passed to the callback (Null in this case).
 */
static void rx_broadcast_poll_timer_cb(void *arg) {
    uint8_t from, to, id, flags, rx_byte_count;
    RHReliableDatagram *manager = (RHReliableDatagram *)arg;

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

//Instantiate the serial driver instance
RH_Serial driver(Serial);

//Instantiate a manager instance.
RHReliableDatagram manager(driver, CLIENT_ADDRESS);

/**
 * @brief Setup the RH driver and manager.
 * @return 0 = no error, -1 = error.
 */
static int setup()
{
  int returnCode = 0;

  // Configure the port RH_Serial will use:
  Serial.begin( mgos_sys_config_get_rh_serial_baud() );

  if (!manager.init()) {
      returnCode = -1;
  }

  return returnCode;
}

enum mgos_app_init_result mgos_app_init(void) {
    if( !setup() ) {
        mgos_set_timer(BROADCAST_PERIOD_MS, MGOS_TIMER_REPEAT, rx_broadcast_poll_timer_cb, (void*)&manager);
    }
    else {
        Serial.println("init failed");
    }
    return MGOS_APP_INIT_SUCCESS;
}
