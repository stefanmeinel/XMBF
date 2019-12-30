#include "fit_mpack_qd.h"


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
         int& steps_needed)
{
  _fitter->set_data(global_fit_data);
  int dof=_fitter->get_dof();
  int cut=_fitter->get_cut();
  if( (dof-cut) > global_fit_data.size() )
  {
    std::cerr << " Error: not enough data sets (after binning) to obtain (pseudo-)inverse of data correlation matrix. Reduce bin size or increase SVD cut" << std::endl << std::endl;
    return false;
  }
  std::cout << "Number of data sets (after binning) is " << global_fit_data.size() << std::endl;
  std::cout << "Kept " << dof-cut << " out of " << dof << " eigenvalues for data correlation matrix." << std::endl << std::endl;
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
    _fitter->set_parameter_name(p, parameter_names[p]);
  }
  _fitter->set_n_parameters_dof(n_parameters_dof);

  if(!(_fitter->test_model()))
  {
    std::cerr << " Error in fit model" << std::endl << std::endl;
    return false;
  }

  steps_needed=_fitter->fit(max_steps, verbose_level);
  return true;
}


void write_data_covariance(const std::string filename,
                           fitter* _fitter)
{
  using namespace std;
  const int precision=10;

  std::ofstream output(filename.c_str());
  if(!output)
  {
    std::cerr << "Error: could not write to file \"" << filename << "\"" << std::endl << std::endl;
    return;
  }

  output.precision(precision);
  output.setf(ios::left);
  
  int n_fit_points=_fitter->get_dof();

  for(int m1=0; m1<n_fit_points; ++m1)
  {
    for(int m2=0; m2<n_fit_points; ++m2)
    {
      output << m1 << "  " << m2 << "  " << _fitter->get_data_covariance(m1, m2) << std::endl;
    }
  }

  output.close();
}




void print_fit_results(fitter* _fitter,
                       bool bayesian,
                       const std::vector< std::string >& parameter_names,
                       const std::vector< double >& start_values,
                       const std::vector< double >& priors,
                       const std::vector< double >& sigmas,
                       int n_parameters_dof)
{
  using namespace std;
  const int precision=10;
  std::cout.precision(precision);
  std::cout.setf(ios::left);

  int max_parameter_length=1;
  int max_start_values_length=1;
  int max_priors_length=1;
  int max_sigmas_length=1;
  for(int p=0; p<parameter_names.size(); ++p)
  {
    if(parameter_names[p].length()>max_parameter_length)
    {
      max_parameter_length=parameter_names[p].length();
    }
    std::stringstream start_values_st;
    start_values_st.precision(precision);
    start_values_st << start_values[p];
    if(start_values_st.str().length()>max_start_values_length)
    {
      max_start_values_length=start_values_st.str().length();
    }
    std::stringstream priors_st;
    priors_st.precision(precision);
    priors_st << priors[p];
    if(priors_st.str().length()>max_priors_length)
    {
      max_priors_length=priors_st.str().length();
    }
    std::stringstream sigmas_st;
    sigmas_st.precision(precision);
    sigmas_st << sigmas[p];
    if(sigmas_st.str().length()>max_sigmas_length)
    {
      max_sigmas_length=sigmas_st.str().length();
    }
  }
  std::cout <<   "     Parameter  " << repeat_string(max(max_parameter_length-9, 0), " ") << "Start  " << repeat_string(max(max_start_values_length-5, 0), " ") << "Prior  " << repeat_string(max(max_priors_length-5, 0), " ") << "Width  " << repeat_string(max(max_sigmas_length-5, 0), " ") << "Fit result        sqrt(cov)" << std::endl;
  std::cout <<   "----------------" << repeat_string(max(max_parameter_length-9, 0), "-") << "-------" << repeat_string(max(max_start_values_length-5, 0), "-") << "-------" << repeat_string(max(max_priors_length-5, 0), "-") << "-------" << repeat_string(max(max_sigmas_length-5, 0), "-") << "---------------------------------" << std::endl;
  for(int p=0; p<parameter_names.size(); ++p)
  {
    std::cout << ">>>  " << setw(max(2+max_parameter_length,11)) << parameter_names[p];
    std::cout << setw(max(2+max_start_values_length,7)) << start_values[p];
    if(bayesian)
    {
      std::cout << setw(max(2+max_priors_length,7)) << priors[p]
           << setw(max(2+max_sigmas_length,7)) << sigmas[p];
    }
    else
    {
      std::cout << "              ";
    }
    std::cout << setw(16) << _fitter->get_parameter(p);
    std::cout << "  " << setw(16) << sqrt(_fitter->get_covariance(p, p));
    std::cout << std::endl;
  }
  std::cout << resetiosflags(ios::left);
  std::cout << setw(0);
  std::cout << std::endl;
  double chisqr=_fitter->get_chi_sqr();
  double dof=_fitter->get_dof()-_fitter->get_cut()-n_parameters_dof;
  if(dof<0.0)
  {
    dof=0.0;
  }
  std::cout << "Effective dof    = " << dof << std::endl;
  std::cout << "chi^2/dof        = " << chisqr/dof << std::endl;
  std::cout << "Q(dof/2,chi^2/2) = " << gsl_sf_gamma_inc_Q(dof/2.0, chisqr/2.0) << std::endl << std::endl;
}


void write_covariance(const std::string filename,
                      fitter* _fitter,
                      const std::vector< std::string >& parameter_names)
{
  using namespace std;
  const int precision=10;

  std::ofstream output(filename.c_str());
  if(!output)
  {
    std::cerr << "Error: could not write to file \"" << filename << "\"" << std::endl << std::endl;
    return;
  }

  output.precision(precision);
  output.setf(ios::left);

  int max_length=1;
  for(int p1=0; p1<parameter_names.size(); ++p1)
  {
    if(parameter_names[p1].length()>max_length)
    {
      max_length=parameter_names[p1].length();
    }
  }

  for(int p1=0; p1<parameter_names.size(); ++p1)
  {
    for(int p2=0; p2<parameter_names.size(); ++p2)
    {
      output << setw(max_length+4) << parameter_names[p1] << setw(max_length+4) << parameter_names[p2] << setw(0) << _fitter->get_covariance(p1, p2) << std::endl;
    }
  }

  output.close();
}


void write_results(const std::string filename,
                   fitter* _fitter,
                   const std::vector< std::string >& parameter_names)
{
  using namespace std;
  const int precision=10;

  std::ofstream output(filename.c_str());
  if(!output)
  {
    std::cerr << "Error: could not write to file \"" << filename << "\"" << std::endl << std::endl;
    return;
  }

  output.precision(precision);
  output.setf(ios::left);

  int max_length=1;
  for(int p1=0; p1<parameter_names.size(); ++p1)
  {
    if(parameter_names[p1].length()>max_length)
    {
      max_length=parameter_names[p1].length();
    }
  }

  for(int p1=0; p1<parameter_names.size(); ++p1)
  {
    output << setw(max_length+4) << parameter_names[p1] << setw(0) << _fitter->get_parameter(p1) << std::endl;
  }

  output.close();
}


void write_results_errors(const std::string filename,
                          fitter* _fitter,
                          const std::vector< std::string >& parameter_names)
{
  using namespace std;
  const int precision=10;

  std::ofstream output(filename.c_str());
  if(!output)
  {
    std::cerr << "Error: could not write to file \"" << filename << "\"" << std::endl << std::endl;
    return;
  }

  output.precision(precision);
  output.setf(ios::left);

  int max_length=1;
  for(int p1=0; p1<parameter_names.size(); ++p1)
  {
    if(parameter_names[p1].length()>max_length)
    {
      max_length=parameter_names[p1].length();
    }
  }

  for(int p1=0; p1<parameter_names.size(); ++p1)
  {
    output << setw(max_length+4) << parameter_names[p1] << setw(0) << _fitter->get_parameter(p1) << "  " << sqrt(_fitter->get_covariance(p1, p1)) << std::endl;
  }

  output.close();
}
