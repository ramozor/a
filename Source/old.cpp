/*
 * old.cpp
 *
 *  Created on: Oct 12, 2024
 *      Author: mvo
 */


void old_main()
{
#if(0)
        while(state == 0) // reset system
        {
            RST1_OFF = 1;
            RST2_OFF = 1;
            RST3_OFF = 1;
            RST4_OFF = 1;
            clockCount = 0;
            //
            while(clockCount <= 20000 && state == 0)     //Wait 1.0 second
            {
                if(clockCount >= 10000)
                {
                    LED4_ON = 1;
                    RST1_ON = 1;
                    RST2_ON = 1;
                    RST3_ON = 1;
                    RST4_ON = 1;
                    //Wait a moment to ensure the complete RST
                    if(clockCount == 20000)
                    {
                        error = 0;
                        LED4_OFF = 1;
                        clockCount = 0;
                        state = 1;
                    }// End if(clockCount == 20000)
                }// End if(clockCount >= 10000)
                else
                {
                    //----------------------
                    RST1_OFF = 1;
                    RST2_OFF = 1;
                    RST3_OFF = 1;
                    RST4_OFF = 1;
                    //----------------------
                }
            }// End while(clockCount <= 20000 && state == 0)
            RST1_OFF = 1;
            RST2_OFF = 1;
            RST3_OFF = 1;
            RST4_OFF = 1;
        }// End while(state == 0)
        while(state == 1)
        {
            LED1_ON = 1;
            //----------------------------------------------
            //Check the state of the system before to start
            //----------------------------------------------
            if(error > 0 || error1 == 1 || error2 == 1 || error3 == 1 || error4 == 1)
            {
                state = 30;       //state 30 - Disable the system - Unable to operate
                break;
            }
            //-----------------------------
            if(state = 1 && error == 0 && error1 == 0 && error2 == 0 && error3 == 0 && error4 == 0)
            {
                //-----------------------------
                //Enable the Isolation Switches
                //-----------------------------
                EN2_ON = 1;         //Enable of Isolation Switches 1 and 2 - Cell 1 and 2
                EN7_ON = 1;         //Enable of Isolation Switches 3 and 4 - Cell 3 and 4
                //-----------------------------
                //Turn-on the Isolation Switches
                //-----------------------------
                ISO_SW1_ON = 1;     //Enable ISO Switch 1
                ISO_SW2_ON = 1;     //Enable ISO Switch 2
                ISO_SW3_ON = 1;     //Enable ISO Switch 3
                ISO_SW4_ON = 1;     //Enable ISO Switch 4
                //-----------------------------
                //Enable the PWM Signals
                //-----------------------------

                EnableEPwm3Gpio (); //PWM Cell 2
                EnableEPwm4Gpio (); //PWM Cell 1
                EnableEPwm5Gpio (); //PWM Cell 4
                EnableEPwm6Gpio (); //PWM Cell 3

                //
                clockCount = 0;
                //
                while(clockCount <= 20000 && state == 1)     //Wait 1.0 second
                {
                    if(clockCount == 20000)
                    {
                        LED1_OFF = 1;
                        clockCount = 0;
                        state = 2;
                    }
                }// End while(clockCount <= 20000 && state == 1)     //Wait 1.0 second
            }//End if else  if(error > 0)
        }//End while(state == 1)

        while(state == 2)
        {
            LED2_ON = 1;
            if(error > 0 || error1 == 1 || error2 == 1 || error3 == 1 || error4 == 1)
            {
                state = 30;
                break;
            }
            if(state == 2 && error == 0 && error1 == 0 && error2 == 0 && error3 == 0 && error4 == 0)
            {
                EN3_ON = 1;         //Enable PWM3A and PWM3B - Cell 2
                EN4_ON = 1;         //Enable PWM4A and PWM4B - Cell 1
                EN5_ON = 1;         //Enable PWM5A and PWM5B - Cell 4
                EN6_ON = 1;         //Enable PWM6A and PWM6B - Cell 3
                clockCount = 0;
                while(clockCount <= 40000 && state == 2)     //Wait 2.0 second
                {
                      Phi_Cell1 =  Phi1*(EPWM_TBPRD/180);
                      Phi_Cell2 =  Phi2*(EPWM_TBPRD/180);
                      Phi_Cell3 =  Phi3*(EPWM_TBPRD/180);
                      Phi_Cell4 =  Phi4*(EPWM_TBPRD/180);
                    if(clockCount == 40000)
                    {
                        EPwm4Regs.TBPHS.bit.TBPHS = Phi_Cell1; //Cell 1
                        EPwm3Regs.TBPHS.bit.TBPHS = Phi_Cell2; //Cell 2
                        EPwm6Regs.TBPHS.bit.TBPHS = Phi_Cell3; //Cell 3
                        EPwm5Regs.TBPHS.bit.TBPHS = Phi_Cell4; //Cell 4
                        LED2_OFF = 1;
                        clockCount = 0;
                        state = 3;
                    }
                }// End while(clockCount <= 40000 && state == 2)
            }//End if(state == 2 && error == 0 && error1 == 0 && error2 == 0 && error3 == 0 && error4 == 0)
        }// End while(state == 2)

        while(state == 3) //QAB MODE - Steady State
        {
            LED3_ON = 1;
            while(Flag == 0 && state == 3)                                //Beacon OFF - LED4 OFF
            {
                clockCount = 0;
                while(clockCount <= 20000 && state == 3 && Flag == 0)     //Wait 1.0 second
                {
                    if(clockCount == 20000)
                    {
                        LED4_ON = 1;
                        clockCount = 0;
                        Flag = 1;
                    }
                }// End while(clockCount <= 20000 && state == 3 && Flag == 0)
                if(error) break; //Jump out
            }// End while(Flag == 0 && error == 0 && state == 3) -----> Flag = 0
            //------------------------------------------------------------------------------------
            //---------------------------------- state 3 ----------------------------------------
            //------------------------------------------------------------------------------------
            while(Flag == 1 && state == 3)                  //Beacon OFF - LED4 OFF
            {
                clockCount = 0;
                while(clockCount <= 20000 && state == 3 && Flag == 1)     //Wait 1.0 second
                {
                    if(clockCount == 20000)
                    {
                        LED4_OFF = 1;
                        clockCount = 0;
                        Flag = 0;
                    }
                }// End while(clockCount <= 20000 && state == 3 && Flag == 1)
                if(error > 0) break; //Jump out
                //
            }//End while(Flag == 1 && error == 0 && state == 3) -----> Flag = 1
            //
        }// End while(state == 3) --------> QAB MODE Steady State
        //
        //
        //##################################################################
        //#        ______   _____    _____     ____    _____               #
        //#       |  ____| |  __ \  |  __ \   / __ \  |  __ \              #
        //#       | |__    | |__) | | |__) | | |  | | | |__) |             #
        //#       |  __|   |  _  /  |  _  /  | |  | | |  _  /              #
        //#       | |____  | | \ \  | | \ \  | |__| | | | \ \              #
        //#       |______| |_|  \_\ |_|  \_\  \____/  |_|  \_\             #
        //#                                                                #
        //##################################################################
        // --------------------------------------------------------------------------
        // state 4 - Disable the system - Unable to operate
        // --------------------------------------------------------------------------
        while(state == 5)
        {
            //----------------------------------
            //Turn-off the Isolation Switches
            //----------------------------------
            ISO_SW1_OFF = 1;            //Disable ISO Switch 4
            ISO_SW2_OFF = 1;            //Disable ISO Switch 4
            ISO_SW3_OFF = 1;            //Disable ISO Switch 4
            ISO_SW4_OFF = 1;            //Disable ISO Switch 4
            //----------------------------------
            //Turn-off the PWM Signals
            //----------------------------------
            DisableEPwm1Gpio  ();       //Disable PWM1A and PWM1B
            DisableEPwm2Gpio  ();       //Disable PWM2A and PWM2B
            DisableEPwm3Gpio  ();       //Disable PWM3A and PWM3B
            DisableEPwm4Gpio  ();       //Disable PWM4A and PWM4B
            DisableEPwm5Gpio  ();       //Disable PWM5A and PWM5B
            DisableEPwm6Gpio  ();       //Disable PWM6A and PWM6B
            DisableEPwm7Gpio  ();       //Disable PWM7A and PWM7B
            DisableEPwm8Gpio  ();       //Disable PWM8A and PWM8B
            //----------------------------------
            //----------------------------------
            //Disable the Isolation Switches
            //----------------------------------
            EN2_OFF = 1;                //Disable of Isolation Switches 1 and 2 - Cell 1 and 2
            EN7_OFF = 1;                //Disable of Isolation Switches 3 and 4 - Cell 3 and 4
            //----------------------------------
            EN1_OFF = 1;                //Disable "ENABLE PWM1" (PWM1A and PWM1B)
            EN3_OFF = 1;                //Disable "ENABLE PWM3" (PWM3A and PWM3B)
            EN4_OFF = 1;                //Disable "ENABLE PWM4" (PWM4A and PWM4B)
            EN5_OFF = 1;                //Disable "ENABLE PWM5" (PWM5A and PWM5B)
            EN6_OFF = 1;                //Disable "ENABLE PWM6" (PWM6A and PWM6B)
            EN8_OFF = 1;                //Disable "ENABLE PWM8" (PWM8A and PWM8B)
            //----------------------------------
            LED1_OFF = 1;
            LED2_OFF = 1;
            LED3_OFF = 1;
            LED4_OFF = 1;
            clockCount = 0;
            //----------------------------------
        }// End while(state == 30)
#endif
}

