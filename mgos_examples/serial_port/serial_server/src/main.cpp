#include "mgos.h"
#include <RHReliableDatagram.h>
#include <RH_Serial.h>

extern "C" {
    enum mgos_app_init_result mgos_app_init(void);
}

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2
//#define SERVER_ADDRESS 255 //Broadcast address

#define MSG_POLL_PERIOD_MS 1000
#define MEM_USAGE_PERIOD_MS 2500

uint8_t data[] = "And hello back to you";
// Dont put this on the stack:
uint8_t buf[RH_SERIAL_MAX_MESSAGE_LEN];

/**
 * @brief A timer callback (called every MSG_POLL_PERIOD_MS) to check for
 * message from a client and send a response back.
 * @param arg Arguments passed to the callback (Null in this case).
 */
static void uart_dispatcher(int uart_no, void *arg) {
//static void rh_server_msg_cb(void *arg) {

  RHReliableDatagram *manager = (RHReliableDatagram *)arg;

  if( !manager ) {
      return;
  }

  if( manager->available() > 0 ) {

      uint8_t len = sizeof(buf);
      uint8_t from;
      if (manager->recvfromAck(buf, &len, &from)) {
          Serial.print("got request from : 0x");
          Serial.print(from, HEX);
          Serial.print(": ");
          Serial.println((char*)buf);

        // Send a reply back to the originator client
        if (!manager->sendtoWait(data, sizeof(data), from)) {
            Serial.println("sendtoWait failed");
        }

      }
  }

  (void) uart_no;
  (void) arg;
}


//Instantiate the serial driver instance
RH_Serial driver(Serial);

//Instantiate a manager instance.
RHReliableDatagram manager(driver, SERVER_ADDRESS);

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
        mgos_uart_set_dispatcher(RH_SERIAL_PORT, uart_dispatcher, (void*)&manager);
    }
    else {
        Serial.println("init failed");
    }

    return MGOS_APP_INIT_SUCCESS;
}

