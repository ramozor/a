/*
 * can_debug_params.c
 *
 *  Created on: 16. 12. 2019
 *      Author: kosan
 */
//#include "../settings.h"

//#include "src_hw/rumm_hwlib.h"
//#include "src/nonvol_params.h"
#include "can_debug.h"
#include "can_debug_params.h"

/*variable_t candbg_req01; // CAN debug speed controller Kp
variable_t candbg_req02; // CAN debug speed controller Tau
variable_t candbg_req03; // CAN debug speed current controller Kp
variable_t candbg_req04; // CAN debug speed current controller Tau
variable_t candbg_req05; // CAN debug speed filter length
variable_t candbg_req06; // CAN debug position controller Kp
variable_t candbg_neighbor_sts; // CAN debug - neighbor
variable_t candbg_prech_status;
variable_t candbg_speed;
variable_t candbg_position;
variable_t candbg_torque;*/


void CanDebugInitParams(void)
{
/*  candbg_prech_status.var_name = "Precharge status ";
  candbg_prech_status.var_type = is_uint32;
  candbg_prech_status.raw_data.uint32_var = 0;
  candbg_prech_status.readonly = 1;
  candbg_prech_status.eeprom_store_ena = 0;
  CanDebugAddVar(&candbg_prech_status);

  // add CAN debug variables
  candbg_req01.var_name = "Motor speed controller Kp";
  candbg_req01.var_type = is_float;
  candbg_req01.raw_data.float_var = 0.07;
  candbg_req01.readonly = 0;
  candbg_req01.eeprom_store_ena = 1;
  CanDebugAddVar(&candbg_req01);

  candbg_req02.var_name = "Motor speed controller Tau";
  candbg_req02.var_type = is_float;
  candbg_req02.raw_data.float_var = 0.4;
  candbg_req02.readonly = 0;
  candbg_req02.eeprom_store_ena = 1;
  CanDebugAddVar(&candbg_req02);

  candbg_req03.var_name = "Motor current controller Kp";
  candbg_req03.var_type = is_float;
  candbg_req03.raw_data.float_var = 20;
  candbg_req03.readonly = 0;
  candbg_req03.eeprom_store_ena = 1;
  CanDebugAddVar(&candbg_req03);

  candbg_req04.var_name = "Motor current controller Tau";
  candbg_req04.var_type = is_float;
  candbg_req04.raw_data.float_var = 0.1;
  candbg_req04.readonly = 0;
  candbg_req04.eeprom_store_ena = 1;
  CanDebugAddVar(&candbg_req04);

  candbg_neighbor_sts.var_name = "Neighbor status";
  candbg_neighbor_sts.var_type = is_uint32;
  candbg_neighbor_sts.raw_data.uint32_var = 0;
  candbg_neighbor_sts.readonly = 1;
  CanDebugAddVar(&candbg_neighbor_sts);

  candbg_req05.var_name = "Speed filter length";
  candbg_req05.var_type = is_uint32;
  candbg_req05.raw_data.uint32_var = 20;
  candbg_req05.readonly = 0;
  CanDebugAddVar(&candbg_req05);

  candbg_req06.var_name = "Position controller Kp";
  candbg_req06.var_type = is_float;
  candbg_req06.raw_data.float_var = 0.8;
  candbg_req06.readonly = 0;
  CanDebugAddVar(&candbg_req06);

*/
}

void CanDebugSaveParams(void)
{
  // To be deleted
  variable_t** array = var_array;
  uint16_t vars_count = var_index;
  uint16_t i;
//  nonvol_param_t vartmp;


  // for(i=0;i<vars_count;i++)
  // {
  //   if (array[i]->eeprom_store_ena == 1)
  //   {
  //     vartmp.id = array[i]->var_id;
  //     vartmp.value.value_uint32 = array[i]->raw_data.uint32_var;
  //     NvSaveParam(&vartmp, 1);
  //   }
  // }
}

void CanDebugLoadParams(void)
{
  variable_t** array = var_array;
  uint16_t vars_count = var_index;
  uint16_t i;
  // nonvol_param_t vartmp;


  // for(i=0;i<vars_count;i++)
  // {
  //   if (array[i]->eeprom_store_ena == 1)
  //   {
  //     vartmp.id = array[i]->var_id;
  //     if (NvLoadParam(&vartmp) == PARAM_OK)
  //     {
  //       array[i]->raw_data.uint32_var = vartmp.value.value_uint32;
  //     }
  //   }
  // }
}


