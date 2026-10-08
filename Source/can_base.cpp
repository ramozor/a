/**
 * @file      can_protocol.c
 * @brief     Description (source file)
 * @author    Tomáš Košan
 * @date      24. 9. 2018
 * @version   1.0
 */

#ifndef SRC_CAN_BASE_C_
#define SRC_CAN_BASE_C_



//#include <src_hw/rumm_hwlib.h>
#include "can_base.h"
#include "rumm_can.h"

static canMsg_t canMsg;

void InitCanProtocol(uint32_t bitrate)
{
    RummCanInit(bitrate);

    // setup transmit message(s)
    canMsg.obj_id = CAN_TEST_MBOXID;
    canMsg.msg_id = CAN_TEST_CANID;
    canMsg.len = 8;
    RummCanSetupMessageObject(&canMsg, MSG_OBJ_TYPE_TRANSMIT);


    // AFTER setup of messages start CAN peripheral
    RummCanStart();
}

void SendCanTestMessage(uint32_t data, uint32_t info)
{
  canMsg.data[0] = data >> 24;
  canMsg.data[1] = data >> 16;
  canMsg.data[2] = data >> 8;
  canMsg.data[3] = data;
  canMsg.data[4] = info >> 24;
  canMsg.data[5] = info >> 16;
  canMsg.data[6] = info >> 8;
  canMsg.data[7] = info;

  //while(RummCanMsgSent(&canMsg)); // wait for send
  RummCanSendMessage(&canMsg, 0);
}


#endif /* SRC_CAN_BASE_C_ */

