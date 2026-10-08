/**
 * @file 3_ADC.h
 * @brief Header file for managing ADC
 * @date Oct 2024
 * @author Martin Votava
 **/

#ifndef SOURCE_3_ADC_H_
#define SOURCE_3_ADC_H_

/**
 * @brief Initiliazes ADCs
 */
extern void InitADC(void);
/**
 * @brief Synchronizes ADCs to EPWM1
 */
extern void ADC_SYNC(void);
/**
 * @brief Converts resultsSynchronizes ADCs to EPWM1
 */
extern void ADC_res_to_float();


typedef struct
{
    uint16_t calibrate_v:1;
}ADC_request_bit_t;

typedef union
{
    ADC_request_bit_t bit;
    uint16_t all;
}ADC_request_t;

extern ADC_request_t ADC_request;

#endif /* SOURCE_3_ADC_H_ */
