#include "get_constants.h"

bool get_constants(combined_model* c_model,
                   std::vector< std::string >& constant_names,
                   std::vector< double >& constant_values,
                   const xmlpp::Node* root_node)
{
  constant_names.clear(); constant_names.resize(c_model->get_n_constants());
  constant_values.clear(); constant_values.resize(c_model->get_n_constants());

  if( (c_model->get_n_constants())==0 )
  {
    return true;
  }

  xmlpp::NodeSet constant_values_nodeset = root_node->find("constant_values");
  if(constant_values_nodeset.size()!=1)
  {
    std::cerr << "Error: \"constant_values\" did not return unique node" << std::endl << std::endl;
    return false;
  }
  xmlpp::NodeSet constants_nodeset=(*constant_values_nodeset.begin())->find("constant");

  for(int c=0; c<c_model->get_n_constants(); ++c)
  {
    xmlpp::Node* constant_node;
    try
    {
      constant_node=get_node(constants_nodeset, "name", c_model->get_constant_name(c));
    }
    catch (int e)
    {
      std::cerr << " Error: did not find unique node for constant " << c_model->get_constant_name(c) << std::endl << std::endl;
      return false;
    }
    double value; try { value=get_double(constant_node, "value"); } catch (int e) { return false; }
    constant_values[c]=value;
    constant_names[c]=c_model->get_constant_name(c);
  }
  return true;
}
