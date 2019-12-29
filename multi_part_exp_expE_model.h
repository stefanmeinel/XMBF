#ifndef MULTI_PART_EXP_EXPE_MODEL_H
#define MULTI_PART_EXP_EXPE_MODEL_H

// this model was added by William Detmold

#include "abstract_model.h"

#include <cmath>
#include <string>
#include <sstream>
#include <iostream>

#include <gsl/gsl_sf_gamma.h>

using namespace std;

class multi_part_exp_expE_model : public abstract_model
{
  public:

    multi_part_exp_expE_model(string _E_str, string _dE_str, string _Z_str,
                              string _A_str, string _B_str, string _t_str,
                              string _T_str, int n_exp, int n_part);


// derived functions

    int get_n_functions();
    int get_n_variables();
    int get_n_parameters();
    int get_n_constants();

    void set_parameters(const vector< double >& parameters);
    void set_constants(const vector< double >& constants);

    double eval_function(int function, const vector< double >& arguments);
    double eval_derivative(int function, int parameter, const vector< double >& arguments);

    int no_of_errors();

    string get_function_name(int function);
    string get_function_name(int function, string variable);
    string get_parameter_name(int parameter);
    string get_variable_name(int variable);
    string get_constant_name(int constant);

  private:

    string E_str, dE_str, Z_str, A_str, B_str, t_str, T_str;

    int nexp;
    int npart;
    int nmax;
    int nparam;

    double T;

    vector< double > params;
};

#endif
