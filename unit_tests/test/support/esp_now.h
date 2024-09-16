
#include <stdint.h>

/* redefined from esp_now.h so mocking is easier */
typedef struct {
    signed rssi : 8;                              /**< the RSSI of the reception frame */
    unsigned rate : 5;                            /**< if cur_bb_format is RX_BB_FORMAT_11B, it's the transmission rate. otherwise it's Rate field of L-SIG */
    unsigned : 1;                                 /**< reserved */
    unsigned : 2;                                 /**< reserved */
    unsigned : 12;                                /**< reserved */
    unsigned rxmatch0 : 1;                        /**< indicate whether the reception frame is from interface 0 */
    unsigned rxmatch1 : 1;                        /**< indicate whether the reception frame is from interface 1 */
    unsigned rxmatch2 : 1;                        /**< indicate whether the reception frame is from interface 2 */
    unsigned rxmatch3 : 1;                        /**< indicate whether the reception frame is from interface 3 */
    uint32_t he_siga1;                            /**< HE-SIGA1 or HT-SIG */
    unsigned rxend_state : 8;                     /**< reception state, 0: successful, others: failure */
    uint16_t he_siga2;                            /**< HE-SIGA2 */
    unsigned : 7;                                 /**< reserved */
    unsigned is_group : 1;                        /**< indicate whether the reception is a group addressed frame */
    unsigned timestamp : 32;                      /**< timestamp. The local time when this packet is received. It is precise only if modem sleep or light sleep is not enabled. unit: microsecond */
    unsigned : 15;                                /**< reserved */
    unsigned : 15;                                /**< reserved */
    unsigned : 2;                                 /**< reserved */
    signed noise_floor : 8;                       /**< the noise floor of the reception frame */
    unsigned channel : 4;                         /**< the primary channel */
    unsigned second : 4;                          /**< the second channel if in HT40 */
    unsigned : 8;                                 /**< reserved */
    unsigned : 8;                                 /**< reserved */
    unsigned : 32;                                /**< reserved */
    unsigned : 32;                                /**< reserved */
    unsigned : 2;                                 /**< reserved */
    unsigned : 4;                                 /**< reserved */
    unsigned : 2;                                 /**< reserved */
    unsigned rx_channel_estimate_len : 10;        /**< the length of the channel information */
    unsigned rx_channel_estimate_info_vld : 1;    /**< indicate the channel information is valid */
    unsigned : 1;                                 /**< reserved */
    unsigned : 11;                                /**< reserved */
    unsigned : 1;                                 /**< reserved */
    unsigned : 24;                                /**< reserved */
    unsigned cur_bb_format : 4;                   /**< the format of the reception frame */
    unsigned cur_single_mpdu : 1;                 /**< indicate whether the reception MPDU is a S-MPDU */
    unsigned : 3;                                 /**< reserved */
    unsigned : 32;                                /**< reserved */
    unsigned : 32;                                /**< reserved */
    unsigned : 32;                                /**< reserved */
    unsigned : 32;                                /**< reserved */
    unsigned : 32;                                /**< reserved */
    unsigned : 32;                                /**< reserved */
    unsigned : 32;                                /**< reserved */
    unsigned : 32;                                /**< reserved */
    unsigned : 8;                                 /**< reserved */
    unsigned he_sigb_len : 6;                     /**< the length of HE-SIGB */
    unsigned : 2;                                 /**< reserved */
    unsigned : 8;                                 /**< reserved */
    unsigned : 8;                                 /**< reserved */
    unsigned : 32;                                /**< reserved */
    unsigned : 7;                                 /**< reserved */
    unsigned : 1;                                 /**< reserved */
    unsigned : 8;                                 /**< reserved */
    unsigned : 16;                                /**< reserved */
    unsigned sig_len : 14;                        /**< the length of the reception MPDU */
    unsigned : 2;                                 /**< reserved */
    unsigned dump_len : 14;                       /**< the length of the reception MPDU excluding the FCS */
    unsigned : 2;                                 /**< reserved */
    unsigned rx_state : 8;                        /**< reception state, 0: successful, others: failure */
    unsigned : 24;                                /**< reserved */
} __attribute__((packed)) wifi_pkt_rx_ctrl_t;

/* redefined from esp_now.h so mocking is easier */
typedef struct esp_now_recv_info {
    uint8_t * src_addr;                      /**< Source address of ESPNOW packet */
    uint8_t * des_addr;                      /**< Destination address of ESPNOW packet */
    wifi_pkt_rx_ctrl_t * rx_ctrl;            /**< Rx control info of ESPNOW packet */
} esp_now_recv_info_t;
