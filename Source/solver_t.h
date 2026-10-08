#ifndef SOLVER_T_H_
#define SOLVER_T_H_

#include "type_definitions.h"


typedef struct {
    FLOAT_PREC learning_rate;
    FLOAT_PREC current_best[5];
} solver_params_t;

// The problem type provides N_PARAMS, cost(), and grad_calc().
// Loops are fully unrolled by default for problems with at most 10 parameters.
template <class PROBLEM_T, bool UNROLL>
class solver_t {
public:
    void init(PROBLEM_T *problem_in, solver_params_t &par_in)
    {
        problem = problem_in;
        learning_rate = par_in.learning_rate;

#ifdef __TI_COMPILER_VERSION__
        // UNROLL_FACTOR is N_PARAMS for full unrolling and 1 otherwise.
        // The loop always executes exactly N_PARAMS times.
//#pragma MUST_ITERATE(N_PARAMS, N_PARAMS, N_PARAMS)
//#pragma UNROLL(UNROLL_FACTOR)
#endif
        for (unsigned int i = 0; i < N_PARAMS; ++i)
        {
           
           v[i] = par_in.current_best[i];
           vbest[i] = par_in.current_best[i];
        }
    }

    void grad_calc()
    {
        FLOAT_PREC grad[N_PARAMS];

        // Calculate the cost and gradient at the current parameters.
        const FLOAT_PREC current_cost = problem->grad_calc(v, grad);
        // force the gradients of 0-4th elements to zero
        

#ifdef __TI_COMPILER_VERSION__
        // Fully unroll this loop when UNROLL is true. UNROLL_FACTOR is 1
        // when UNROLL is false, which disables automatic loop unrolling.
//#pragma MUST_ITERATE(N_PARAMS, N_PARAMS, N_PARAMS)
//#pragma UNROLL(UNROLL_FACTOR)
#endif
        for (unsigned int i = 0; i < N_PARAMS; ++i)
        {
            v[i] = v[i] - (learning_rate * grad[i]);
        }
        

        // Save the updated parameters only when they reduce the cost.
        const FLOAT_PREC new_cost = cost_calc();
        if (new_cost < current_cost)
        {
#ifdef __TI_COMPILER_VERSION__
            // Apply the same unrolling policy when copying the best parameters.
//#pragma MUST_ITERATE(N_PARAMS, N_PARAMS, N_PARAMS)
//#pragma UNROLL(UNROLL_FACTOR)
#endif
            for (unsigned int i = 0; i < N_PARAMS; ++i)
            {
                vbest[i] = v[i];
            }
        }
    }

    FLOAT_PREC cost_calc()
    {
        return problem->cost(v);
    }



    void get_best_parameters(FLOAT_PREC *dest) const
{
    for (unsigned int i = 0; i < N_PARAMS; ++i)
    {
        dest[i] = vbest[i];
    }
}


    void set_initial_guess(const FLOAT_PREC *new_guess)
{
    for (unsigned int i = 0; i < N_PARAMS; ++i)
    {
        vbest[i] = new_guess[i];
        v[i] = new_guess[i];
    }
}


 void reset_parameters(const FLOAT_PREC *default_params) {
        for (unsigned int i = 0; i < N_PARAMS; ++i) {
            v[i] = default_params[i];
            vbest[i] = default_params[i];
        }
}



private:
    enum {
        N_PARAMS = PROBLEM_T::N_PARAMS,
        UNROLL_FACTOR = UNROLL ? N_PARAMS : 1
    };

    FLOAT_PREC learning_rate;
    FLOAT_PREC vbest[N_PARAMS];
    FLOAT_PREC v[N_PARAMS];

    PROBLEM_T *problem;
};

#endif
