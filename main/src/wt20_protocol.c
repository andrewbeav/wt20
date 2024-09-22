/**
 ********************************************************************************
 * @file    wt20_protocol.c
 * @author  Andrew Bevelhymer
 * @date    2024/09/14
 * @brief   Communication layer between wt20 devices, built on top of esp now
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include <string.h>
#include <stdbool.h>

#include "wt20_protocol.h"
#include "espnow_link.h"

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/

static bool initialized = false;

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
WT20_ERR_T wt20_write(const uint8_t* peer_mac,
                WT20_COMMAND_T command,
                const uint8_t* payload,
                uint16_t payload_length)
{
    WT20_ERR_T ret;

    if (initialized)
    {
        uint8_t data[ESPNOW_DATA_BYTES];

        data[0] = (uint8_t)command;

        if (payload_length > 0U)
        {
            memset((data+1), 0U, ESPNOW_DATA_BYTES-1U);
            memcpy((data+1), payload, payload_length);
        }

        espnow_link_write(peer_mac, data, payload_length + 1U);

        ret = WT20_ERR_NONE;
    }
    else
    {
        ret = WT20_NOT_INITIALIZED;
    }

    return ret;
}

WT20_ERR_T wt20_protocol_function(const WT20_MSG_T* msg_buffer)
{
    WT20_ERR_T ret;
    ESPNOW_LINK_MSG_T recv_msg;

    if (initialized)
    {
        if (espnow_link_messages_available())
        {
            /* get message */
            espnow_link_read(&recv_msg);

            /* convert to WT20 format for higher level processing */
            memcpy(&(msg_buffer->src_mac), recv_msg.recv_info.src_addr, 6U);
            memcpy(&(msg_buffer->command), recv_msg.data, ESPNOW_DATA_BYTES);

            ret = WT20_ERR_NONE;
        }
        else
        {
            ret = WT20_NO_DATA_AVAILABLE;
        }
    }
    else
    {
        ret = WT20_NOT_INITIALIZED;
    }

    return ret;
}

WT20_ERR_T wt20_init(void)
{
    initialized = true;

    ESPNOW_LINK_ERR_T esp_err;
    esp_err = espnow_link_init();

    return (esp_err == ESPNOW_LINK_ERR_NONE) ? WT20_ERR_NONE : WT20_INITIALIZATION_ERR;
}

WT20_ERR_T wt20_deinit(void)
{
    ESPNOW_LINK_ERR_T espnow_err;
    WT20_ERR_T ret;

    /* close espnow link */
    espnow_err = espnow_link_close();

    /* set initialized to false so write calls fail */
    initialized = false;

    ret = (espnow_err = ESPNOW_LINK_ERR_NONE) ? WT20_ERR_NONE : WT20_DEINIT_FAILURE;

    return ret;
}

WT20_ERR_T wt20_get_device_mac(const uint8_t* buffer)
{
    
    espnow_link_get_device_mac(buffer);

    return WT20_ERR_NONE;
}

WT20_ERR_T wt20_add_contact(const uint8_t* mac)
{
    espnow_link_register_peer(mac);

    return WT20_ERR_NONE;
}