/*
 * can_debug.c
 *
 *  Created on: 15. 11. 2017
 *      Author: zmatkar
 */

/**
 * \defgroup can_debugger CAN debug @{
 */

#ifndef SRC_CAN_DEBUG_C_
#define SRC_CAN_DEBUG_C_
#include <stdint.h>
#include "General.h"
//#include "../settings.h"
/*
#ifdef __cplusplus
extern "C" {
#endif*/

/** Maximum number of variables  - it directly affects size of allocated RAM. */
#define MAX_VARS            (256)
/** Maximum number of post-mort buffers  - it directly affects size of allocated RAM. */
#define MAX_POST_MORTS      (32)

#define NO_ACTION (0)

/**
 * If defined then hardware can save debug variables to EEPROM.
 */
#define SUPPORTS_EEPROM_STORAGE (1<<0)
/**
 * If defined then support for post-mort is enabled.
 */
#define SUPPORTS_POSTMORST      (1<<1)
/**
 * If defined then motor can be controlled by GUI - ON/OFF and control mode.
 */
#define SUPPORTS_MOTOR_CTRL     (1<<2)

/*!
 * Variable type enumerator.
 */
typedef enum
{
    is_int8 = 0,
    is_uint8 = 1,
    is_int16 = 2,
    is_uint16 = 3,
    is_int32 = 4,
    is_uint32 = 5,
    is_float = 6,
    is_status = 7
} var_type_t;

/*!
 *
 */
typedef enum
{
    SPEED_CTRL = (1),
    POSITION_CTRL = (2),
    TORQUE_CTRL = (3),
    CURRENT_CTRL = (4)
} ctrl_type_e;

/*!
 *
 */
typedef enum
{
    RUN = (1<<0),
    RUNNING = (1<<1),
    STOPPED = (1<<3),
    STOP = (1<<2),
    ERROR = (1<<4),
    OVERCURRENT  = (1<<5),
    OVERSPEED    = (1<<6),
    OVER_TEMPERATURE = (1<<7)
} ctrl_state_e;

/*!
 * Commands ID definition type.
 */
typedef enum can_debug_e
{
    CAN_DEBUG_READ = 0,
    CAN_DEBUG_WRITE = 1,
    CAN_DEBUG_CHANGED = 2,
    CAN_DEBUG_VAR_INFO = 3,
    CAN_DEBUG_READ_POSTMORT = 4,
    CAN_DEBUG_ERROR = 5,
    CAN_DEBUG_VARPROPS = 6,
    CAN_DEBUG_POST_INFO = 7,
    CAN_DEBUG_SET_CONTROLER_STATE = 8,
    CAN_DEBUG_APPLY_PARAM = 9,
    CAN_DEBUG_SAVE_PARAM = 10
}cmdid_t;

/*!
 * Structure type used for store controller state.
 */
typedef struct
{
    ctrl_state_e ctrl_state   : 8;
    ctrl_type_e control_type : 8;
    uint16_t version      : 8;
    uint16_t reserve     : 8;
}controler_state_t;

/*!
 * Mixed type to enable store float/int32 and uint32
 */
union MIXED_TYPES{
    float float_var;
    uint32_t uint32_var;
    int32_t int32_var;
};

/*!
 *
 */
union MIXED_PTR{
    uint32_t* p_uint32;
    uint16_t* p_uint16;
};

/*!
 * Variable structure.
 */
typedef struct variable_s
{
    uint16_t readonly : 1;      /** Set to zero if variable can be changed by CAN access. */
    uint16_t user_write : 1;    /** Set to 1 if user can access this variable for writing. Readonly flag suppresses this settings. */
    uint16_t user_read  : 1;    /** Set to 1 if user can read this variable. */
    uint16_t changed : 1;       /** Indicate that value was changed. */
    uint16_t auto_send : 1;     /** Enable auto send feature. */
    uint16_t change_ack : 1;    /** Acknowledge change flag. */
    var_type_t var_type;        /** Type of variable. */
    uint16_t var_id;            /** Unique identificator of variable instance. It is automatically filled upon execution of CanDebugAddVar() function.*/
    union MIXED_TYPES raw_data; /** Raw data of variable.*/
    const char* var_name;             /** Variable name string. */
    uint16_t eeprom_store_ena;  /** If >1 then saving and loading this parameter from eeprom is enabled.*/
} variable_t;

/*!
 * Post-mort structure
 */
typedef struct postmort_s
{
    union MIXED_PTR p_data;           /** Pointer to post-mort buffer.*/
    uint32_t  length;           /** Length of buffer. */
    uint16_t  postmort_id;      /** Unique identificator of post-mort buffer instance.  It is automatically filled upon execution of CanDebugAddPostMort() function.*/
    var_type_t var_type;        /** Type of variable, currently are supported just 16-bit or 32-bit wide types. */
    const char* postmort_name;        /** Post-mort name string.*/
}postmort_t;

/**
 * Pointer to function with CAN message data as parameter.
 */
typedef void (*f_ptr_status_type) (controler_state_t* state);

/**
 * CAN debug protocol releated CAN message structure.
 */
typedef struct
{
    uint16_t obj_id;  ///< Number of CAN mailbox
    uint16_t msg_id;  ///< CAN message ID
    uint16_t length;  ///< CAN DLC
    uint16_t cmd_id;  ///< Protocol command ID
    uint16_t len;     ///< Length of data
    uint16_t ext_len; ///< Packet has exteded length - postmort
    uint16_t var_id;  ///< Variable/postmort ID
    uint16_t data[4]; ///< Data chunk
} canMsgDbg_t;

/**
 * Exported variables array. DO not alter this variable.
 */
extern variable_t* var_array[MAX_VARS];
/**
 * Exported number of registered variables.
 */
extern uint16_t var_index;

/*!
 * Setup CANA peripheral.
 * @param base_id Required CAN base ID.
 */
void CanDebugInit(uint16_t base_id, uint16_t obj_id);

/*!
 * Add variable to variable array. Passed variable must be created by user.
 * @param variable Pointer to variable structure variable_t which has to be added to variable array.
 */
void CanDebugAddVar(variable_t* variable);

/*!
 * Add postmort buffer to CAN debugger. Creating of buffer itself is user responsibility.
 * @param post Pointer to post-mort buffer structure postmort_t which has to be added to post-mort array.
 */
void CanDebugAddPostMort(postmort_t* post);

/*!
 * Process input CAN message. It simply implements whole protocol, when message arrives, it is processed by
 * this function.
 */
void CanDebugProcessInput(void);

/*!
 * EEPROM save function callback setter.
 * If user push button Save to EEPROM in GUI then this callback is invoked.
 */
void CanDebugSetEWCallback(f_ptr_type fptr);

/*!
 * Apply button callback function setter.
 * If user push button APPLY in CAN debugger GUI then this callback is invoked.
 */
void CanDebugSetApplyCallback(f_ptr_type fptr);

/*!
 * Status callback setter.
 * If user changes controller mode or on/off state in GUI this callback is called.
 */
void CanDebugSetStatusCallback(f_ptr_status_type fptr);

/**
 * Force state to be one of enumerated values.
 */
void CanDebugSetState(ctrl_state_e state);

/**
 * Force mode
 */
void CanDebugSetMode(ctrl_type_e mode);

/**
 * This function sends status of controller - i.e. control mode etc.
 * To enable auto-detection of connected units, this function has to be called
 * periodically. The period then directly influences speed of detection.
 */
void CanDebugSendStatus(void);

/**
 * Get address of registered variables array.
 * @param array Pointer to array of registered variables.
 * @return Size of returned array.
 */
uint16_t CanDebugGetVarArray(variable_t** array);


extern void CanDebugRefreshInputs();

void masterwrite(uint16_t target_var_id, float target_value);
void requestdata(uint16_t target_var_id);

/** @} */
/*
#ifdef __cplusplus
}
#endif
*/
#endif /* SRC_CAN_DEBUG_C_ */

