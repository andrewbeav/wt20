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
#include "esp_log.h"
#include "esp_now.h"
#include "esp_mac.h"
#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define TAG "ESPNOW_LINK"
#define MAC_LENGTH_BYTES_D (6U)

/************************************
 * STATIC VARIABLES
 ************************************/
static uint8_t device_mac[MAC_LENGTH_BYTES_D];

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
    switch (status)
    {
    case ESP_NOW_SEND_SUCCESS:
        ESP_LOGI(TAG, "Sent message to mac " MACSTR, MAC2STR(mac_addr));
        break;
    case ESP_NOW_SEND_FAIL:
        ESP_LOGE(TAG, "Send message to mac " MACSTR " FAILED", MAC2STR(mac_addr));
        break;
    default:
        ESP_LOGE(TAG, "Invalid esp now status in send callback");
        break;
    }
}

void espnow_receive_callback(const esp_now_recv_info_t* esp_now_info, const uint8_t* data, int data_len)
{
    static uint32_t gpio_level = 0U;
    ESP_LOGI(TAG, "got message from " MACSTR ", sent to " MACSTR, MAC2STR(esp_now_info->src_addr), MAC2STR(esp_now_info->des_addr));
    printf("message, datalen = %d: %.*s\n", data_len, data_len, data);
    switch (data[0])
    {
    case ESPNOW_LINK_COMMAND_TOGGLE_LED:
        gpio_set_level(2U, (gpio_level++) & 1U); /* TODO: Set flag telling other subsystem to toggle GPIO */
        break;
    
    default:
        break;
    }
}

/* TODO: Eventually may want to expand peer system */
void send_message_to_peer(const uint8_t* peer_mac, const uint8_t* message, uint16_t message_length)
{
    esp_now_send(peer_mac, message, message_length);
}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void espnow_link_init(void)
{
    /* Initiailize NVS Flash for WiFi to store configuration information */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );

    /* Initialize WiFi */
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_ERROR_CHECK(esp_wifi_get_mac(WIFI_IF_STA, device_mac));

    /* Initiailze ESP NOW */
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_send_cb(espnow_send_callback));
    ESP_ERROR_CHECK(esp_now_register_recv_cb(espnow_receive_callback));
}

void espnow_link_register_peer(const uint8_t* peer_mac_address)
{
    esp_now_peer_info_t peer;

    memset(&peer, 0, sizeof(esp_now_peer_info_t));
    memcpy(peer.peer_addr, peer_mac_address, 6);

    esp_now_add_peer(&peer);
}

void espnow_link_command_peer(const uint8_t* peer_mac, ESPNOW_LINK_COMMAND_T command, const char* str_message)
{
    switch (command)
    {
    case ESPNOW_LINK_COMMAND_TOGGLE_LED:
        send_message_to_peer(peer_mac, (const uint8_t*) &command, 1U);
        break;
    case ESPNOW_LINK_COMMAND_SEND_STR:
        send_message_to_peer(peer_mac, (const uint8_t*) str_message, strlen(str_message));
        break;
    
    default:
        break;
    }
}

void espnow_link_get_device_mac(const uint8_t* buffer)
{
    memcpy(buffer, device_mac, MAC_LENGTH_BYTES_D);
}