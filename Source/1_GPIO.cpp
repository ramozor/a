//###########################################################################
//
// FILE:    1_GPIO.c
//
// TITLE:   Device GPIO Setup v1.0
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
//
//
//
// ------------------------- GPIO Functions ---------------------------------
//
void InitGPIO(void)
{
    //
    // =====================================================================================
    // --------------------------------- IOS --------------------------------------------
    // =====================================================================================
    EALLOW;
    //IO1
    // GpioDataRegs.GPBCLEAR.bit.GPIO63 = 1;   // Clear GPIO63 IO1
    // GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 0;    // Set GPIO41 as an IO port
    // GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;     // Enable pullup on GPIO31 (Disable = 1 and Enable = 0)
    // GpioDataRegs.GPBDAT.bit.GPIO63 = 1;     // Lock the latch to enable the output port
    // GpioCtrlRegs.GPBDIR.bit.GPIO63 = 1;     // Set GPIO41 as the output -- input 0, output 1.
    // GpioDataRegs.GPBCLEAR.bit.GPIO63 = 1;   // Clear GPIO63 IO1 (or maybe only Latch)

    // //IO2
    // GpioDataRegs.GPBCLEAR.bit.GPIO62 = 1;   // Clear GPIO37 IO2
    // GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 0;    // Set GPIO41 as an IO port
    // GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;     // Enable pullup on GPIO31
    // GpioDataRegs.GPBDAT.bit.GPIO62 = 1;     // Lock the latch to enable the output port
    // GpioCtrlRegs.GPBDIR.bit.GPIO62 = 1;     // Set GPIO41 as the output -- input 0, output 1.
    // GpioDataRegs.GPBCLEAR.bit.GPIO62 = 1;   // Clear GPIO37 IO2

    // //IO3
    GpioDataRegs.GPBCLEAR.bit.GPIO57 = 1;   // Clear GPIO57 IO3
    GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0;    // Set GPIO41 as an IO port
    GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0;     // Enable pullup on GPIO31
    GpioDataRegs.GPBDAT.bit.GPIO57 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPBDIR.bit.GPIO57 = 1;     // Set GPIO41 as the output -- input 0, output 1.
    GpioDataRegs.GPBCLEAR.bit.GPIO57 = 1;   // Clear GPIO57 IO3

    //IO4
    GpioDataRegs.GPBCLEAR.bit.GPIO56 = 1;   // Clear GPIO56 IO4
    GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 0;    // Set GPIO41 as an IO port
    GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;     // Enable pullup on GPIO31
    GpioDataRegs.GPBDAT.bit.GPIO56 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPBDIR.bit.GPIO56 = 1;     // Set GPIO41 as the output -- input 0, output 1.
    GpioDataRegs.GPBCLEAR.bit.GPIO56 = 1;   // Clear GPIO56 IO4

    //IO5
    GpioDataRegs.GPBCLEAR.bit.GPIO55 = 1;   // Clear GPIO55 IO55
    GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 0;    // Set GPIO41 as an IO port
    GpioCtrlRegs.GPBPUD.bit.GPIO55 = 0;     // Enable pullup on GPIO31
    GpioDataRegs.GPBDAT.bit.GPIO55 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPBDIR.bit.GPIO55 = 1;     // Set GPIO41 as the output -- input 0, output 1.
    GpioDataRegs.GPBCLEAR.bit.GPIO55 = 1;   // Clear GPIO55 IO55

    //IO6
    GpioDataRegs.GPBCLEAR.bit.GPIO54 = 1;   // Clear GPIO54 IO6
    GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 0;    // Set GPIO41 as an IO port
    GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0;     // Enable pullup on GPIO31
    GpioDataRegs.GPBDAT.bit.GPIO54 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPBDIR.bit.GPIO54 = 1;     // Set GPIO41 as the output -- input 0, output 1.
    GpioDataRegs.GPBCLEAR.bit.GPIO54 = 1;   // Clear GPIO54 IO6

    //IO7
    GpioDataRegs.GPBCLEAR.bit.GPIO41 = 1;   // Clear GPIO41 IO7
    GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 0;    // Set GPIO41 as an IO port
    GpioCtrlRegs.GPBPUD.bit.GPIO41 = 0;     // Enable pullup on GPIO31
    GpioDataRegs.GPBDAT.bit.GPIO41 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPBDIR.bit.GPIO41 = 1;     // Set GPIO41 as the output -- input 0, output 1.
    GpioDataRegs.GPBCLEAR.bit.GPIO41 = 1;   // Clear GPIO41 IO7

    //IO8
    GpioDataRegs.GPBCLEAR.bit.GPIO40 = 1;   // Clear GPIO40 IO8
    GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 0;    // Set GPIO41 as an IO port
    GpioCtrlRegs.GPBPUD.bit.GPIO40 = 0;     // Enable pullup on GPIO31
    GpioDataRegs.GPBDAT.bit.GPIO40 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPBDIR.bit.GPIO40 = 1;     // Set GPIO41 as the output -- input 0, output 1.
    GpioDataRegs.GPBCLEAR.bit.GPIO40 = 1;   // Clear GPIO40 IO8

    //IO9
    GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1;   // Clear GPIO39 IO9
    GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 0;    // Set GPIO41 as an IO port
    GpioCtrlRegs.GPBPUD.bit.GPIO39 = 0;     // Enable pullup on GPIO31
    GpioDataRegs.GPBDAT.bit.GPIO39 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;     // Set GPIO41 as the output -- input 0, output 1.
    GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1;   // Clear GPIO39 IO9

    //IO10
    GpioDataRegs.GPBCLEAR.bit.GPIO38 = 1;   // Clear GPIO38 IO10
    GpioCtrlRegs.GPBMUX1.bit.GPIO38 = 0;    // Set GPIO41 as an IO port
    GpioCtrlRegs.GPBPUD.bit.GPIO38 = 0;     // Enable pullup on GPIO31
    GpioDataRegs.GPBDAT.bit.GPIO38 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPBDIR.bit.GPIO38 = 1;     // Set GPIO41 as the output -- input 0, output 1.
    GpioDataRegs.GPBCLEAR.bit.GPIO38 = 1;   // Clear GPIO38 IO10

    //IO11
    GpioDataRegs.GPBCLEAR.bit.GPIO37 = 1;   // Clear GPIO37 IO11
    GpioCtrlRegs.GPBMUX1.bit.GPIO37 = 0;    // Set GPIO37 as an IO port
    GpioCtrlRegs.GPBPUD.bit.GPIO37 = 0;     // Enable pullup on GPIO37
    GpioDataRegs.GPBDAT.bit.GPIO37 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPBDIR.bit.GPIO37 = 1;     // Set GPIO37 as the output -- input 0, output 1.
    GpioDataRegs.GPBCLEAR.bit.GPIO37 = 1;   // Clear GPIO37 IO11

    //IO12
    GpioDataRegs.GPBCLEAR.bit.GPIO36 = 1;   // Clear GPIO36 IO12
    GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 0;    // Set GPIO36 as an IO port
    GpioCtrlRegs.GPBPUD.bit.GPIO36 = 0;     // Enable pullup on GPIO36
    GpioDataRegs.GPBDAT.bit.GPIO36 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPBDIR.bit.GPIO36 = 1;     // Set GPIO36 as the output -- input 0, output 1.
    GpioDataRegs.GPBCLEAR.bit.GPIO36 = 1;   // Clear GPIO36 IO12

    // =====================================================================================
    // -------------------------------- LEDS --------------------------------------------
    // =====================================================================================

    //LED1 - GPIO67
    GpioDataRegs.GPCCLEAR.bit.GPIO67 = 1;   // Clear GPIO67 - Turn off LED1
    GpioCtrlRegs.GPCPUD.bit.GPIO67 = 1;     // Disable pullup on GPIO67
    GpioCtrlRegs.GPCMUX1.bit.GPIO67 = 0;    // Set GPIO67 as an IO port
    GpioDataRegs.GPCDAT.bit.GPIO67 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPCDIR.bit.GPIO67 = 1;     // Set GPIO67 as the output -- input 0, output 1.
    GpioDataRegs.GPCCLEAR.bit.GPIO67 = 1;   // Clear GPIO67 - Turn off LED1

    //LED2 - GPIO43
    GpioDataRegs.GPBCLEAR.bit.GPIO43 = 1;   // Clear GPIO43 - Turn off LED2
    GpioCtrlRegs.GPBPUD.bit.GPIO43 = 1;     // Disable pullup on GPIO43
    GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 0;    // Set GPIO43 as an IO port
    GpioDataRegs.GPBDAT.bit.GPIO43 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPBDIR.bit.GPIO43 = 1;     // Set GPIO43 as the output -- input 0, output 1.
    GpioDataRegs.GPBCLEAR.bit.GPIO43 = 1;   // Clear GPIO43 - Turn off LED2

    //LED3 - GPIO42
    GpioDataRegs.GPBCLEAR.bit.GPIO42 = 1;   // Clear GPIO42 - Turn off LED3
    GpioCtrlRegs.GPBPUD.bit.GPIO42 = 1;     // Disable pullup on GPIO42
    GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 0;    // Set GPIO42 as an IO port
    GpioDataRegs.GPBDAT.bit.GPIO42 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPBDIR.bit.GPIO42 = 1;     // Set GPIO42 as the output -- input 0, output 1.
    GpioDataRegs.GPBCLEAR.bit.GPIO42 = 1;   // Clear GPIO42 - Turn off LED3

    //LED4 - GPIO47
    GpioDataRegs.GPBCLEAR.bit.GPIO47 = 1;   // Clear GPIO47 - Turn off LED4
    GpioCtrlRegs.GPBPUD.bit.GPIO47 = 1;     // Disable pullup on GPIO47
    GpioCtrlRegs.GPBMUX1.bit.GPIO47 = 0;    // Set GPIO47 as an IO port
    GpioDataRegs.GPBDAT.bit.GPIO47 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPBDIR.bit.GPIO47 = 1;     // Set GPIO47 as the output -- input 0, output 1.
    GpioDataRegs.GPBCLEAR.bit.GPIO47 = 1;   // Clear GPIO47 - Turn off LED4

    // =====================================================================================
    // -------------------------------- Enables --------------------------------------------
    // =====================================================================================

    //ENABLE_PWM1 - GPIO84
    GpioDataRegs.GPCCLEAR.bit.GPIO84 = 1;   // Clear GPIO84
    GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 0;    // Set GPIO84 as an IO port
    GpioCtrlRegs.GPCPUD.bit.GPIO84 = 1;     // Disable pullup on GPIO84 - External Pull Down
    GpioDataRegs.GPCDAT.bit.GPIO84 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPCDIR.bit.GPIO84 = 1;     // Set GPIO84 as the output -- input 0, output 1.
    GpioDataRegs.GPCCLEAR.bit.GPIO84 = 1;   // Clear GPIO84

    //ENABLE_PWM2 - GPIO85
    GpioDataRegs.GPCCLEAR.bit.GPIO85 = 1;   // Clear GPIO85
    GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 0;    // Set GPIO85 as an IO port
    GpioCtrlRegs.GPCPUD.bit.GPIO85 = 1;     // Disable pullup on GPIO85 - External Pull Down
    GpioDataRegs.GPCDAT.bit.GPIO85 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPCDIR.bit.GPIO85 = 1;     // Set GPIO85 as the output -- input 0, output 1.
    GpioDataRegs.GPCCLEAR.bit.GPIO85 = 1;   // Clear GPIO85

    //ENABLE_PWM3 - GPIO88
    GpioDataRegs.GPCCLEAR.bit.GPIO88 = 1;   // Clear GPIO88
    GpioCtrlRegs.GPCMUX2.bit.GPIO88 = 0;    // Set GPIO88 as an IO port
    GpioCtrlRegs.GPCPUD.bit.GPIO88 = 1;     // Disable pullup on GPIO88 - External Pull Down
    GpioDataRegs.GPCDAT.bit.GPIO88 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPCDIR.bit.GPIO88 = 1;     // Set GPIO88 as the output -- input 0, output 1.
    GpioDataRegs.GPCCLEAR.bit.GPIO88 = 1;   // Clear GPIO88

    //ENABLE_PWM4 - GPIO89
    GpioDataRegs.GPDCLEAR.bit.GPIO99 = 1;   // Clear GPIO89
    GpioCtrlRegs.GPCMUX2.bit.GPIO89 = 0;    // Set GPIO89 as an IO port
    GpioCtrlRegs.GPCPUD.bit.GPIO89 = 1;     // Disable pullup on GPIO89 - External Pull Down
    GpioDataRegs.GPCDAT.bit.GPIO89 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPCDIR.bit.GPIO89 = 1;     // Set GPIO89 as the output -- input 0, output 1.
    GpioDataRegs.GPDCLEAR.bit.GPIO99 = 1;   // Clear GPIO89

    //ENABLE_PWM5 - GPIO90
    GpioDataRegs.GPCCLEAR.bit.GPIO87 = 1;   // Clear GPIO90
    GpioCtrlRegs.GPCMUX2.bit.GPIO87 = 0;    // Set GPIO90 as an IO port
    GpioCtrlRegs.GPCPUD.bit.GPIO87 = 1;     // Disable pullup on GPIO90 - External Pull Down
    GpioDataRegs.GPCDAT.bit.GPIO87 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPCDIR.bit.GPIO87 = 1;     // Set GPIO90 as the output -- input 0, output 1.
    GpioDataRegs.GPCCLEAR.bit.GPIO87 = 1;   // Clear GPIO90

    //ENABLE_PWM6 - GPIO91
    GpioDataRegs.GPCCLEAR.bit.GPIO91 = 1;   // Clear GPIO91
    GpioCtrlRegs.GPCMUX2.bit.GPIO91 = 0;    // Set GPIO91 as an IO port
    GpioCtrlRegs.GPCPUD.bit.GPIO91 = 1;     // Disable pullup on GPIO91 - External Pull Down
    GpioDataRegs.GPCDAT.bit.GPIO91 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPCDIR.bit.GPIO91 = 1;     // Set GPIO91 as the output -- input 0, output 1.
    GpioDataRegs.GPCCLEAR.bit.GPIO91 = 1;   // Clear GPIO91

    //ENABLE_PWM7 - GPIO92
    GpioDataRegs.GPCCLEAR.bit.GPIO92 = 1;   // Clear GPIO92
    GpioCtrlRegs.GPCMUX2.bit.GPIO92 = 0;    // Set GPIO92 as an IO port
    GpioCtrlRegs.GPCPUD.bit.GPIO92 = 1;     // Disable pullup on GPIO92 - External Pull Down
    GpioDataRegs.GPCDAT.bit.GPIO92 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPCDIR.bit.GPIO92 = 1;     // Set GPIO92 as the output -- input 0, output 1.
    GpioDataRegs.GPCCLEAR.bit.GPIO92 = 1;   // Clear GPIO92

    //ENABLE_PWM8 - GPIO90
    GpioDataRegs.GPCCLEAR.bit.GPIO90 = 1;   // Clear GPIO93
    GpioCtrlRegs.GPCMUX2.bit.GPIO90 = 0;    // Set GPIO93 as an IO port
    GpioCtrlRegs.GPCPUD.bit.GPIO90 = 1;     // Disable pullup on GPIO93 - External Pull Down
    GpioDataRegs.GPCDAT.bit.GPIO90 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPCDIR.bit.GPIO90 = 1;     // Set GPIO93 as the output -- input 0, output 1.
    GpioDataRegs.GPCCLEAR.bit.GPIO90 = 1;   // Clear GPIO93

    //ENABLE_PWM9 - GPIO94
    GpioDataRegs.GPCCLEAR.bit.GPIO94 = 1;   // Clear GPIO94
    GpioCtrlRegs.GPCMUX2.bit.GPIO94 = 0;    // Set GPIO94 as an IO port
    GpioCtrlRegs.GPCPUD.bit.GPIO94 = 1;     // Disable pullup on GPIO94 - External Pull Down
    GpioDataRegs.GPCDAT.bit.GPIO94 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPCDIR.bit.GPIO94 = 1;     // Set GPIO94 as the output -- input 0, output 1.
    GpioDataRegs.GPCCLEAR.bit.GPIO94 = 1;   // Clear GPIO94

    //ENABLE_PWM10 - GPIO99
    GpioDataRegs.GPDCLEAR.bit.GPIO99 = 1;   // Clear GPIO99
    GpioCtrlRegs.GPDMUX1.bit.GPIO99 = 0;    // Set GPIO99 as an IO port
    GpioCtrlRegs.GPDPUD.bit.GPIO99 = 1;     // Disable pullup on GPIO99- External Pull Down
    GpioDataRegs.GPDDAT.bit.GPIO99 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPDDIR.bit.GPIO99 = 1;     // Set GPIO99 as the output -- input 0, output 1.
    GpioDataRegs.GPDCLEAR.bit.GPIO99 = 1;   // Clear GPIO99

    //ENABLE_PWM11 - GPIO24
    GpioDataRegs.GPACLEAR.bit.GPIO24 = 1;   // Clear GPIO24
    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0;    // Set GPIO24 as an IO port
    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 1;     // Disable pullup on GPIO24 - External Pull Down
    GpioDataRegs.GPADAT.bit.GPIO24 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPADIR.bit.GPIO24 = 1;     // Set GPIO24 as the output -- input 0, output 1.
    GpioDataRegs.GPACLEAR.bit.GPIO24 = 1;   // Clear GPIO24

    //ENABLE_PWM12 - GPIO25
    GpioDataRegs.GPACLEAR.bit.GPIO25 = 1;   // Clear GPIO25
    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;    // Set GPIO25 as an IO port
    GpioCtrlRegs.GPAPUD.bit.GPIO25 = 1;     // Disable pullup on GPIO25 - External Pull Down
    GpioDataRegs.GPADAT.bit.GPIO25 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPADIR.bit.GPIO25 = 1;     // Set GPIO25 as the output -- input 0, output 1.
    GpioDataRegs.GPACLEAR.bit.GPIO25 = 1;   // Clear GPIO25

    // =====================================================================================
    // -------------------------------- Optic Fibers RX ------------------------------------
    // =====================================================================================
    // INPUTS
 #ifndef LAUNCHPAD
    //RX1
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;    // Set GPIO16 as an IO port
    GpioCtrlRegs.GPAINV.bit.GPIO16  = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 1;     // Disable pullup on GPIO16
    GpioCtrlRegs.GPADIR.bit.GPIO16 = 0;     // Set GPIO16 as the INPUT -- input 0, output 1.
    //RX2
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0;    // Set GPIO16 as an IO port
    GpioCtrlRegs.GPAINV.bit.GPIO18 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 1;     // Enable pullup on GPIO16
    GpioCtrlRegs.GPADIR.bit.GPIO18 = 0;     // Set GPIO16 as the INPUT -- input 0, output 1.
    //RX3
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;    // Set GPIO16 as an IO port
    GpioCtrlRegs.GPAPUD.bit.GPIO20 = 1;     // Enable pullup on GPIO16
    GpioCtrlRegs.GPADIR.bit.GPIO20 = 0;     // Set GPIO16 as the INPUT -- input 0, output 1.
    //RX4
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;    // Set GPIO16 as an IO port
    GpioCtrlRegs.GPAINV.bit.GPIO21 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO21 = 1;     // Enable pullup on GPIO16
    GpioCtrlRegs.GPADIR.bit.GPIO21 = 0;     // Set GPIO16 as the INPUT -- input 0, output 1.
    //RX5
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;    // Set GPIO16 as an IO port
    GpioCtrlRegs.GPAINV.bit.GPIO22 = 1;
    GpioCtrlRegs.GPAPUD.bit.GPIO22 = 1;     // Enable pullup on GPIO16
    GpioCtrlRegs.GPADIR.bit.GPIO22 = 0;     // Set GPIO16 as the INPUT -- input 0, output 1.
    //RX6
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 0;    // Set GPIO16 as an IO port
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 1;     // Enable pullup on GPIO16
    GpioCtrlRegs.GPADIR.bit.GPIO23 = 0;     // Set GPIO16 as the INPUT -- input 0, output 1.

    // =====================================================================================
    // -------------------------------- UNUSED PINS ------------------------------------
    // =====================================================================================

    //UNUSED GPIO17 --------
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;    // Set GPIO17 as an IO port
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;     // Enable pullup on GPIO17
    GpioCtrlRegs.GPADIR.bit.GPIO17 = 0;     // Set GPIO17 as the INPUT -- input 0, output 1.

    //UNUSED GPIO19 --------
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0;    // Set GPIO19 as an IO port
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;     // Enable pullup on GPIO19
    GpioCtrlRegs.GPADIR.bit.GPIO19 = 0;     // Set GPIO19 as the INPUT -- input 0, output 1.

    //UNUSED GPIO26 --------
    GpioDataRegs.GPACLEAR.bit.GPIO26 = 1;   // Clear GPIO26
    GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;    // Set GPIO26 as an IO port
    GpioCtrlRegs.GPAPUD.bit.GPIO26 = 1;     // Disable pullup on GPIO26 - External Pull Down
    GpioDataRegs.GPADAT.bit.GPIO26 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPADIR.bit.GPIO26 = 1;     // Set GPIO26 as the output -- input 0, output 1.
    GpioDataRegs.GPACLEAR.bit.GPIO26 = 1;   // Clear GPIO26

    //UNUSED GPIO27 --------
    GpioDataRegs.GPACLEAR.bit.GPIO27 = 1;   // Clear GPIO27
    GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;    // Set GPIO27 as an IO port
    GpioCtrlRegs.GPAPUD.bit.GPIO27 = 1;     // Disable pullup on GPIO27 - External Pull Down
    GpioDataRegs.GPADAT.bit.GPIO27 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPADIR.bit.GPIO27 = 1;     // Set GPIO27 as the output -- input 0, output 1.
    GpioDataRegs.GPACLEAR.bit.GPIO27 = 1;   // Clear GPIO27

    //UNUSED GPIO28 --------
    GpioDataRegs.GPACLEAR.bit.GPIO28 = 1;   // Clear GPIO28
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;    // Set GPIO28 as an IO port
    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 1;     // Disable pullup on GPIO28 - External Pull Down
    GpioDataRegs.GPADAT.bit.GPIO28 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPADIR.bit.GPIO28 = 1;     // Set GPIO28 as the output -- input 0, output 1.
    GpioDataRegs.GPACLEAR.bit.GPIO28 = 1;   // Clear GPIO28

    //UNUSED GPIO29 --------
    GpioDataRegs.GPACLEAR.bit.GPIO29 = 1;   // Clear GPIO29
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 0;    // Set GPIO29 as an IO port
    GpioCtrlRegs.GPAPUD.bit.GPIO29 = 1;     // Disable pullup on GPIO29 - External Pull Down
    GpioDataRegs.GPADAT.bit.GPIO29 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPADIR.bit.GPIO29 = 1;     // Set GPIO29 as the output -- input 0, output 1.
    GpioDataRegs.GPACLEAR.bit.GPIO29 = 1;   // Clear GPIO29

    //UNUSED GPIO30 --------
   // GpioDataRegs.GPACLEAR.bit.GPIO30 = 1;   // Clear GPIO30
    //GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 0;    // Set GPIO30 as an IO port
    //GpioCtrlRegs.GPAPUD.bit.GPIO30 = 1;     // Disable pullup on GPIO30 - External Pull Down
    //GpioDataRegs.GPADAT.bit.GPIO30 = 1;     // Lock the latch to enable the output port
    //GpioCtrlRegs.GPADIR.bit.GPIO30 = 1;     // Set GPIO30 as the output -- input 0, output 1.
    //GpioDataRegs.GPACLEAR.bit.GPIO30 = 1;   // Clear GPIO30

    //UNUSED GPIO31 -------- LED2 GPIO31 - ControlCard Test
    //GpioDataRegs.GPACLEAR.bit.GPIO31 = 1;   // Clear GPIO31
    //GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;    // Set GPIO31 as an IO port
    //GpioCtrlRegs.GPAPUD.bit.GPIO31 = 1;     // Disable pullup on GPIO31 - External Pull Down
    //GpioDataRegs.GPADAT.bit.GPIO31 = 1;     // Lock the latch to enable the output port
    //GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;     // Set GPIO31 as the output -- input 0, output 1.
    //GpioDataRegs.GPACLEAR.bit.GPIO31 = 1;   // Clear GPIO31

    //UNUSED GPIO32 --------
    GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;   // Clear GPIO32
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;    // Set GPIO32 as an IO port
    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 1;     // Disable pullup on GPIO32 - External Pull Down
    GpioDataRegs.GPBDAT.bit.GPIO32 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;     // Set GPIO31 as the output -- input 0, output 1.
    GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;   // Clear GPIO32

    //UNUSED GPIO33 --------
    GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1;   // Clear GPIO33
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;    // Set GPIO31 as an IO port
    GpioCtrlRegs.GPBPUD.bit.GPIO33 = 1;     // Disable pullup on GPIO33 - External Pull Down
    GpioDataRegs.GPBDAT.bit.GPIO33 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;     // Set GPIO33 as the output -- input 0, output 1.
    GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1;   // Clear GPIO33

    //UNUSED GPIO34 -------- LED3 GPIO34 - ControlCard Test
    GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;   // Clear GPIO34
    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;    // Set GPIO34 as an IO port
    GpioCtrlRegs.GPBPUD.bit.GPIO34 = 1;     // Disable pullup on GPIO34 - External Pull Down
    GpioDataRegs.GPBDAT.bit.GPIO34 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;     // Set GPIO34 as the output -- input 0, output 1.
    GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;   // Clear GPIO34

    //UNUSED GPIO35 --------
    GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1;   // Clear GPIO35
    GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 0;    // Set GPIO35 as an IO port
    GpioCtrlRegs.GPBPUD.bit.GPIO35 = 1;     // Disable pullup on GPIO35 - External Pull Down
    GpioDataRegs.GPBDAT.bit.GPIO35 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPBDIR.bit.GPIO35 = 1;     // Set GPIO35 as the output -- input 0, output 1.
    GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1;   // Clear GPIO35

    // Isolation switches: GPIO1, 0, 13, 12
    GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;   // Clear GPIO1
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;    // Set GPIO1 as an IO port
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;     // Disable pullup on GPIO1 - External Pull Down
    GpioDataRegs.GPADAT.bit.GPIO1 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;     // Set GPIO1 as the output -- input 0, output 1.
    GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;   // Clear GPIO1

    GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;   // Clear GPIO2
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;    // Set GPIO2 as an IO port
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;     // Disable pullup on GPIO2 - External Pull Down
    GpioDataRegs.GPADAT.bit.GPIO0 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;     // Set GPIO2 as the output -- input 0, output 1.
    GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;   // Clear GPIO2


    GpioDataRegs.GPACLEAR.bit.GPIO13 = 1;   // Clear GPIO13
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;    // Set GPIO13 as an IO port
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 1;     // Disable pullup on GPIO13 - External Pull Down
    GpioDataRegs.GPADAT.bit.GPIO13 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;     // Set GPIO13 as the output -- input 0, output 1.
    GpioDataRegs.GPACLEAR.bit.GPIO13 = 1;   // Clear GPIO13


    GpioDataRegs.GPACLEAR.bit.GPIO12 = 1;   // Clear GPIO12
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;    // Set GPIO12 as an IO port
    GpioCtrlRegs.GPAPUD.bit.GPIO12 = 1;     // Disable pullup on GPIO12 - External Pull Down
    GpioDataRegs.GPADAT.bit.GPIO12 = 1;     // Lock the latch to enable the output port
    GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;     // Set GPIO12 as the output -- input 0, output 1.
    GpioDataRegs.GPACLEAR.bit.GPIO12 = 1;   // Clear GPIO12
#endif
 #ifdef LAUNCHPAD
    // Launchpad -> Tripzone testing
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0; // GPIO22 as IO
    GpioCtrlRegs.GPADIR.bit.GPIO22 = 1; // GPIO22 as output
    GpioDataRegs.GPASET.bit.GPIO22 = 1; // Indicate that the driver is okay
 #endif


    // unused TX:
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0; // Set the function of GPIO as I/O
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0; // Set the function of GPIO as I/O
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0; // Set the function of GPIO as I/O
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0; // Set the function of GPIO as I/O

#if (DEBUG_TX_A_B_O_P == 1)
    // User can set/reset TxA TxB TxO TxP
    GpioDataRegs.GPCSET.bit.GPIO93 = 1; // Enable function of TxO and TxP
    GpioDataRegs.GPCSET.bit.GPIO84 = 1; // Enable function of TxA and TxB
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;     // Set GPIO0 as the OUTPUT -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;     // Set GPIO0 as the OUTPUT -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO14 = 1;     // Set GPIO0 as the OUTPUT -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;     // Set GPIO0 as the OUTPUT -- input 0, output 1.
    GpioDataRegs.GPACLEAR.bit.GPIO0 = 1; // Set the GPIO output to 0
    GpioDataRegs.GPACLEAR.bit.GPIO1 = 1; // Set the GPIO output to 0
    GpioDataRegs.GPACLEAR.bit.GPIO14 = 1; // Set the GPIO output to 0
    GpioDataRegs.GPACLEAR.bit.GPIO15 = 1; // Set the GPIO output to 0

#else
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 0;     // Set GPIO0 as the INPUT -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 0;     // Set GPIO0 as the INPUT -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO14 = 0;     // Set GPIO0 as the INPUT -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO15 = 0;     // Set GPIO0 as the INPUT -- input 0, output 1.
#endif

    ////// CAN GPIO
    ////////////////////
    //EALLOW;
    //GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 2; // CANRXA
    //GpioCtrlRegs.GPAGMUX1.bit.GPIO12 = 1;// CANRXA
    //GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 2; // CANTXA
    //GpioCtrlRegs.GPAGMUX2.bit.GPIO17 = 1;// CANTXA
    //GpioCtrlRegs.GPADIR.bit.GPIO12 = 0;
    //GpioCtrlRegs.GPADIR.bit.GPIO17 = 1;
    //GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 3; // Asynchronuous
    //GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 0;
    //GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0; // Pull-up
    //EDIS;
    //GpioCtrlRegs.
EALLOW;

// --------------------------------------------------------
// CAN-B TX (Yazma) -> GPIO12 (GPA MUX1 Grubunda)
// --------------------------------------------------------
GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 2;   // CANTXB (MUX = 2)
GpioCtrlRegs.GPAGMUX1.bit.GPIO12 = 0;  // GMUX = 0
GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;    // Yön: Çıkış (TX)
GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;    // Pull-up aktif

// --------------------------------------------------------
// CAN-B RX (Okuma) -> GPIO17 (GPA MUX2 Grubunda)
// --------------------------------------------------------
GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 2;   // CANRXB (MUX = 2)
GpioCtrlRegs.GPAGMUX2.bit.GPIO17 = 0;  // GMUX = 0
GpioCtrlRegs.GPADIR.bit.GPIO17 = 0;    // Yön: Giriş (RX)
GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;  // Asenkron okuma
GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;    // Pull-up aktif

EDIS;
//
//
}// End void InitGPIO(void)
//
//
//=============================================================================================
// ----------------------------- Enable/Disable PWM Functions ---------------------------------
//=============================================================================================
//
//
//*********************************************************************************************
//------------------------------------ PWM1A & PWM1B ------------------------------------------
//*********************************************************************************************
void EnableEPwm1Gpio(void) //FullBridge Inverter Leg 1 ePWM1 GPIO Enable Function
{
    EALLOW;
    //PWM1A & PWM1B - ENABLE by Software
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;      // Disable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;      // Disable pull-up on GPIO1 (EPWM1B)
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;     // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;     // Configure GPIO1 as EPWM1B
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    EDIS;
}
void DisableEPwm1Gpio(void) //FullBridge Inverter Leg 1 ePWM1 GPIO Disable
{
    EALLOW;
    GpioDataRegs.GPCCLEAR.bit.GPIO84 = 1;   // Disable the ENABLE signal of the PWM1A and PWM1B
    //PWM1A & PWM1B - DISABLE by Software
    GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;    // Set GPIO1 Low - CLEAR
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;      // Enable pull-up on GPIO0
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;      // Enable pull-up on GPIO1
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;     // Configure GPIO0 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;     // Configure GPIO1 as GPIO
    GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;    // Set GPIO1 Low - CLEAR
    EDIS;
}
//*********************************************************************************************
//------------------------------------ PWM2A & PWM2B ------------------------------------------
//*********************************************************************************************
void EnableEPwm2Gpio(void) //FullBridge Inverter Leg 1 ePWM2 GPIO Enable Function
{
    EALLOW;
    //PWM2A & PWM2B - ENABLE by Software
    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;      // Disable pull-up on GPIO2 (EPWM2A)
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;      // Disable pull-up on GPIO3 (EPWM2B)
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;     // Configure GPIO2 as EPWM2A
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;     // Configure GPIO3 as EPWM2B
    GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;      // Configure GPIO2 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;      // Configure GPIO3 as an output -- input 0, output 1.
    EDIS;
}
void DisableEPwm2Gpio(void) //FullBridge Inverter Leg 1 ePWM2 GPIO Disable
{
    EALLOW;
    GpioDataRegs.GPCCLEAR.bit.GPIO85 = 1;   // Disable the ENABLE signal of the PWM2A and PWM2B
    //PWM2A & PWM2B - DISABLE by Software
    GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;    // Set GPIO2 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;    // Set GPIO3 Low - CLEAR
    GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;      // Configure GPIO2 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;      // Configure GPIO3 as an output -- input 0, output 1.
    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;      // Enable pull-up on GPIO2
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;      // Enable pull-up on GPIO3
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;     // Configure GPIO2 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;     // Configure GPIO3 as GPIO
    GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;    // Set GPIO2 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;    // Set GPIO3 Low - CLEAR
    EDIS;
}

uint16_t EPwm3_6_status()
{
    return (GpioCtrlRegs.GPAMUX1.bit.GPIO4 == 1) && (GpioCtrlRegs.GPAMUX1.bit.GPIO5 == 1) &&
           (GpioCtrlRegs.GPAMUX1.bit.GPIO6 == 1) && (GpioCtrlRegs.GPAMUX1.bit.GPIO7 == 1) &&
           (GpioCtrlRegs.GPAMUX1.bit.GPIO8 == 1) && (GpioCtrlRegs.GPAMUX1.bit.GPIO9 == 1) &&
           (GpioCtrlRegs.GPAMUX1.bit.GPIO10 == 1) && (GpioCtrlRegs.GPAMUX1.bit.GPIO11 == 1);
}
//*********************************************************************************************
//------------------------------------ PWM3A & PWM3B ------------------------------------------
//*********************************************************************************************
void EnableEPwm3Gpio(void) //FullBridge Inverter Leg 1 ePWM3 GPIO Enable Function
{
    EALLOW;
    //PWM3A & PWM3B - ENABLE by Software
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;      // Disable pull-up on GPIO4 (EPWM3A)
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;      // Disable pull-up on GPIO5 (EPWM3B)
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;     // Configure GPIO4 as EPWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;     // Configure GPIO5 as EPWM3B
    GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;      // Configure GPIO4 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;      // Configure GPIO5 as an output -- input 0, output 1.
    EDIS;
}
void DisableEPwm3Gpio(void) //FullBridge Inverter Leg 1 ePWM3 GPIO Disable
{
    EALLOW;
    GpioDataRegs.GPCCLEAR.bit.GPIO88 = 1;   // Disable the ENABLE signal of the PWM3A and PWM3B
    //PWM3A & PWM3B - DISABLE by Software
    GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;    // Set GPIO1 Low - CLEAR
    GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;      // Enable pull-up on GPIO0
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;      // Enable pull-up on GPIO1
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;     // Configure GPIO0 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;     // Configure GPIO1 as GPIO
    GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;    // Set GPIO1 Low - CLEAR
    EDIS;
}
//*********************************************************************************************
//------------------------------------ PWM4A & PWM4B ------------------------------------------
//*********************************************************************************************
void EnableEPwm4Gpio(void) //FullBridge Inverter Leg 1 ePWM4 GPIO Enable Function
{
    EALLOW;
    //PWM4A & PWM4B - ENABLE by Software
    GpioCtrlRegs.GPAPUD.bit.GPIO6= 1;       // Disable pull-up on GPIO0 (EPWM4A)
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;      // Disable pull-up on GPIO1 (EPWM4B)
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;     // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;     // Configure GPIO1 as EPWM1B
    GpioCtrlRegs.GPADIR.bit.GPIO6 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO7 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    EDIS;
}
void DisableEPwm4Gpio(void) //FullBridge Inverter Leg 1 ePWM4 GPIO Disable
{
    EALLOW;
    GpioDataRegs.GPCCLEAR.bit.GPIO89 = 1;   // Disable the ENABLE signal of the PWM4A and PWM4B
    //PWM4A & PWM4B - DISABLE by Software
    GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;    // Set GPIO1 Low - CLEAR
    GpioCtrlRegs.GPADIR.bit.GPIO6 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO7 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;      // Enable pull-up on GPIO0
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;      // Enable pull-up on GPIO1
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;     // Configure GPIO0 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;     // Configure GPIO1 as GPIO
    GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;    // Set GPIO1 Low - CLEAR
    EDIS;
}
//*********************************************************************************************
//------------------------------------ PWM5A & PWM5B ------------------------------------------
//*********************************************************************************************
void EnableEPwm5Gpio(void) //FullBridge Inverter Leg 1 ePWM5 GPIO Enable Function
{
    EALLOW;
    //PWM5A & PWM5B - ENABLE by Software
    GpioCtrlRegs.GPAPUD.bit.GPIO8= 1;       // Disable pull-up on GPIO0 (EPWM5A)
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;      // Disable pull-up on GPIO1 (EPWM5B)
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;     // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;     // Configure GPIO1 as EPWM1B
    GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    EDIS;
}
void DisableEPwm5Gpio(void) //FullBridge Inverter Leg 1 ePWM5 GPIO Disable
{
    EALLOW;
    GpioDataRegs.GPCCLEAR.bit.GPIO90 = 1;   // Disable the ENABLE signal of the PWM5A and PWM5B
    //PWM5A & PWM5B - DISABLE by Software
    GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;    // Set GPIO1 Low - CLEAR
    GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;      // Enable pull-up on GPIO0
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;      // Enable pull-up on GPIO1
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;     // Configure GPIO0 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;     // Configure GPIO1 as GPIO
    GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;    // Set GPIO1 Low - CLEAR
    EDIS;
}
//*********************************************************************************************
//------------------------------------ PWM6A & PWM6B ------------------------------------------
//*********************************************************************************************
void EnableEPwm6Gpio(void) //FullBridge Inverter Leg 1 ePWM6 GPIO Enable Function
{
    EALLOW;
    //PWM6A & PWM6B - ENABLE by Software
    GpioCtrlRegs.GPAPUD.bit.GPIO10= 1;       // Disable pull-up on GPIO0 (EPWM6A)
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;      // Disable pull-up on GPIO1 (EPWM6B)
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;     // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;     // Configure GPIO1 as EPWM1B
    GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    EDIS;
}
void DisableEPwm6Gpio(void) //FullBridge Inverter Leg 1 ePWM6 GPIO Disable
{
    EALLOW;
    GpioDataRegs.GPCCLEAR.bit.GPIO91 = 1;    // Disable the ENABLE signal of the PWM6A and PWM6B
    //PWM6A & PWM6B - DISABLE by Software
    GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;    // Set GPIO1 Low - CLEAR
    GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;      // Enable pull-up on GPIO0
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;      // Enable pull-up on GPIO1
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;     // Configure GPIO0 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;     // Configure GPIO1 as GPIO
    GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;    // Set GPIO1 Low - CLEAR
    EDIS;
}
//*********************************************************************************************
//------------------------------------ PWM7A & PWM7B ------------------------------------------
//*********************************************************************************************
void EnableEPwm7Gpio(void) //FullBridge Inverter Leg 1 ePWM7 GPIO Enable Function
{
    EALLOW;
    //PWM7A & PWM7B - ENABLE by Software
    GpioCtrlRegs.GPBPUD.bit.GPIO34= 1;       // Disable pull-up on GPIO0 (EPWM7A)
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 1;      // Disable pull-up on GPIO1 (EPWM7B)
    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 1;     // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;     // Configure GPIO1 as EPWM1B
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    EDIS;
}
void DisableEPwm7Gpio(void) //FullBridge Inverter Leg 1 ePWM7 GPIO Disable
{
    EALLOW;
    GpioDataRegs.GPCCLEAR.bit.GPIO92 = 1;    // Disable the ENABLE signal of the PWM7A and PWM7B
    //PWM7A & PWM7B - DISABLE by Software
    GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO13 = 1;    // Set GPIO1 Low - CLEAR
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    GpioCtrlRegs.GPBPUD.bit.GPIO34 = 0;      // Enable pull-up on GPIO0
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;      // Enable pull-up on GPIO1
    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;     // Configure GPIO0 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;     // Configure GPIO1 as GPIO
    GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO13 = 1;    // Set GPIO1 Low - CLEAR
    EDIS;
}
//*********************************************************************************************
//------------------------------------ PWM8A & PWM8B ------------------------------------------
//*********************************************************************************************
void EnableEPwm8Gpio(void) //FullBridge Inverter Leg 1 ePWM8 GPIO Enable Function
{
    EALLOW;
    //PWM8A & PWM8B - ENABLE by Software
    GpioCtrlRegs.GPAPUD.bit.GPIO14= 1;       // Disable pull-up on GPIO0 (EPWM8A)
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 1;      // Disable pull-up on GPIO1 (EPWM8B)
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;     // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;     // Configure GPIO1 as EPWM1B
    GpioCtrlRegs.GPADIR.bit.GPIO14 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    EDIS;
}
void DisableEPwm8Gpio(void) //FullBridge Inverter Leg 1 ePWM8 GPIO Disable
{
    EALLOW;
    GpioDataRegs.GPCCLEAR.bit.GPIO90 = 1;    // Disable the ENABLE signal of the PWM8A and PWM8B
    //PWM8A & PWM8B - DISABLE by Software
    GpioDataRegs.GPACLEAR.bit.GPIO14 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO15 = 1;    // Set GPIO1 Low - CLEAR
    GpioCtrlRegs.GPADIR.bit.GPIO14 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;      // Enable pull-up on GPIO0
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;      // Enable pull-up on GPIO1
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;     // Configure GPIO0 as GPIO
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;     // Configure GPIO1 as GPIO
    GpioDataRegs.GPACLEAR.bit.GPIO14 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO15 = 1;    // Set GPIO1 Low - CLEAR
    EDIS;
}
//*********************************************************************************************
//------------------------------------ PWM9A & PWM9B ------------------------------------------
//*********************************************************************************************
void EnableEPwm9Gpio(void) //FullBridge Inverter Leg 1 ePWM9 GPIO Enable Function
{
    EALLOW;
    //PWM9A & PWM9B - ENABLE by Software
    GpioCtrlRegs.GPAPUD.bit.GPIO16= 1;       // Disable pull-up on GPIO0 (EPWM9A)
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 1;      // Disable pull-up on GPIO1 (EPWM9B)
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1;     // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1;     // Configure GPIO1 as EPWM1B
    GpioCtrlRegs.GPADIR.bit.GPIO16 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO17 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    EDIS;
}
void DisableEPwm9Gpio(void) //FullBridge Inverter Leg 1 ePWM9 GPIO Disable
{
    EALLOW;
    GpioDataRegs.GPCCLEAR.bit.GPIO95 = 1;    // Disable the ENABLE signal of the PWM9A and PWM9B
    //PWM9A & PWM9B - DISABLE by Software
    GpioDataRegs.GPACLEAR.bit.GPIO16 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;    // Set GPIO1 Low - CLEAR
    GpioCtrlRegs.GPADIR.bit.GPIO16 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO17 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;      // Enable pull-up on GPIO0
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;      // Enable pull-up on GPIO1
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;     // Configure GPIO0 as GPIO
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;     // Configure GPIO1 as GPIO
    GpioDataRegs.GPACLEAR.bit.GPIO16 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;    // Set GPIO1 Low - CLEAR
    EDIS;
}
//*********************************************************************************************
//------------------------------------ PWM10A & PWM10B ----------------------------------------
//*********************************************************************************************
void EnableEPwm10Gpio(void) //FullBridge Inverter Leg 1 ePWM10 GPIO Enable Function
{
    EALLOW;
    //PWM9A & PWM9B - ENABLE by Software
    GpioCtrlRegs.GPAPUD.bit.GPIO18= 1;       // Disable pull-up on GPIO0 (EPWM10A)
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 1;      // Disable pull-up on GPIO1 (EPWM10B)
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1;     // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1;     // Configure GPIO1 as EPWM1B
    GpioCtrlRegs.GPADIR.bit.GPIO18 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO19 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    EDIS;
}
void DisableEPwm10Gpio(void) //FullBridge Inverter Leg 1 ePWM10 GPIO Disable
{
    EALLOW;
    GpioDataRegs.GPDCLEAR.bit.GPIO99 = 1;    // Disable the ENABLE signal of the PWM10A and PWM10B
    //PWM10A & PWM10B - DISABLE by Software
    GpioDataRegs.GPACLEAR.bit.GPIO18 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;    // Set GPIO1 Low - CLEAR
    GpioCtrlRegs.GPADIR.bit.GPIO18 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO19 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;      // Enable pull-up on GPIO0
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;      // Enable pull-up on GPIO1
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0;     // Configure GPIO0 as GPIO
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0;     // Configure GPIO1 as GPIO
    GpioDataRegs.GPACLEAR.bit.GPIO18 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;    // Set GPIO1 Low - CLEAR
    EDIS;
}
//*********************************************************************************************
//------------------------------------ PWM11A & PWM11B ----------------------------------------
//*********************************************************************************************
void EnableEPwm11Gpio(void) //FullBridge Inverter Leg 1 ePWM11 GPIO Enable Function
{
    EALLOW;
    //PWM11A & PWM11B - ENABLE by Software
    GpioCtrlRegs.GPAPUD.bit.GPIO20= 1;       // Disable pull-up on GPIO0 (EPWM11A)
    GpioCtrlRegs.GPAPUD.bit.GPIO21 = 1;      // Disable pull-up on GPIO1 (EPWM11B)
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 1;     // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 1;     // Configure GPIO1 as EPWM1B
    GpioCtrlRegs.GPADIR.bit.GPIO20 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO21 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    EDIS;
}
void DisableEPwm11Gpio(void) //FullBridge Inverter Leg 1 ePWM11 GPIO Disable
{
    EALLOW;
    GpioDataRegs.GPACLEAR.bit.GPIO24 = 1;    // Disable the ENABLE signal of the PWM11A and PWM11B
    //PWM11A & PWM11B - DISABLE by Software
    GpioDataRegs.GPACLEAR.bit.GPIO20 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO21 = 1;    // Set GPIO1 Low - CLEAR
    GpioCtrlRegs.GPADIR.bit.GPIO20 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO21 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0;      // Enable pull-up on GPIO0
    GpioCtrlRegs.GPAPUD.bit.GPIO21 = 0;      // Enable pull-up on GPIO1
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;     // Configure GPIO0 as GPIO
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;     // Configure GPIO1 as GPIO
    GpioDataRegs.GPACLEAR.bit.GPIO20 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO21 = 1;    // Set GPIO1 Low - CLEAR
    EDIS;
}
//*********************************************************************************************
//------------------------------------ PWM12A & PWM12B ----------------------------------------
//*********************************************************************************************
void EnableEPwm12Gpio(void) //FullBridge Inverter Leg 1 ePWM12 GPIO Enable Function
{
    EALLOW;
    //PWM12A & PWM12B - ENABLE by Software
    GpioCtrlRegs.GPAPUD.bit.GPIO22= 1;       // Disable pull-up on GPIO0 (EPWM12A)
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 1;      // Disable pull-up on GPIO1 (EPWM12B)
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 1;     // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 1;     // Configure GPIO1 as EPWM1B
    GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO23 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    EDIS;
}
void DisableEPwm12Gpio(void) //FullBridge Inverter Leg 1 ePWM12 GPIO Disable
{
    EALLOW;
    GpioDataRegs.GPACLEAR.bit.GPIO25 = 1;    // Disable the ENABLE signal of the PWM12A and PWM12B
    //PWM12A & PWM12B - DISABLE by Software
    GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO23 = 1;    // Set GPIO1 Low - CLEAR
    GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;      // Configure GPIO0 as an output -- input 0, output 1.
    GpioCtrlRegs.GPADIR.bit.GPIO23 = 1;      // Configure GPIO1 as an output -- input 0, output 1.
    GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;      // Enable pull-up on GPIO0
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;      // Enable pull-up on GPIO1
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;     // Configure GPIO0 as GPIO
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 0;     // Configure GPIO1 as GPIO
    GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;    // Set GPIO0 Low - CLEAR
    GpioDataRegs.GPACLEAR.bit.GPIO23 = 1;    // Set GPIO1 Low - CLEAR
    EDIS;
}
//*********************************************************************************************
//---------------------------------------------------------------------------------------------
//*********************************************************************************************
//
//
//
//
//
// End of file
//
