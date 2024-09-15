/**
 ********************************************************************************
 * @file    espnow_link.h
 * @author  Andrew Bevelhymer
 * @date    2024/09/15
 * @brief   Abstraction layer for espnow
 ********************************************************************************
 */

#ifndef ESPNOW_LINK_H
#define ESPNOW_LINK_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include <stdint.h>
#include "esp_now.h"

/************************************
 * MACROS AND DEFINES
 ************************************/
#define ESPNOW_DATA_BYTES ESP_NOW_MAX_DATA_LEN
#define ESPNOW_LINK_QUEUE_LENGTH (16U)

/************************************
 * TYPEDEFS
 ************************************/
typedef enum
{
    ESPNOW_LINK_ERR_NONE,
    ESPNOW_LINK_ERR
} ESPNOW_LINK_ERR_T;

typedef struct
{
    esp_now_recv_info_t recv_info;
    uint8_t data[ESPNOW_DATA_BYTES];
} ESPNOW_LINK_MSG_T;

typedef struct
{
    ESPNOW_LINK_MSG_T array[ESPNOW_LINK_QUEUE_LENGTH];
    uint8_t head_index;
    uint8_t tail_index;
} ESPNOW_LINK_MSG_QUEUE_T;

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

/**
 * \brief Initializes wifi and espnow
 */
ESPNOW_LINK_ERR_T espnow_link_init(void);

/**
 * \brief De-Initializes wifi and espnow
 */
ESPNOW_LINK_ERR_T espnow_link_close(void);

/**
 * \brief Adds peer to network
 */
ESPNOW_LINK_ERR_T espnow_link_register_peer(const uint8_t* peer_mac_address);

/**
 * \brief send command
 * 
 * \param peer_mac[in] 6 byte MAC address for peer
 * \param buffer[in] data to send. Length must be less than ESP_NOW_MAX_DATA_LEN
 */
ESPNOW_LINK_ERR_T espnow_link_write(const uint8_t* peer_mac, const uint8_t* data, uint16_t data_length);

/**
 * \brief puts device mac address into buffer
 * 
 * \param buffer[out] buffer to put device mac address in
 */
ESPNOW_LINK_ERR_T espnow_link_get_device_mac(const uint8_t* buffer);

/**
 * \brief puts device mac address into buffer
 * 
 * \param esp_now_info[out] buffer to put esp_now_info into
 * \param data_buffer[out] buffer to put device mac address in
 */
ESPNOW_LINK_ERR_T espnow_link_read(const ESPNOW_LINK_MSG_T* msg_buffer);

/**
 * \brief returns whether any messages are available to read
 */
bool espnow_link_messages_available(void);

#ifdef __cplusplus
}
#endif

#endif 