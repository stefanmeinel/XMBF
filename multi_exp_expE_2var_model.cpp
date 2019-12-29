#include "multi_exp_expE_2var_model.h"

multi_exp_expE_2var_model::multi_exp_expE_2var_model(string _E_str, string _dE_str, string _A_str, string _B_str, string _t_str, string _T_str, int n_exp)
{
  E_str=_E_str;
  dE_str=_dE_str;
  A_str=_A_str;
  B_str=_B_str;
  t_str=_t_str;
  T_str=_T_str;
  model=new multi_exp_expE_model(E_str, dE_str, A_str, B_str, T_str, n_exp);
}


multi_exp_expE_2var_model::~multi_exp_expE_2var_model()
{
  delete model;
}


int multi_exp_expE_2var_model::get_n_functions()
{
  return model->get_n_functions();
}


int multi_exp_expE_2var_model::get_n_variables()
{
  return 2;
}


int multi_exp_expE_2var_model::get_n_parameters()
{
  return model->get_n_parameters();
}

int multi_exp_expE_2var_model::get_n_constants()
{
  return 0;
}


void multi_exp_expE_2var_model::set_parameters(const vector< double >& parameters)
{
  model->set_parameters(parameters);
}


void multi_exp_expE_2var_model::set_constants(const vector< double >& constants)
{
}


double multi_exp_expE_2var_model::eval_function(int function, const vector< double >& arguments)
{
  vector< double > new_arguments(1);
  new_arguments[0]=arguments[1];
  return model->eval_function(function, new_arguments);
}


double multi_exp_expE_2var_model::eval_derivative(int function, int parameter, const vector< double >& arguments)
{
  vector< double > new_arguments(1);
  new_arguments[0]=arguments[1];
  return model->eval_derivative(function, parameter, new_arguments);
}


int multi_exp_expE_2var_model::no_of_errors()
{
  return model->no_of_errors();
}


string multi_exp_expE_2var_model::get_function_name(int function)
{
  return model->get_function_name(function, T_str);
}


string multi_exp_expE_2var_model::get_parameter_name(int parameter)
{
  return model->get_parameter_name(parameter);
}


string multi_exp_expE_2var_model::get_variable_name(int variable)
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

string multi_exp_expE_2var_model::get_constant_name(int constant)
{
  return "";
}
