//###########################################################################
//
// FILE:    3_ADC.c
//
// TITLE:   PWM Setup v1.0
//
// Author: Thiago Pereira - March 2020
//
//
//###########################################################################
//
// ------------------------- Included Files ---------------------------------
//
#include "F28x_Project.h"
#include "General.h"
#include "Defines.h"
#include "globals.h"
#if (PORT_OPTION_SEL == PORTS_SEP)
#include "ctrl_var/ctrl_t.h"
#endif
#if (PORT_OPTION_SEL == PORTS_PAR)
#include "ctrl_parallel_t.h"
#endif
#if (PORT_OPTION_SEL == PORTS_SEP_IDEAL_DECOUPLING)
#include "ctrl_ideal_decoupling_t.h"
#endif
#include "state_machine_t.h"
#include "3_ADC.h"
#if (USE_CLA_ACQ == 1)
#include "cla_prog.h"
#endif


uint16_t debug_adc = 0;

#if (AUTOMATIC_CURRENT_OFFSET == 1)
typedef struct
{
    float cell1;
    float cell2;
    float cell3;
    float cell4;
    uint16_t step;
}sensor_offsets_t;

static sensor_offsets_t current_offsets;
static sensor_offsets_t voltage_offsets;

ADC_request_t ADC_request = {.all = 0};
#endif
//
//
//
void InitADC(void)
{

#if (AUTOMATIC_CURRENT_OFFSET == 1)
    // Offset init
    current_offsets.cell1 = ADC_CELL1_CUR_OFF;
    current_offsets.cell2 = ADC_CELL2_CUR_OFF;
    current_offsets.cell3 = ADC_CELL3_CUR_OFF;
    current_offsets.cell4 = ADC_CELL4_CUR_OFF;

    voltage_offsets.cell1 = ADC_CELL1_VOLT_OFF;
    voltage_offsets.cell2 = ADC_CELL2_VOLT_OFF;
    voltage_offsets.cell3 = ADC_CELL3_VOLT_OFF;
    voltage_offsets.cell4 = ADC_CELL4_VOLT_OFF;
    voltage_offsets.step = 0;

#endif
    EALLOW;                                    // Write configurations
    //--- Reset the ADC.  This is good programming practice.
    DevCfgRegs.SOFTPRES13.bit.ADC_A = 1;        // ADC is reset
    DevCfgRegs.SOFTPRES13.bit.ADC_B = 1;        // ADC is reset
    DevCfgRegs.SOFTPRES13.bit.ADC_C = 1;        // ADC is reset
    DevCfgRegs.SOFTPRES13.bit.ADC_D = 1;        // ADC is reset
    DevCfgRegs.SOFTPRES13.bit.ADC_A = 0;        // ADC is released from reset
    DevCfgRegs.SOFTPRES13.bit.ADC_B = 0;        // ADC is released from reset
    DevCfgRegs.SOFTPRES13.bit.ADC_C = 0;        // ADC is released from reset
    DevCfgRegs.SOFTPRES13.bit.ADC_D = 0;        // ADC is released from reset

    // Turn on sequence
    CpuSysRegs.PCLKCR13.bit.ADC_A = 1;         // Set the bit to enable the desired ADC clock in the PCLKCR13 register
    CpuSysRegs.PCLKCR13.bit.ADC_B = 1;         // Set the bit to enable the desired ADC clock in the PCLKCR13 register
    CpuSysRegs.PCLKCR13.bit.ADC_C = 1;         // Set the bit to enable the desired ADC clock in the PCLKCR13 register
    CpuSysRegs.PCLKCR13.bit.ADC_D = 1;         // Set the bit to enable the desired ADC clock in the PCLKCR13 register
    //
    // Set ADC Mode
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE); //ADC A
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE); //ADC B
    AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE); //ADC C
    AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE); //ADC D
    //
    //Device_cal();                            // The Device_cal function copies the ADC calibration values from
    //                                            TI reserved OTP into the ADCREFSEL and ADCOFFTRIM registers
    //
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6;         // Set ADCCLK divider to /4
    AdcaRegs.ADCCTL2.bit.RESOLUTION = ADC_RESOLUTION_12BIT;
    AdcbRegs.ADCCTL2.bit.PRESCALE = 6;         // Set ADCCLK divider to /4
    AdcbRegs.ADCCTL2.bit.RESOLUTION = ADC_RESOLUTION_12BIT;
    AdccRegs.ADCCTL2.bit.PRESCALE = 6;         // Set ADCCLK divider to /4
    AdccRegs.ADCCTL2.bit.RESOLUTION = ADC_RESOLUTION_12BIT;
    AdcdRegs.ADCCTL2.bit.PRESCALE = 6;         // Set ADCCLK divider to /4
    AdcdRegs.ADCCTL2.bit.RESOLUTION = ADC_RESOLUTION_12BIT;

    // The base ADC clock is provided directly by the system clock (SYSCLK)
    // This clock is used to generate the ADC acquisition window.
    // The register ADCCTL2 has a PRESCALE field which determines the ADCCLK.
    // The ADCCLK is used to clock the converter.
    // ------- 16 BITS 29.5 ADCCLK
    // ------- 12 BITS 10.2 ADCCLK
    //
    //
    //The ADC can be configured to generate the EOC pulse at either the
    //end of the acquisition window or at the end of the voltage conversion.
    //This is configured using the bit INTPULSEPOS in the ADCCTL1 register
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;      //Set pulse positions to late ---- Late = 1 and Early = 0
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;      //Set pulse positions to late ---- Late = 1 and Early = 0
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;      //Set pulse positions to late ---- Late = 1 and Early = 0
    AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;      //Set pulse positions to late ---- Late = 1 and Early = 0

    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;         //power up the ADC
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;         //power up the ADC
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;         //power up the ADC
    AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;         //power up the ADC
    //
    DELAY_US(1000);                            //delay for 1ms to allow ADC time to power up
    //
    //============================================================================================
    // ------------------------------- ADC Channels A0 - A5 --------------------------------------
    //============================================================================================
    //
    //
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0;     //End of SOC0 will set INT1 flag - EOC is trigger for ADCINT1
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;       //Enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;     //Make sure INT1 flag is cleared
    //
    // Select the channels to convert and END of conversion flag
    //
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;         //SOC0 will convert pin A0
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;
#endif

    //Channel A1
    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 1;         //SOC1 will convert pin A1
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;
#endif
    //Channel A2
    AdcaRegs.ADCSOC2CTL.bit.CHSEL = 2;         //SOC2 will convert pin A2
    AdcaRegs.ADCSOC2CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;
#endif
    //Channel A3
    AdcaRegs.ADCSOC3CTL.bit.CHSEL = 3;         //SOC3 will convert pin A3
    AdcaRegs.ADCSOC3CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdcaRegs.ADCSOC3CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdcaRegs.ADCSOC3CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;
#endif
    //Channel A4
    AdcaRegs.ADCSOC4CTL.bit.CHSEL = 4;         //SOC3 will convert pin A4
    AdcaRegs.ADCSOC4CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdcaRegs.ADCSOC4CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdcaRegs.ADCSOC4CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;
#endif
    //Channel A5
    AdcaRegs.ADCSOC5CTL.bit.CHSEL = 5;         //SOC3 will convert pin A5
    AdcaRegs.ADCSOC5CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdcaRegs.ADCSOC5CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdcaRegs.ADCSOC5CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;
#endif
    //Channel ADCIN14
    AdcaRegs.ADCSOC6CTL.bit.CHSEL = 14;         //SOC3 will convert pin ADCIN14
    AdcaRegs.ADCSOC6CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdcaRegs.ADCSOC6CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdcaRegs.ADCSOC6CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;
#endif
    //Channel ADCIN15
    AdcaRegs.ADCSOC7CTL.bit.CHSEL = 15;         //SOC3 will convert pin ADCIN15
    AdcaRegs.ADCSOC7CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdcaRegs.ADCSOC7CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdcaRegs.ADCSOC7CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;
#endif
    //
    //============================================================================================
    // ------------------------------- ADC Channels B0 - B3 --------------------------------------
    //============================================================================================
    //
    //
    AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 5;     //End of SOC5 will set INT1 flag - EOC is trigger for ADCINT1
    AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;       //Enable INT1 flag
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;     //Make sure INT1 flag is cleared
    //

    //Channel B0
    AdcbRegs.ADCSOC0CTL.bit.CHSEL = 0;         //SOC0 will convert pin B0
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;
#endif
    //Channel B1
    AdcbRegs.ADCSOC1CTL.bit.CHSEL = 1;         //SOC1 will convert pin B1
    AdcbRegs.ADCSOC1CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;
#endif
    //Channel B2
    AdcbRegs.ADCSOC2CTL.bit.CHSEL = 2;         //SOC2 will convert pin B2
    AdcbRegs.ADCSOC2CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdcbRegs.ADCSOC2CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdcbRegs.ADCSOC2CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;
#endif
    //Channel B3
    AdcbRegs.ADCSOC3CTL.bit.CHSEL = 3;         //SOC3 will convert pin B3
    AdcbRegs.ADCSOC3CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdcbRegs.ADCSOC3CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdcbRegs.ADCSOC3CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;
#endif
    // Select the channels to convert and END of conversion flag
    //

    //
    //============================================================================================
    // ------------------------------- ADC Channels C2 - C4 --------------------------------------
    //============================================================================================
    //
    //
    AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 0;     //End of SOC0 will set INT1 flag - EOC is trigger for ADCINT1
    AdccRegs.ADCINTSEL1N2.bit.INT1E = 1;       //Enable INT1 flag
    AdccRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;     //Make sure INT1 flag is cleared
    //
    // Select the channels to convert and END of conversion flag
    //
    // Channel C0
    AdccRegs.ADCSOC0CTL.bit.CHSEL = 0;         //SOC2 will convert pin C2
    AdccRegs.ADCSOC0CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;       //trigger on ePWM1 SOCA/C
#endif
    // Channel C1
    AdccRegs.ADCSOC1CTL.bit.CHSEL = 1;         //SOC2 will convert pin C2
    AdccRegs.ADCSOC1CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdccRegs.ADCSOC1CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdccRegs.ADCSOC1CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;       //trigger on ePWM1 SOCA/C
#endif
    //Channel C2
    AdccRegs.ADCSOC2CTL.bit.CHSEL = 2;         //SOC2 will convert pin C2
    AdccRegs.ADCSOC2CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdccRegs.ADCSOC2CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdccRegs.ADCSOC2CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;
#endif
    //Channel C3
    AdccRegs.ADCSOC3CTL.bit.CHSEL = 3;         //SOC3 will convert pin C3
    AdccRegs.ADCSOC3CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdccRegs.ADCSOC3CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdccRegs.ADCSOC3CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;
#endif
    //Channel C4
    AdccRegs.ADCSOC4CTL.bit.CHSEL = 4;         //SOC3 will convert pin C4
    AdccRegs.ADCSOC4CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdccRegs.ADCSOC4CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdccRegs.ADCSOC4CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;       //trigger on ePWM1 SOCA/C
#endif
    //
    //============================================================================================
    // ------------------------------- ADC Channels D0 - D5 --------------------------------------
    //============================================================================================
    //
    //
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 1;     //End of SOC0 will set INT1 flag - EOC is trigger for ADCINT1
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;       //Enable INT1 flag
#if (USE_CLA_ACQ != 1)
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;     //Make sure INT1 flag is cleared
#endif


    //Channel D0
    AdcdRegs.ADCSOC0CTL.bit.CHSEL = 0;         //SOC0 will convert pin D0
    AdcdRegs.ADCSOC0CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = 5;       //trigger selection please refer to Table 11-55. ADCSOC0CTL Register Field Descriptions ePWM1 SOCA/C
#else
    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;
#endif
    //Channel D1
    AdcdRegs.ADCSOC1CTL.bit.CHSEL = 1;         //SOC1 will convert pin D1
    AdcdRegs.ADCSOC1CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdcdRegs.ADCSOC1CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdcdRegs.ADCSOC1CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;       //trigger on ePWM1 SOCA/C
#endif
    //Channel D2
    AdcdRegs.ADCSOC2CTL.bit.CHSEL = 2;         //SOC2 will convert pin D2
    AdcdRegs.ADCSOC2CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdcdRegs.ADCSOC2CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdcdRegs.ADCSOC2CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;       //trigger on ePWM1 SOCA/C
#endif
    //Channel D3
    AdcdRegs.ADCSOC3CTL.bit.CHSEL = 3;         //SOC3 will convert pin D3
    AdcdRegs.ADCSOC3CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdcdRegs.ADCSOC3CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdcdRegs.ADCSOC3CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;       //trigger on ePWM1 SOCA/C
#endif
    //Channel D4
    AdcdRegs.ADCSOC4CTL.bit.CHSEL = 4;         //SOC3 will convert pin D4
    AdcdRegs.ADCSOC4CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdcdRegs.ADCSOC4CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdcdRegs.ADCSOC4CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;       //trigger on ePWM1 SOCA/C
#endif
    //Channel D5
    AdcdRegs.ADCSOC5CTL.bit.CHSEL = 5;         //SOC3 will convert pin D5
    AdcdRegs.ADCSOC5CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
#if (USE_CLA_ACQ != 1)
    AdcdRegs.ADCSOC5CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#else
    AdcdRegs.ADCSOC5CTL.bit.TRIGSEL = EPWM_ADC_TRIG_SEL;       //trigger on ePWM1 SOCA/C
#endif
    //
    // Select the channels to convert and END of conversion flag
    //

    //
    //
#if (0) // Not connected ADC channels
    //Channel A0
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;         //SOC0 will convert pin A0
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C

    //Channel B0
    AdcbRegs.ADCSOC0CTL.bit.CHSEL = 0;         //SOC0 will convert pin B0
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
    //Channel B1
    AdcbRegs.ADCSOC1CTL.bit.CHSEL = 1;         //SOC1 will convert pin B1
    AdcbRegs.ADCSOC1CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
    //Channel B2
    AdcbRegs.ADCSOC2CTL.bit.CHSEL = 2;         //SOC2 will convert pin B2
    AdcbRegs.ADCSOC2CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
    AdcbRegs.ADCSOC2CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
    //Channel B3
    AdcbRegs.ADCSOC3CTL.bit.CHSEL = 3;         //SOC3 will convert pin B3
    AdcbRegs.ADCSOC3CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
    AdcbRegs.ADCSOC3CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C


    //Channel D0
    AdcdRegs.ADCSOC0CTL.bit.CHSEL = 0;         //SOC0 will convert pin D0
    AdcdRegs.ADCSOC0CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
    //Channel D1
    AdcdRegs.ADCSOC1CTL.bit.CHSEL = 1;         //SOC1 will convert pin D1
    AdcdRegs.ADCSOC1CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
    AdcdRegs.ADCSOC1CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
    //Channel D2
    AdcdRegs.ADCSOC2CTL.bit.CHSEL = 2;         //SOC2 will convert pin D2
    AdcdRegs.ADCSOC2CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
    AdcdRegs.ADCSOC2CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
    //Channel D3
    AdcdRegs.ADCSOC3CTL.bit.CHSEL = 3;         //SOC3 will convert pin D3
    AdcdRegs.ADCSOC3CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
    AdcdRegs.ADCSOC3CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
    //Channel D4
    AdcdRegs.ADCSOC4CTL.bit.CHSEL = 4;         //SOC3 will convert pin D4
    AdcdRegs.ADCSOC4CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
    AdcdRegs.ADCSOC3CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
    //Channel D5
    AdcdRegs.ADCSOC5CTL.bit.CHSEL = 5;         //SOC3 will convert pin D5
    AdcdRegs.ADCSOC5CTL.bit.ACQPS = ACQPS_ADC; //sample window is 100 SYSCLK cycles
    AdcdRegs.ADCSOC5CTL.bit.TRIGSEL = 5;       //trigger on ePWM1 SOCA/C
#endif



    EDIS;
} //End void InitADC(void)
//
void ADC_SYNC()
{
#if (USE_CLA_ACQ != 1)
    EALLOW;
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;       // Disable SOC on A group
    EPwm1Regs.ETSEL.bit.SOCASEL = 2;      // Select SOC on up-count
    EPwm1Regs.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event
    EDIS;
#endif
#if (USE_CLA_ACQ == 1)
    EALLOW;
    EPWM_ADC_CLA_TIMING_MODULE.ETSEL.bit.SOCAEN = 1;       // Disable SOC on A group
    EPWM_ADC_CLA_TIMING_MODULE.ETSEL.bit.SOCASEL = 2;      // Select SOC on up-count
    EPWM_ADC_CLA_TIMING_MODULE.ETPS.bit.SOCAPRD = 1;       // Generate pulse on 1st event
    EDIS;
#endif
} //End void ADC_SYNC()


void ADC_res_to_float()
{

#if (USE_CLA_ACQ != 1)
#if (AUTOMATIC_CURRENT_OFFSET == 1)
    // Currents
    // Cell 1
    in.p1.cur = ADC_CELL1_CUR_RES * ADC_CELL1_CUR_GAIN + current_offsets.cell1;
    // Cell 2
    in.p2.cur = ADC_CELL2_CUR_RES * ADC_CELL2_CUR_GAIN + current_offsets.cell2;
    // Cell 3
    in.p3.cur = ADC_CELL3_CUR_RES * ADC_CELL3_CUR_GAIN + current_offsets.cell3;
    // Cell 4
    in.p4.cur = ADC_CELL4_CUR_RES * ADC_CELL4_CUR_GAIN + current_offsets.cell4;
    // Voltages
    // Cell 1
    in.p1.volt = ADC_CELL1_VOLT_RES * ADC_CELL1_VOLT_GAIN + voltage_offsets.cell1;
    // Cell 2
    in.p2.volt = ADC_CELL2_VOLT_RES * ADC_CELL2_VOLT_GAIN + voltage_offsets.cell2;
    // Cell 3
    in.p3.volt = ADC_CELL3_VOLT_RES * ADC_CELL3_VOLT_GAIN + voltage_offsets.cell3;
    // Cell 4
    in.p4.volt = ADC_CELL4_VOLT_RES * ADC_CELL4_VOLT_GAIN + voltage_offsets.cell4;
    // Calibrate offsets if the converter is in reset state
    if (state_machine_t::get_state().state_spec.label == QAB_RESET)
    {
        // low-pass filter of ADC value when the currents suppose to be zero
        current_offsets.cell1 = LOW_PASS_CAL_CONST * current_offsets.cell1 - (1 - LOW_PASS_CAL_CONST) * ADC_CELL1_CUR_RES * ADC_CELL1_CUR_GAIN;
        current_offsets.cell2 = LOW_PASS_CAL_CONST * current_offsets.cell2 - (1 - LOW_PASS_CAL_CONST) * ADC_CELL2_CUR_RES * ADC_CELL2_CUR_GAIN;
        current_offsets.cell3 = LOW_PASS_CAL_CONST * current_offsets.cell3 - (1 - LOW_PASS_CAL_CONST) * ADC_CELL3_CUR_RES * ADC_CELL3_CUR_GAIN;
        current_offsets.cell4 = LOW_PASS_CAL_CONST * current_offsets.cell4 - (1 - LOW_PASS_CAL_CONST) * ADC_CELL4_CUR_RES * ADC_CELL4_CUR_GAIN;

    }
    if (ADC_request.bit.calibrate_v == 1) // Voltage sensor offset are calibrated only upon special request and in disabled state
    {
        if (state_machine_t::get_state().state_spec.label == QAB_DISABLED)
        {
        voltage_offsets.cell1 = LOW_PASS_CAL_CONST * voltage_offsets.cell1 - (1 - LOW_PASS_CAL_CONST) * ADC_CELL1_VOLT_RES * ADC_CELL1_VOLT_GAIN;
        voltage_offsets.cell2 = LOW_PASS_CAL_CONST * voltage_offsets.cell2 - (1 - LOW_PASS_CAL_CONST) * ADC_CELL2_VOLT_RES * ADC_CELL2_VOLT_GAIN;
        voltage_offsets.cell3 = LOW_PASS_CAL_CONST * voltage_offsets.cell3 - (1 - LOW_PASS_CAL_CONST) * ADC_CELL3_VOLT_RES * ADC_CELL3_VOLT_GAIN;
        voltage_offsets.cell4 = LOW_PASS_CAL_CONST * voltage_offsets.cell4 - (1 - LOW_PASS_CAL_CONST) * ADC_CELL4_VOLT_RES * ADC_CELL4_VOLT_GAIN;
        voltage_offsets.step++;
        if (voltage_offsets.step == 10000)
        {
            ADC_request.bit.calibrate_v = 0;
            voltage_offsets.step = 0;
        }
        }
        else
        {
            ADC_request.bit.calibrate_v = 0;
        }
    }
#else
    // Cell 1
    in.p1.cur = ADC_CELL1_CUR_RES * ADC_CELL1_CUR_GAIN + ADC_CELL1_CUR_OFFSET;
    // Cell 2
    in.p2.cur = ADC_CELL2_CUR_RES * ADC_CELL2_CUR_GAIN + ADC_CELL2_CUR_OFFSET;
    // Cell 3
    in.p3.cur = ADC_CELL3_CUR_RES * ADC_CELL3_CUR_GAIN + ADC_CELL3_CUR_OFFSET;
    // Cell 4
    in.p4.cur = ADC_CELL4_CUR_RES * ADC_CELL4_CUR_GAIN + ADC_CELL4_CUR_OFFSET;

    in.p1.volt = ADC_CELL1_VOLT_RES * ADC_CELL1_VOLT_GAIN + ADC_CELL1_VOLT_OFFSET;
    in.p2.volt = ADC_CELL2_VOLT_RES * ADC_CELL2_VOLT_GAIN + ADC_CELL2_VOLT_OFFSET;
    in.p3.volt = ADC_CELL3_VOLT_RES * ADC_CELL3_VOLT_GAIN + ADC_CELL3_VOLT_OFFSET;
    in.p4.volt = ADC_CELL4_VOLT_RES * ADC_CELL4_VOLT_GAIN + ADC_CELL4_VOLT_OFFSET;
#endif
#endif // #if (USE_CLA_ACQ != 1)
#if (USE_CLA_ACQ == 1)
#if (AUTOMATIC_CURRENT_OFFSET == 1)
#if (MANUAL_SET_OF_ADC_RESULTS != 1)
    // Currents
    debug_adc = ADC_filt.i_sample;
    // Cell 1
    in.p1.meas.cur = ADC_filt.avg[ADC_CELL1_CUR_INDEX] * ADC_CELL1_CUR_GAIN + current_offsets.cell1;
    // Cell 2
    in.p2.meas.cur = ADC_filt.avg[ADC_CELL2_CUR_INDEX] * ADC_CELL2_CUR_GAIN + current_offsets.cell2;
    // Cell 3
    in.p3.meas.cur = ADC_filt.avg[ADC_CELL3_CUR_INDEX] * ADC_CELL3_CUR_GAIN + current_offsets.cell3;
    // Cell 4
    in.p4.meas.cur = ADC_filt.avg[ADC_CELL4_CUR_INDEX] * ADC_CELL4_CUR_GAIN + current_offsets.cell4;
    // Voltages
    // Cell 1
    in.p1.meas.volt = ADC_filt.avg[ADC_CELL1_VOLT_INDEX] * ADC_CELL1_VOLT_GAIN + voltage_offsets.cell1;
    // Cell 2
    in.p2.meas.volt = ADC_filt.avg[ADC_CELL2_VOLT_INDEX] * ADC_CELL2_VOLT_GAIN + voltage_offsets.cell2;
    // Cell 3
    in.p3.meas.volt = ADC_filt.avg[ADC_CELL3_VOLT_INDEX] * ADC_CELL3_VOLT_GAIN + voltage_offsets.cell3;
    // Cell 4
    in.p4.meas.volt = ADC_filt.avg[ADC_CELL4_VOLT_INDEX] * ADC_CELL4_VOLT_GAIN + voltage_offsets.cell4;
    // Calibrate offsets if the converter is in reset state
    if (state_machine_t::get_state().state_spec.label == QAB_RESET)
    {
        // low-pass filter of ADC value when the currents suppose to be zero
        current_offsets.cell1 = LOW_PASS_CAL_CONST * current_offsets.cell1 - (1 - LOW_PASS_CAL_CONST) * ADC_filt.avg[ADC_CELL1_CUR_INDEX] * ADC_CELL1_CUR_GAIN;
        current_offsets.cell2 = LOW_PASS_CAL_CONST * current_offsets.cell2 - (1 - LOW_PASS_CAL_CONST) * ADC_filt.avg[ADC_CELL2_CUR_INDEX] * ADC_CELL2_CUR_GAIN;
        current_offsets.cell3 = LOW_PASS_CAL_CONST * current_offsets.cell3 - (1 - LOW_PASS_CAL_CONST) * ADC_filt.avg[ADC_CELL3_CUR_INDEX] * ADC_CELL3_CUR_GAIN;
        current_offsets.cell4 = LOW_PASS_CAL_CONST * current_offsets.cell4 - (1 - LOW_PASS_CAL_CONST) * ADC_filt.avg[ADC_CELL4_CUR_INDEX] * ADC_CELL4_CUR_GAIN;

    }
    if (ADC_request.bit.calibrate_v == 1) // Voltage sensor offset are calibrated only upon special request and in disabled state
    {
        if (state_machine_t::get_state().state_spec.label == QAB_DISABLED)
        {
        voltage_offsets.cell1 = LOW_PASS_CAL_CONST * voltage_offsets.cell1 - (1 - LOW_PASS_CAL_CONST) * ADC_filt.avg[ADC_CELL1_VOLT_INDEX] * ADC_CELL1_VOLT_GAIN;
        voltage_offsets.cell2 = LOW_PASS_CAL_CONST * voltage_offsets.cell2 - (1 - LOW_PASS_CAL_CONST) * ADC_filt.avg[ADC_CELL2_VOLT_INDEX] * ADC_CELL2_VOLT_GAIN;
        voltage_offsets.cell3 = LOW_PASS_CAL_CONST * voltage_offsets.cell3 - (1 - LOW_PASS_CAL_CONST) * ADC_filt.avg[ADC_CELL3_VOLT_INDEX] * ADC_CELL3_VOLT_GAIN;
        voltage_offsets.cell4 = LOW_PASS_CAL_CONST * voltage_offsets.cell4 - (1 - LOW_PASS_CAL_CONST) * ADC_filt.avg[ADC_CELL4_VOLT_INDEX] * ADC_CELL4_VOLT_GAIN;
        voltage_offsets.step++;
        if (voltage_offsets.step == 10000)
        {
            ADC_request.bit.calibrate_v = 0;
            voltage_offsets.step = 0;
        }
        }
        else
        {
            ADC_request.bit.calibrate_v = 0;
        }
    }
#endif // (MANUAL_SET_OF_ADC_RESULTS != 1)
#else
    // Cell 1
    in.p1.cur = ADC_filt.avg[ADC_CELL1_CUR_INDEX] * ADC_CELL1_CUR_GAIN + ADC_CELL1_CUR_OFFSET;
    // Cell 2
    in.p2.cur = ADC_filt.avg[ADC_CELL2_CUR_INDEX] * ADC_CELL2_CUR_GAIN + ADC_CELL2_CUR_OFFSET;
    // Cell 3
    in.p3.cur = ADC_filt.avg[ADC_CELL3_CUR_INDEX] * ADC_CELL3_CUR_GAIN + ADC_CELL3_CUR_OFFSET;
    // Cell 4
    in.p4.cur = ADC_filt.avg[ADC_CELL4_CUR_INDEX] * ADC_CELL4_CUR_GAIN + ADC_CELL4_CUR_OFFSET;

    in.p1.volt = ADC_filt.avg[ADC_CELL1_VOLT_INDEX] * ADC_CELL1_VOLT_GAIN + ADC_CELL1_VOLT_OFFSET;
    in.p2.volt = ADC_filt.avg[ADC_CELL2_VOLT_INDEX] * ADC_CELL2_VOLT_GAIN + ADC_CELL2_VOLT_OFFSET;
    in.p3.volt = ADC_filt.avg[ADC_CELL3_VOLT_INDEX] * ADC_CELL3_VOLT_GAIN + ADC_CELL3_VOLT_OFFSET;
    in.p4.volt = ADC_filt.avg[ADC_CELL4_VOLT_INDEX] * ADC_CELL4_VOLT_GAIN + ADC_CELL4_VOLT_OFFSET;
#endif
#endif // #if (USE_CLA_ACQ == 1)


    // long filtering:
    // Voltage
    meas_filt_100ms.p1.volt = (1-FILT_CONST_100MS)*meas_filt_100ms.p1.volt + FILT_CONST_100MS*in.p1.meas.volt;
    meas_filt_100ms.p2.volt = (1-FILT_CONST_100MS)*meas_filt_100ms.p2.volt + FILT_CONST_100MS*in.p2.meas.volt;
    meas_filt_100ms.p3.volt = (1-FILT_CONST_100MS)*meas_filt_100ms.p3.volt + FILT_CONST_100MS*in.p3.meas.volt;
    meas_filt_100ms.p4.volt = (1-FILT_CONST_100MS)*meas_filt_100ms.p4.volt + FILT_CONST_100MS*in.p4.meas.volt;
    // Current
    meas_filt_100ms.p1.cur = (1-FILT_CONST_100MS)*meas_filt_100ms.p1.cur + FILT_CONST_100MS*in.p1.meas.cur;
    meas_filt_100ms.p2.cur = (1-FILT_CONST_100MS)*meas_filt_100ms.p2.cur + FILT_CONST_100MS*in.p2.meas.cur;
    meas_filt_100ms.p3.cur = (1-FILT_CONST_100MS)*meas_filt_100ms.p3.cur + FILT_CONST_100MS*in.p3.meas.cur;
    meas_filt_100ms.p4.cur = (1-FILT_CONST_100MS)*meas_filt_100ms.p4.cur + FILT_CONST_100MS*in.p4.meas.cur;

    // 10 ms
    // Voltage
    meas_filt_10ms.p1.volt = (1-FILT_CONST_10MS)*meas_filt_10ms.p1.volt + FILT_CONST_10MS*in.p1.meas.volt;
    meas_filt_10ms.p2.volt = (1-FILT_CONST_10MS)*meas_filt_10ms.p2.volt + FILT_CONST_10MS*in.p2.meas.volt;
    meas_filt_10ms.p3.volt = (1-FILT_CONST_10MS)*meas_filt_10ms.p3.volt + FILT_CONST_10MS*in.p3.meas.volt;
    meas_filt_10ms.p4.volt = (1-FILT_CONST_10MS)*meas_filt_10ms.p4.volt + FILT_CONST_10MS*in.p4.meas.volt;
    // Current
    meas_filt_10ms.p1.cur = (1-FILT_CONST_10MS)*meas_filt_10ms.p1.cur + FILT_CONST_10MS*in.p1.meas.cur;
    meas_filt_10ms.p2.cur = (1-FILT_CONST_10MS)*meas_filt_10ms.p2.cur + FILT_CONST_10MS*in.p2.meas.cur;
    meas_filt_10ms.p3.cur = (1-FILT_CONST_10MS)*meas_filt_10ms.p3.cur + FILT_CONST_10MS*in.p3.meas.cur;
    meas_filt_10ms.p4.cur = (1-FILT_CONST_10MS)*meas_filt_10ms.p4.cur + FILT_CONST_10MS*in.p4.meas.cur;


   /* Cell 1: Conn1
    Current sensor : 7 : ADCIn1 : ADCCIN_C4
    Voltage sensor : 8 : ADCIn2 : ADCCIN_C3

    Cell 2: Conn2
    Current sensor : 7 : ADCIn3 : ADCCIN_C2
    Voltage sensor : 8 : ADCIn4 : ADCCIN_A5

    Cell 3: Conn3
    Current sensor : 7 : ADCIn5 : ADCCIN_A4
    Voltage sensor : 8 : ADCIn6 : ADCCIN_A3

    Cell 4: Conn4
    Current sensor : 7 : ADCIn7 : ADCCIN_A2
    Voltage sensor : 8 : ADCIn8 : ADCCIN_A1*/

}
