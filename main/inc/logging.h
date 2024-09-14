/**
 ********************************************************************************
 * @file    logging.h
 * @author  Andrew Bevelhymer
 * @date    2024/09/14
 * @brief   Wrapper for ESP logging system, so it can be more easily mocked
 ********************************************************************************
 */

#ifndef LOGGING_H
#define LOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "esp_log.h"

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/
typedef enum
{
    LOG_LEVEL_NONE,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_VERBOSE
} logging_level_t;

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

/**
 * \brief calls ESP_LOGI, ESP_LOGD, etc. based on level
 * 
 * \param level[in] log level
 * \param tag[in] tag to pass to esp logging system
 * \param message[in] message to pass to esp logging system
 */
void logging_log(logging_level_t level, const char* tag, const char* format, ...);

/**
 * \brief sets logging level for a particular tag
 * 
 * \param level[in] log level
 * \param tag[in] tag the minimum log level should apply to
 * 
 * \warning some messages still won't go through if global level is higher than level.
 *          run idf.py menuconfig to set global level
 */
void logging_set_level_for_tag(logging_level_t level, const char* tag);

#ifdef __cplusplus
}
#endif

#endif 