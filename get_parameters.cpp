#include "get_constants.h"

bool get_parameters(combined_model* c_model,
                    bool bayesian,
                    std::vector< std::string >& parameter_names,
                    std::vector< double >& start_values,
                    std::vector< double >& priors,
                    std::vector< double >& sigmas,
                    const xmlpp::Node* root_node)
{
  parameter_names.clear(); parameter_names.resize(c_model->get_n_parameters());
  start_values.clear();    start_values.resize(c_model->get_n_parameters());
  priors.clear();          priors.resize(c_model->get_n_parameters());
  sigmas.clear();          sigmas.resize(c_model->get_n_parameters());

  xmlpp::NodeSet parameter_values_nodeset = root_node->find("parameter_values");
  if(parameter_values_nodeset.size()!=1)
  {
    std::cerr << "Error: \"parameter_values\" did not return unique node" << std::endl << std::endl;
    return false;
  }
  xmlpp::NodeSet parameters_nodeset=(*parameter_values_nodeset.begin())->find("parameter");
  for(int p=0; p<c_model->get_n_parameters(); ++p)
  {
    xmlpp::Node* parameter_node;
    try
    {
      parameter_node=get_node(parameters_nodeset, "name", c_model->get_parameter_name(p));
    }
    catch (int e)
    {
      std::cerr << " Error: did not find unique node for parameter " << c_model->get_parameter_name(p) << std::endl << std::endl;
      return false;
    }
    double start_value; try { start_value=get_double(parameter_node, "start_value"); } catch (int e) { return false; }
    start_values[p]=start_value;
    parameter_names[p]=c_model->get_parameter_name(p);
    if(bayesian)
    {
      double prior; try { prior=get_double(parameter_node, "prior"); } catch (int e) { return false; }
      priors[p]=prior;
      double sigma; try { sigma=get_positive_double(parameter_node, "prior_width"); } catch (int e) { return false; }
      sigmas[p]=sigma;
    }
  }
  return true;
}
