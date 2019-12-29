#include "get_fit_settings_mpack_qd.h"

bool get_fit_settings(bool bootstrap,
                      bool& restrict_data_range,
                      int& data_range_min,
                      int& data_range_max,
                      bool& bayesian,
                      bool& chi_sqr_extra_term,
                      bool& random_priors,
                      bool& num_diff,
                      bool& second_deriv_covariance,
                      bool& second_deriv_minimization,
                      double& num_diff_step,
                      double& start_lambda,
                      double& lambda_factor,
                      double& chi_sqr_tolerance,
                      bool& chi_sqr_tolerance_dof,
                      int& n_parameters_dof,
                      inversion_method& inv_method,
                      int& svd_fixed_cut,
                      double& svd_ratio_cut,
                      double& svd_absolute_cut,
                      int& max_iterations,
                      int& bin_size,
                      int& bootstrap_samples,
                      bool& use_bse_file,
                      std::string& bse_file,
                      bool& restrict_bootstrap_range,
                      int& bootstrap_range_min,
                      int& bootstrap_range_max,
                      bool& bootstrap_normalization,
                      const xmlpp::Node* root_node)
{
  xmlpp::NodeSet fitsettings_nodeset = root_node->find("fit_settings");
  if(fitsettings_nodeset.size()!=1)
  {
    std::cerr << "Error: \"fitsettings\" did not return unique node" << std::endl << std::endl;
    return false;
  }
  try { restrict_data_range=get_bool(*fitsettings_nodeset.begin(), "restrict_data_range"); } catch (int e) { return false; }
  if(restrict_data_range)
  {
    try { data_range_min=get_positive_int(*fitsettings_nodeset.begin(), "data_range_min"); } catch (int e) { return false; }
    try { data_range_max=get_positive_int(*fitsettings_nodeset.begin(), "data_range_max"); } catch (int e) { return false; }
  }
  try { bayesian=get_bool(*fitsettings_nodeset.begin(), "bayesian"); } catch (int e) { return false; }

  chi_sqr_extra_term=false;
  if(name_exists(*fitsettings_nodeset.begin(), "chi_sqr_extra_term_enabled"))
  {
    try { chi_sqr_extra_term=get_bool(*fitsettings_nodeset.begin(), "chi_sqr_extra_term_enabled"); } catch (int e) { return false; }
  }

  try { start_lambda=get_positive_double(*fitsettings_nodeset.begin(), "start_lambda"); } catch (int e) { return false; }
  try { lambda_factor=get_positive_double(*fitsettings_nodeset.begin(), "lambda_factor"); } catch (int e) { return false; }

  chi_sqr_tolerance_dof=false;
  try { chi_sqr_tolerance_dof=get_bool(*fitsettings_nodeset.begin(), "chi_sqr_per_dof_tolerance"); } catch (int e) { return false; }

  n_parameters_dof=-1;
  if(name_exists(*fitsettings_nodeset.begin(), "n_parameters_dof"))
  {
    try { n_parameters_dof=get_non_negative_int(*fitsettings_nodeset.begin(), "n_parameters_dof"); } catch (int e) { return false; }
  }

  try { chi_sqr_tolerance=get_positive_double(*fitsettings_nodeset.begin(), "chi_sqr_tolerance"); } catch (int e) { return false; }
  std::string inv_method_str; try { inv_method_str=get_string(*fitsettings_nodeset.begin(), "inversion_method"); } catch (int e) { return false; }
  if(inv_method_str=="LU")
  {
    inv_method=LU_inversion;
  }
  else if(inv_method_str=="diagonal")
  {
    inv_method=diagonal;
  }
  else if(inv_method_str=="svd_fixed_cut")
  {
    inv_method=simple_cut;
    try { svd_fixed_cut=get_non_negative_int(*fitsettings_nodeset.begin(), "svd_fixed_cut"); } catch (int e) { return false; }
  }
  else if(inv_method_str=="svd_ratio_cut")
  {
    inv_method=ratio_cut;
    try { svd_ratio_cut=get_positive_double(*fitsettings_nodeset.begin(), "svd_ratio_cut"); } catch (int e) { return false; }
  }
  else if(inv_method_str=="svd_absolute_cut")
  {
    inv_method=absolute_cut;
    try { svd_absolute_cut=get_positive_double(*fitsettings_nodeset.begin(), "svd_absolute_cut"); } catch (int e) { return false; }
  }
  else
  {
    std::cerr << "Error: unknown inversion method: \"" << inv_method_str << "\"" << std::endl << std::endl;
    return false;
  }
  try { max_iterations=get_positive_int(*fitsettings_nodeset.begin(), "max_iterations"); } catch (int e) { return false; }
  try { bin_size=get_positive_int(*fitsettings_nodeset.begin(), "bin_size"); } catch (int e) { return false; }
  restrict_bootstrap_range=false;
  if(bootstrap)
  {
    if(bayesian)
    {
      try { random_priors=get_bool(*fitsettings_nodeset.begin(), "random_priors"); } catch (int e) { return false; }
    }
    try { bootstrap_samples=get_positive_int(*fitsettings_nodeset.begin(), "bootstrap_samples"); } catch (int e) { return false; }
    try { use_bse_file=get_bool(*fitsettings_nodeset.begin(), "use_bse_file"); } catch (int e) { return false; }
    if(use_bse_file)
    {
      try { bse_file=get_string(*fitsettings_nodeset.begin(), "bse_file"); } catch (int e) { return false; }
    }
    if(name_exists(*fitsettings_nodeset.begin(), "restrict_bootstrap_range"))
    {
      try { restrict_bootstrap_range=get_bool(*fitsettings_nodeset.begin(), "restrict_bootstrap_range"); } catch (int e) { return false; }
      if(restrict_bootstrap_range)
      {
        try { bootstrap_range_min=get_positive_int(*fitsettings_nodeset.begin(), "bootstrap_range_min"); } catch (int e) { return false; }
        try { bootstrap_range_max=get_positive_int(*fitsettings_nodeset.begin(), "bootstrap_range_max"); } catch (int e) { return false; }
      }
    }
  }
  bootstrap_normalization=false;
  if(name_exists(*fitsettings_nodeset.begin(), "bootstrap_normalization"))
  {
    try { bootstrap_normalization=get_bool(*fitsettings_nodeset.begin(), "bootstrap_normalization"); } catch (int e) { return false; }
  }
  if(bootstrap_normalization)
  {
    if(bin_size!=1)
    {
      std::cerr << "Warning: setting bin size to 1 because of bootstrap_normalization" << std::endl;
      bin_size=1;
    }
  }



  num_diff=false;
  if(name_exists(*fitsettings_nodeset.begin(), "num_diff_first_order"))
  {
    try { num_diff=get_bool(*fitsettings_nodeset.begin(), "num_diff_first_order"); } catch (int e) { return false; }
  }

  second_deriv_covariance=true;
  if(name_exists(*fitsettings_nodeset.begin(), "second_deriv_covariance"))
  {
    try { second_deriv_covariance=get_bool(*fitsettings_nodeset.begin(), "second_deriv_covariance"); } catch (int e) { return false; }
  }

  second_deriv_minimization=false;
  if(name_exists(*fitsettings_nodeset.begin(), "second_deriv_minimization"))
  {
    try { second_deriv_minimization=get_bool(*fitsettings_nodeset.begin(), "second_deriv_minimization"); } catch (int e) { return false; }
  }

  if(num_diff || second_deriv_covariance || second_deriv_minimization)
  {
    try { num_diff_step=get_positive_double(*fitsettings_nodeset.begin(), "num_diff_step"); } catch (int e) { return false; }
  }

  return true;
}
