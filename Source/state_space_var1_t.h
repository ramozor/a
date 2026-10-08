#ifndef STATE_SPACE_VAR1_T
#define STATE_SPACE_VAR1_T
#include "type_definitions.h"

typedef struct
{
    FLOAT_PREC a11,a12,a21,a22;
    FLOAT_PREC b1;
}state_space_var1_par_t;

class state_space_var1_t
{
    private:
    FLOAT_PREC a11,a12,a21,a22;
    FLOAT_PREC b1;
    FLOAT_PREC x1, x2;
    


    static const int HISTORY_SIZE = 60;
    FLOAT_PREC hist_u[HISTORY_SIZE];
    FLOAT_PREC hist_x1[HISTORY_SIZE];
    FLOAT_PREC hist_x2[HISTORY_SIZE];
    int hist_index;
    int hist_count;

    FLOAT_PREC u_prev;
    
    public :
    void set_initial_states(FLOAT_PREC initial_x1, FLOAT_PREC initial_x2)
    {
        x1 = initial_x1;
        x2 = initial_x2;
        u_prev = 0;
    }
    inline void step(const FLOAT_PREC u)
    {
        // Store current states and input into the circular buffer
        
        
        
        
        if (hist_count < HISTORY_SIZE) {
            hist_count++;
        }

        const FLOAT_PREC next_x1 = (a11 * x1) + (a12 * x2) + (b1 * u_prev);
        const FLOAT_PREC next_x2 = (a21 * x1) + (a22 * x2);
        
        x1 = next_x1;
        x2 = next_x2;

        hist_x1[hist_index] = x1;
        hist_x2[hist_index] = x2;

        hist_u[hist_index] = u_prev;

        u_prev = u;

        hist_index = (hist_index + 1) % HISTORY_SIZE;
    }
    
    void reset()
    {
        x1 = 0;
        x2 = 0;
        hist_index = 0;
        hist_count = 0;
        for(int i = 0; i < HISTORY_SIZE; ++i) {
            hist_u[i] = 0;
            hist_x1[i] = 0;
            hist_x2[i] = 0;
        }
        u_prev = 0;
    }

    inline FLOAT_PREC get_y()
    {
        return x1; // C = [1 0]
    }

    void init(state_space_var1_par_t &par_in)
    {
        a11 = par_in.a11;
        a12 = par_in.a12;
        a21 = par_in.a21;
        a22 = par_in.a22;
        b1  = par_in.b1;
    }

    

};
#endif