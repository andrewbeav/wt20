/**
 ********************************************************************************
 * @file    logging.c
 * @author  Andrew Bevelhymer
 * @date    2024/09/14
 * @brief   Wrapper for ESP logging system, so it can be more easily mocked
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "logging.h"
#include <stdio.h>
#include <stdarg.h>

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define TAG "LOGGING_WRAPPER"
#define LOG_BUFFER_LENGTH_D (250U)

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void logging_log(logging_level_t level, const char* tag, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char buffer[LOG_BUFFER_LENGTH_D];

    if (vsnprintf(buffer, LOG_BUFFER_LENGTH_D, format, args) > LOG_BUFFER_LENGTH_D)
    {
        ESP_LOGE(
            TAG,
            "Message sent to logging_log() too long. Must be < %d characters\n", LOG_BUFFER_LENGTH_D
        );
    }

    switch (level)
    {
    case LOG_LEVEL_ERROR:
        ESP_LOGE(tag, "%s", buffer);
        break;
    case LOG_LEVEL_DEBUG:
        ESP_LOGD(tag, "%s", buffer);
        break;
    case LOG_LEVEL_INFO:
        ESP_LOGI(tag, "%s", buffer);
        break;
    case LOG_LEVEL_WARNING:
        ESP_LOGW(tag, "%s", buffer);
        break;
    case LOG_LEVEL_VERBOSE:
    default:
        ESP_LOGV(tag, "%s", buffer);
        break;
    }

    va_end(args);
}

void logging_set_level_for_tag(logging_level_t level, const char* tag)
{

    /*
     * could define logging_level_t to be in same order as esp_log_level_t,
     * but then if esp_log_level_t ever changes this breaks. better to be verbose
    */

    switch (level)
    {
    case LOG_LEVEL_ERROR:
        esp_log_level_set(tag, ESP_LOG_ERROR);
        break;
    case LOG_LEVEL_DEBUG:
        esp_log_level_set(tag, ESP_LOG_DEBUG);
        break;
    case LOG_LEVEL_INFO:
        esp_log_level_set(tag, ESP_LOG_INFO);
        break;
    case LOG_LEVEL_WARNING:
        esp_log_level_set(tag, ESP_LOG_WARN);
        break;
    case LOG_LEVEL_NONE:
        esp_log_level_set(tag, ESP_LOG_NONE);
        break;
    case LOG_LEVEL_VERBOSE:
    default: /* default to verbose */
        esp_log_level_set(tag, ESP_LOG_VERBOSE);
        break;
    }
}
