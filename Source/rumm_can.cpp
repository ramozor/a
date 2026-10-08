/**
 * @file      rumm_can.c
 * @brief     CAN basic driver (source file)
 * @author    Tomáš Košan
 * @date      15. 11. 2017
 * @version   1.0
 *
 */
#include "F28x_Project.h"
#include "rumm_can.h"

#define CAN_MAX_BIT_DIVISOR     (13)        ///< The maximum CAN bit timing divisor.
#define CAN_MIN_BIT_DIVISOR     (5)         ///< The minimum CAN bit timing divisor.
#define CAN_MAX_PRE_DIVISOR     (1024)      ///< The maximum CAN pre-divisor.
#define CAN_MIN_PRE_DIVISOR     (1)         ///< The minimum CAN pre-divisor.
#define CAN_BTR_BRP_M           (0x3F)      ///< ???
#define CAN_BTR_BRPE_M          (0xF0000)   ///< ???
#define CAN_MSG_ID_SHIFT        18U         ///< Shift of short CAN ID.

uint32_t txloops = 0;

volatile uint16_t can_activity = 0;

uint32_t can_sts_error = 0;

volatile uint16_t can_tx_error = 0;

static const uint16_t canBitValues[] =
{
    0x1100, // TSEG2 2, TSEG1 2, SJW 1, Divide 5
    0x1200, // TSEG2 2, TSEG1 3, SJW 1, Divide 6
    0x2240, // TSEG2 3, TSEG1 3, SJW 2, Divide 7
    0x2340, // TSEG2 3, TSEG1 4, SJW 2, Divide 8
    0x3340, // TSEG2 4, TSEG1 4, SJW 2, Divide 9
    0x3440, // TSEG2 4, TSEG1 5, SJW 2, Divide 10
    0x3540, // TSEG2 4, TSEG1 6, SJW 2, Divide 11
    0x3640, // TSEG2 4, TSEG1 7, SJW 2, Divide 12
    0x3740  // TSEG2 4, TSEG1 8, SJW 2, Divide 13
};

//
// CanGetMessage - Check the message object for new data.
//                 If new data, data written into array and return true.
//                 If no new data, return false.
//
bool RummCanGetMessage(canMsg_t* canMsg)
{
    bool status;

    uint16_t i = 0;

    //
    // Set the Message Data A, Data B, and control values to be read
    // on request for data from the message object.
    //
    CanbRegs.CAN_IF2CMD.all = 0;
    CanbRegs.CAN_IF2CMD.bit.Control = 1;
    CanbRegs.CAN_IF2CMD.bit.DATA_A = 1;
    CanbRegs.CAN_IF2CMD.bit.DATA_B = 1;

    //
    // Transfer the message object to the message object IF register.
    //
    CanbRegs.CAN_IF2CMD.bit.MSG_NUM = canMsg->obj_id;

    //
    // Wait for busy bit to clear.
    //
    while(CanbRegs.CAN_IF2CMD.bit.Busy)
    {
    }

    //
    // See if there is new data available.
    //
    if(CanbRegs.CAN_IF2MCTL.bit.NewDat == 1)
    {
        can_activity++;

        canMsg->data[0] = CanbRegs.CAN_IF2DATA.bit.Data_0;
        canMsg->data[1] = CanbRegs.CAN_IF2DATA.bit.Data_1;
        canMsg->data[2] = CanbRegs.CAN_IF2DATA.bit.Data_2;
        canMsg->data[3] = CanbRegs.CAN_IF2DATA.bit.Data_3;
        canMsg->data[4] = CanbRegs.CAN_IF2DATB.bit.Data_4;
        canMsg->data[5] = CanbRegs.CAN_IF2DATB.bit.Data_5;
        canMsg->data[6] = CanbRegs.CAN_IF2DATB.bit.Data_6;
        canMsg->data[7] = CanbRegs.CAN_IF2DATB.bit.Data_7;

        // get message length
        canMsg->len = CanbRegs.CAN_IF2MCTL.bit.DLC;

        // clear rest
        for(i=canMsg->len;i<8;i++)
        {
          canMsg->data[i] = 0;
        }

        //
        // Clear New Data Flag
        //
        CanbRegs.CAN_IF2CMD.bit.TxRqst = 1;

        //
        // Wait for busy bit to clear.
        //
        while(CanbRegs.CAN_IF2CMD.bit.Busy)
        {
        }

        //
        // Transfer the message object to the message object IF register.
        //
        CanbRegs.CAN_IF2CMD.bit.MSG_NUM = canMsg->obj_id;

        status = true;
    }
    else
    {
        status = false;
    }

    return(status);
}

//
// sendCANMessage - Transmit data from the specified message object
//
uint32_t RummCanSendMessage(canMsg_t* canMsg, uint16_t wait_send)
{
    can_activity++;

    // 1. Posta kutusuna müdahale etmeden önce donanımın müsait olmasını bekle
    while(CanbRegs.CAN_IF1CMD.bit.Busy)
    {
    }

    // 2. ARBITRATION (Kimlik ve Geçerlilik)
    // Eski kodunda bu yoktu! Mesaj ID'sini ve "MsgVal" (Geçerli Mesaj) bitini 
    // donanıma söylemezsen, Launchpad gönderim isteğini doğrudan çöpe atar.
    // 0xA0000000 = MsgVal (Bit 31) ve Dir (Bit 29 - Transmit) aktif.
    CanbRegs.CAN_IF1ARB.all = 0xA0000000 | ((uint32_t)canMsg->msg_id << 18);

    // 3. CONTROL (Veri Uzunluğu - DLC)
    // 0x00000080 = End of Block (EoB) aktif.
    CanbRegs.CAN_IF1MCTL.all = 0x00000080 | (canMsg->len & 0x0F);

    // 4. DATA (Verileri Yerleştir)
    CanbRegs.CAN_IF1DATA.bit.Data_0 = canMsg->data[0];
    CanbRegs.CAN_IF1DATA.bit.Data_1 = canMsg->data[1];
    CanbRegs.CAN_IF1DATA.bit.Data_2 = canMsg->data[2];
    CanbRegs.CAN_IF1DATA.bit.Data_3 = canMsg->data[3];
    CanbRegs.CAN_IF1DATB.bit.Data_4 = canMsg->data[4];
    CanbRegs.CAN_IF1DATB.bit.Data_5 = canMsg->data[5];
    CanbRegs.CAN_IF1DATB.bit.Data_6 = canMsg->data[6];
    CanbRegs.CAN_IF1DATB.bit.Data_7 = canMsg->data[7];

    // 5. COMMAND (Tetikleyici)
    // Eski kod bit bazlı yazarak donanımı kilitliyordu. 
    // Tüm komutlar (Arb, Control, DataA, DataB ve TXRQST) tek bir 32-bit hamlede yazılmalı.
    // 0x00B70000, tüm bu komutların aktif olduğu maske değeridir.
    CanbRegs.CAN_IF1CMD.all = 0x00B70000 | (canMsg->obj_id & 0xFF);

    if (wait_send == 1)
    {
        txloops = 0;

        // 6. GİZLİ HATA ÇÖZÜMÜ: 1UL (Unsigned Long)
        // C2000'de "1" rakamı 16-bit kabul edilir. 1UL yapmazsak ve obj_id 17'den 
        // büyük gelirse döngü taşma yapar ve kilitlenir.
        while (CanbRegs.CAN_TXRQ_21 & (1UL << (canMsg->obj_id - 1)))
        {
            txloops++;
            can_sts_error = CanbRegs.CAN_ES.all;
            CanbRegs.CAN_ES.all = 0x0;
            if (txloops > MAX_TX_ITERS)
            {
              can_tx_error |= CAN_TX_TIMEOUT;
              break;
            }
        }
    }
    
    return txloops;
}

//
// CanSetupMessageObject - Setup message object as Transmit or Receive
//
void RummCanSetupMessageObject(canMsg_t* canMsg, msgObjType msgType)
{
    //
    // Wait for busy bit to clear.
    //
    while(CanbRegs.CAN_IF1CMD.bit.Busy)
    {
    }

    //
    // Clear and Write out the registers to program the message object.
    //
    CanbRegs.CAN_IF1CMD.all = 0;
    CanbRegs.CAN_IF1MSK.all = 0;
    CanbRegs.CAN_IF1ARB.all = 0;
    CanbRegs.CAN_IF1MCTL.all = 0;

    //
    // Set the Control, Mask, and Arb bit so that they get transferred to the
    // Message object.
    //
    CanbRegs.CAN_IF1CMD.bit.Control = 1;
    CanbRegs.CAN_IF1CMD.bit.Arb = 1;
    CanbRegs.CAN_IF1CMD.bit.Mask = 1;
    CanbRegs.CAN_IF1CMD.bit.DIR = 1;

    //
    // Set direction to transmit
    //
    if(msgType == MSG_OBJ_TYPE_TRANSMIT)
    {
        CanbRegs.CAN_IF1ARB.bit.Dir = 1;
    }

    //
    // Set Message ID (this example assumes 11 bit ID mask)
    //
    CanbRegs.CAN_IF1ARB.bit.ID = ((uint32_t)canMsg->msg_id << CAN_MSG_ID_SHIFT);
    CanbRegs.CAN_IF1ARB.bit.MsgVal = 1;

    //
    // Set the data length since this is set for all transfers.  This is
    // also a single transfer and not a FIFO transfer so set EOB bit.
    //
    CanbRegs.CAN_IF1MCTL.bit.DLC = canMsg->len;
    CanbRegs.CAN_IF1MCTL.bit.EoB = 1;

    //
    // Transfer data to message object RAM
    //
    CanbRegs.CAN_IF1CMD.bit.MSG_NUM = canMsg->obj_id;
}

//
// CanSetBitRate - Set the CAN bit rate based on device clock (Hz)
//                 and desired bit rate (Hz)
//
static uint32_t RummCanSetBitRate(uint32_t sourceClock, uint32_t bitRate)
{
    uint32_t desiredRatio;
    uint32_t canBits;
    uint32_t preDivide;
    uint32_t regValue;
    uint16_t canControlValue;

    //
    // Calculate the desired clock rate.
    //
    desiredRatio = sourceClock / bitRate;

    //
    // Make sure that the Desired Ratio is not too large.  This enforces the
    // requirement that the bit rate is larger than requested.
    //
    if((sourceClock / desiredRatio) > bitRate)
    {
        desiredRatio += 1;
    }

    //
    // Check all possible values to find a matching value.
    //
    while(desiredRatio <= CAN_MAX_PRE_DIVISOR * CAN_MAX_BIT_DIVISOR)
    {
        //
        // Loop through all possible CAN bit divisors.
        //
        for(canBits = CAN_MAX_BIT_DIVISOR;
            canBits >= CAN_MIN_BIT_DIVISOR;
            canBits--)
        {
            //
            // For a given CAN bit divisor save the pre divisor.
            //
            preDivide = desiredRatio / canBits;

            //
            // If the calculated divisors match the desired clock ratio then
            // return these bit rate and set the CAN bit timing.
            //
            if((preDivide * canBits) == desiredRatio)
            {
                //
                // Start building the bit timing value by adding the bit timing
                // in time quanta.
                //
                regValue = canBitValues[canBits - CAN_MIN_BIT_DIVISOR];

                //
                // To set the bit timing register, the controller must be
                // placed
                // in init mode (if not already), and also configuration change
                // bit enabled.  The state of the register should be saved
                // so it can be restored.
                //
                canControlValue = CanbRegs.CAN_CTL.all;
                CanbRegs.CAN_CTL.bit.Init = 1;
                CanbRegs.CAN_CTL.bit.CCE = 1;

                //
                // Now add in the pre-scalar on the bit rate.
                //
                regValue |= ((preDivide - 1) & CAN_BTR_BRP_M) |
                            (((preDivide - 1) << 10) & CAN_BTR_BRPE_M);

                //
                // Set the clock bits in the and the bits of the
                // pre-scalar.
                //
                CanbRegs.CAN_BTR.all = regValue;

                //
                // Restore the saved CAN Control register.
                //
                CanbRegs.CAN_CTL.all = canControlValue;

                //
                // Return the computed bit rate.
                //
                return(sourceClock / ( preDivide * canBits));
            }
        }

        //
        // Move the divisor up one and look again.  Only in rare cases are
        // more than 2 loops required to find the value.
        //
        desiredRatio++;
    }
    return 0;
}

void RummCanInit(uint32_t bitrate)
{
    // Initialize the CAN-A controller
    InitCAN();
    // Setup CAN to be clocked from the SYSCLKOUT
    ClkCfgRegs.CLKSRCCTL2.bit.CANBBCLKSEL = 0;

    //
    // Set up the bit rate for the CAN bus.  This function sets up the CAN
    // bus timing for a nominal configuration.
    // In this example, the CAN bus is set to 500 kHz.
    //
    // Consult the data sheet for more information about
    // CAN peripheral clocking.
    //
    RummCanSetBitRate(200000000, bitrate); //setup CAN

    // enable auto-bus on
    CanbRegs.CAN_CTL.bit.ABO = 1;
}


/* EOF*/

