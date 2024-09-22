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
// #include "gpio.h"
#include "driver/gpio.h"
#include "wt20_protocol.h"

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define TAG "ESPNOW_LINK_EXAMPLE"

/************************************
 * STATIC VARIABLES
 ************************************/
static uint8_t mac1[6] = {0x40, 0x4C, 0xCA, 0x4D, 0xDF, 0x80};
static uint8_t mac2[6] = {0x40, 0x4C, 0xCA, 0x4D, 0x67, 0xEC};

// static uint32_t gpio_level = GPIO_PIN_OFF;
static uint32_t gpio_level = 0U;

/************************************
 * STATIC FUNCTIONS
 ************************************/

void wt20_protocol_task(void* params)
{

    printf("Test\n");
    WT20_MSG_T msg;
    WT20_ERR_T ret;

    while (1U)
    {
        /* get message */
        ret = wt20_protocol_function(&msg);

        if (ret == WT20_ERR_NONE)
        {
            logging_log(
                LOG_LEVEL_VERBOSE,
                TAG,
                "Message received from mac " MACSTR ". Command = %d", msg.command
            );

            /* process message */
            switch (msg.command)
            {
            case WT20_COMMAND_TOGGLE_LED:
                // gpio_set_pin_level(LED_PIN, gpio_level);
                gpio_set_level(2U, gpio_level);
                // gpio_level = (gpio_level == GPIO_PIN_ON) ? GPIO_PIN_OFF : GPIO_PIN_ON;
                gpio_level = !gpio_level;
                break;

            case WT20_COMMAND_SEND_PAYLOAD:
                printf("Message: %s\n", (char*)msg.payload);
            
            default:
                break;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1U));
    }
}

void app_main(void)
{
    /* Initialize gpio */
    // gpio_setup_pin(LED_PIN, GPIO_PIN_OUTPUT);
    gpio_reset_pin(2U);
    gpio_set_direction(2U, GPIO_MODE_OUTPUT);

    /* Initialize WT20 */
    wt20_init();

    /* get mac and determine whether to register mac1 or mac2 as a peer */
    uint8_t device_mac[6U];
    wt20_get_device_mac(device_mac);
    logging_log(LOG_LEVEL_INFO, TAG, "Device mac = " MACSTR, MAC2STR(device_mac));
    bool is_mac1 = (memcmp(device_mac, mac1, 6U) == 0U);
    uint8_t *peer_mac = is_mac1 ? mac2 : mac1;

    /* add contact */
    wt20_add_contact(peer_mac);

    /* start protocol */
    xTaskCreate(
        wt20_protocol_task,
        "wt20_protocol_task",
        4096, /* 4kb stack */
        NULL,
        1,
        NULL
    );

    /* send 250 messages to peer*/
    // char send_buffer[250U];
    for(int i = 0; i < 2000; i++)
    {
        wt20_write(peer_mac, WT20_COMMAND_TOGGLE_LED, NULL, 0U);
        vTaskDelay(pdMS_TO_TICKS(10U));

        // gpio_set_pin_level(LED_PIN, current_led);
        // current_led = !current_led;
        // logging_log(LOG_LEVEL_VERBOSE, TAG, "set led to status %d", current_led);
    }

    espnow_link_close();

}
