#include "multifit_mpi.h"

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
              std::vector< double >& all_chisqr)
{

  gsl_rng_default_seed=0;
  gsl_rng_env_setup();
  const gsl_rng_type* Trng=gsl_rng_mt19937;
  gsl_rng* rng=gsl_rng_alloc(Trng);

  if(bayesian)
  {
    _gaussian_prior->set_enabled(true);
    for(int p=0; p<priors.size(); ++p)
    {
      _gaussian_prior->set_prior(p, priors[p]);
      _gaussian_prior->set_sigma(p, sigmas[p]);
    }
  }
  else
  {
    _gaussian_prior->set_enabled(false);
  }
  for(int p=0; p<start_values.size(); ++p)
  {
    _fitter->set_starting_value(p, start_values[p]);
  }

  // skip random numbers for priors
  if( random_priors && bayesian)
  {
    for(int n=0; n<multifit_min; ++n)
    {
      for(int p=0; p<parameter_names.size(); ++p)
      {
        _gaussian_prior->set_prior(p, priors[p]+gsl_ran_gaussian(rng, sigmas[p]));
      }
    }
  }

  _fitter->set_data(global_fit_data);

  for(int n=multifit_min; n<=multifit_max; ++n)
  {

    _fitter->set_average_data(global_fit_data[n]);

    if(random_priors && bayesian)
    {
      for(int p=0; p<parameter_names.size(); ++p)
      {
        _gaussian_prior->set_prior(p, priors[p]+gsl_ran_gaussian(rng, sigmas[p]));
      }
    }
    int steps_needed=_fitter->fit(max_steps, 0);
    double chisqr=_fitter->get_chi_sqr();
    double dof=_fitter->get_dof()-_fitter->get_cut()-n_parameters_dof;
    if(dof<0.0)
    {
      dof=0.0;
    }
    all_chisqr[n]=chisqr/dof;
    all_steps_needed[n]=steps_needed;
    for(int p=0; p<parameter_names.size(); ++p)
    {
      multifit_results[n][p] = _fitter->get_parameter(p);
    }
  }
  gsl_rng_free(rng);

  return true;
}
