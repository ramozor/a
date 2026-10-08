/**
 * @file      rumm_can.h
 * @brief     CAN basic driver (header file)
 * @author    Tomáš Košan
 * @date      15. 11. 2017
 * @version   1.0
 *
 */

#ifndef SRC_CAN_H_
#define SRC_CAN_H_


#include "F28x_Project.h"

//#include "../RUMM_A_elektroline/src/settings.h"



/**
 * Wait for dispatch of previous message.
 */
#define RUMM_CAN_WAIT_SEND   (1)
/**
 * Do not wait for previous message to be sent, possible overwrite.
 */
#define RUMM_CAN_FORCE_SEND  (0)

/**
 * Maximum number of iterations spend on transmit.
 */
#define MAX_TX_ITERS  (65000)

/**
 * CAN TX timeout error flag.
 */
#define CAN_TX_TIMEOUT (1<<0)

/**
 * Definition of CAN message type.
 */
typedef enum
{
        //! Transmit message object.
        MSG_OBJ_TYPE_TRANSMIT,//!< MSG_OBJ_TYPE_TRANSMIT

        //! Receive message object.
        MSG_OBJ_TYPE_RECEIVE  //!< MSG_OBJ_TYPE_RECEIVE
}
msgObjType;

/**
 * Union for multiple way access to CAN message data.
 */
union can_data_u{
    uint16_t data[8];   ///< Basic CAN data array.
    uint64_t all;       ///< All 8 bytes joined together.
    uint32_t dataw[2];  ///< Data splitted to two 32-bit variables.
};

/**
 * CAN message/mailbox basic structure.
 */
typedef struct
{
    uint16_t obj_id;    ///< CAN message object ID - i.e. index of CAN mailbox.
    uint16_t msg_id;    ///< CAN message ID.
    uint16_t len;       ///< If receive mailbox then it consist of length of data received. If transmit then it defines message length during setup and afterwards.
    uint16_t data[8];   ///< Data array
} canMsg_t;

/**
 * CAN activity counter.
 */
extern volatile uint16_t can_activity;

/**
 * If other than zero then transmit error occured.
 */
extern volatile uint16_t can_tx_error;

/**
 * Check if CAN message was sent.
 */
inline uint32_t RummCanMsgSent(canMsg_t *canMsg)
{
    return CanbRegs.CAN_TXRQ_21 & (1 << (canMsg->obj_id-1));
};

/*!
 * Setup one CAN message mailbox.
 * @param canMsg Pointer to user filled CAN message structure.
 * @param msgType Type of mailbox, receive or transmit.
 */
void  RummCanSetupMessageObject(canMsg_t* canMsg, msgObjType msgType);

/*!
 * Send message to CAN bus.
 * @param canMsg Pointer to CAN message structure to be send.
 * @param wait_send If set to 1 then function will block until CAN message is sent. If 0 then return immediately.
 * @return Number of txloops spend on waiting for transmit. If this value equals to
 */
uint32_t RummCanSendMessage(canMsg_t* canMsg, uint16_t wait_send);

/*!
 * Receive message from CAN bus.
 * @param canMsg Pointer to CAN message structure to be filled with received message.
 * @return True if a new message was received, false if no new message available.
 */
bool RummCanGetMessage(canMsg_t* canMsg);

/**
 * Setup Canb to required bitrate.
 * @param bitrate Required bitrate.
 */
void RummCanInit(uint32_t bitrate);

/**
 * Enable Canb to full function.
 */
#define RummCanStart() (CanbRegs.CAN_CTL.bit.Init = 0)




#endif /* SRC_CAN_H_ */
