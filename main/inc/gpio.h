/**
 ********************************************************************************
 * @file    gpio.h
 * @author  Andrew Bevelhymer
 * @date    2024/09/15
 * @brief   Basic abstraction layer for gpio. Functionality added on as-needed basis
 ********************************************************************************
 */

#ifndef GPIO_H
#define GPIO_H

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

/* pin mapping */
#define LED_PIN (2U)

/************************************
 * TYPEDEFS
 ************************************/
typedef uint16_t GPIO_PIN_T;
typedef enum
{
    GPIO_PIN_OUTPUT,
    GPIO_PIN_INPUT
} GPIO_DIRECTION_T;

typedef enum
{
    GPIO_PIN_OFF = 0U,
    GPIO_PIN_ON = 1U
} GPIO_VALUE_T;

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

/**
 * \brief Reset's pin and sets it as input or output
 * 
 * \param pin pin number
 * \param direction input or output?
 */
void gpio_setup_pin(GPIO_PIN_T pin, GPIO_DIRECTION_T direction);

/**
 * \brief sets pin up as input or output. Does not reset pin
 * 
 * \param pin pin number
 * \param direction input or output?
 */
void gpio_set_pin_direction(GPIO_PIN_T pin, GPIO_DIRECTION_T direction);

/**
 * \brief sets pin level. pin should already be configured as output
 * 
 * \param pin pin number
 * \param level 0 = off 1 = on
 */
void gpio_set_pin_level(GPIO_PIN_T pin, GPIO_VALUE_T level);

#ifdef __cplusplus
}
#endif

#endif 