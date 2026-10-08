/*
 * can_debug_save_params.h
 *
 *  Created on: 16. 12. 2019
 *      Author: kosan
 */

#ifndef SRC_CAN_DEBUG_PARAMS_H_
#define SRC_CAN_DEBUG_PARAMS_H_


extern variable_t candbg_req01; // CAN debug speed controller Kp
extern variable_t candbg_req02; // CAN debug speed controller Tau
extern variable_t candbg_req03; // CAN debug speed current controller Kp
extern variable_t candbg_req04; // CAN debug speed current controller Tau
extern variable_t candbg_req05; // CAN debug speed filter length
extern variable_t candbg_req06; // CAN debug position controller Kp
extern variable_t candbg_prech_status;
extern variable_t candbg_neighbor_sts;

/**
 * Init and register debug variables.
 */
void CanDebugInitParams(void);

/**
 * Save values of debug variables to nonvolatile memory.
 */
void CanDebugSaveParams(void);

/**
 * Load values of debug variables from memory.
 */
void CanDebugLoadParams(void);




#endif /* SRC_CAN_DEBUG_PARAMS_H_ */
