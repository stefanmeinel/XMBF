#ifndef THREEPT_CONSTANT_SQR_MODEL_H
#define THREEPT_CONSTANT_SQR_MODEL_H

#include "abstract_model.h"

#include <cmath>
#include <string>
#include <sstream>

using namespace std;

class threept_constant_sqr_model : public abstract_model
{
  public:

    threept_constant_sqr_model(string _C_str, string _t_str, string _T_str);

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

    string C_str, t_str, T_str;
    vector< double > params;
};

#endif
