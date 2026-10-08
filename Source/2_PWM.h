/**
 * @file 2_PWM.h
 * @brief Header file for managing PWM
 * @date Oct 2024
 * @author Martin Votava
 **/

#ifndef SOURCE_2_PWM_H_
#define SOURCE_2_PWM_H_

#include "F28x_Project.h"

#include "Defines.h"


typedef struct{   
    union   AQCTLA_REG                       AQCTLA;                       // Action Qualifier Control Register For Output A 
    union   AQCTLB_REG                       AQCTLB;                       // Action Qualifier Control Register For Output B   
    union   CMPA_REG                         CMPA;                         // Counter Compare A Register 
    union   CMPB_REG                         CMPB;                         // Compare B Register
    uint16_t                                 TBPRD;                        // ePWM Period Register      
}epwm_mod_t;

extern epwm_mod_t ePwm2vals;
extern epwm_mod_t ePwm3vals;
extern epwm_mod_t ePwm4vals;
extern epwm_mod_t ePwm5vals;
extern epwm_mod_t ePwm6vals;
extern epwm_mod_t ePwm7vals;
extern epwm_mod_t ePwm8vals;






/**
 * @brief Enables GPIO for EPwm1.
 */
extern void EnableEPwm1Gpio       (void);
/**
 * @brief Disables GPIO for EPwm1.
 */
extern void DisableEPwm1Gpio      (void);
/**
 * @brief Enables GPIO for EPwm2.
 */
extern void EnableEPwm2Gpio       (void);
/**
 * @brief Disables GPIO for EPwm2.
 */
extern void DisableEPwm2Gpio      (void);
/**
 * @brief Enables GPIO for EPwm3.
 */
extern void EnableEPwm3Gpio       (void);
/**
 * @brief Disables GPIO for EPwm3.
 */
extern void DisableEPwm3Gpio      (void);
/**
 * @brief Enables GPIO for EPwm4.
 */
extern void EnableEPwm4Gpio       (void);
/**
 * @brief Disables GPIO for EPwm4.
 */
extern void DisableEPwm4Gpio      (void);
/**
 * @brief Enables GPIO for EPwm5.
 */
extern void EnableEPwm5Gpio       (void);
/**
 * @brief Disables GPIO for EPwm5.
 */
extern void DisableEPwm5Gpio      (void);
/**
 * @brief Enables GPIO for EPwm6.
 */
extern void EnableEPwm6Gpio       (void);
/**
 * @brief Disables GPIO for EPwm6.
 */
extern void DisableEPwm6Gpio      (void);
/**
 * @brief Enables GPIO for EPwm7.
 */
extern void EnableEPwm7Gpio       (void);
/**
 * @brief Disables GPIO for EPwm7.
 */
extern void DisableEPwm7Gpio      (void);
/**
 * @brief Enables GPIO for EPwm8.
 */
extern void EnableEPwm8Gpio       (void);
/**
 * @brief Disables GPIO for EPwm8.
 */
extern void DisableEPwm8Gpio      (void);
/**
 * @brief Enables GPIO for EPwm9.
 */
extern void EnableEPwm9Gpio       (void);
/**
 * @brief Disables GPIO for EPwm9.
 */
extern void DisableEPwm9Gpio      (void);
/**
 * @brief Enables GPIO for EPwm10.
 */
extern void EnableEPwm10Gpio      (void);
/**
 * @brief Disables GPIO for EPwm10.
 */
extern void DisableEPwm10Gpio     (void);
/**
 * @brief Enables GPIO for EPwm11.
 */
extern void EnableEPwm11Gpio      (void);
/**
 * @brief Disables GPIO for EPwm11.
 */
extern void DisableEPwm11Gpio     (void);
/**
 * @brief Enables GPIO for EPwm12.
 */
extern void EnableEPwm12Gpio      (void);
/**
 * @brief Disables GPIO for EPwm12.
 */
extern void DisableEPwm12Gpio     (void);


/**
 * @brief Initializes the PWM system.
 *
 * This function initializes the PWM system, setting up necessary registers
 * and configuring the ePWM modules for operation. It must be called before
 * using any PWM-related functionality.
 *
 * This includes configuring the GPIOs for PWM signals, setting up initial
 * parameters such as frequency, duty cycle, and enabling PWM output.
 *
 *
 *
 * @note This function must be called during system initialization.
 */
extern void InitPWM               (void);

/**
 * @brief Clears the trip zone fault.
 *
 * This function clears any faults in the trip zone for the ePWM modules,
 * allowing PWM outputs to resume operation after a fault condition has occurred.
 *
 *
 */
extern void EPwm_clear_trip_zone();

/**
 * @brief Checks for ePWM faults.
 *
 * This function checks the status of any faults related to the ePWM modules,
 * such as overcurrent or undervoltage conditions.
 *
 * @return The function returns a 16-bit value representing the fault status.
 *
 * @retval 0 No faults detected.
 * @retval non-zero Fault condition present.
 */
extern uint16_t EPwm_faults();


/**
 * @brief Checks if EPWM 3-6 are operating.
 *
 * If the EPWMs are operating, then mux is equal of one.
 *
 * @return The function returns a 16-bit value representing the EPWM status.
 *
 * @retval 0 One or more PWMs are not operating
 * @retval 1 All the PWMs are operating.
 */
extern uint16_t EPwm3_6_status();



/**
 * @brief Sets the phase shift for a given ePWM module.
 *
 * This function configures the phase shift for the specified ePWM module. The shift is
 * applied to synchronize the PWM signal with other PWM modules or signals. The phase
 * shift value is provided in degrees and is converted internally to the appropriate
 * register value.
 *
 * @param EPwm Pointer to the ePWM module registers.
 * @param shift The desired phase shift in degrees (float). A positive value shifts the
 *        signal forward, and a negative value shifts it backward.
 */
#if ENABLE_VAR_SW == 1
extern void EPwm_set_phase_shift(volatile epwm_mod_t* EPwm, float shift);
#else
extern void EPwm_set_phase_shift(volatile EPWM_REGS* EPwm, float shift);
#endif


extern void EPwm_set_pointer_slave_cell_reference(volatile EPWM_REGS* EPwm_leg1, volatile EPWM_REGS* EPwm_leg2, float P, float shift);


extern void EPwm_set_frequency(float fsw);

#if ENABLE_VAR_SW == 1
extern void Epwm_write_ePWM_regs();
#endif


#if FIXED_SW_COMB == 1
/**
 * @brief Sets the duty cycle for an EPWM module.
 *
 * Computes and updates CMPA and CMPB registers using:
 * \f$ CMP = 0.5 \cdot TBPRD \cdot (1.0 + \frac{2.0 \cdot duty}{\pi}) \f$.
 *
 * @param EPwm Pointer to the EPWM register structure (volatile EPWM_REGS*).
 * @param duty Desired duty cycle as a float, typically between -1.0 and 1.0.
 *
 * @note This function is available only when `FIXED_SW_COMB` is defined as `1`.
 * @pre The `PI` macro must be defined and represent \f$\pi\f$.
 */
void EPwm_set_duty(volatile EPWM_REGS* EPwm, float duty);

#endif

#endif /* SOURCE_2_PWM_H_ */
