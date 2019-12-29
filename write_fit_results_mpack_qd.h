#ifndef WRITE_FIT_RESULTS_MPACK_QD_H
#define WRITE_FIT_RESULTS_MPACK_QD_H

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <libxml++/libxml++.h>
#include <boost/lexical_cast.hpp>
#include <gsl/gsl_sf_gamma.h>

#include "fitter_mpack_qd.h"
#include "string_tools.h"
#include "xml_tools.h"

bool write_fit_results(fitter* _fitter, bool bayesian, bool converged, int n_parameters_dof, int steps, const std::vector< std::string >& parameter_names, const std::string& inputfile, const std::string& outputfile);

#endif
