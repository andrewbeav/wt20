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

WT20_ERR_T wt20_protocol_function(void)
{
    WT20_ERR_T ret;

    if (initialized)
    {
        ret = WT20_ERR_NONE;

        if (espnow_link_messages_available())
        {
            /* get message and process */
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