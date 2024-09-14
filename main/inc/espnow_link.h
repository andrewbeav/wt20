#include <stdint.h>

/* commands that end up being sent to peer */
typedef enum
{
    ESPNOW_LINK_COMMAND_TOGGLE_LED,
    ESPNOW_LINK_COMMAND_SEND_STR,
    ESPNOW_LINK_COMMAND_NONE
} ESPNOW_LINK_COMMAND_T;

/**
 * \brief Initializes wifi and espnow
 */
void espnow_link_init(void);

/**
 * \brief Adds peer to network
 */
void espnow_link_register_peer(const uint8_t* peer_mac_address);

/**
 * \brief send command
 */
void espnow_link_command_peer(const uint8_t* peer_mac, ESPNOW_LINK_COMMAND_T command, const char* str_message);

/**
 * \brief puts device mac address into buffer
 * 
 * \param buffer[out] buffer to put device mac address in
 */
void espnow_link_get_device_mac(const uint8_t* buffer);