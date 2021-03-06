#ifndef MULTIFIT_MPI_H
#define MULTIFIT_MPI_H

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

bool multifit(fitter* _fitter,
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
              int multifit_min,
              int multifit_max,
              std::vector< std::vector< double > >& multifit_results,
              std::vector< int >& all_steps_needed,
              std::vector< double >& all_chisqr);

#endif
