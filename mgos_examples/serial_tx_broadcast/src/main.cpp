#include "mgos.h"
#include <RHReliableDatagram.h>
#include <RH_Serial.h>

extern "C" {
    enum mgos_app_init_result mgos_app_init(void);
}

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 255 //Broadcast address
#define BROADCAST_PERIOD_MS 1000

//uint8_t data[] = "U";
uint8_t data[] = "Hello World!";
// Dont put this on the stack:
uint8_t buf[RH_SERIAL_MAX_MESSAGE_LEN];
#define RX_BUFFER_SIZE 80
uint8_t rx_buf[RX_BUFFER_SIZE];

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
        mgos_set_timer(BROADCAST_PERIOD_MS, MGOS_TIMER_REPEAT, tx_broadcast_poll_timer_cb, (void*)&manager);
    }
    else {
        Serial.println("init failed");
    }
    return MGOS_APP_INIT_SUCCESS;
}
