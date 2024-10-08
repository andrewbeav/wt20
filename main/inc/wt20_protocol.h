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
    WT20_COMMAND_TOGGLE_LED,
    WT20_COMMAND_SEND_PAYLOAD,
    WT20_COMMAND_NONE
} WT20_COMMAND_T;

/* errors */
typedef enum
{
    WT20_ERR_NONE,
    WT20_INITIALIZATION_ERR,
    WT20_NOT_INITIALIZED,
    WT20_DEINIT_FAILURE,
    WT20_NO_DATA_AVAILABLE
} WT20_ERR_T;

/* messages */
/* TODO: Update to have version, CRC, etc. */
typedef struct
{
    uint8_t src_mac[6U];
    uint8_t command;
    uint8_t payload[249U];
} WT20_MSG_T;


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
WT20_ERR_T wt20_write(const uint8_t* peer_mac, WT20_COMMAND_T command, const uint8_t* payload, uint16_t payload_length);

/**
 * \brief Should be called periodically. Checks for new messages from peers and performs actions
 */
WT20_ERR_T wt20_protocol_function(const WT20_MSG_T* msg_buffer);

/**
 * \brief Sets up wt20 protocol, initialized espnow
 */
WT20_ERR_T wt20_init(void);

/**
 * \brief Deinits espnow and wt20 protocol
 */
WT20_ERR_T wt20_deinit(void);

/**
 * \brief puts device mac address into buffer
 * 
 * \param buffer[out] buffer to put device mac address in
 */
WT20_ERR_T wt20_get_device_mac(const uint8_t* buffer);

/**
 * \brief adds contact. Currently just adds peer to espnow
 * 
 * \todo Eventually expand contact system with IDs, etc.
 */
WT20_ERR_T wt20_add_contact(const uint8_t* mac);

#ifdef __cplusplus
}
#endif

#endif 