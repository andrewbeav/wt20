/**
 ********************************************************************************
 * @file    gpio.c
 * @author  Andrew Bevelhymer
 * @date    2024/09/21
 * @brief   Implements high level interface for gpio
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "gpio.h"
#include "driver/gpio.h"

/************************************
 * GLOBAL FUNCTIONS
 ************************************/

void gpio_setup_pin(GPIO_PIN_T pin, GPIO_DIRECTION_T direction)
{
    gpio_reset_pin(pin);
    gpio_set_pin_direction(pin, direction);
}

void gpio_set_pin_direction(GPIO_PIN_T pin, GPIO_DIRECTION_T direction)
{
    switch (direction)
    {
    case GPIO_PIN_OUTPUT:
        gpio_set_direction(pin, GPIO_MODE_OUTPUT);
        break;
    case GPIO_PIN_INPUT:
    default:
        gpio_set_direction(pin, GPIO_MODE_INPUT) ;
        break;
    }
}

void gpio_set_pin_level(GPIO_PIN_T pin, GPIO_VALUE_T level)
{
    gpio_set_level(pin, level);
}