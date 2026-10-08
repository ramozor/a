/**
 * @file      can_protocol.h
 * @brief     Basic CAN initialization for testing purposes (header file)
 * @author    Tomáš Košan
 * @date      24. 9. 2018
 * @version   1.0
 */

#ifndef SRC_CAN_BASE_H_
#define SRC_CAN_BASE_H_

#include <stdint.h>
#include "rumm_can.h"



#define CAN_TEST_MBOXID  (0x1)
#define CAN_TEST_CANID   (0x1000)

/**
 * Initialize CAN bus for basic operation. It will use CAN_TEST_ID mailbox for sending simple status from tests.
 * @param bitrate
 */
void InitCanProtocol(uint32_t bitrate);

/**
 * Send test/debug CAN message. Used by testing code.
 * @param data
 * @param info
 */
void SendCanTestMessage(uint32_t data, uint32_t info);

#endif /* SRC_CAN_BASE_H_ */
