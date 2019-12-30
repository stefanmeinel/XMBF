#ifndef FIT_MPACK_QD_H
#define FIT_MPACK_QD_H

#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <string>
#include <algorithm>
#include <gsl/gsl_sf_gamma.h>

#include "fitter_mpack_qd.h"
#include "gaussian_prior.h"
#include "string_tools.h"

bool fit(fitter* _fitter,
         bool bayesian,
         gaussian_prior* _gaussian_prior,
         const std::vector< std::vector< double > >& global_fit_data,
         const std::vector< std::string >& parameter_names,
         const std::vector< double >& start_values,
         const std::vector< double >& priors,
         const std::vector< double >& sigmas,
         int n_parameters_dof,
         int max_steps,
         int verbose_level,
         int& steps_needed);

void write_data_covariance(const std::string filename,
                           fitter* _fitter);

void print_fit_results(fitter* _fitter,
                       bool bayesian,
                       const std::vector< std::string >& parameter_names,
                       const std::vector< double >& start_values,
                       const std::vector< double >& priors,
                       const std::vector< double >& sigmas,
                       int n_parameters_dof);

void write_covariance(const std::string filename,
                      fitter* _fitter,
                      const std::vector< std::string >& parameter_names);

void write_results(const std::string filename,
                   fitter* _fitter,
                   const std::vector< std::string >& parameter_names);

void write_results_errors(const std::string filename,
                          fitter* _fitter,
                          const std::vector< std::string >& parameter_names);


#endif
