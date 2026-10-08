#ifndef TOY_PROBLEM_T
#define TOY_PROBLEM_T

#include "problem_base_t.h"

typedef struct {
    FLOAT_PREC c1;
    FLOAT_PREC c2;
}toy_problem_params_t;

// Implements the following two-variable optimization problem:
// (v[0] + c1)^2 + (v[1] + c2)^2 + (v[0] * v[1] + c1 * c2)^2
class toy_problem_t : public problem_base_t<2> {
public:
  // Returns the objective value at v without modifying v.
  FLOAT_PREC cost(const vector_t &v) const {
    return (v[0] + c1) * (v[0] + c1) + (v[1] + c2) * (v[1] + c2) +
           (v[0] * v[1] + c1 * c2) * (v[0] * v[1] + c1 * c2);
  }

  // Writes the gradient at v to grad and returns the objective value at v.
  FLOAT_PREC grad_calc(const vector_t &v, vector_t &grad) const {
    grad[0] = 2 * (v[0] + c1) + 2 * v[1] * (v[0] * v[1] + c1 * c2);
    grad[1] = 2 * (v[1] + c2) + 2 * v[0] * (v[0] * v[1] + c1 * c2);
    return cost(v);
  }

  // Stores the problem-specific constants used by cost() and grad_calc().
  void init(toy_problem_params_t &par_in) {
    c1 = par_in.c1;
    c2 = par_in.c2;
  }

private:
  FLOAT_PREC c1;
  FLOAT_PREC c2;
};
#endif