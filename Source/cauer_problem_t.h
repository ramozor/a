#ifndef CAUER_PROBLEM_T
#define CAUER_PROBLEM_T

#include "problem_base_t.h"
#include "F28x_Project.h"

// we are introducing a global gradient array for debugging
extern volatile FLOAT_PREC debug_grad[5]; 

#define N_DATA_POINTS 50 

typedef struct {
    FLOAT_PREC y_data[N_DATA_POINTS];
    FLOAT_PREC u_data[N_DATA_POINTS];
    FLOAT_PREC Ts; 
} cauer_params_t;

// v[0] = R1, v[1] = tau1, v[2] = R2, v[3] = tau2, v[4] = x2_hat[0]
class cauer_problem_t : public problem_base_t<5> {
public:

    FLOAT_PREC get_the_oldest_y()
    {

        return y_data[head_index];
    }

      
    // call it in interrupt
    unsigned int head_index = 0;
    void update_measurements(FLOAT_PREC new_u, FLOAT_PREC new_y) {

    // Overwrite the oldest data with the new data (O(1) operation)
    u_data[head_index] = new_u;
    y_data[head_index] = new_y;

    // Move the pointer forward. If it hits the end, wrap around to 0.
    head_index = (head_index + 1) % N_DATA_POINTS;

} 
    void reset_buffers() {
        // disable interrupts temprorarily
        DINT; 
        
        for(unsigned int i = 0; i < N_DATA_POINTS; ++i) {
            u_data[i] = 0.0;
            y_data[i] = 0.0;

            u_data_shadow[i] = 0.0;
            y_data_shadow[i] = 0.0;
        }

        head_index = 0;
        head_index_shadow = 0;
        
        // enable interrupts
        EINT; 
    }


FLOAT_PREC u_data_shadow[N_DATA_POINTS];
FLOAT_PREC y_data_shadow[N_DATA_POINTS];
unsigned int head_index_shadow = 0;

// 2. call it in main before grad calc
void snapshot_for_cost() {
    // disable interrupts temporarily
    DINT; 
    
    // copy the datas to shadow memory
    for(unsigned int i = 0; i < N_DATA_POINTS; ++i) {
        u_data_shadow[i] = u_data[i];
        y_data_shadow[i] = y_data[i];
    }
    head_index_shadow = head_index;
    
    // enable interrupts
    EINT; 
}

    FLOAT_PREC cost(const vector_t &v) const {
    FLOAT_PREC total_cost = 0;
    FLOAT_PREC x2_hat = v[4]; 
    
    FLOAT_PREC R1 = v[0], tau1 = v[1], R2 = v[2], tau2 = v[3];
    
    FLOAT_PREC a11 = 1.0 - Ts / tau1;
    FLOAT_PREC a12 = Ts / tau1;
    FLOAT_PREC a21 = (R2 * Ts) / (R1 * tau2);
    FLOAT_PREC a22 = 1.0 - a21 - Ts / tau2;
    FLOAT_PREC b1  = (R1 * Ts) / tau1;
    
    // Start reading from the oldest data in the shadow array
    unsigned int idx = head_index_shadow; 


        
        // outside the for cycle
        // initialize x:,1_hat = [y_k[head_index];x2_hat]
        // That translate into y_hat_next = y_k[head_index]
        
        FLOAT_PREC y_hat = y_data_shadow[idx];

        for (unsigned int idx = idx + 1; idx < N_DATA_POINTS; ++idx) {
            //unsigned int idx_next = (idx + 1 == N_DATA_POINTS) ? 0 : idx + 1;
            FLOAT_PREC u_k_1 = u_data_shadow[idx];
            FLOAT_PREC y_hat_next = a11 * y_hat + a12 * x2_hat + b1 * u_k_1;
            // Instead of y_k use y_hat_next
            
            FLOAT_PREC err = y_data_shadow[idx] - y_hat_next;
            
            FLOAT_PREC scaled_err = err;
            total_cost += (scaled_err * scaled_err);
            
            x2_hat = a21 * y_hat + a22 * x2_hat;
            y_hat = y_hat_next;
        }

       
        for (unsigned int idx = 0; idx < head_index; ++idx) {
            //unsigned int idx_next = idx + 1;
            
            FLOAT_PREC u_k_1 = u_data_shadow[idx];
            FLOAT_PREC y_hat_next = a11 * y_hat + a12 * x2_hat + b1 * u_k_1;
            // Instead of y_k use y_hat_next
            
            FLOAT_PREC err = y_data_shadow[idx] - y_hat_next;
            
            FLOAT_PREC scaled_err = err;
            total_cost += (scaled_err * scaled_err);
            
            x2_hat = a21 * y_hat + a22 * x2_hat;
            y_hat = y_hat_next;
        }
        
        return total_cost;
}

    FLOAT_PREC grad_calc(const vector_t &v, vector_t &grad) const {
        for (int i = 0; i < 5; ++i) grad[i] = 0;
        
        FLOAT_PREC R1 = v[0], tau1 = v[1], R2 = v[2], tau2 = v[3];
        
        FLOAT_PREC a11 = 1.0 - Ts / tau1;
        FLOAT_PREC a12 = Ts / tau1;
        FLOAT_PREC a21 = (R2 * Ts) / (R1 * tau2);
        FLOAT_PREC a22 = 1.0 - a21 - Ts / tau2;
        FLOAT_PREC b1  = (R1 * Ts) / tau1;

        FLOAT_PREC da21_dR1 = -(R2 * Ts) / (R1 * R1 * tau2);
        FLOAT_PREC da22_dR1 = -da21_dR1;
        FLOAT_PREC db1_dR1  = Ts / tau1;

        FLOAT_PREC da11_dtau1 = Ts / (tau1 * tau1);
        FLOAT_PREC da12_dtau1 = -Ts / (tau1 * tau1);
        FLOAT_PREC db1_dtau1  = -(R1 * Ts) / (tau1 * tau1);

        FLOAT_PREC da21_dR2 = Ts / (R1 * tau2);
        FLOAT_PREC da22_dR2 = -da21_dR2;

        FLOAT_PREC da21_dtau2 = -(R2 * Ts) / (R1 * tau2 * tau2);
        FLOAT_PREC da22_dtau2 = -da21_dtau2 + Ts / (tau2 * tau2);
        
        FLOAT_PREC total_cost = 0;
        FLOAT_PREC x2_hat = v[4]; 
        
        FLOAT_PREC dx2_dR1 = 0, dx2_dtau1 = 0, dx2_dR2 = 0, dx2_dtau2 = 0;
        FLOAT_PREC dx2_dv4 = 1.0; 

        unsigned int current_head = head_index;
        FLOAT_PREC y_k = y_data_shadow[current_head];
         
        
        unsigned int loop_count = 0;
        for (unsigned int idx = current_head; ((idx < N_DATA_POINTS)&&(loop_count < (N_DATA_POINTS - 1))); ++idx) {
            unsigned int idx_next = (idx + 1 == N_DATA_POINTS) ? 0 : idx + 1;
            
            
            FLOAT_PREC u_k = u_data_shadow[idx];
            
            FLOAT_PREC y_hat_next = a11 * y_k + a12 * x2_hat + b1 * u_k;
            FLOAT_PREC err = y_data_shadow[idx_next] - y_hat_next;
            FLOAT_PREC scaled_err = err;
            
            FLOAT_PREC dy_dR1   = a12 * dx2_dR1 + db1_dR1 * u_k;
            FLOAT_PREC dy_dtau1 = da11_dtau1 * y_k + da12_dtau1 * x2_hat + a12 * dx2_dtau1 + db1_dtau1 * u_k;
            FLOAT_PREC dy_dR2   = a12 * dx2_dR2;
            FLOAT_PREC dy_dtau2 = a12 * dx2_dtau2;
            FLOAT_PREC dy_dv4   = a12 * dx2_dv4; 
            
            grad[0] += -2.0 * scaled_err  * dy_dR1;
            grad[1] += -2.0 * scaled_err  * dy_dtau1;
            grad[2] += -2.0 * scaled_err  * dy_dR2;
            grad[3] += -2.0 * scaled_err  * dy_dtau2;
            grad[4] += -2.0 * scaled_err  * dy_dv4; 
            
            total_cost += (scaled_err * scaled_err);
            
            FLOAT_PREC next_x2_hat = a21 * y_k + a22 * x2_hat;
            
            
            dx2_dR1   = da21_dR1 * y_k + da22_dR1 * x2_hat + a22 * dx2_dR1;
            dx2_dtau1 = a22 * dx2_dtau1;
            dx2_dR2   = da21_dR2 * y_k + da22_dR2 * x2_hat + a22 * dx2_dR2;
            dx2_dtau2 = da21_dtau2 * y_k + da22_dtau2 * x2_hat + a22 * dx2_dtau2;
            dx2_dv4   = a22 * dx2_dv4; 
            
            
            x2_hat = next_x2_hat;
            y_k = y_hat_next;
            loop_count++;
        }

        // for (unsigned int idx = 0; idx < current_head-1; ++idx) {
        //     unsigned int idx_next = idx + 1;
        for (unsigned int idx = 0; loop_count < (N_DATA_POINTS-1); ++idx) {
             unsigned int idx_next = idx + 1;
            
            //FLOAT_PREC y_k = y_data_shadow[idx];

            FLOAT_PREC u_k = u_data_shadow[idx];
            
            FLOAT_PREC y_hat_next = a11 * y_k + a12 * x2_hat + b1 * u_k;
            FLOAT_PREC err = y_data_shadow[idx_next] - y_hat_next;
            FLOAT_PREC scaled_err = err;
            
            FLOAT_PREC dy_dR1   = a12 * dx2_dR1 + db1_dR1 * u_k;
            FLOAT_PREC dy_dtau1 = da11_dtau1 * y_k + da12_dtau1 * x2_hat + a12 * dx2_dtau1 + db1_dtau1 * u_k;
            FLOAT_PREC dy_dR2   = a12 * dx2_dR2;
            FLOAT_PREC dy_dtau2 = a12 * dx2_dtau2;
            FLOAT_PREC dy_dv4   = a12 * dx2_dv4; 
            
            grad[0] += -2.0 * scaled_err  * dy_dR1;
            grad[1] += -2.0 * scaled_err  * dy_dtau1;
            grad[2] += -2.0 * scaled_err  * dy_dR2;
            grad[3] += -2.0 * scaled_err  * dy_dtau2;
            grad[4] += -2.0 * scaled_err  * dy_dv4; 
            
            total_cost += (scaled_err * scaled_err);
            
            FLOAT_PREC next_x2_hat = a21 * y_k + a22 * x2_hat;
            
            
            dx2_dR1   = da21_dR1 * y_k + da22_dR1 * x2_hat + a22 * dx2_dR1;
            dx2_dtau1 = a22 * dx2_dtau1;
            dx2_dR2   = da21_dR2 * y_k + da22_dR2 * x2_hat + a22 * dx2_dR2;
            dx2_dtau2 = da21_dtau2 * y_k + da22_dtau2 * x2_hat + a22 * dx2_dtau2;
            dx2_dv4   = a22 * dx2_dv4; 
            
            x2_hat = next_x2_hat;
            y_k = y_hat_next;
            loop_count++;
        }

        debug_grad[0] = grad[0];
        debug_grad[1] = grad[1];
        debug_grad[2] = grad[2];
        debug_grad[3] = grad[3];
        debug_grad[4] = grad[4]; 
        
        return total_cost;
    }
    void init(cauer_params_t &par_in) {
        Ts = par_in.Ts;
        for (unsigned int k = 0; k < N_DATA_POINTS; ++k) u_data[k] = par_in.u_data[k];
        for (unsigned int k = 0; k <= N_DATA_POINTS; ++k) y_data[k] = par_in.y_data[k];
    }

    

private:
    FLOAT_PREC y_data[N_DATA_POINTS];
    FLOAT_PREC u_data[N_DATA_POINTS];
    FLOAT_PREC Ts;
};
#endif
