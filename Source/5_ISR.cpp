//###########################################################################
//
// FILE:    5_ISR.c
//
// TITLE:    Interruptions Setup v1.0
//
// Author: Thiago Pereira - March 2020
//
//
//###########################################################################
//
//=
// ------------------------- Included Files ---------------------------------
//
#include "F28x_Project.h"
#include "General.h"
#include "Defines.h"
#include "math.h"

#include "globals.h"
#if (PORT_OPTION_SEL == PORTS_SEP)
#include "ctrl_var/ctrl_t.h"
#endif
#if (PORT_OPTION_SEL == PORTS_PAR)
#include "ctrl_parallel_t.h"
#endif
#include "protection_t.h"
#include "2_PWM.h"
#include "3_ADC.h"
#include "test_t.h"
#include "state_machine_t.h"



//===========================================================================
// ------------------------- Interruptions Functions ------------------------
//===========================================================================
__interrupt void adca1_isr(void);

interrupt void EPWM_x_main_routine_isr();

interrupt void EPWM_x_load_routine();

inline void main_ISR_routine()
{

    // TODO: update QAB measurement
    ADC_res_to_float();
    // automatic setpoint based UDC1


#if AUTOMATIC_SETPOINT == 1
#if (EXP_SETUP == FAULT_CONTROL)
    in.p2.req.volt = (1 - 0.0005) * in.p2.req.volt + 0.0005 * in.p1.meas.volt;
    in.p3.req.volt = in.p2.req.volt;
    in.p4.req.volt = in.p2.req.volt;
#else
    in.p2.req.volt = (1 - 0.0005) * in.p2.req.volt + 0.0005 * in.p1.meas.volt;
    in.p3.req.volt = in.p2.req.volt;
    in.p4.req.volt = in.p2.req.volt;
#endif
#endif
    

    protection_t::routine();
#if (EXP_SETUP == PHIL_EMULATION)
    state_machine_t::PHiL_routine();

    ctrl.set_volt_reference(meas_filt_100ms.p3.volt);
    in.p2.req.volt = meas_filt_100ms.p3.volt;
    in.p4.req.volt = meas_filt_100ms.p3.volt;

#endif
    if (state_machine_t::get_state().state_spec.value == QAB_OPERATION)
    {
        ctrl.routine();
    }
    else
    {
        ctrl.reset();
        out.p1.phi2 = M_PI;
    }

    // Set phase shifts
#if (FIXED_SW_COMB != 1)
#if (OPEN_LOOP_TESTING == 1)

    EPwm_set_frequency(out.fsw);
    // Port 1 - Independent legs
    EPwm_set_phase_shift(&ePwm8vals, test_t::open_loop_test.p1_phi1 - 0.5*test_t::open_loop_test.p1_phi2); // left leg
    EPwm_set_phase_shift(&ePwm7vals, test_t::open_loop_test.p1_phi1 + M_PI + 0.5*test_t::open_loop_test.p1_phi2); // right leg
    // Port 2 - Complementary legs
    EPwm_set_phase_shift(&ePwm3vals, test_t::open_loop_test.p2_phi1);
    // Port 3 - Complementary legs
    EPwm_set_phase_shift(&ePwm6vals, test_t::open_loop_test.p3_phi1);
    // Port 4 - Independent legs
    EPwm_set_phase_shift(&ePwm5vals, test_t::open_loop_test.p4_phi1 - 0.5*test_t::open_loop_test.p4_phi2); // left leg
    EPwm_set_phase_shift(&ePwm8vals, test_t::open_loop_test.p4_phi1 + M_PI + test_t::open_loop_test.p4_phi2); // right leg
#else

    // Set PWM frequency
    EPwm_set_frequency(out.fsw);
    EPwm_set_phase_shift(&ePwm4vals, out.p1.phi1 - 0.5*out.p1.phi2); // Cell 1 - left leg
    EPwm_set_phase_shift(&ePwm2vals, out.p1.phi1 + M_PI + 0.5*out.p1.phi2); // Cell 1 - right leg
    EPwm_set_phase_shift(&ePwm3vals, out.p2.phi1); // Cell 2 - complementary
    EPwm_set_phase_shift(&ePwm6vals, out.p3.phi1); // Cell 3 - complementary
    EPwm_set_phase_shift(&ePwm5vals, out.p4.phi1 - 0.5*out.p4.phi2); // Cell 4 - left leg
    EPwm_set_phase_shift(&ePwm8vals, out.p4.phi1 + M_PI + 0.5*out.p4.phi2); // Cell 4 - right leg
    // Update if there is enough time, otherwise update during the interrupt of EPWM_REGISTER_LOAD_TIMING_MODULE
    if ((EPwm2Regs.TBSTS.bit.CTRDIR == 1) || (EPwm2Regs.TBCTR > 200))
    {
        Epwm_write_ePWM_regs();
    }



#endif // (FIXED_SW_COMB != 1)
#endif // OPEN_LOOP_TESTING == 1

state_machine_t::routine();

}



//
void ISR (void)
{
    // Enable ISR function
    // Map ISR functions
    //
    EALLOW;
    IER |= M_INT2;                                  //Enable groups 2 - interrupts

    IER |= M_INT3;                                  //Enable groups 3 - interrupts

    //EINT;                                           //Enable Global interrupt INTM
    //ERTM;                                           //Enable Global realtime interrupt DBGM

    PieVectTable.EPWM_CTRL_TIMING_MODULE_PIE_VECT = &EPWM_x_main_routine_isr;

#if (ENABLE_VAR_SW == 1)
    PieVectTable.EPWM_REGISTER_LOAD_TIMING_MODULE_PIE_VECT = &EPWM_x_load_routine;
#endif
    //PieVectTable.EPWM5_TZ_INT = &EPWM_x_load_routine;
    //PieVectTable.EPWM5_TZ_INT = &EPWM_cell4_tripzone_isr;
    

    PieCtrlRegs.PIEIER1.bit.INTx4 = 1; // external interrupt
    //INTx.3 INTx.4 INTx.5 INTx.6

    PieCtrlRegs.PIEIER3.bit.EPWM_CTRL_TIMING_MODULE_PIE_EN = 1;

    PieCtrlRegs.PIEIER2.bit.INTx3 = 1;//all |= 0xf<<2; // Enable tripzone interrupts
    PieCtrlRegs.PIEIER2.bit.INTx4 = 1;
    PieCtrlRegs.PIEIER2.bit.INTx5 = 1;
    PieCtrlRegs.PIEIER2.bit.INTx6 = 1;

    PieCtrlRegs.PIEIER3.bit.EPWM_REGISTER_LOAD_TIMING_MODULE_PIE_EN = 1;

    
    //
    //

    //
    // sync ePWM

    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
//
//
}// End void ISR (void)
//
//
__interrupt void adca1_isr(void)
{
    //main_ISR_routine();

    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;         //Clear INT1 flag
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void EPWM_x_main_routine_isr()
{

    main_ISR_routine();
    EPWM_CTRL_TIMING_MODULE.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.bit.ACK3 = 1;
}

interrupt void EPWM_x_load_routine()
{
    Epwm_write_ePWM_regs();
    EPWM_REGISTER_LOAD_TIMING_MODULE.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.bit.ACK3 = 1;
}

