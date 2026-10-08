/*
 * HW.cpp
 *
 *  Created on: Nov 17, 2024
 *      Author: mvo
 */

#include "1_GPIO.h"
#include "2_PWM.h"
#include "3_ADC.h"
#include "5_ISR.h"
#include "F28x_Project.h"
#include "cla_handling.h"

#include "peripheral.h"
#include "rumm_CAN.h"
#include "can_debug.h"
#include "can_base.h"
#include "Defines.h"

void peripheral_init()
{
#ifdef _FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
    InitFlash();             //-- Initialize Flash
#endif

    InitSysCtrl();           //-- Initialize System Control

    //
    DINT;
    InitPieCtrl();           //-- Initialize PIE control registers to their default state
    InitCpuTimers();         //-- Initialize CPU Timer
    IER = 0x0000;            //-- Disable CPU __interrupts and clear all CPU __interrupt flags -------
    IFR = 0x0000;
    InitPieVectTable();      //-- Initialize the PIE vector table
    //
    //
    //================================================================================================
    //

    //
    //================================================================================================
    //
    CLA_initCpu1Cla1(); // Initialize CLA
    InitADC           ();       //Initialize ADC
    ADC_SYNC          ();       //Initialize EPWM SOC
    InitPWM           ();       //Initialize PWM
    InitGPIO          ();       //Initialize GPIO
#if (ENABLE_CAN == 1)
// setup CAN
	InitCanProtocol(500000);

	// use predefined constant from global_def_var.h as CAN ID for CAN debug
	CanDebugInit(CAN_DEBUG_ID, 2);

	// setup variables
	//CanDebugInitParams();
	// load stored values from EEPROM
	//CanDebugLoadParams();
	// setup CanDebug EEPROM write request callback
	//CanDebugSetEWCallback(CanDebugSaveParams);
#if (CAN_TEST_COM == 1)
    CanDebugSendStatus();
    //can_test_loop();
#endif // #if (CAN_TEST == 1)




#endif // #if (ENABLE_CAN == 1)



    DELAY_US(50);
}

