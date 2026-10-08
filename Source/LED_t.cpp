/*
 * LED_t.cpp
 *
 *  Created on: 14 Oct 2024
 *      Author: mde
 */
#include "F28x_Project.h"
#include "1_GPIO.h"
#include "LED_t.h"




// declaration of LED_t static members
LEDs_state_t LED_t::LEDs;
uint16_t LED_t::state_1s;
uint16_t LED_t::state_500ms;

// Definition of LED_t methods
void LED_t::set_LED_state(LED_ID_enum_t LED_id, LED_state_enum_t new_state)
{
    switch (LED_id)
    {
    case LED1:
        LEDs.LED_1 = new_state;
        break;
    case LED2:
        LEDs.LED_2 = new_state;
        break;
    case LED3:
        LEDs.LED_3 = new_state;
        break;
    case LED4:
        LEDs.LED_4 = new_state;
        break;
    }
}

LED_state_enum_t LED_t::get_LED_state(LED_ID_enum_t LED_id)
{
    switch (LED_id)
    {
    case LED1:
        return LEDs.LED_1;
    case LED2:
        return LEDs.LED_2;
    case LED3:
        return LEDs.LED_3;
    case LED4:
        return LEDs.LED_4;
    }
    return LED_OFF;

}

void LED_t::routine_1s() // Slow LED toggle
{
    state_1s = !state_1s;
    if (LEDs.LED_1 == LED_BLINK_1s)
    {
        GpioDataRegs.GPCDAT.bit.GPIO67 = state_1s;
    }
    if (LEDs.LED_2 == LED_BLINK_1s)
    {
        GpioDataRegs.GPBDAT.bit.GPIO43 = state_1s;
    }
    if (LEDs.LED_3 == LED_BLINK_1s)
    {
        GpioDataRegs.GPBDAT.bit.GPIO42 = state_1s;
    }
    if (LEDs.LED_4 == LED_BLINK_1s)
    {
        GpioDataRegs.GPBDAT.bit.GPIO47 =state_1s;
    }
}

void LED_t::routine_500ms() // Fast LED toggle
{
    state_500ms = !state_500ms;
    if (LEDs.LED_1 == LED_BLINK_500ms)
    {
        GpioDataRegs.GPCDAT.bit.GPIO67 = state_500ms;
    }
    if (LEDs.LED_2 == LED_BLINK_500ms)
    {
        GpioDataRegs.GPBDAT.bit.GPIO43 = state_500ms;
    }
    if (LEDs.LED_3 == LED_BLINK_500ms)
    {
        GpioDataRegs.GPBDAT.bit.GPIO42 = state_500ms;
    }
    if (LEDs.LED_4 == LED_BLINK_500ms)
    {
        GpioDataRegs.GPBDAT.bit.GPIO47 = state_500ms;
    }
}

void LED_t::background_routine()
{
    if (LEDs.LED_1 == LED_ON)
    {
        GpioDataRegs.GPCSET.bit.GPIO67;  // Turn LED1 (GPIO67) on
    }
    else if (LEDs.LED_1 == LED_OFF)
    {
        GpioDataRegs.GPCCLEAR.bit.GPIO67;  // Turn LED1 (GPIO67) off
    }

    if (LEDs.LED_2 == LED_ON)
    {
        GpioDataRegs.GPBSET.bit.GPIO43;  // Turn LED2 (GPIO43) on
    }
    else if (LEDs.LED_2 == LED_OFF)
    {
        GpioDataRegs.GPBCLEAR.bit.GPIO43;  // Turn LED2 (GPIO43) off
    }

    if (LEDs.LED_3 == LED_ON)
    {
        GpioDataRegs.GPBSET.bit.GPIO42;  // Turn LED3 (GPIO42) on
    }
    else if (LEDs.LED_3 == LED_OFF)
    {
        GpioDataRegs.GPBCLEAR.bit.GPIO42;  // Turn LED3 (GPIO42) off
    }

    if (LEDs.LED_4 == LED_ON)
    {
        GpioDataRegs.GPBSET.bit.GPIO47;  // Turn LED4 (GPIO47) on
    }
    else if (LEDs.LED_4 == LED_OFF)
    {
        GpioDataRegs.GPBCLEAR.bit.GPIO47;  // Turn LED4 (GPIO47) off
    }
}
