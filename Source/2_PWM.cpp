//###########################################################################
//
// FILE:    2_PWM.c
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
#include "2_PWM.h"
#include "General.h"
#include "Defines.h"
#include "protection_t.h"

//
//
//


epwm_mod_t ePwm2vals;
epwm_mod_t ePwm3vals;
epwm_mod_t ePwm4vals;
epwm_mod_t ePwm5vals;
epwm_mod_t ePwm6vals;
epwm_mod_t ePwm7vals;
epwm_mod_t ePwm8vals;

#if ENABLE_VAR_SW == 1
static uint16_t write_pwm_rq = 0;
#endif

void initEPwm_AQCTL_cascaded(volatile EPWM_REGS* EPwm)
{
    // Zero - go to one
    EPwm->AQCTLA.bit.ZRO = AQ_SET;      // Set PWM2A at zero
    EPwm->AQCTLB.bit.ZRO = AQ_CLEAR;    // Clear PWM2B at zero (complementary)
    // Counting up and cmpA is hit - go to zero
    EPwm->AQCTLA.bit.CAU = AQ_CLEAR;    // Clear PWM2A on event A, UP count
    EPwm->AQCTLB.bit.CAU = AQ_SET;      // Set PWM2B on event A, UP count (complementary)
    // Counting up and cmpB is hit - no action
    EPwm->AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm->AQCTLB.bit.CBU = AQ_NO_ACTION;
    // Prd - go to one
    EPwm->AQCTLA.bit.PRD = AQ_SET;    // Clear PWM2A at period
    EPwm->AQCTLB.bit.PRD = AQ_CLEAR;      // Set PWM2B at period (complementary)
    // Counting down and cmpb is hit - go to zero
    EPwm->AQCTLA.bit.CBD = AQ_CLEAR;    // Clear PWM2A on event B, DOWN count
    EPwm->AQCTLB.bit.CBD = AQ_SET;      // Set PWM2B on event B, DOWN count (complementary)
    // Counting down and cmbA is hit - no action
    EPwm->AQCTLA.bit.CAD = AQ_NO_ACTION;
    EPwm->AQCTLB.bit.CAD = AQ_NO_ACTION;
    // Shadowed Compare logic:
    EPwm->AQCTL.bit.SHDWAQAMODE = 1;
    EPwm->AQCTL.bit.SHDWAQBMODE = 1;

    EPwm->TBPHS.bit.TBPHS = 0.5*EPwm->TBPRD;
    EPwm->TBCTL.bit.PHSDIR = 0;
    EPwm->TBCTL.bit.PHSEN = TB_ENABLE;
}


void initEPwm_single(volatile EPWM_REGS* EPwm)
{
    // ... Other configurations ...

    EPwm->TBPRD = EPWM_TBPRD;                   // Set timer period TPWM = 2 x TBPRD x TTBCLK and FPWM = 1/TPWM
    EPwm->TBPHS.bit.TBPHS = 0x0000;             // Phase is 0
    EPwm->TBCTR = 0x0000;                       // Clear counter
    // Set Compare values
    EPwm->CMPA.bit.CMPA = Start_DutyCycle;      // Set compare A value - 50 %
    EPwm->CMPB.bit.CMPB = Start_DutyCycle;      // Set compare B value - 50 %
    // Setup counter mode
    EPwm->TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;  // Count up and down
    //EPwm->TBCTL.bit.PHSEN = TB_DISABLE;       // Disable phase loading
    EPwm->TBCTL.bit.PHSEN = TB_ENABLE;          // Enable phase loading
    EPwm->TBCTL.bit.SYNCOSEL = TB_SYNC_IN;      // sync flow-through
    EPwm->TBCTL.bit.HSPCLKDIV = TB_DIV1;        // Clock ratio to SYSCLKOUT
    EPwm->TBCTL.bit.CLKDIV = TB_DIV1;
    // Setup shadowing
    EPwm->CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm->CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm->CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;   // Load on Zero
    EPwm->CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    // Shadowed Compare logic:
    EPwm->AQCTL.bit.SHDWAQAMODE = 1;
    EPwm->AQCTL.bit.SHDWAQBMODE = 1;
    // Set actions
    EPwm->AQCTLA.bit.CAU = AQ_SET;            // Clear PWMA on event A, UP count
    EPwm->AQCTLB.bit.CAU = AQ_CLEAR;            // Clear PWMB on event B, UP count
    EPwm->AQCTLA.bit.CBD = AQ_CLEAR;              // Set PWMA on event A, DOWN count
    EPwm->AQCTLB.bit.CBD = AQ_SET;              // Set PWMB on event B, DOWN count

    // SetupDead band
    EPwm->DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;  // Enable Deadband
    EPwm->DBCTL.bit.POLSEL = DB_ACTV_HIC;       // Logic
    EPwm->DBCTL.bit.IN_MODE = DBA_ALL;          // Reference channel
    EPwm->DBRED.bit.DBRED = EPWM1_DB;           // Deadband value channel A
    EPwm->DBFED.bit.DBFED = EPWM1_DB;           // Deadband value channel B

    



    // ... Rest of the function ...
}

//void initEPwm_single_inverted(volatile EPWM_REGS* EPwm)
//{
//    // Same as initEPwm_single but with inverted logic
//    initEPwm_single(EPwm);
//    EPwm->AQCTLB.bit.CAU = AQ_SET;            // Clear PWMB on event A, UP count
//    EPwm->AQCTLA.bit.CAU = AQ_CLEAR;            // Clear PWMA on event B, UP count
//    EPwm->AQCTLB.bit.CBD = AQ_CLEAR;              // Set PWMB on event A, DOWN count
//    EPwm->AQCTLA.bit.CBD = AQ_SET;              // Set PWMA on event B, DOWN count
//}

static void EPwm_setup_tripzone(volatile EPWM_REGS* EPwm)
{
    EALLOW;
#if (ENABLE_TRIP_ZONE == 1)
#if (SEPARATED_TRIP_ZONES == 0)
    EPwm->TZSEL.bit.OSHT1 = 1;              // Enables TZ1 as a one-shot event source for
    EPwm->TZSEL.bit.OSHT2 = 1;              // Enables TZ2 as a one-shot event source for
    EPwm->TZSEL.bit.OSHT3 = 1;              // Enables TZ3 as a one-shot event source for
#endif
    EPwm->TZCTL.bit.TZA = 2;                // Force to Low state
    EPwm->TZCTL.bit.TZB = 2;                // Force to Low state



    EPwm->TZCLR.bit.OST = 1;                // Clear Flag for one-shot trip
    EPwm->TZCLR.bit.INT = 1;                // Clear interrupt Flag for this EPwm module
    EPwm->TZOSTCLR.bit.OST1 = 1;            // Clear Flag for one-shot trip latch
    EPwm->TZOSTCLR.bit.OST2 = 1;            // Clear Flag for one-shot trip latch
    EPwm->TZOSTCLR.bit.OST3 = 1;            // Clear Flag for one-shot trip latch
#if DISABLE_EMU_STOP_TRIPZONE != 1
    EPwm->TZSEL.bit.CBC6 = 1;               // Emu stop tripzone -
#endif
    EPwm->TZCLR.bit.CBC = 1;
    EPwm->TZCLR.bit.OST = 1;
    EPwm->TZCLR.bit.DCAEVT1 = 1;
    EPwm->TZCLR.bit.INT = 1;

    // !important -> when reset during operation the PWM stays open (on) for a few microseconds ->
    // this will prevent such situations
#endif
    EDIS;
}

interrupt void EPWM_cell1_tripzone_isr()
{
    protection_t::set_error(FAULT_P1);
    //EALLOW;
    //EPwm4Regs.TZCLR.bit.INT = 1;
    //EDIS;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}

interrupt void EPWM_cell2_tripzone_isr()
{
    protection_t::set_error(FAULT_P2);

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}

interrupt void EPWM_cell3_tripzone_isr()
{
    protection_t::set_error(FAULT_P3);

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}

interrupt void EPWM_cell4_tripzone_isr()
{
    protection_t::set_error(FAULT_P4);

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}

interrupt void EPWM_cell4_tripzone_sw_force_isr()
{
    protection_t::set_error(FAULT_P4);

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}



void InitPWM(void)
{
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0; // temporarily stop the PWM time-base
    EDIS;

    // make sure that PWMs controlling the cells are disabled before PWM init
    DisableEPwm1Gpio  ();       //Disable PWM1A and PWM1B
    DisableEPwm2Gpio  ();       //Disable PWM2A and PWM2B
    DisableEPwm3Gpio  ();       //Disable PWM3A and PWM3B - CELL 2
    DisableEPwm4Gpio  ();       //Disable PWM4A and PWM4B - CELL 1
    DisableEPwm5Gpio  ();       //Disable PWM5A and PWM5B - CELL 4
    DisableEPwm6Gpio  ();       //Disable PWM6A and PWM6B - CELL 3
    DisableEPwm7Gpio  ();       //Disable PWM7A and PWM7B
    DisableEPwm8Gpio  ();       //Disable PWM8A and PWM8B
    DisableEPwm9Gpio  ();       //Disable PWM9A and PWM9B
    DisableEPwm10Gpio ();       //Disable PWM10A and PWM10B
//    DisableEPwm11Gpio ();       //Disable PWM11A and PWM11B
//    DisableEPwm12Gpio ();       //Disable PWM12A and PWM12B
    // EPWM init all in the same manner
    // Cascaded cell 1

#if CASCADED_QAB == 1

    initEPwm_single(&EPwm1Regs);
    initEPwm_single(&EPwm2Regs); // Cell1
    initEPwm_single(&EPwm3Regs);
    initEPwm_single(&EPwm4Regs); // Cell1
    initEPwm_single(&EPwm5Regs);
    initEPwm_single(&EPwm6Regs);
    initEPwm_single(&EPwm7Regs);
    initEPwm_single(&EPwm8Regs);

    // cascaded cell 4
    initEPwm_AQCTL_cascaded(&EPwm5Regs); // First leg
    initEPwm_AQCTL_cascaded(&EPwm8Regs); // Second leg
    // cascaded cell 1
    initEPwm_AQCTL_cascaded(&EPwm2Regs); // Second leg
    initEPwm_AQCTL_cascaded(&EPwm4Regs); // First leg
#else
    initEPwm_single(&EPwm1Regs); // Not connected, GPIOs are used to control isolation switches
    initEPwm_single(&EPwm2Regs); // Cell 1 - right leg inverted
    initEPwm_single(&EPwm3Regs); // Cell 2
    initEPwm_single(&EPwm4Regs); // Cell 1 - left leg
    initEPwm_single(&EPwm5Regs); // Cell 4 - left leg
    initEPwm_single(&EPwm6Regs); // Cell 3
    initEPwm_single(&EPwm7Regs); // Not connected, GPIOs are used to control isolation switches
    initEPwm_single(&EPwm8Regs); // Cell 4 - right leg
#endif


    // EPwms for CLA1 and CPU1 synchronization
    initEPwm_single(&EPWM_ADC_CLA_TIMING_MODULE);
    initEPwm_single(&EPWM_CTRL_TIMING_MODULE);
#if ENABLE_VAR_SW == 1
    // EPwm for timing the load phase and new switching period. Must be after EPWM_CTRL_TIMING_MODULE becasue of ISR priority.
    initEPwm_single(&EPWM_REGISTER_LOAD_TIMING_MODULE);
#endif
    EALLOW;
    EPWM_ADC_CLA_TIMING_MODULE.ETSEL.bit.INTEN = 1; // Enable EPWM interrupt
    EPWM_ADC_CLA_TIMING_MODULE.ETSEL.bit.INTSEL = 1; // CLA is called at the bottom of EPWM
    EPWM_ADC_CLA_TIMING_MODULE.ETPS.bit.INTPRD = 1; // Call interrupt every service routine
    EPWM_ADC_CLA_TIMING_MODULE.TBPRD = EPWM_TBPRD/ADC_PWM_RAT;
    EPWM_ADC_CLA_TIMING_MODULE.TBCTL.bit.PHSEN = TB_DISABLE;  // Do not allow sync
    EPWM_CTRL_TIMING_MODULE.ETSEL.bit.INTEN = 1; // Enable EPWM interrupt
    EPWM_CTRL_TIMING_MODULE.ETSEL.bit.INTSEL = 2; // CPU is interrupted at the top of EPWM
    EPWM_CTRL_TIMING_MODULE.ETPS.bit.INTPRD = 1; // Call interrupt every service routine
    EPWM_CTRL_TIMING_MODULE.TBCTL.bit.PHSEN = TB_DISABLE;     // Do not allow sync
    EPWM_CTRL_TIMING_MODULE.ETCLR.bit.INT = 1; // Clear the interrupt flags
#if ENABLE_VAR_SW == 1
    EPWM_REGISTER_LOAD_TIMING_MODULE.ETSEL.bit.INTEN = 1; // Enable EPWM interrupt
    EPWM_REGISTER_LOAD_TIMING_MODULE.ETSEL.bit.INTSEL = 3; // CPU is interrupted both at the top and bottom of EPWM
    EPWM_REGISTER_LOAD_TIMING_MODULE.ETPS.bit.INTPRD = 1; // Call interrupt every service routine
    EPWM_REGISTER_LOAD_TIMING_MODULE.TBCTL.bit.PHSEN = TB_DISABLE;     // Do not allow sync
    EPWM_REGISTER_LOAD_TIMING_MODULE.ETCLR.bit.INT = 1; // Clear the interrupt flags
#endif

#if EXTERNAL_PWM_SYNC == 1
    EALLOW;
    InputXbarRegs.INPUT5SELECT = 20;
    EDIS;
#endif // #if EXTERNAL_PWM_SYNC == 1

// If variable switching frequency
#if ENABLE_VAR_SW == 1
  EPwm_set_phase_shift(&ePwm2vals, 0);
  EPwm_set_phase_shift(&ePwm3vals, 0);
  EPwm_set_phase_shift(&ePwm4vals, 0);
  EPwm_set_phase_shift(&ePwm5vals, 0);
  EPwm_set_phase_shift(&ePwm6vals, 0);
  EPwm_set_phase_shift(&ePwm8vals, 0);

#endif

  EDIS;

  EALLOW;
  PieVectTable.EPWM3_TZ_INT = &EPWM_cell2_tripzone_isr;
  PieVectTable.EPWM4_TZ_INT = &EPWM_cell1_tripzone_isr;
  PieVectTable.EPWM5_TZ_INT = &EPWM_cell4_tripzone_isr;
  PieVectTable.EPWM6_TZ_INT = &EPWM_cell3_tripzone_isr;
  PieVectTable.XINT1_INT = &EPWM_cell4_tripzone_sw_force_isr;

  // CPU EPwm interrupt

  EDIS;

  EPwm_setup_tripzone(&EPwm3Regs);
  EPwm_setup_tripzone(&EPwm4Regs);
  EPwm_setup_tripzone(&EPwm5Regs);
  EPwm_setup_tripzone(&EPwm6Regs);
//    EPwm_setup_tripzone(&EPwm8Regs);
#if (SEPARATED_TRIP_ZONES == 1)
    EALLOW;
    EPwm4Regs.TZSEL.bit.OSHT1 = 1;              // Enables TZ1 as a one-shot event source for
    EPwm3Regs.TZSEL.bit.OSHT2 = 1;              // Enables TZ2 as a one-shot event source for
    EPwm6Regs.TZSEL.bit.OSHT3 = 1;              // Enables TZ3 as a one-shot event source for
    // Cell 4 fault only triggers interrupt XINT1
//    XintRegs.XINT1CR.bit.POLARITY = 0;          // Falling edge interrupt
    //
    // Enable XINT1
    //
//    XintRegs.XINT1CR.bit.ENABLE = 1;            // Enable XINT1

    // Enable TZ interrupt
    //

#endif
    // Enable TZ interrupt
    //
    EPwm5Regs.TZEINT.bit.OST = 1;
    EPwm5Regs.TZEINT.bit.DCAEVT1 = 1;
    EPwm5Regs.DCACTL.bit.EVT1FRCSYNCSEL = 1;


    // Map INPUT1 to TRIP4 of the ePWM X-BAR
    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX7 = 1;  // Enable INPUT1 to TRIP4
    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX7 = 1;    // Enable MUX0 for TRIP4

#if (ENABLE_TRIP_ZONE == 1)
    EPwm5Regs.DCALTRIPSEL.bit.TRIPINPUT4 = 1;
    EPwm5Regs.DCTRIPSEL.bit.DCALCOMPSEL = 3;


    //EPwm1Regs.TZSEL.bit.OSHT4 = 1;          // Enable TRIP4 as one-shot trip
    //EPwm1Regs.DC

    EPwm5Regs.TZCTL.bit.DCAEVT1 = 1;

    //EPwm1Regs.TZCTL.bit.DCAEVT2 = 1;
    EPwm5Regs.TZDCSEL.bit.DCAEVT1 = 3;

    EPwm5Regs.TZSEL.bit.DCAEVT1 = 1;
#endif


    EDIS;


    // InputXBar as EPwm tripzone
    //You are selecting input for TripZone
#ifndef LAUNCHPAD
    EALLOW;
    InputXbarRegs.INPUT1SELECT = 16; //cell1 EPWM4
    InputXbarRegs.INPUT2SELECT = 18; //Cell2 EPWM3
    InputXbarRegs.INPUT3SELECT = 22; //Cell3 EPWM6
    InputXbarRegs.INPUT4SELECT = 21; //Cell4 EPWM5
    EDIS;
#endif
#ifdef LAUNCHPAD
    EALLOW;
    InputXbarRegs.INPUT1SELECT = 60;
    InputXbarRegs.INPUT2SELECT = 60;
    InputXbarRegs.INPUT3SELECT = 60;
    InputXbarRegs.INPUT4SELECT = 60;
    EDIS;
#endif
    //GpioCtrlRegs.GPCINV.bit.GPIO68 = 1;
    /*
    Receive - possible fault signals
    RxA ... DataOut1 ... PWM9A  ... GPIO16
    RxB ... DataOut2 ... PWM10A ... GPIO18
    RxC ... DataOut3 ... PWM11A ... GPIO20
    RxD ... DataOut4 ... PWM11B ... GPIO21
    RxE ... DataOut5 ... PWM12A ... GPIO22
    RxF ... DataOut6 ... PWM12B ... GPIO23
    */

    // Sync from EPWM1
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;      // sync flow-through






    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;



}

#if ENABLE_VAR_SW == 1
static inline void EPwm_single_set_tbreg(volatile epwm_mod_t& EPwm, uint16_t new_val)
#else
static inline void EPwm_single_set_tbreg(volatile EPWM_REGS& EPwm, uint16_t new_val)
#endif
{
    EPwm.TBPRD = new_val;
}

#if ENABLE_VAR_SW == 1
void EPwm_set_frequency(float fsw)
{
    uint16_t tbprd_val = (EPWM_TBPRD/ADC_PWM_RAT)*((uint16_t)((0.5/fsw*PWM_CLK_FREQ) + 0.5)/(EPWM_TBPRD/ADC_PWM_RAT));//uint16_t tbprd_val = ADC_PWM_RAT*((uint16_t)((0.5/fsw*PWM_CLK_FREQ) + 0.5)/ADC_PWM_RAT);
    EPwm_single_set_tbreg(ePwm7vals, tbprd_val); // Cell 1 - right leg inverted
    EPwm_single_set_tbreg(ePwm3vals, tbprd_val); // Cell 2
    EPwm_single_set_tbreg(ePwm8vals, tbprd_val); // Cell 1 - left leg
    EPwm_single_set_tbreg(ePwm5vals, tbprd_val); // Cell 4 - left leg
    EPwm_single_set_tbreg(ePwm6vals, tbprd_val); // Cell 3
    EPwm_single_set_tbreg(ePwm8vals, tbprd_val); // Cell 4 - right leg
}
#endif // ENABLE_VAR_SW == 1



void EPwm_clear_tripzone_single(volatile EPWM_REGS* EPwm)
{
    EALLOW;
    EPwm->TZCLR.bit.OST = 1;
    EPwm->TZCLR.bit.CBC = 1;
    EPwm->TZCLR.bit.DCAEVT1 = 1;
    EPwm->TZCLR.bit.INT = 1;
    EDIS;
}

void EPwm_clear_trip_zone()
{
    EPwm_clear_tripzone_single(&EPwm3Regs);
    EPwm_clear_tripzone_single(&EPwm4Regs);
    EPwm_clear_tripzone_single(&EPwm5Regs);
    EPwm_clear_tripzone_single(&EPwm6Regs);
    EPwm_clear_tripzone_single(&EPwm7Regs);
    EPwm_clear_tripzone_single(&EPwm8Regs);
}

uint16_t EPwm_faults()
{
    return (EPwm4Regs.TZFLG.all != 0) | (EPwm3Regs.TZFLG.all != 0)<< 1 | (EPwm6Regs.TZFLG.all != 0)<< 2 | (EPwm5Regs.TZFLG.all != 0)<< 3;
}

inline void Epwm_write_ePWM_regs_single(volatile epwm_mod_t* EPwm_vals, volatile EPWM_REGS* EPwm_regs)
{
    EPwm_regs->AQCTLA.all = EPwm_vals->AQCTLA.all;
    EPwm_regs->AQCTLB.all = EPwm_vals->AQCTLB.all;
    EPwm_regs->CMPA.all = EPwm_vals->CMPA.all;
    EPwm_regs->CMPB.all = EPwm_vals->CMPB.all;
    EPwm_regs->TBPRD = EPwm_vals->TBPRD;

}

#if ENABLE_VAR_SW == 1
void Epwm_write_ePWM_regs()
{
  
    // Update PWM period and phase shifts
    EPwm1Regs.TBPRD = ePwm2vals.TBPRD;
    Epwm_write_ePWM_regs_single(&ePwm2vals, &EPwm2Regs);
    Epwm_write_ePWM_regs_single(&ePwm3vals, &EPwm3Regs);
    Epwm_write_ePWM_regs_single(&ePwm4vals, &EPwm4Regs);
    Epwm_write_ePWM_regs_single(&ePwm5vals, &EPwm5Regs);
    Epwm_write_ePWM_regs_single(&ePwm6vals, &EPwm6Regs);
    //EPwm7Regs.TBPRD = ePwm2vals.TBPRD;
    Epwm_write_ePWM_regs_single(&ePwm7vals, &EPwm7Regs);
    Epwm_write_ePWM_regs_single(&ePwm8vals, &EPwm8Regs);

}
#endif



#if ENABLE_VAR_SW == 1
void EPwm_set_phase_shift(volatile epwm_mod_t* EPwm, float shift)
#else
void EPwm_set_phase_shift(volatile EPWM_REGS* EPwm, float shift)
#endif
{
    if (shift > M_PI)
    {
        shift -= 2*M_PI;
    }
    else if (shift < -M_PI)
    {
        shift += 2*M_PI;
    }
    // Do not forget to deal with priorities
    int32_t cmpa = 0.5f * EPwm->TBPRD * (1.0f + (2.0f * shift / M_PI));
    int32_t cmpb = 0.5f * EPwm->TBPRD * (1.0f - (2.0f * shift / M_PI));

    if (cmpa > EPwm->TBPRD)
    {
        EPwm->AQCTLB.bit.CAU = AQ_SET;            // Set PWMB on event A, UP count
        EPwm->AQCTLA.bit.CAU = AQ_CLEAR;            // Clear PWMA on event B, UP count
        EPwm->AQCTLB.bit.CBD = AQ_CLEAR;              // Set PWMB on event A, DOWN count
        EPwm->AQCTLA.bit.CBD = AQ_SET;              // Clear PWMA on event B, DOWN count
        // Ensure safe transient between two conditions: within +/-90 degrees and outside of +/-90 degrees
        // Note: The zero event has lower priority then CAU, CAD, CBU and CBD
        EPwm->AQCTLB.bit.ZRO = AQ_CLEAR;    // Clear PWMB on Zero event
        EPwm->AQCTLA.bit.ZRO = AQ_SET;      // Set PWMA on Zero event


        EPwm->CMPA.bit.CMPA = cmpa - EPwm->TBPRD;
        EPwm->CMPB.bit.CMPB = cmpb + EPwm->TBPRD;
    }
    else if (cmpb > EPwm->TBPRD)
    {
        EPwm->AQCTLB.bit.CAU = AQ_SET;            // Set PWMB on event A, UP count
        EPwm->AQCTLA.bit.CAU = AQ_CLEAR;            // Clear PWMA on event B, UP count
        EPwm->AQCTLB.bit.CBD = AQ_CLEAR;              // Set PWMB on event A, DOWN count
        EPwm->AQCTLA.bit.CBD = AQ_SET;              // Clear PWMA on event B, DOWN count
        // Ensure safe transient between two conditions: within +/-90 degrees and outside of +/-90 degrees
        // Note: The zero event has lower priority then CAU, CAD, CBU and CBD
        EPwm->AQCTLB.bit.ZRO = AQ_CLEAR;    // Clear PWMB on Zero event
        EPwm->AQCTLA.bit.ZRO = AQ_SET;      // Set PWMA on Zero event

        EPwm->CMPA.bit.CMPA = cmpa + EPwm->TBPRD;
        EPwm->CMPB.bit.CMPB = cmpb - EPwm->TBPRD;

    }
    else
    {
        EPwm->AQCTLA.bit.CAU = AQ_SET;            // Clear PWMB on event A, UP count
        EPwm->AQCTLB.bit.CAU = AQ_CLEAR;            // Clear PWMA on event B, UP count
        EPwm->AQCTLA.bit.CBD = AQ_CLEAR;              // Set PWMB on event A, DOWN count
        EPwm->AQCTLB.bit.CBD = AQ_SET;              // Set PWMA on event B, DOWN count
        EPwm->AQCTLA.bit.ZRO = AQ_CLEAR;    // Clear PWMB on Zero event
        EPwm->AQCTLB.bit.ZRO = AQ_SET;      // Set PWMA on Zero event
        EPwm->CMPA.bit.CMPA = cmpa;// + EPwm->TBPRD;
        EPwm->CMPB.bit.CMPB = cmpb;// - EPwm->TBPRD;

    }


}


void EPwm_set_pointer_slave_cell_reference(volatile EPWM_REGS* EPwm_leg1, volatile EPWM_REGS* EPwm_leg2, float P, float shift)
{
    if (P > 0.5)
    {
        P = 0.5;
    }
    else if (P < 0)
    {
        P = 0;
    }
    EPwm_leg1->CMPA.bit.CMPA = EPwm_leg1->TBPRD * (1 - P);
    EPwm_leg1->CMPB.bit.CMPB = EPwm_leg1->TBPRD * (1 - P);

    EPwm_leg2->CMPA.bit.CMPA = EPwm_leg2->TBPRD * P;
    EPwm_leg2->CMPB.bit.CMPB = EPwm_leg2->TBPRD * P;

    // Extra phase shift
    if (shift > 0.5*M_PI)
    {
        shift = 0.5*M_PI;
    }
    else if (shift < -0.5*M_PI)
    {
        shift = -0.5*M_PI;
    }
    EPwm_leg1->TBPHS.bit.TBPHS = shift/M_PI*EPwm_leg1->TBPRD + 0.5*EPwm_leg1->TBPRD;
    EPwm_leg2->TBPHS.bit.TBPHS = shift/M_PI*EPwm_leg2->TBPRD + 0.5*EPwm_leg2->TBPRD;
}

#if FIXED_SW_COMB == 1
void EPwm_set_duty(volatile EPWM_REGS* EPwm, float duty)
{
    EPwm->CMPA.bit.CMPA = EPwm->TBPRD * duty;
    EPwm->CMPB.bit.CMPB = EPwm->TBPRD * duty;
}
#endif


