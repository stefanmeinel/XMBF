#include "multifit.h"

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
              const std::string& output_dir,
              const std::string& xml_filename)
{

  gsl_rng_default_seed=0;
  gsl_rng_env_setup();
  const gsl_rng_type* Trng=gsl_rng_mt19937;
  gsl_rng* rng=gsl_rng_alloc(Trng);

  std::vector< std::ofstream* > all_output(parameter_names.size());
  for(int p=0; p<parameter_names.size(); ++p)
  {
    std::string filename;
    filename=output_dir+xml_filename.substr(xml_filename.find_last_of( '/' )+1)+"_multifit_"+parameter_names[p]+".dat";
    std::ofstream* output = new std::ofstream(filename.c_str());
    if(!(*output))
    {
      std::cerr << "Cannot write file "  << filename << std::endl << std::endl;
      return false;
    }
    output -> precision(15);
    all_output[p]=output;
  }

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

  std::cout << "Performing multifit with " << global_fit_data.size() << " samples." << std::endl;
  std::cout << "Output directory is: " << output_dir << std::endl << std::endl;

  _fitter->set_data(global_fit_data);

  std::vector< double > multifit_results_tmp(parameter_names.size());
  std::vector< std::vector< double > > multifit_results(global_fit_data.size(), multifit_results_tmp);

  for(int n=0; n<global_fit_data.size(); ++n)
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
    if(steps_needed==max_steps+1)
    {
      std::cout << "WARNING: multifit #" << n+1 << " did not converge after " << max_steps << " iterations. " << "chi^2/dof = " << chisqr/dof << std::endl;
    }
    else
    {
      std::cout << "multifit #" << n+1 << " converged after " << steps_needed << " iterations. " << "chi^2/dof = " << chisqr/dof << std::endl;
    }
    for(int p=0; p<parameter_names.size(); ++p)
    {
      multifit_results[n][p] = _fitter->get_parameter(p);
      *all_output[p] << multifit_results[n][p] << std::endl;
    }
  }
  gsl_rng_free(rng);
  std::cout << std::endl << "Multifit completed." << std::endl << std::endl;
  for(int p=0; p<parameter_names.size(); ++p)
  {
    all_output[p] -> close();
    delete all_output[p];
  }


  std::vector< double >multifit_av(parameter_names.size(), 0.0);
  std::vector< double >multifit_sigma(parameter_names.size(), 0.0);

  for(int p=0; p<parameter_names.size(); ++p)
  {
    std::vector< double > range_temp(global_fit_data.size(), 0.0);
    for(int n=0; n<global_fit_data.size(); ++n)
    {
      range_temp[n]=multifit_results[n][p];
    }

    std::stable_sort(range_temp.begin(), range_temp.end());

    int min_pos=static_cast<int>(0.158655*(global_fit_data.size()));
    int max_pos=static_cast<int>(0.841345*(global_fit_data.size()));

    for(int pos=min_pos; pos<max_pos; ++pos)
    {
      multifit_av[p]+=range_temp[pos];
    }
    multifit_av[p]/=double(max_pos-min_pos);

    multifit_sigma[p]=(range_temp[max_pos]-range_temp[min_pos])/2.0;
  }

  const int precision=10;
  std::cout.precision(precision);
  std::cout.setf(ios::left);

  int max_parameter_length=1;
  for(int p=0; p<parameter_names.size(); ++p)
  {
    if(parameter_names[p].length()>max_parameter_length)
    {
      max_parameter_length=parameter_names[p].length();
    }
  }
  std::cout <<   "Parameter  " << repeat_string(max(max_parameter_length-4, 0), " ") << "Average         68% width"      << std::endl;
  std::cout <<   "-----------" << repeat_string(max(max_parameter_length-4, 0), "-") << "-------------------------------" << std::endl;
  for(int p=0; p<parameter_names.size(); ++p)
  {
    std::cout << setw(max(2+max_parameter_length,11)) << parameter_names[p];
    std::cout << setw(16) << multifit_av[p];
    std::cout << setw(16) << multifit_sigma[p];
    std::cout << std::endl;
  }
  std::cout << resetiosflags(ios::left);
  std::cout << setw(0);
  std::cout << std::endl;

  return true;
}
