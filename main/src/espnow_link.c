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
#include "espnow_link.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include "esp_mac.h"
#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
#include "logging.h"

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define TAG "ESPNOW_LINK"
#define MAC_LENGTH_BYTES_D (6U)

/************************************
 * STATIC VARIABLES
 ************************************/
static uint8_t device_mac[MAC_LENGTH_BYTES_D];
static bool send_callback_called = false;
static esp_now_send_status_t send_status = ESP_NOW_SEND_SUCCESS;
static ESPNOW_LINK_MSG_QUEUE_T message_receive_queue;

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/
void espnow_send_callback(const uint8_t* mac_addr, esp_now_send_status_t status);
void espnow_receive_callback(const esp_now_recv_info_t* esp_now_info, const uint8_t* data, int data_len);
void send_message_to_peer(const uint8_t* peer_mac, const uint8_t* message, uint16_t message_length);

/************************************
 * STATIC FUNCTIONS
 ************************************/
void espnow_send_callback(const uint8_t* mac_addr, esp_now_send_status_t status)
{
    send_callback_called = true;
    send_status = status;

    switch (status)
    {
    case ESP_NOW_SEND_SUCCESS:
        logging_log(LOG_LEVEL_VERBOSE, TAG, "Sent message to mac " MACSTR, MAC2STR(mac_addr));
        break;
    case ESP_NOW_SEND_FAIL:
        logging_log(LOG_LEVEL_ERROR, TAG, "Send message to mac " MACSTR " FAILED", MAC2STR(mac_addr));
        break;
    default:
        logging_log(LOG_LEVEL_ERROR, TAG, "Invalid esp now status in send callback");
        break;
    }
}

void enqueue(ESPNOW_LINK_MSG_T msg)
{
    message_receive_queue.array[message_receive_queue.tail_index] = msg;

    message_receive_queue.tail_index = (message_receive_queue.tail_index + 1U) % ESPNOW_LINK_QUEUE_LENGTH;
}

ESPNOW_LINK_MSG_T dequeue(void)
{
    ESPNOW_LINK_MSG_T retval;

    retval = message_receive_queue.array[message_receive_queue.head_index];
    message_receive_queue.head_index = (message_receive_queue.head_index + 1U) % ESPNOW_LINK_QUEUE_LENGTH;

    return retval;
}

void espnow_receive_callback(const esp_now_recv_info_t* esp_now_info, const uint8_t* data, int data_len)
{
    ESPNOW_LINK_MSG_T msg;

    logging_log(LOG_LEVEL_VERBOSE, TAG, "Received message from mac " MACSTR, MAC2STR(esp_now_info->src_addr));

    memset(&msg, 0U, sizeof(ESPNOW_LINK_MSG_T));
    memcpy(&(msg.data), data, data_len);
    memcpy(&(msg.recv_info), esp_now_info, sizeof(esp_now_recv_info_t));

    enqueue(msg);
}

void send_message_to_peer(const uint8_t* peer_mac, const uint8_t* message, uint16_t message_length)
{
    esp_now_send(peer_mac, message, message_length);
}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
ESPNOW_LINK_ERR_T espnow_link_init(void)
{
    /* Initiailize NVS Flash for WiFi to store configuration information */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        ret = nvs_flash_init();
    }

    /* Initialize WiFi */
    ret = esp_netif_init();
    ret = esp_event_loop_create_default();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ret = esp_wifi_init(&cfg);
    ret = esp_wifi_set_storage(WIFI_STORAGE_RAM);
    ret = esp_wifi_set_mode(WIFI_MODE_STA);
    ret = esp_wifi_start();

    ret = esp_wifi_get_mac(WIFI_IF_STA, device_mac);

    /* Initiailze ESP NOW */
    ret = esp_now_init();
    ret = esp_now_register_send_cb(espnow_send_callback);
    ret = esp_now_register_recv_cb(espnow_receive_callback);

    if (ret == ESP_OK)
    {
        return ESPNOW_LINK_ERR_NONE;
    }

    return ESPNOW_LINK_ERR;
}

ESPNOW_LINK_ERR_T espnow_link_register_peer(const uint8_t* peer_mac_address)
{
    esp_err_t ret;
    esp_now_peer_info_t peer;

    memset(&peer, 0, sizeof(esp_now_peer_info_t));
    memcpy(peer.peer_addr, peer_mac_address, 6);

    ret = esp_now_add_peer(&peer);

    return (ret == ESP_OK) ? ESPNOW_LINK_ERR_NONE : ESPNOW_LINK_ERR;
}

ESPNOW_LINK_ERR_T espnow_link_write(const uint8_t* peer_mac, const uint8_t* data, uint16_t data_length)
{
    ESPNOW_LINK_ERR_T ret = ESPNOW_LINK_ERR_NONE;

    if (data_length > ESP_NOW_MAX_DATA_LEN)
    {
        ret = ESPNOW_LINK_ERR;
    }

    send_message_to_peer(peer_mac, data, data_length);

    while(!send_callback_called); /* wait for send callback to finish */
    send_callback_called = false;

    ret = (send_status == ESP_NOW_SEND_SUCCESS) ? ESPNOW_LINK_ERR_NONE : ESPNOW_LINK_ERR;

    return ret;
}

ESPNOW_LINK_ERR_T espnow_link_get_device_mac(const uint8_t* buffer)
{
    memcpy(buffer, device_mac, MAC_LENGTH_BYTES_D);

    return ESPNOW_LINK_ERR_NONE;
}

ESPNOW_LINK_ERR_T espnow_link_read(const ESPNOW_LINK_MSG_T* msg_buffer)
{
    ESPNOW_LINK_ERR_T ret = ESPNOW_LINK_ERR;
    ESPNOW_LINK_MSG_T msg;

    if (espnow_link_messages_available())
    {
        msg = dequeue();    
        memcpy(msg_buffer, &msg, sizeof(ESPNOW_LINK_MSG_T));
        ret = ESPNOW_LINK_ERR_NONE;
    }

    return ret;
}

bool espnow_link_messages_available(void)
{
    return message_receive_queue.head_index != message_receive_queue.tail_index;
}

ESPNOW_LINK_ERR_T espnow_link_close(void)
{
    esp_err_t ret;
    ret = esp_now_deinit();
    ret = esp_wifi_stop();

    return (ret == ESP_OK) ? ESPNOW_LINK_ERR_NONE : ESPNOW_LINK_ERR; 
}