#include "get_chi_sqr_extra_term.h"

bool get_chi_sqr_extra_term(std::string& function,
                            std::vector< std::string >& constant_names,
                            std::vector< double >& constant_values,
                            double& num_diff_step,
                            bool& mixed_second_derivatives_specified,
                            std::vector< std::pair< std::string, std::string > >& mixed_second_derivatives,
                            const xmlpp::Node* root_node)
{

  xmlpp::NodeSet chi_sqr_extra_term_nodeset = root_node->find("chi_sqr_extra_term");
  if(chi_sqr_extra_term_nodeset.size()!=1)
  {
    std::cerr << "Error: \"chi_sqr_extra_term\" did not return unique node" << std::endl << std::endl;
    return false;
  }

  try { function=get_string(*chi_sqr_extra_term_nodeset.begin(), "function"); } catch (int e) { return false; }


  if(name_exists(*chi_sqr_extra_term_nodeset.begin(), "num_diff_step"))
  {
    try { num_diff_step=get_positive_double(*chi_sqr_extra_term_nodeset.begin(), "num_diff_step"); } catch (int e) { return false; }
  }

  constant_names.clear();
  constant_values.clear();
  xmlpp::NodeSet constants_nodeset=(*chi_sqr_extra_term_nodeset.begin())->find("constant");
  for(xmlpp::NodeSet::iterator it=constants_nodeset.begin(); it!=constants_nodeset.end(); ++it)
  {
    std::string name;
    try { name=get_string(*it, "name"); } catch (int e) { return false; }
    double value;
    try { value=get_double(*it, "value"); } catch (int e) { return false; }
    if(std::find(constant_names.begin(), constant_names.end(), name)!=constant_names.end())
    {
      std::cerr << "Error: multiple definitions for chi_sqr_extra_term constant \"" << name << "\"" << std::endl << std::endl;
      return false;
    }
    constant_names.push_back(name);
    constant_values.push_back(value);
  }

  mixed_second_derivatives_specified=false;
  mixed_second_derivatives.clear();
  if(name_exists(*chi_sqr_extra_term_nodeset.begin(), "mixed_second_derivatives"))
  {
    mixed_second_derivatives_specified=true;
    xmlpp::NodeSet mixed_second_derivatives_nodeset = (*chi_sqr_extra_term_nodeset.begin())->find("mixed_second_derivatives");
    if(mixed_second_derivatives_nodeset.size()!=1)
    {
      std::cerr << "Error: \"mixed_second_derivatives\" did not return unique node" << std::endl << std::endl;
      return false;
    }
    xmlpp::NodeSet combinations_nodeset=(*mixed_second_derivatives_nodeset.begin())->find("combination");
    for(xmlpp::NodeSet::iterator combinations_iter=combinations_nodeset.begin(); combinations_iter!=combinations_nodeset.end(); ++combinations_iter)
    {
      std::string name1; try { name1=get_string(*combinations_iter, "name1"); } catch (int e) { return false; }
      std::string name2; try { name2=get_string(*combinations_iter, "name2"); } catch (int e) { return false; }
      std::pair< std::string, std::string > combination(name1, name2);
      mixed_second_derivatives.push_back(combination);
    }
  }
  
  return true;
}
