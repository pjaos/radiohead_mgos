#include "mgos.h"
#include <RHReliableDatagram.h>
#include <RH_Serial.h>

extern "C" {
    enum mgos_app_init_result mgos_app_init(void);
}

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2
//#define SERVER_ADDRESS 255 //Broadcast address

#define BROADCAST_PERIOD_MS 1000

//uint8_t data[] = "U";
uint8_t data[] = "Hello World!";
// Dont put this on the stack:
uint8_t buf[RH_SERIAL_MAX_MESSAGE_LEN];

IRAM static void send_msg_cb(void * pvParameters) {
    LOG(LL_INFO, ("%s",__FUNCTION__) );

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

//    vTaskDelete(NULL);
    (void) pvParameters;
}

/**
 * @brief A timer callback (called every BROADCAST_PERIOD_MS) to send a message to the radio
 * @param arg Arguments passed to the callback (Null in this case).
 */
static void sys_stats_timer_cb(void *arg) {

  RHReliableDatagram *manager = (RHReliableDatagram *)arg;

  send_msg_cb(manager);
  //log_mem_usage();
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
        mgos_set_timer(BROADCAST_PERIOD_MS, MGOS_TIMER_REPEAT, sys_stats_timer_cb, (void*)&manager);
    }
    else {
        Serial.println("init failed");
    }
    return MGOS_APP_INIT_SUCCESS;
}
