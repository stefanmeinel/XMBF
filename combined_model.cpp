#include "combined_model.h"

// warning: currently no checks of bounds etc

combined_model::combined_model()
{
  errors=0;
}


void combined_model::add_model(abstract_model* model, int model_group)
{
  models.push_back(model);
  model_groups.push_back(model_group);

  for(int p=0; p<model->get_n_parameters(); ++p)
  {
    if(find(parameter_names.begin(), parameter_names.end(), model->get_parameter_name(p))==parameter_names.end())
    {
      parameter_names.push_back(model->get_parameter_name(p));
    }
  }
  individual_parameters.clear();
  for(unsigned int model_index=0; model_index<models.size(); ++model_index)
  {
    vector< int > ips(parameter_names.size());
    for(unsigned int p=0; p<parameter_names.size(); ++p)
    {
      ips[p]=-1;
      for(int ip=0; ip<models[model_index]->get_n_parameters(); ++ip)
      {
        if( (models[model_index]->get_parameter_name(ip))==parameter_names[p] )
        {
          ips[p]=ip;
        }
      }
    }
    individual_parameters.push_back(ips);
  }

  for(int c=0; c<model->get_n_constants(); ++c)
  {
    if(find(constant_names.begin(), constant_names.end(), model->get_constant_name(c))==constant_names.end())
    {
      constant_names.push_back(model->get_constant_name(c));
    }
  }
  individual_constants.clear();
  for(unsigned int model_index=0; model_index<models.size(); ++model_index)
  {
    vector< int > ics(constant_names.size());
    for(unsigned int c=0; c<constant_names.size(); ++c)
    {
      ics[c]=-1;
      for(int ic=0; ic<models[model_index]->get_n_constants(); ++ic)
      {
        if( (models[model_index]->get_constant_name(ic))==constant_names[c] )
        {
          ics[c]=ic;
        }
      }
    }
    individual_constants.push_back(ics);
  }
}



void combined_model::set_all_arguments(const vector< vector< vector< double > > >& arguments)
{
  model_number.clear();
  data_point.clear();
  function.clear();
  all_arguments.clear();
  for(unsigned int model_index=0; model_index<models.size(); ++model_index)              // models
  {
    for(unsigned int m=0; m<arguments[model_index].size(); ++m)              // data points
    {
      for(int f=0; f<models[model_index]->get_n_functions(); ++f)   // functions
      {
        model_number.push_back(model_index);
        data_point.push_back(m);
        function.push_back(f);
        all_arguments.push_back(arguments[model_index][m]);
      }
    }
  }
}


int combined_model::get_n_fit_points()
{
  return model_number.size();
}


int combined_model::get_n_parameters()
{
  return parameter_names.size();
}


int combined_model::get_n_constants()
{
  return constant_names.size();
}


void combined_model::set_parameters(const vector< double >& params)
{
  for(unsigned int model_index=0; model_index<models.size(); ++model_index)
  {
    vector< double > parameter_values(models[model_index]->get_n_parameters());
    for(unsigned int p=0; p<parameter_names.size(); ++p)
    {
      if(individual_parameters[model_index][p]!=-1)
      {
        parameter_values[individual_parameters[model_index][p]]=params[p];
      }
    }
    models[model_index]->set_parameters(parameter_values);
  }
}


void combined_model::set_constants(const vector< double >& consts)
{
  for(unsigned int model_index=0; model_index<models.size(); ++model_index)
  {
    vector< double > constant_values(models[model_index]->get_n_constants());
    for(unsigned int c=0; c<constant_names.size(); ++c)
    {
      if(individual_constants[model_index][c]!=-1)
      {
        constant_values[individual_constants[model_index][c]]=consts[c];
      }
    }
    models[model_index]->set_constants(constant_values);
  }
}


int combined_model::model_group(int fit_point)
{
  return model_groups[model_number[fit_point]];
}


double combined_model::eval_function(int fit_point)
{
  tmp_d=models[model_number[fit_point]]->eval_function(function[fit_point], all_arguments[fit_point]);
  errors=models[model_number[fit_point]]->no_of_errors();
  return tmp_d;
}


double combined_model::eval_derivative(int fit_point, int parameter)
{
  int model_index=model_number[fit_point];
  int ip=individual_parameters[model_index][parameter];
  if(ip!=-1)
  {
    tmp_d=models[model_index]->eval_derivative(function[fit_point], ip, all_arguments[fit_point]);
    errors=models[model_number[fit_point]]->no_of_errors();
    return tmp_d;
  }
  else
  {
    return 0.0;
  }
}


string combined_model::get_parameter_name(int parameter)
{
  return parameter_names[parameter];
}


string combined_model::get_constant_name(int constant)
{
  return constant_names[constant];
}


int combined_model::no_of_errors()
{
  return errors;
}

