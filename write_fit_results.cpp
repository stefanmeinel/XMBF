#include "write_fit_results.h"


bool write_fit_results(fitter* _fitter, bool bayesian, bool converged, int n_parameters_dof, int steps_needed, const std::vector< std::string >& parameter_names, const std::string& inputfile, const std::string& outputfile)
{
  #ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
  try
  {
  #endif //LIBXMLCPP_EXCEPTIONS_ENABLED
    xmlpp::Document document;

    //foo is the default namespace prefix.
    xmlpp::Element* root = document.create_root_node("fit_results");

    addchild(root, "input_file", inputfile);

    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    std::string time_string=asctime(timeinfo);
    remove_newlines(time_string);
    addchild(root, "time", time_string);

    addchild(root, "fit_converged", bool_to_string(converged));

    addchild(root, "iterations", steps_needed);

    double chisqr=_fitter->get_chi_sqr();
    double dof=_fitter->get_dof()-_fitter->get_cut()-n_parameters_dof;
    if(dof<0.0)
    {
      dof=0.0;
    }

    addchild(root, "dof", dof);

    addchild(root, "chi_sqr_per_dof", chisqr/dof);

    addchild(root, "Q", gsl_sf_gamma_inc_Q(dof/2.0, chisqr/2.0));

    xmlpp::Element* parameters_node = root->add_child("parameter_values");

    for(int p=0; p<parameter_names.size(); ++p)
    {
      xmlpp::Element* parameter_node = parameters_node->add_child("parameter");

      addchild(parameter_node, "name", parameter_names[p]);

      addchild(parameter_node, "result", _fitter->get_parameter(p));

      addchild(parameter_node, "error", sqrt(_fitter->get_covariance(p, p)));
    }

    document.write_to_file_formatted(outputfile);
  #ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
  }
  catch(const std::exception& ex)
  {
    std::cout << "Exception caught: " << ex.what() << std::endl;
    return false;
  }
  #endif //LIBXMLCPP_EXCEPTIONS_ENABLED
  return true;
}
