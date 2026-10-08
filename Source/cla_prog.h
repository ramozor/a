/*
 * cla.h
 *
 *  Created on: Jan 21, 2025
 *      Author: mvo
 */

#ifndef SOURCE_CLA_H_
#define SOURCE_CLA_H_
#include "Defines.h"

typedef struct ADC_filt_t
{
    uint32_t sum[8];
    float avg[8];
    // uint16_t ADC_history[8][ADC_PWM_RAT];
    uint16_t i_sample; // Indicates
    uint16_t min1[8];
    uint16_t min2[8];
    uint16_t max1[8];
    uint16_t max2[8];

}ADC_filt_t;


#ifdef __cplusplus
extern "C" {
#endif
__interrupt void Cla1Task7 ( void );
__interrupt void Cla1Task6 ( void );
extern volatile ADC_filt_t ADC_filt;

#ifdef __cplusplus
}
#endif
#endif /* SOURCE_CLA_H_ */
