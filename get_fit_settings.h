#ifndef GET_FIT_SETTINGS_H
#define GET_FIT_SETTINGS_H

#include <vector>
#include <string>
#include <iostream>
#include <libxml++/libxml++.h>
#include "xml_tools.h"
#include "fitter.h"

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
                      const xmlpp::Node* root_node);

#endif
