/**
 ********************************************************************************
 * @file    wt20_protocol.h
 * @author  Andrew Bevelhymer
 * @date    2024/09/15
 * @brief   Communication layer between wt20 devices, built on top of esp now
 ********************************************************************************
 */

#ifndef WT20_PROTOCOL_H
#define WT20_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include <stdint.h>

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/

/* commands that are sent to peer */
typedef enum
{
    ESPNOW_LINK_COMMAND_TOGGLE_LED,
    ESPNOW_LINK_COMMAND_SEND_PAYLOAD,
    ESPNOW_LINK_COMMAND_NONE
} WT20_COMMAND_T;

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

/**
 * \brief Send command and optional payload to peer
 * 
 * \param peer_mac MAC address of peer to send message to (TODO: Create peer abstraction system)
 * \param command Command to send
 * \param payload[in] pointer to optional payload (pass NULL if not used)
 * \param payload_length length (in bytes) of payload (pass 0 if no payload)
 */
void wt20_write(const uint8_t* peer_mac, WT20_COMMAND_T command, const uint8_t* payload, uint16_t payload_length);

/**
 * \brief Should be called periodically. Checks for new messages from peers and performs actions
 */
void wt20_protocol_function(void);

#ifdef __cplusplus
}
#endif

#endif 