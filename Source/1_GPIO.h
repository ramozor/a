/**
 * @file 1_GPIO.h
 * @brief Header file for managing GPIO
 * @date Oct 2024
 * @author Martin Votava
 **/

#ifndef SOURCE_1_GPIO_H_
#define SOURCE_1_GPIO_H_

/**
 * @brief Initializes General Purpose Input/Output (GPIO) pins for the system.
 *
 * This function sets up the GPIO pins required for UHEART, configuring them
 * as inputs, outputs, or alternate functions based on the application's needs.
 * It also sets any initial states for output pins.
 *
 */
extern void InitGPIO(void);



#endif /* SOURCE_1_GPIO_H_ */
