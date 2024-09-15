/**
 ********************************************************************************
 * @file    espnow_link.c
 * @author  Andrew Bevelhymer
 * @date    2024/09/13
 * @brief   Implements high level interface for esp now communications
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "esp_mac.h"

#include "espnow_link.h"
#include "logging.h"
#include "gpio.h"

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define TAG "ESPNOW_LINK_EXAMPLE"

/************************************
 * STATIC VARIABLES
 ************************************/
static uint8_t mac1[6] = {0x40, 0x4C, 0xCA, 0x4D, 0xDF, 0x80};
static uint8_t mac2[6] = {0x40, 0x4C, 0xCA, 0x4D, 0x67, 0xEC};

/************************************
 * STATIC FUNCTIONS
 ************************************/
void app_main(void)
{
    /* Initialize gpio */
    gpio_setup_pin(LED_PIN, GPIO_PIN_OUTPUT);
    GPIO_VALUE_T current_led = GPIO_PIN_ON;

    /* Initialize espnow link */
    espnow_link_init();

    /* get mac and determine whether to register mac1 or mac2 as a peer */
    uint8_t device_mac[6U];
    espnow_link_get_device_mac(device_mac);
    logging_log(LOG_LEVEL_INFO, TAG, "Device mac = " MACSTR, MAC2STR(device_mac));
    bool is_mac1 = (memcmp(device_mac, mac1, 6U) == 0U);
    uint8_t *peer_mac = is_mac1 ? mac2 : mac1;

    /* register peer */
    espnow_link_register_peer(peer_mac);

    /* send 250 messages to peer*/
    char send_buffer[250U];
    for(int i = 0; i < 2000; i++)
    {
        sprintf(send_buffer, "Hello, message %d", i);
        vTaskDelay(pdMS_TO_TICKS(100U));

        gpio_set_pin_level(LED_PIN, current_led);
        current_led = !current_led;
        logging_log(LOG_LEVEL_VERBOSE, TAG, "set led to status %d", current_led);
    }

    espnow_link_close();

}
