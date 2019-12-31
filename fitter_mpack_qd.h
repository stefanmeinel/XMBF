#ifndef FITTER_MPACK_QD_H
#define FITTER_MPACK_QD_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <qd/qd_real.h>
#include <mblas_qd.h>
#include <mlapack_qd.h>

#include <cmath>
#include <limits>


#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_errno.h>

#include "abstract_global_model.h"
#include "abstract_prior.h"
#include "chisqr_extra_term.h"

using namespace std;

enum inversion_method
{
  LU_inversion = 0,
  simple_cut = 1,
  ratio_cut = 2,
  absolute_cut = 3,
  diagonal = 4,
  off_diagonal_rescale = 6
};

enum cov_normalization
{
  standard_normalization = 0,
  bootstrap_normalization = 1,
  jackknife_normalization = 2
};

const double default_start_lambda=0.001;
const double default_lambda_fac=10.0;
const bool default_tolerance_dof=true;
const double default_tolerance=0.0001;
const inversion_method default_inversion_method=LU_inversion;
const int default_svd_cut=0;
const double default_svd_ratio=0.000001;
const double default_svd_value=0.000000000001;
const cov_normalization default_cov_normalization=standard_normalization;
const double default_num_diff_step=1e-08;
const double default_off_diagonal_rescale_factor=0.9;

class fitter
{
  public:
    fitter(abstract_global_model* fit_model,
           abstract_prior* fit_prior,
           chisqr_extra_term* fit_extra_term);

    ~fitter();

    void set_inversion_method(inversion_method method);
    void set_svd_cut(int scut);
    void set_svd_cut_ratio(double ratio);
    void set_svd_cut_absolute(double value);
    void set_off_diagonal_rescale_factor(double value);

    void set_data(const vector< vector< double > >& data);
//-----------------------------------------------------------------------------
//    data[n_data_sets][n_fit_points]
//-----------------------------------------------------------------------------

    void set_average_data(const vector< double >& average_data);

    void set_tolerance_dof(bool value);

    void set_cov_normalization(cov_normalization cov_norm);

    void set_parameter_name(int parameter, string name);

    void set_num_diff(bool b);

    void set_num_diff_step(double d);

    void set_second_deriv_minimization(bool b);

    void set_second_deriv_covariance(bool b);

    void set_starting_value(int parameter, double value);

    void set_initial_lambda(double initial_lambda);

    void set_lambda_factor(double lambda_factor);

    void set_tolerance(double dchisqr_tolerance);

    int fit(int max_steps, int verbose_level);

    double get_parameter(int parameter);

    double get_covariance(int parameter_1, int parameter_2);

    bool covariance_positive_definite();

    double get_chi_sqr();

    double get_chi_sqr_start();

    int get_cut();

    void set_n_parameters_dof(int n);

    int get_dof();  // number of parameters NOT subtracted!

    bool test_model();  // call only *after* set_data!

    bool stop;

  private:

    abstract_global_model* model;
    abstract_prior* prior;
    chisqr_extra_term* extra_term;

    bool num_diff;
    bool second_deriv_minimization;
    bool second_deriv_covariance;

    double num_diff_step;

    int n_fit_points;
    int n_parameters;

    double start_lambda;
    double lambda_fac;
    double tolerance;
    bool tolerance_dof;

    cov_normalization cn;

    vector< double > start_vals;

    vector< qd_real > average;

    vector< string > parameter_names;

    qd_real* inv_corr;

    gsl_matrix* result_cov;

    vector< double > result_params;

    vector< double > tmp_params;

    inversion_method inv_method;
    int svd_cut;
    double svd_ratio;
    double svd_value;
    double off_diagonal_rescale_factor;

    int n_parameters_dof;
    int cut;

    double chi_sqr(const vector< double >& params);
    void beta(const vector< double >& params, gsl_vector* result);
    void alpha(const vector< double >& params, double lambda, bool secondderivs, gsl_matrix* result);
};

#endif
