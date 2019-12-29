#include <iostream>
#include <algorithm>
#include <string>

#include <libxml++/libxml++.h>
#include <boost/lexical_cast.hpp>

#include "xml_tools.h"


void print_usage()
{
  std::cout << "usage: XMBF_results_to_startvals_and_priors results.xml input.xml target.xml" << std::endl << std::endl;
  std::cout << std::endl;
}

int main(int argc, char *argv[])
{
  if(argc!=4)
  {
    print_usage();
    return 1;
  }
  std::string resultsfile=argv[1];
  std::string inputfile=argv[2];
  std::string targetfile=argv[3];

  #ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
  try
  {
  #endif //LIBXMLCPP_EXCEPTIONS_ENABLED
    xmlpp::DomParser results_parser(resultsfile);
    xmlpp::DomParser input_parser(inputfile);
    if(results_parser && input_parser)
    {
      const xmlpp::Node* results_root = results_parser.get_document()->get_root_node(); //deleted by DomParser.
      const xmlpp::Node* input_root = input_parser.get_document()->get_root_node(); //deleted by DomParser.
      if(results_root && input_root)
      {

        xmlpp::NodeSet results_parameter_values_nodeset = results_root->find("parameter_values");
        if(results_parameter_values_nodeset.size()!=1)
        {
          std::cerr << "Error: " << resultsfile << " : \"parameter_values\" did not return unique node" << std::endl << std::endl;
          return 1;
        }
        xmlpp::NodeSet input_parameter_values_nodeset = input_root->find("parameter_values");
        if(input_parameter_values_nodeset.size()!=1)
        {
          std::cerr << "Error: " << inputfile << " : \"parameter_values\" did not return unique node" << std::endl << std::endl;
          return 1;
        }
        xmlpp::NodeSet results_parameters_nodeset=(*results_parameter_values_nodeset.begin())->find("parameter");
        xmlpp::NodeSet input_parameters_nodeset=(*input_parameter_values_nodeset.begin())->find("parameter");
        for(xmlpp::NodeSet::iterator ipns_it=results_parameters_nodeset.begin(); ipns_it!=results_parameters_nodeset.end(); ++ipns_it)
        {
          std::string parameter_name;
          double result;
          try
          {
            parameter_name=get_string(*ipns_it, "name");
            result=get_double(*ipns_it, "result");
          }
          catch (int e) { goto end_of_loop; }

          xmlpp::Node* input_parameter_node;
          try
          {
            input_parameter_node=get_node(input_parameters_nodeset, "name", parameter_name);
          }
          catch (int e) { goto end_of_loop; }
          try
          {
            set_double(input_parameter_node, "start_value", result, "    ", false);
            set_double(input_parameter_node, "prior", result, "    ", true);
          }
          catch (int e) { goto end_of_loop; }
          end_of_loop: ;
        }
        xmlpp::Document* doc=input_parser.get_document();
        doc -> write_to_file(targetfile);
      }
    }
  #ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
  }
  catch(const std::exception& ex)
  {
    std::cout << "XML exception caught : " << ex.what() << std::endl << std::endl;
    return 1;
  }
  #endif //LIBXMLCPP_EXCEPTIONS_ENABLED
}
