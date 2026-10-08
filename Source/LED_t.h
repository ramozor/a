/**
 * @file LED_t.h
 * @brief Header file for managing LED states and operations.
 *
 * This file defines the structures and functions used to control the state of multiple LEDs,
 * including turning them on, off, and making them blink at specified intervals.
 *
 * @date Oct 2024
 * @author mde
 */

#ifndef SOURCE_LED_T_H_
#define SOURCE_LED_T_H_

#include <stdint.h>        /* Includes uint16_t definition */

/**
 * @enum LED_state_enum_t
 * @brief Enumeration of possible LED states.
 *
 * This enum defines the possible states for each LED.
 */
typedef enum
{
    LED_OFF,        /**< LED is off. */
    LED_BLINK_1s,   /**< LED blinks every 1 second. */
    LED_BLINK_500ms,/**< LED blinks every 500 milliseconds. */
    LED_ON,         /**< LED is always on. */
} LED_state_enum_t;

/**
 * @enum LED_ID_enum_t
 * @brief Enumeration of LED identifiers.
 *
 * This enum defines unique IDs for each LED.
 */
typedef enum
{
    LED1 = 0, /**< LED 1 ID */
    LED2 = 1, /**< LED 2 ID */
    LED3 = 2, /**< LED 3 ID */
    LED4 = 3, /**< LED 4 ID */
} LED_ID_enum_t;

/**
 * @struct LEDs_state_t
 * @brief Structure that holds the states of all LEDs.
 *
 * This structure maintains the state of each LED.
 */
typedef struct
{
    LED_state_enum_t LED_1; /**< State of LED 1 */
    LED_state_enum_t LED_2; /**< State of LED 2 */
    LED_state_enum_t LED_3; /**< State of LED 3 */
    LED_state_enum_t LED_4; /**< State of LED 4 */
} LEDs_state_t;

/**
 * @class LED_t
 * @brief A class that manages the states and routines of LEDs.
 *
 * This class provides methods to set and get LED states and to manage background
 * routines for LED blinking and on/off behavior.
 */
class LED_t
{
public:
    /**
     * @brief Sets the state of a specified LED.
     *
     * This function updates the state of the given LED (on, off, or blinking).
     *
     * @param LED_id The ID of the LED to modify.
     * @param new_state The new state to assign to the LED.
     */
    static void set_LED_state(LED_ID_enum_t LED_id, LED_state_enum_t new_state);

    /**
     * @brief Gets the current state of a specified LED.
     *
     * This function retrieves the current state of the given LED.
     *
     * @param LED_id The ID of the LED to query.
     * @return The current state of the LED.
     */
    static LED_state_enum_t get_LED_state(LED_ID_enum_t LED_id);

    /**
     * @brief Handles the slow (1 second interval) blinking routine for LEDs.
     *
     * This function toggles LEDs that are set to blink every 1 second.
     */
    static void routine_1s(); // Slow LED toggle

    /**
     * @brief Handles the fast (500 millisecond interval) blinking routine for LEDs.
     *
     * This function toggles LEDs that are set to blink every 500 milliseconds.
     */
    static void routine_500ms(); // Fast LED toggle

    /**
     * @brief Ensures LEDs are on or off based on their configured state.
     *
     * This function checks each LED's state and ensures they are turned on or off
     * as configured (either on, off, or blinking).
     */
    static void background_routine(); // Ensure LED states match their configuration

private:
    static LEDs_state_t LEDs; /**< Holds the current state of all LEDs. */
    static uint16_t state_1s; /**< Internal variable to track the 1 second toggle state. */
    static uint16_t state_500ms; /**< Internal variable to track the 500 millisecond toggle state. */

    /**
     * @brief Constructor made private to prevent instantiation of the LED_t class.
     */
    LED_t(){}; // Private constructor to prevent instantiation
};

#endif /* SOURCE_LED_T_H_ */
