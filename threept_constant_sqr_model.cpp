#include "threept_constant_sqr_model.h"

threept_constant_sqr_model::threept_constant_sqr_model(string _C_str, string _t_str, string _T_str)
{
  C_str=_C_str;
  t_str=_t_str;
  T_str=_T_str;
  params.resize(1);
}


int threept_constant_sqr_model::get_n_functions()
{
  return 1;
}


int threept_constant_sqr_model::get_n_variables()
{
  return 2;
}


int threept_constant_sqr_model::get_n_parameters()
{
  return 1;
}

int threept_constant_sqr_model::get_n_constants()
{
  return 0;
}


void threept_constant_sqr_model::set_parameters(const vector< double >& parameters)
{
  params=parameters;
}


void threept_constant_sqr_model::set_constants(const vector< double >& constants)
{
}


double threept_constant_sqr_model::eval_function(int function, const vector< double >& arguments)
{
  return params[0]*params[0];
}


double threept_constant_sqr_model::eval_derivative(int function, int parameter, const vector< double >& arguments)
{
  if(parameter==0)
  {
    return 2.0*params[0];
  }
  else
  {
    return 0.0;
  }
}


int threept_constant_sqr_model::no_of_errors()
{
  return 0;
}


string threept_constant_sqr_model::get_function_name(int function, string variable)
{
  if(function!=0)
  {
    return "";
  }
  else
  {
    return C_str+"*"+C_str;
  }
}


string threept_constant_sqr_model::get_function_name(int function)
{
  return get_function_name(function, t_str);
}


string threept_constant_sqr_model::get_parameter_name(int parameter)
{
  if(parameter!=0)
  {
    return "";
  }
  else
  {
    return C_str;
  }
}


string threept_constant_sqr_model::get_variable_name(int variable)
{
  if(variable==0)
  {
    return t_str;
  }
  else if(variable==1)
  {
    return T_str;
  }
  else
  {
    return "";
  }
}

string threept_constant_sqr_model::get_constant_name(int constant)
{
  return "";
}
