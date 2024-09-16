#include "unity.h"

#include <string.h>

#include "wt20_protocol.h"
#include "mock_espnow_link.h"

static uint8_t peer_mac1[6U] = {0x56, 0x78, 0x12, 0xFE, 0x4A, 0x5B};

static bool espnow_link_write_called;
static uint8_t mac_src[6U];
static WT20_COMMAND_T command_sent;
static uint16_t data_length_sent;
static uint8_t payload_sent[ESPNOW_DATA_BYTES - 1U];

void setUp(void) { } 

void tearDown(void)
{
    espnow_link_write_called = false;
}

ESPNOW_LINK_ERR_T espnow_link_write_callback(const uint8_t* peer_mac,
                                             const uint8_t* data,
                                             uint16_t data_length,
                                             int cmock_num_calls)
{
    ESPNOW_LINK_ERR_T ret = ESPNOW_LINK_ERR;

    espnow_link_write_called = true;
    data_length_sent = data_length;
    memcpy(mac_src, peer_mac, 6U);

    if (data_length > 0U)
    {
        command_sent = data[0]; /* command in first byte of data */
        ret = ESPNOW_LINK_ERR_NONE;
    }
    else if (data_length > 1U)
    {
        memcpy(payload_sent, data, data_length);
    }

    return ret;
}

void test_wt20_write_toggle_led_command(void)
{
    WT20_ERR_T err;

    /* need to initialize first */
    espnow_link_init_IgnoreAndReturn(ESPNOW_LINK_ERR_NONE);
    wt20_init();

    /* we should expect espnow_link_write to be called */
    espnow_link_write_Stub(espnow_link_write_callback);

    err = wt20_write(peer_mac1, WT20_COMMAND_TOGGLE_LED, NULL, 0U);

    /* per protocol definition, wt20 should have written 1 byte to peer containing command */
    TEST_ASSERT_EQUAL_INT(WT20_ERR_NONE, err);
    TEST_ASSERT_EQUAL_INT(0U, memcmp(mac_src, peer_mac1, 6U));
    TEST_ASSERT_EQUAL_INT(WT20_COMMAND_TOGGLE_LED, command_sent);
    TEST_ASSERT_EQUAL_INT(1U, data_length_sent);

    espnow_link_close_IgnoreAndReturn(ESPNOW_LINK_ERR_NONE);
    wt20_deinit();
}

void test_wt20_write_not_initialized(void)
{
    WT20_ERR_T err;

    err = wt20_write(peer_mac1, WT20_COMMAND_TOGGLE_LED, NULL, 0U);

    TEST_ASSERT_EQUAL_INT(WT20_NOT_INITIALIZED, err);
}

void test_wt20_init(void)
{
    WT20_ERR_T err;

    /* function should initialize espnow, mock success */
    espnow_link_init_ExpectAndReturn(ESPNOW_LINK_ERR_NONE);
    err = wt20_init();
    TEST_ASSERT_EQUAL_INT(WT20_ERR_NONE, err);

    /* mock espnow initialization failure */
    espnow_link_init_ExpectAndReturn(ESPNOW_LINK_ERR);
    err = wt20_init();
    TEST_ASSERT_EQUAL_INT(WT20_INITIALIZATION_ERR, err);
}

void test_wt20_protocol_function(void)
{
    WT20_ERR_T err;

    /* init first */
    espnow_link_init_IgnoreAndReturn(ESPNOW_LINK_ERR_NONE);
    wt20_init();

    /* function should always check to see if messages are avaiable */
    espnow_link_messages_available_ExpectAndReturn(false);
    err = wt20_protocol_function();

    TEST_ASSERT_EQUAL_INT(WT20_ERR_NONE, err);

    /* deinit for next test */
    espnow_link_close_IgnoreAndReturn(ESPNOW_LINK_ERR_NONE);
    wt20_deinit();
}

void test_wt20_protocol_function_not_initialized(void)
{
    WT20_ERR_T err;

    err = wt20_protocol_function();

    TEST_ASSERT_EQUAL_INT(WT20_NOT_INITIALIZED, err);
}