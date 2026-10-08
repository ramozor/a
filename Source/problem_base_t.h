#ifndef PROBLEM_BASE_T_H_
#define PROBLEM_BASE_T_H_

#include "type_definitions.h"

// Provides the compile-time information shared by all optimization problems.
//
// N is the number of decision variables used by the problem. It must be known
// at compile time so the solver can allocate fixed-size arrays and apply its
// selected loop-unrolling policy.
//
// This base deliberately contains no virtual methods. The solver is templated
// on the concrete problem type, so virtual dispatch and its runtime overhead
// are unnecessary.
template <unsigned int N>
class problem_base_t {
public:
    // Number of decision variables expected by cost() and grad_calc().
    enum { N_PARAMS = N };

    // Fixed-size vector type used for both decision variables and gradients.
    // Passing vector_t by reference preserves its size for compile-time checks.
    typedef FLOAT_PREC vector_t[N_PARAMS];

protected:
    // Construction and destruction are restricted to derived problem classes.
    // This prevents problem_base_t from being used as a standalone problem.
    problem_base_t()
    {
    }

    ~problem_base_t()
    {
    }
};

#endif