#ifndef PLANT_SIMULATOR_T_H_
#define PLANT_SIMULATOR_T_H_

#include "type_definitions.h"
#include "state_space_var1_t.h"
#include <math.h>

class plant_simulator_t {
public:
    void init();
    void step(uint32_t tick, FLOAT_PREC* out_u, FLOAT_PREC* out_y);

    state_space_var1_t dummy_hardware;
};

#endif