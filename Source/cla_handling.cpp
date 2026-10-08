/*
 * cla_handling.cpp
 *
 *  Created on: Jan 20, 2025
 *      Author: mvo
 */

#include "F28x_Project.h"
#include "F2837xD_Cla_defines.h"
#include <stdint.h>

#include "cla_prog.h"



interrupt void CPU_PWM11_ISR();
//
// CLA_initCpu1Cla1 - Initialize CLA1 task vectors and end of task interrupts
//
void CLA_initCpu1Cla1(void)
{






    //
    // CLA_configClaMemory - Configure CLA memory sections
    //

    extern uint32_t Cla1funcsRunStart, Cla1funcsLoadStart, Cla1funcsLoadSize;
    EALLOW;

#ifdef _FLASH
    //
    // Copy over code from FLASH to RAM
    //
    memcpy((uint32_t *)&Cla1funcsRunStart, (uint32_t *)&Cla1funcsLoadStart,
           (uint32_t)&Cla1funcsLoadSize);
#endif //_FLASH

    //
    // Initialize and wait for CLA1ToCPUMsgRAM
    //
    MemCfgRegs.MSGxINIT.bit.INIT_CLA1TOCPU = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CLA1TOCPU != 1){};

    //
    // Initialize and wait for CPUToCLA1MsgRAM
    //
    MemCfgRegs.MSGxINIT.bit.INIT_CPUTOCLA1 = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CPUTOCLA1 != 1){};

    //
    // Select LS2RAM and LS3RAM to be the programming space for the CLA
    // First configure the CLA to be the master for LS4 and LS5 and then
    // set the space to be a program block
    //

    MemCfgRegs.LSxMSEL.bit.MSEL_LS2 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS2 = 1;
    MemCfgRegs.LSxMSEL.bit.MSEL_LS3 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS3 = 1;

    //
    // Next configure LS0RAM and LS1RAM as data spaces for the CLA
    // First configure the CLA to be the master for LS0(1) and then
    // set the spaces to be code blocks
    //

    MemCfgRegs.LSxMSEL.bit.MSEL_LS0 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS0 = 0;

    MemCfgRegs.LSxMSEL.bit.MSEL_LS1 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS1 = 0;

    //
    // Compute all CLA task vectors
    // On Type-1 CLAs the MVECT registers accept full 16-bit task addresses as
    // opposed to offsets used on older Type-0 CLAs
    //

    // CLA task 7
    DmaClaSrcSelRegs.CLA1TASKSRCSEL2.bit.TASK7 = 1;//46; // 46 EPwm11 is the source. 1 ADCA is the source
    Cla1Regs.MVECT7 = (uint16_t)(&Cla1Task7);
    // CLA task 6
    /*DmaClaSrcSelRegs.CLA1TASKSRCSEL2.bit.TASK6 = 0; //Table 6-1. Configuration Options
    Cla1Regs.MVECT6 = (uint16_t)(&Cla1Task6);
    Cla1Regs.MCTL.bit.IACKE = 1; // Enable sw trigger
    Cla1Regs.MIFR.bit.INT6 = 1; // Force CLA
    Cla1Regs.MCTL.bit.IACKE = 0; // Disable sw trigger*/
    // Trigger CPU1 by PWM to debug:
#if (DEBUG_EPWM_TIMING == 1)
    PieVectTable.EPWM11_INT = &CPU_PWM11_ISR;
    PieCtrlRegs.PIEIER3.bit.INTx11 = 1;
    IER |= M_INT3;
#endif
    //
    // Enable the IACK instruction to start a task on CLA in software
    // for all  8 CLA tasks. Also, globally enable all 8 tasks (or a
    // subset of tasks) by writing to their respective bits in the
    // MIER register
    //
    Cla1Regs.MCTL.bit.IACKE = 1;
    Cla1Regs.MIER.all = M_INT7;


    //
    // Configure the vectors for the end-of-task interrupt for all
    // 8 tasks
    /*    //
    PieVectTable.CLA1_1_INT = &cla1Isr1;
    PieVectTable.CLA1_2_INT = &cla1Isr2;
    PieVectTable.CLA1_3_INT = &cla1Isr3;
    PieVectTable.CLA1_4_INT = &cla1Isr4;
    PieVectTable.CLA1_5_INT = &cla1Isr5;
    PieVectTable.CLA1_6_INT = &cla1Isr6;
    PieVectTable.CLA1_7_INT = &cla1Isr7;
    PieVectTable.CLA1_8_INT = &cla1Isr8;
     */
    //
    // Set the adca.1 as the trigger for task 7
    //
    //    DmaClaSrcSelRegs.CLA1TASKSRCSEL2.bit.TASK7 = 1;

    //
    // Enable CLA interrupts at the group and subgroup levels
    //
//    PieCtrlRegs.PIEIER11.all = 0xFFFF;
//    IER |= (M_INT11 );
    EDIS;
}

#if (DEBUG_EPWM_TIMING == 1)
interrupt void CPU_PWM11_ISR()
{
//    EPWM_ADC_CLA_TIMING_MODULE.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.bit.ACK3 = 1;
}
#endif
