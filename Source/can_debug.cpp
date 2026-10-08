/*
 * can_debug.c
 *
 *  Created on: 15. 11. 2017
 *      Author: zmatkar
 */

//#include "src_hw/rumm_hwlib.h"
#include "can_debug.h"
#include "string.h"
#include "rumm_can.h"
#include "can_base.h"
//#include "ctrl.h"
//#include "../protections.h"
//#include "postmorts.h"
//#include "../settings.h"





extern uint16_t warning_state;

//extern F32 uw_grid;

extern void CanDebugRefreshVars();

//extern uint32_t Shutdown_Error_state;

//extern CTRL_state_enum ctrl_state;

/**
 * Minimum delay inserted between each CAN packet.
 */
#define PACKET_PAUSE_US (1000)

// mailboxes
static canMsgDbg_t canRxMsg;
static canMsgDbg_t canTxMsg;

variable_t* var_array[MAX_VARS] = {NULL};
uint16_t var_index = 0;

static postmort_t* post_array[MAX_POST_MORTS] = {NULL};
static uint32_t post_index = 0;

static variable_t info_var = {1,1,0,0,0,0,is_int16,0,1,"Variables Info"};
#if (RICE_CONTROL2_TEST != 1)
static variable_t can_dg_errors = {1, 0, 1, 0, 1, 0, is_uint32, 0, 0, "Errors", 0};
static variable_t can_dg_clear_errors;// = {0, 1, 1, 0, 0, 0, is_uint32, 0, 0, "Clear errors", 0};
static variable_t can_dg_warnings = {1, 0, 1, 0, 1, 0, is_uint16, 0, 0, "Warnings", 0};
static variable_t can_dg_KSI_U_rq;
static variable_t can_dg_KSII_U_rq;
static variable_t can_dg_temp_KSI = {1, 0, 1, 0, 1, 0, is_float, 0, 0, "Temp KSI", 0};
static variable_t can_dg_temp_KSII = {1, 0, 1, 0, 1, 0, is_float, 0, 0, "Temp KSII", 0};
static variable_t can_dg_temp_KSII_C = {1, 0, 1, 0, 1, 0, is_float, 0, 0, "Temp C KSII", 0};
static variable_t can_dg_temp_Ambient = {1, 0, 1, 0, 1, 0, is_float, 0, 0, "Temp Ambient", 0};
static variable_t can_dg_KSI_UC1 = {1, 0, 1, 0, 1, 0, is_float, 0, 0, "Uc1_KSI", 0};
static variable_t can_dg_KSI_UC2 = {1, 0, 1, 0, 1, 0, is_float, 0, 0, "Uc2_KSI", 0};
static variable_t can_dg_cat_volt = {1, 0, 1, 0, 1, 0, is_float, 0, 0, "Ucat", 0};
static variable_t can_dg_I_trolley = {1, 0, 1, 0, 1, 0, is_float, 0, 0, "I_trolley", 0};
static variable_t can_dg_I_res = {1, 0, 1, 0, 1, 0, is_float, 0, 0, "I_res", 0};
static variable_t can_dg_KSII_UC1 = {1, 0, 1, 0, 1, 0, is_float, 0, 0, "Uc1_KSII", 0};
static variable_t can_dg_KSII_UC2 = {1, 0, 1, 0, 1, 0, is_float, 0, 0, "Uc2_KSII", 0};
static variable_t can_dg_Iu_RMS = {1, 0, 1, 0, 1, 0, is_float, 0, 0, "Iu_RMS", 0};
static variable_t can_dg_Iv_RMS = {1, 0, 1, 0, 1, 0, is_float, 0, 0, "Iv_RMS", 0};
static variable_t can_dg_Iw_RMS = {1, 0, 1, 0, 1, 0, is_float, 0, 0, "Iw_RMS", 0};
#if (SEND_RAW_KSI_TEMP == 1)
static variable_t can_dg_temp_KSI_raw = {1, 0, 1, 0, 1, 0, is_uint16, 0, 0, "KSI_Temp_raw", 0};
#endif
#if (SEND_RAW_KSII_TEMP == 1)
static variable_t can_dg_temp_KSII_raw = {1, 0, 1, 0, 1, 0, is_uint16, 0, 0, "KSII_Temp_raw", 0};
#endif
static variable_t can_dg_Shutdown_Error = {1, 0, 1, 0, 1, 0, is_uint32, 0, 0, "Shutdown_State_Error", 0};
#endif
#if (RICE_CONTROL2_TEST == 1)
static variable_t can_dg_run_cmd = {1, 0, 1, 0, 1, 0, is_uint16, 0, 0, "RUN", 0};
static variable_t can_dg_clear_errors = {1, 0, 1, 0, 1, 0, is_uint16, 0, 0, "CLEAR ERRORS", 0};
#endif


static postmort_t info_post = {&post_index, 1, 0, is_int32, "PostMortInfo"};

#if (ENABLE_KSI_POSTMORT == 1)
static postmort_t I_trolley_post = {(uint32_t*)I_trolley_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "I_trolley"};
static postmort_t Uc1_KSI_post = {(uint32_t*)Uc1_KSI_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "Uc1_KSI"};
static postmort_t Uc2_KSI_post = {(uint32_t*)Uc2_KSI_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "Uc2_KSI"};
static postmort_t U_cat_post = {(uint32_t*)U_cat_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "U_cat"};
static postmort_t I_res_post = {(uint32_t*)I_res_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "I_res"};
#endif
#if (ENABLE_KSII_POSTMORT == 1)
static postmort_t Uc1_KSII_post = {(uint32_t*)Uc1_KSII_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "Uc1_KSII"};
static postmort_t Uc2_KSII_post = {(uint32_t*)Uc2_KSII_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "Uc2_KSII"};
#endif
#if (ENABLE_TEMP_POSTMORT == 1)
static postmort_t KSI_temp_post = {(uint32_t*)KSI_temp_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "KSI_Temp"};
static postmort_t KSII_temp_post = {(uint32_t*)KSII_temp_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "KSII_Temp"};
#endif
#if (ENABLE_KSII_POSTMORT == 1)
static postmort_t Iu_post = {(uint32_t*)Iu_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "Iu"};
static postmort_t Iv_post = {(uint32_t*)Iv_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "Iv"};
static postmort_t Iw_post = {(uint32_t*)Iw_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "Iw"};
static postmort_t Uu_post = {(uint32_t*)Uu_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "Uu"};
static postmort_t Uv_post = {(uint32_t*)Uv_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "Uv"};
static postmort_t Uw_post = {(uint32_t*)Uw_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "Uw"};
#endif
#if (ENABLE_KSI_POSTMORT == 1)
static postmort_t VdcI_output_post = {(uint32_t*)VdcI_output_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "VdcI_out"};
static postmort_t VdcII_output_post = {(uint32_t*)VdcII_output_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "VdcII_out"};
static postmort_t I_trolley_output_post = {(uint32_t*)I_trolley_output_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "I_trolley"};
static postmort_t z_up_KSI_output_post = {(uint32_t*)z_up_KSI_output_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "z_up_KSI"};
static postmort_t z_do_KSI_output_post = {(uint32_t*)z_do_KSI_output_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "z_do_KSI"};
#endif
#if (ENABLE_KSII_POSTMORT == 1)
static postmort_t Uu_output_post = {(uint32_t*)Uu_output_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "Uu_output"};
static postmort_t Uv_output_post = {(uint32_t*)Uv_output_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "Uv_output"};
static postmort_t Uw_output_post = {(uint32_t*)Uw_output_buffer, POSTMORT_BUFFER_SIZE, 0, is_float, "Uw_output"};
#endif
#if (ENABLE_STATE_ERROR_POSTMORT == 1)
static postmort_t State_error_post = {(uint32_t*)State_error_buffer, POSTMORT_BUFFER_SIZE, 0, is_uint32, "State_Error"};
#endif

static f_ptr_type ew_fptr = 0;
static f_ptr_status_type status_fptr = 0;
static f_ptr_type apply_fptr = 0;

uint16_t can_apply_params;
uint16_t can_params_applied;
uint16_t can_params_save;
uint16_t can_params_saved_ok;
controler_state_t control_status;

#ifndef SW_VERSION
#define SW_VERSION (0)
#endif

uint16_t CanDebugGetVarArray(variable_t** array)
{
  array = var_array;
  return var_index;

}


void CanDebugInit(uint16_t base_id, uint16_t obj_id)
{
    canMsg_t msg_tmp;

    control_status.control_type = SPEED_CTRL;
    control_status.ctrl_state = STOPPED;
    control_status.version = SW_VERSION;

    //
    // Prepare mailboxes

    canRxMsg.cmd_id = 0;
    canRxMsg.len = 0;
    canRxMsg.msg_id = base_id;
    canRxMsg.obj_id = obj_id;
    canRxMsg.length = 0;
    canRxMsg.ext_len = 0;
    canRxMsg.var_id = 0;

    uint16_t i;
    for(i=0;i<4;i++)
    {
      canRxMsg.data[i] = 0;
      canTxMsg.data[i] = 0;
    }

    msg_tmp.msg_id = base_id;
    msg_tmp.obj_id = obj_id;
    msg_tmp.len = 8;

    //
    // Initialize the message object that will be used for receiving CAN
    // messages.
    //
    RummCanSetupMessageObject(&msg_tmp, MSG_OBJ_TYPE_RECEIVE);

    canTxMsg.cmd_id = 0;
    canTxMsg.len = 0;
    canTxMsg.msg_id = base_id+1;
    canTxMsg.obj_id = obj_id+1;
    canTxMsg.length = 0;
    canTxMsg.ext_len = 0;
    canTxMsg.var_id = 0;

    msg_tmp.msg_id = base_id+1;
    msg_tmp.obj_id = obj_id+1;
    msg_tmp.len = 8;

    //
    // Initialize the message object that will be used for sending CAN
    // messages.
    //
    RummCanSetupMessageObject(&msg_tmp, MSG_OBJ_TYPE_TRANSMIT);

    //
    // Add info variable
    //
    CanDebugAddVar(&info_var);
#if (RICE_CONTROL2_TEST != 1)
    CanDebugAddVar(&can_dg_errors);
    CanDebugAddVar(&can_dg_warnings);

    can_dg_clear_errors.var_name = "Clear errors";
    can_dg_clear_errors.var_type = is_uint16;
    can_dg_clear_errors.raw_data.uint32_var = 0;
    can_dg_clear_errors.readonly = 0;
    can_dg_clear_errors.eeprom_store_ena = 0;

    CanDebugAddVar(&can_dg_clear_errors);

    can_dg_KSI_U_rq.var_name = "Uc_rq_KSI";
    can_dg_KSI_U_rq.var_type = is_float;
    can_dg_KSI_U_rq.raw_data.uint32_var = 0;
    can_dg_KSI_U_rq.readonly = 0;
    can_dg_KSI_U_rq.eeprom_store_ena = 0;

    CanDebugAddVar(&can_dg_KSI_U_rq);

    can_dg_KSII_U_rq.var_name = "Uw_grid";
    can_dg_KSII_U_rq.var_type = is_float;
    can_dg_KSII_U_rq.raw_data.uint32_var = 0;
    can_dg_KSII_U_rq.readonly = 0;
    can_dg_KSII_U_rq.eeprom_store_ena = 0;

    CanDebugAddVar(&can_dg_KSII_U_rq);


#if (SEND_RAW_KSI_TEMP == 1)
    CanDebugAddVar(&can_dg_temp_KSI_raw);
#endif

#if (SEND_RAW_KSI_TEMP == 1)
    CanDebugAddVar(&can_dg_temp_KSII_raw);
#endif



    CanDebugAddVar(&can_dg_temp_KSI);
    CanDebugAddVar(&can_dg_temp_KSII);
    CanDebugAddVar(&can_dg_temp_KSII_C);
    CanDebugAddVar(&can_dg_temp_Ambient);
    CanDebugAddVar(&can_dg_KSI_UC1);
    CanDebugAddVar(&can_dg_KSI_UC2);
    CanDebugAddVar(&can_dg_cat_volt);
    CanDebugAddVar(&can_dg_I_trolley);
    CanDebugAddVar(&can_dg_I_res);
    CanDebugAddVar(&can_dg_KSII_UC1);
    CanDebugAddVar(&can_dg_KSII_UC2);
    CanDebugAddVar(&can_dg_Iu_RMS);
    CanDebugAddVar(&can_dg_Iv_RMS);
    CanDebugAddVar(&can_dg_Iw_RMS);
    CanDebugAddVar(&can_dg_Shutdown_Error);

#else
    CanDebugAddVar(&can_dg_run_cmd);
    CanDebugAddVar(&can_dg_clear_errors);
#endif

    //
    // Add post mort info
    //
    CanDebugAddPostMort(&info_post);
#if (ENABLE_KSI_POSTMORT == 1)
    CanDebugAddPostMort(&I_trolley_post); // 1
    CanDebugAddPostMort(&Uc1_KSI_post); // 2
    CanDebugAddPostMort(&Uc2_KSI_post); // 3
    CanDebugAddPostMort(&U_cat_post); // 4
    CanDebugAddPostMort(&I_res_post);// 5
#endif
#if (ENABLE_KSII_POSTMORT == 1)
    CanDebugAddPostMort(&Uc1_KSII_post); // 6
    CanDebugAddPostMort(&Uc2_KSII_post);// 7
#endif
#if (ENABLE_TEMP_POSTMORT == 1)
    CanDebugAddPostMort(&KSI_temp_post); // 8
    CanDebugAddPostMort(&KSII_temp_post);// 9
#endif
#if (ENABLE_KSII_POSTMORT == 1)
    CanDebugAddPostMort(&Iu_post); // 10
    CanDebugAddPostMort(&Iv_post); // 11
    CanDebugAddPostMort(&Iw_post); // 12
    CanDebugAddPostMort(&Uu_post); // 13
    CanDebugAddPostMort(&Uv_post); // 14
    CanDebugAddPostMort(&Uw_post); // 15
#endif
#if (ENABLE_KSI_POSTMORT == 1)
    CanDebugAddPostMort(&VdcI_output_post); // 16
    CanDebugAddPostMort(&VdcII_output_post);// 17
    CanDebugAddPostMort(&I_trolley_output_post);// 18
    CanDebugAddPostMort(&z_up_KSI_output_post); // 19
    CanDebugAddPostMort(&z_do_KSI_output_post); // 20
#endif
#if (ENABLE_KSII_POSTMORT == 1)
    CanDebugAddPostMort(&Uu_output_post); // 21
    CanDebugAddPostMort(&Uv_output_post); // 22
    CanDebugAddPostMort(&Uw_output_post); // 23
#endif
#if (ENABLE_STATE_ERROR_POSTMORT == 1)
    CanDebugAddPostMort(&State_error_post);
#endif

}

void CanDebugSetState(ctrl_state_e state)
{
    control_status.ctrl_state = state;
}

void CanDebugSetMode(ctrl_type_e mode)
{
    control_status.control_type = mode;
}

void CanDebugAddVar(variable_t* variable)
{
    if (var_index < (MAX_VARS-1))
    {
        var_array[var_index] = variable;
        variable->var_id = var_index;
        var_index++;
        info_var.raw_data.uint32_var = var_index;
    }
}

void CanDebugSetEWCallback(f_ptr_type fptr)
{
    ew_fptr = fptr;
}

void CanDebugSetApplyCallback(f_ptr_type fptr)
{
    apply_fptr = fptr;
}

void CanDebugSetStatusCallback(f_ptr_status_type fptr)
{
    status_fptr = fptr;
}


void CanDebugAddPostMort(postmort_t* post)
{
    if (post_index < (MAX_POST_MORTS-1))
    {
        post_array[post_index] = post;
        post->postmort_id = post_index;
        post_index++;
    }
}

// wrapper function to support platform dependent CAN implementation
static void CanDebugSendMsg(canMsgDbg_t* msg)
{
    canMsg_t msg_tmp;
    uint16_t index;

    msg_tmp.obj_id = msg->obj_id;
    msg_tmp.msg_id = msg->msg_id;
    msg_tmp.len = 8;
    msg_tmp.data[0] = msg->cmd_id;
    if (msg->ext_len > 0)
    {
        msg_tmp.data[1] = msg->len >> 8;
        msg_tmp.data[2] = msg->len;
        msg_tmp.data[3] = msg->var_id;
    }
    else
    {
        msg_tmp.data[1] = msg->len;
        msg_tmp.data[2] = msg->var_id >> 8;
        msg_tmp.data[3] = msg->var_id;
    }
    for(index = 0; index < 4; index++)
    {
        msg_tmp.data[4+index] = msg->data[index];
    }

    RummCanSendMessage(&msg_tmp, 1);
}

// wrapper function to support platform dependent CAN implementation
static bool CanDebugGetMsg(canMsgDbg_t* msg)
{
    canMsg_t msg_tmp;
    uint16_t index;
    bool retval;

    msg_tmp.obj_id = msg->obj_id;
    msg_tmp.msg_id = msg->msg_id;

    retval = RummCanGetMessage(&msg_tmp);

    if (retval)
    {
        msg->cmd_id = msg_tmp.data[0];
        msg->len = msg_tmp.data[1];
        msg->var_id = (msg_tmp.data[2] << 8) | msg_tmp.data[3];
        for(index = 0; index < 4; index++)
        {
            msg->data[index] = msg_tmp.data[4+index];
        }
    }
    return retval;
}

void CanDebugSendStatus(void)
{
  // refresh vars
  CanDebugRefreshVars();
  static uint16_t id_byte = 0x55;

  //send back
  canTxMsg.data[0] = control_status.ctrl_state;
  canTxMsg.data[1] = control_status.control_type;
  canTxMsg.data[2] = control_status.version;
  canTxMsg.data[3] = control_status.reserve;
  // this is used for autodetection, periodically switches between 0x55 and 0xAA
  //control_status.id_byte ^= 0xFF;

  // set size of variable
  canTxMsg.len = 4;
  canTxMsg.cmd_id = is_status;
  canTxMsg.cmd_id |= ((uint16_t)CAN_DEBUG_SET_CONTROLER_STATE) << 4;
  canTxMsg.var_id = id_byte;
  canTxMsg.length = 8;

  id_byte ^= 0xFF;

  CanDebugSendMsg(&canTxMsg);
}
void masterwrite(uint16_t target_var_id, float target_value)
{
    canMsgDbg_t masterTxMsg;
    uint16_t i; // automated counter 
    
    // convert to HEX format
    union { float f; uint32_t u; } converter;
    converter.f = target_value;

    // target ids
    masterTxMsg.obj_id = CAN_TEST_MBOXID; 
    masterTxMsg.msg_id = CAN_TEST_CANID + 1; 
    masterTxMsg.ext_len = 0;
    masterTxMsg.len = 4;

    // using can dbg write
    masterTxMsg.cmd_id = ((uint16_t)CAN_DEBUG_WRITE) << 4; 
    masterTxMsg.var_id = target_var_id;

    // divide the data
    for(i = 0; i < 4; i++)
    {
        // divide 32-bit data to 4 bytes
        masterTxMsg.data[i] = ((converter.u >> ((3-i)<<3)) & 0xFF);
    }
    
    //sends it
    CanDebugSendMsg(&masterTxMsg);   
}

void CanDebugProcessInput(void)
{
    uint16_t idx, i, j;
    uint16_t last_index;
    uint16_t start_index = 0;
    //static uint32_t sayac = 0;
    //sayac++;
    //if(sayac>= 100000) // its a counter that enables us to send 1 mesg per 1s ( its in microseconds)
    //{
        //masterwrite(9, 25.5); // target id, target value
        //sayac=0;
    //}
    
    
   
    // do we have a message ?
    if (CanDebugGetMsg(&canRxMsg))
    {
        // normal length by default
        canTxMsg.ext_len = 0;

        switch ((canRxMsg.cmd_id >> 4))
        {
            case CAN_DEBUG_VAR_INFO:
                // broadcast all names is default
                last_index = var_index;
                start_index = 0;
                //canTxMsg.ext_len = 1;

                // user wants exactly one ID name
                if (canRxMsg.var_id < var_index)
                {
                    start_index = canRxMsg.var_id;
                    last_index = start_index + 1;
                }
                // user requests variable string names
                for(idx=start_index; idx<last_index; idx++)
                {
                    // get length of description string
                    uint16_t desclen = strlen(var_array[idx]->var_name);
                    // fill data to header packet
                    canTxMsg.cmd_id = (((uint16_t)CAN_DEBUG_VAR_INFO) << 4) | 0x1; // uint8_t
                    canTxMsg.var_id = idx;

                    // fill data
                    uint16_t data_idx = 0;
                    uint16_t data_sent = 0;
                    for(i=0;i<desclen;i++)
                    {
                        canTxMsg.data[data_idx++] = var_array[idx]->var_name[i];
                        if (data_idx == 4)
                        {
                            data_idx = 0;
                            canTxMsg.len = desclen - data_sent; // length is what rests
                            CanDebugSendMsg(&canTxMsg);
                            DELAY_US(PACKET_PAUSE_US);
                            data_sent += 4;
                        }
                    }
                    // add rest of data and finally end
                    if ((desclen - data_sent) > 0)
                    {
                        canTxMsg.len = desclen - data_sent; // length is what rests
                        for(i=desclen-data_sent;i<4;i++)
                        {
                            canTxMsg.data[i] = 0x0;
                        }
                        CanDebugSendMsg(&canTxMsg);
                        DELAY_US(PACKET_PAUSE_US);
                    }
                }
                break;

            case CAN_DEBUG_READ:
                // broadcast all names is default
                last_index = var_index;
                start_index = 0;
                //canTxMsg.ext_len = 1;

                // user wants exactly one ID name
                if (canRxMsg.var_id < var_index)
                {
                    start_index = canRxMsg.var_id;
                    last_index = start_index + 1;
                }
                // user requests variable string names
                for(idx=start_index; idx<last_index; idx++)
                // fill data to header packet
                //if (canRxMsg.var_id <= var_index)
                {
                    for(i=0;i<4;i++)
                    {
                        canTxMsg.data[i] = ((var_array[idx]->raw_data.uint32_var >> ((3-i)<<3)) & 0xFF);
                    }
                    // set size of variable
                    canTxMsg.len = 4;
                    canTxMsg.cmd_id = ((uint16_t)var_array[idx]->var_type) & 0xF;
                    canTxMsg.cmd_id |= ((uint16_t)CAN_DEBUG_READ) << 4;
                    canTxMsg.var_id = idx;

                    CanDebugSendMsg(&canTxMsg);
                    DELAY_US(PACKET_PAUSE_US);
                }
                break;

            case CAN_DEBUG_VARPROPS:
              // broadcast all names is default
              last_index = var_index;
              start_index = 0;
              //canTxMsg.ext_len = 1;

              // user wants exactly one ID name
              if (canRxMsg.var_id < var_index)
              {
                  start_index = canRxMsg.var_id;
                  last_index = start_index + 1;
              }
              // user requests variable string names
              for(idx=start_index; idx<last_index; idx++)
              {
                  for(i=0;i<4;i++)
                  {
                      canTxMsg.data[i] = 0;
                  }
                  // set size of variable
                  canTxMsg.len = 4;
                  canTxMsg.cmd_id = ((uint16_t)var_array[idx]->var_type) & 0xF;
                  canTxMsg.cmd_id |= ((uint16_t)CAN_DEBUG_VARPROPS) << 4;
                  canTxMsg.var_id = idx;
                  canTxMsg.data[0] = (var_array[idx]->eeprom_store_ena << 3) |
                                   (var_array[idx]->user_write << 2) |
                                   (var_array[idx]->user_read << 1) |
                                   (var_array[idx]->readonly << 0);

                  CanDebugSendMsg(&canTxMsg);
                  DELAY_US(PACKET_PAUSE_US);
              }
              break;

            case CAN_DEBUG_WRITE:
                // fill data to header packet
                if (canRxMsg.var_id <= var_index)
                {
                    if (var_array[canRxMsg.var_id]->readonly == 0)
                    {
                        var_array[canRxMsg.var_id]->raw_data.uint32_var = ((uint32_t)canRxMsg.data[0] << 24) | ((uint32_t)canRxMsg.data[1] << 16) | ((uint32_t)canRxMsg.data[2] << 8) | (canRxMsg.data[3]);
                    }
                    //send back
                    for(i=0;i<4;i++)
                    {
                        canTxMsg.data[i] = ((var_array[canRxMsg.var_id]->raw_data.uint32_var >> ((3-i)<<3)) & 0xFF);
                    }
                    // set size of variable
                    canTxMsg.len = 4;
                    canTxMsg.cmd_id = ((uint16_t)var_array[canRxMsg.var_id]->var_type) & 0xF;
                    canTxMsg.cmd_id |= ((uint16_t)CAN_DEBUG_WRITE) << 4;
                    canTxMsg.var_id = canRxMsg.var_id;

                    CanDebugSendMsg(&canTxMsg);
                }
                break;

            case CAN_DEBUG_READ_POSTMORT:
                if (canRxMsg.var_id <= post_index)
                {
                    // set size of variable
                    canTxMsg.len = 4;
                    canTxMsg.cmd_id = ((uint16_t)post_array[canRxMsg.var_id]->var_type) & 0xF;
                    canTxMsg.cmd_id |= ((uint16_t)CAN_DEBUG_READ_POSTMORT) << 4;
                    canTxMsg.var_id = (uint16_t)post_array[canRxMsg.var_id]->postmort_id;
                    canTxMsg.ext_len = 1;
                    uint32_t data_sent = 0;
                    uint16_t array_len = post_array[canRxMsg.var_id]->length;

                    // detect 16-bit variables
                    if ((post_array[canRxMsg.var_id]->var_type == is_uint16) || (post_array[canRxMsg.var_id]->var_type == is_int16))
                    {
                        uint16_t* data = post_array[canRxMsg.var_id]->p_data.p_uint16; //get pointer to array
                        uint16_t data_len = 0;
                        // walk through post mort array
                        for(i=0;i<(array_len);i+=2)
                        {
                            // place data to CAN message
                            for(j=0;j<4;j++)
                            {
                                if ((i+(j>>1))<array_len)
                                {
                                    canTxMsg.data[j] = ((*(data + i + (j>>1))>>((3-j)<<3)) & 0xFF);
                                }
                                else
                                {
                                    canTxMsg.data[j] = 0;
                                }
                            }
                            canTxMsg.len = (post_array[canRxMsg.var_id]->length >> 1) - data_sent;
                            CanDebugSendMsg(&canTxMsg);
                            data_sent++;
                            DELAY_US(PACKET_PAUSE_US);
                        }
                    }
                    else
                    {
                        // walk through post mort array
                        for(i=0;i<array_len;i++)
                        {
                            uint32_t* data = post_array[canRxMsg.var_id]->p_data.p_uint32; //get pointer to array
                            // place data to CAN message
                            /*for(j=0;j<4;j++)
                            {
                                canTxMsg.data[j] = ((*(data + i)>>((3-j)<<3)) & 0xFF);
                            }
                            */
                            canTxMsg.data[0] = (data[i] >> 24) & 0xFF;
                            canTxMsg.data[1] = (data[i] >> 16) & 0xFF;
                            canTxMsg.data[2] = (data[i] >> 8) & 0xFF;
                            canTxMsg.data[3] = data[i] & 0xFF;
                            canTxMsg.len = post_array[canRxMsg.var_id]->length - data_sent;
                            CanDebugSendMsg(&canTxMsg);
                            data_sent++;
                            DELAY_US(PACKET_PAUSE_US);
                        }
                    }
                }
                canTxMsg.ext_len = 0; //setup normal CAN message structure
                break;

            case CAN_DEBUG_POST_INFO:
                // broadcast all names is default
                last_index = post_index;
                start_index = 0;
                //canTxMsg.ext_len = 1;

                // user wants exactly one ID name
                if (canRxMsg.var_id < post_index)
                {
                    start_index = canRxMsg.var_id;
                    last_index = start_index + 1;
                }
                // user requests variable string names
                for(idx=start_index; idx<last_index; idx++)
                {
                    // get length of description string
                    uint16_t desclen = strlen(post_array[idx]->postmort_name);
                    // fill data to header packet
                    canTxMsg.cmd_id = (((uint16_t)CAN_DEBUG_POST_INFO) << 4) | 0x1;
                    canTxMsg.var_id = idx;

                    // fill data
                    uint16_t data_idx = 0;
                    uint16_t data_sent = 0;
                    for(i=0;i<desclen;i++)
                    {
                        canTxMsg.data[data_idx++] = post_array[idx]->postmort_name[i];
                        if (data_idx == 4)
                        {
                            data_idx = 0;
                            canTxMsg.len = desclen - data_sent; // length is what rests
                            CanDebugSendMsg(&canTxMsg);
                            DELAY_US(PACKET_PAUSE_US);
                            data_sent += 4;
                        }
                    }
                    // add rest of data and finally end
                    if ((desclen - data_sent) > 0)
                    {
                        canTxMsg.len = desclen - data_sent; // length is what rests
                        for(i=desclen-data_sent;i<4;i++)
                        {
                            canTxMsg.data[i] = 0x0;
                        }
                        CanDebugSendMsg(&canTxMsg);
                        DELAY_US(PACKET_PAUSE_US);
                    }
                }
                break;

            case CAN_DEBUG_SET_CONTROLER_STATE:

                if (canRxMsg.data[0] != NO_ACTION)
                {
                    control_status.ctrl_state = (ctrl_state_e)canRxMsg.data[0];
                }

                if (canRxMsg.data[1] != NO_ACTION)
                {
                    control_status.control_type = (ctrl_type_e)canRxMsg.data[1];
                }

                // read only
                if (canRxMsg.data[2] != NO_ACTION)
                {
                    //control_status.version = canRxMsg.data[2];
                }

                // readonly
                if (canRxMsg.data[3] != NO_ACTION)
                {
                    //control_status.reserve2 = canRxMsg.data[3];
                }

                // send status back
                CanDebugSendStatus();

                // run callback function
                if (status_fptr != 0)
                {
                  status_fptr(&control_status);
                }

                break;

            case CAN_DEBUG_SAVE_PARAM:
                // set size of variable
                canTxMsg.len = 4;
                canTxMsg.cmd_id = ((uint16_t)var_array[canRxMsg.var_id]->var_type) & 0xF;
                canTxMsg.cmd_id |= ((uint16_t)CAN_DEBUG_SAVE_PARAM) << 4;
                canTxMsg.var_id = canRxMsg.var_id;
                CanDebugSendMsg(&canTxMsg);
                // run callback
                if (ew_fptr != 0)
                {
                    ew_fptr();
                }
                break;

            case CAN_DEBUG_APPLY_PARAM:
                can_apply_params = 1;
                // set size of variable
                canTxMsg.len = 4;
                canTxMsg.cmd_id = ((uint16_t)var_array[canRxMsg.var_id]->var_type) & 0xF;
                canTxMsg.cmd_id |= ((uint16_t)CAN_DEBUG_APPLY_PARAM) << 4;
                canTxMsg.var_id = canRxMsg.var_id;
                CanDebugSendMsg(&canTxMsg);

                // run callback
                if (apply_fptr != 0)
                {
                  apply_fptr();
                }

                break;

            default:
                break;

        }

    }
}

void CanDebugRefreshInputs()
{
//     static uint16_t run_counter = 0;
//     static uint16_t reset_counter = 1;
//     static uint16_t allow_write_into_shutdown_error_indicator;
        static uint32_t mesaj_sayaci = 0;
    mesaj_sayaci++;

    // CAN hattını kitlememek için mesajı yavaşlatıyoruz (Örn: Saniyede 1 kez)
   
//     //CTRL_state_enum ctrl_state = Get_ctrl_state();
//     run_counter++;

//     if (can_dg_clear_errors.raw_data.uint32_var != 0)
//     {
//         //ctrl_state_shadow = Clear_Errors;
//         can_dg_clear_errors.raw_data.uint32_var = 0;
//     }
//     else
//     {
//         if (control_status.ctrl_state == RUN)
//         {
//             if (ctrl_state == Stopped)
//             {
//                 ctrl_state_shadow = Start;
//                 if (reset_counter == 1)
//                 {
//                     run_counter = 0;
//                     reset_counter = 0;
//                 }
//             }
//         }
//     }
//     if ((ctrl_state == Precharging) || (ctrl_state == Charging) || (ctrl_state == Running) || (ctrl_state == Bypassing_res))
//     {
//         run_counter = 0;
//         reset_counter = 1;

//     }
//     if (run_counter == 5)
//     {
//         control_status.ctrl_state = STOPPED;
//         reset_counter = 1;
//     }
//     if (control_status.ctrl_state == STOP)
//     {
//         control_status.ctrl_state = STOPPED;
//         ctrl_state_shadow = Stop;
//     }
// #if (RICE_CONTROL2_TEST != 1)
//     inputs.step_up.request.Vdcw_total = can_dg_KSI_U_rq.raw_data.float_var;
//     uw_grid = can_dg_KSII_U_rq.raw_data.float_var;
// #endif

// #if (RICE_CONTROL2_TEST == 1)
//     comm_data.cmds.bit.RUN = can_dg_run_cmd.raw_data.uint32_var;
//     comm_data.cmds.bit.RESET = can_dg_clear_errors.raw_data.uint32_var;
// #endif
}

void CanDebugRefreshVars()
{

#if (RICE_CONTROL2_TEST != 1)
    can_dg_errors.raw_data.uint32_var = 0;
    can_dg_Shutdown_Error.raw_data.uint32_var = 0;
    can_dg_warnings.raw_data.uint32_var = 0;
    can_dg_temp_KSI.raw_data.float_var = 1;
    can_dg_temp_KSII.raw_data.float_var = 2;
    can_dg_KSI_UC1.raw_data.float_var = 3;
    can_dg_KSI_UC2.raw_data.float_var = 4;
    can_dg_cat_volt.raw_data.float_var = 5;
    can_dg_I_trolley.raw_data.float_var = 6;
    can_dg_I_res.raw_data.float_var = 7;
    can_dg_KSII_UC1.raw_data.float_var = 8;
    can_dg_KSII_UC2.raw_data.float_var = 9;
    can_dg_temp_KSII_C.raw_data.float_var = 10;
    can_dg_temp_Ambient.raw_data.float_var = 11;
    can_dg_Iu_RMS.raw_data.float_var = 12;
    can_dg_Iv_RMS.raw_data.float_var = 13;
    can_dg_Iw_RMS.raw_data.float_var = 14;
#if (SEND_RAW_KSI_TEMP == 1)
    can_dg_temp_KSI_raw.raw_data.uint32_var = AdcdResultRegs.ADCRESULT3;
#endif
#if (SEND_RAW_KSII_TEMP == 1)
    can_dg_temp_KSII_raw.raw_data.uint32_var = AdcdResultRegs.ADCRESULT2;
#endif
#endif


}


