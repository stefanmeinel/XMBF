#ifndef BOOTSTRAP_MPI_H
#define BOOTSTRAP_MPI_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#include "fitter.h"
#include "gaussian_prior.h"
#include "chisqr_extra_term.h"
#include "string_tools.h"
#include "prepare_fit_data.h"
#include "abstract_model.h"
#include "combined_model.h"
#include "parser.h"

bool bootstrap(int proc,
               fitter* _fitter,
               bool bayesian,
               gaussian_prior* _gaussian_prior,
               const std::vector< std::vector< double > >& global_fit_data,
               const std::vector< std::string >& parameter_names,
               const std::vector< double >& start_values,
               const std::vector< double >& priors,
               const std::vector< double >& sigmas,
               int n_parameters_dof,
               int max_steps,
               bool random_priors,
               int bootstrap_samples,
               bool use_bse_file,
               int* bse_config,
               int boot_min,
               int boot_max,
               std::vector< std::vector< double > >& bootstrap_results,
               std::vector< int >& all_steps_needed,
               std::vector< double >& all_chisqr);


bool bootstrap_with_range_change(int proc,
                                 combined_model* c_model,
                                 std::vector<abstract_model*> models,
                                 const std::vector< double >& constant_values,
                                 const std::vector< std::string >& constant_names,
                                 int bin_size, bool restrict_data, int start_n_data, int stop_n_data,
                                 const std::vector< std::vector< std::vector< std::string > > >& fit_min,
                                 const std::vector< std::vector< std::vector< std::string > > >& fit_max,
                                 const std::vector< std::vector< std::vector< bool > > >& has_fit_step,
                                 const std::vector< std::vector< std::vector< double > > >& fit_step,
                                 const std::vector< std::vector< std::vector< bool > > >& has_range_bootstrap_file,
                                 const std::vector< std::vector< std::vector< std::string > > >& range_bootstrap_file,
                                 const std::vector< std::vector< std::vector< std::vector< double > > > >& all_file_data,
                                 const std::vector< std::vector< std::vector< double > > >& all_file_arguments,
                                 fitter* _fitter,
                                 bool num_diff,
                                 bool second_deriv_covariance,
                                 bool second_deriv_minimization,
                                 double num_diff_step,
                                 double start_lambda,
                                 double lambda_factor,
                                 double chi_sqr_tolerance,
                                 bool chi_sqr_tolerance_dof,
                                 inversion_method inv_method,
                                 int svd_fixed_cut,
                                 double svd_ratio_cut,
                                 double svd_absolute_cut,
                                 bool bootstrap_normalization,
                                 bool bayesian,
                                 gaussian_prior* _gaussian_prior,
                                 chisqr_extra_term* _chisqr_extra_term,
                                 const std::vector< std::string >& parameter_names,
                                 const std::vector< double >& start_values,
                                 const std::vector< double >& priors,
                                 const std::vector< double >& sigmas,
                                 int n_parameters_dof,
                                 int max_steps,
                                 bool random_priors,
                                 int bootstrap_samples,
                                 bool use_bse_file,
                                 int* bse_config,
                                 int boot_min,
                                 int boot_max,
                                 std::vector< std::vector< double > >& bootstrap_results,
                                 std::vector< int >& all_steps_needed,
                                 std::vector< double >& all_chisqr);


#endif
