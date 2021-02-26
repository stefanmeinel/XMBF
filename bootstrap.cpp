#include "bootstrap.h"

bool bootstrap(fitter* _fitter,
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
               const std::string& bse_file,
               bool restrict_bootstrap_range,
               int bootstrap_range_min,
               int bootstrap_range_max,
               const std::string& output_dir,
               const std::string& xml_filename)
{
  std::vector< std::vector< double > > bootstrap_data=global_fit_data;

  gsl_rng_default_seed=0;
  gsl_rng_env_setup();
  const gsl_rng_type* Trng=gsl_rng_mt19937;
  gsl_rng* rng=gsl_rng_alloc(Trng);

  std::vector< int > bootstrap_config_tmp(global_fit_data.size(), 0);
  std::vector< std::vector< int > > bootstrap_config;

  if(use_bse_file)
  {
    std::ifstream bse(bse_file.c_str());
    if(!bse)
    {
      std::cerr << "Error: could not open bootstrap ensemble file " << bse_file << std::endl << std::endl;
      return false;
    }
    bse >> bootstrap_samples;
    int bse_n_data_sets=0;
    bse >> bse_n_data_sets;
    if(bse_n_data_sets!=global_fit_data.size())
    {
      std::cerr << "Error: bootstrap ensemble file has wrong number of data sets" << std::endl << std::endl;
      return false;
    }
    bootstrap_config.resize(bootstrap_samples, bootstrap_config_tmp);
    for(int boot=0; boot<bootstrap_samples; ++boot)
    {
      for(unsigned int b=0; b<global_fit_data.size(); ++b)
      {
        bse >> bootstrap_config[boot][b];
        bootstrap_config[boot][b]-=1;
      }
    }
    bse.close();
  }
  else
  {
    bootstrap_config.resize(bootstrap_samples, bootstrap_config_tmp);
    for(int boot=0; boot<bootstrap_samples; ++boot)
    {
      for(unsigned int b=0; b<global_fit_data.size(); ++b)
      {
        bootstrap_config[boot][b]=static_cast<int>(gsl_rng_uniform(rng)*global_fit_data.size());
      }
    }
  }

  std::vector< std::ofstream* > all_output(parameter_names.size());
  for(int p=0; p<parameter_names.size(); ++p)
  {
    std::string filename;
    filename=output_dir+xml_filename.substr(xml_filename.find_last_of( '/' )+1)+"_bootstrap_"+parameter_names[p]+".dat";
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

  std::cout << "Performing bootstrap with " << bootstrap_samples << " samples." << std::endl;
  std::cout << "Output directory is: " << output_dir << std::endl << std::endl;

  std::vector< double > bootstrap_results_tmp(parameter_names.size());
  std::vector< std::vector< double > > bootstrap_results(bootstrap_samples, bootstrap_results_tmp);

  int boot_min=0;
  int boot_max=bootstrap_samples-1;

  if(restrict_bootstrap_range)
  {
    if( (bootstrap_range_max<bootstrap_range_min) || (bootstrap_range_max>bootstrap_samples) )
    {
      std::cerr << "Error: bootstrap range incorrect"  << std::endl << std::endl;
      return false;
    }
    boot_min=bootstrap_range_min-1;
    boot_max=bootstrap_range_max-1;

    // skip random numbers for priors
    if( random_priors && bayesian)
    {
      for(int boot=0; boot<boot_min; ++boot)
      {
        for(int p=0; p<parameter_names.size(); ++p)
        {
          _gaussian_prior->set_prior(p, priors[p]+gsl_ran_gaussian(rng, sigmas[p]));
        }
      }
    }
  }

  for(int boot=boot_min; boot<=boot_max; ++boot)
  {
    for(unsigned int b=0; b<global_fit_data.size(); ++b)
    {
      int r=bootstrap_config[boot][b];
      bootstrap_data[b]=global_fit_data[r];
    }

    _fitter->set_data(bootstrap_data);

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
      std::cout << "WARNING: Bootstrap fit #" << boot+1 << " did not converge after " << max_steps << " iterations. " << "chi^2/dof = " << chisqr/dof << std::endl;
    }
    else
    {
      std::cout << "Bootstrap fit #" << boot+1 << " converged after " << steps_needed << " iterations. " << "chi^2/dof = " << chisqr/dof << std::endl;
    }
    for(int p=0; p<parameter_names.size(); ++p)
    {
      bootstrap_results[boot][p] = _fitter->get_parameter(p);
      *all_output[p] << bootstrap_results[boot][p] << std::endl;
    }
  }
  gsl_rng_free(rng);
  std::cout << std::endl << "Bootstrap completed." << std::endl << std::endl;
  for(int p=0; p<parameter_names.size(); ++p)
  {
    all_output[p] -> close();
    delete all_output[p];
  }


  std::vector< double >boot_av(parameter_names.size(), 0.0);
  std::vector< double >boot_sigma(parameter_names.size(), 0.0);

  for(int p=0; p<parameter_names.size(); ++p)
  {
    std::vector< double > range_temp(boot_max-boot_min+1, 0.0);
    for(int boot=boot_min; boot<=boot_max; ++boot)
    {
      range_temp[boot-boot_min]=bootstrap_results[boot][p];
    }

    std::stable_sort(range_temp.begin(), range_temp.end());
    
    int min_pos=static_cast<int>(0.158655*(boot_max-boot_min+1));
    int max_pos=static_cast<int>(0.841345*(boot_max-boot_min+1));
    
    for(int pos=min_pos; pos<max_pos; ++pos)
    {
      boot_av[p]+=range_temp[pos];
    }
    boot_av[p]/=double(max_pos-min_pos);
    
    boot_sigma[p]=(range_temp[max_pos]-range_temp[min_pos])/2.0;
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
    std::cout << setw(16) << boot_av[p];
    std::cout << setw(16) << boot_sigma[p];
    std::cout << std::endl;
  }
  std::cout << resetiosflags(ios::left);
  std::cout << setw(0);
  std::cout << std::endl;

  return true;
}









bool bootstrap_with_range_change(combined_model* c_model,
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
                                 double off_diagonal_rescale_factor,
                                 cov_normalization cn,
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
                                 const std::string& bse_file,
                                 bool restrict_bootstrap_range,
                                 int bootstrap_range_min,
                                 int bootstrap_range_max,
                                 const std::string& output_dir,
                                 const std::string& xml_filename)
{
  int n_data_sets=all_file_data[0].size();

  int fit_n_data_sets;
  if(restrict_data)
  {
    fit_n_data_sets=stop_n_data-start_n_data+1;
    if (stop_n_data>n_data_sets)
    {
      std::cerr << "Error: range of data sets exceeds data file" << std::endl << std::endl;
      return false;
    }
    if (fit_n_data_sets<5)
    {
      std::cerr << "Error: fewer than 5 data samples" << std::endl << std::endl;
      return false;
    }
  }
  else
  {
    start_n_data=1;
    stop_n_data=n_data_sets;
    fit_n_data_sets=n_data_sets;
  }

  gsl_rng_default_seed=0;
  gsl_rng_env_setup();
  const gsl_rng_type* Trng=gsl_rng_mt19937;
  gsl_rng* rng=gsl_rng_alloc(Trng);

  std::vector< int > bootstrap_config_tmp(fit_n_data_sets, 0);
  std::vector< std::vector< int > > bootstrap_config;

  if(use_bse_file)
  {
    std::ifstream bse(bse_file.c_str());
    if(!bse)
    {
      std::cerr << "Error: could not open bootstrap ensemble file " << bse_file << std::endl << std::endl;
      return false;
    }
    bse >> bootstrap_samples;
    int bse_n_data_sets=0;
    bse >> bse_n_data_sets;
    if(bse_n_data_sets!=fit_n_data_sets)
    {
      std::cerr << "Error: bootstrap ensemble file has wrong number of data sets" << std::endl << std::endl;
      return false;
    }
    bootstrap_config.resize(bootstrap_samples, bootstrap_config_tmp);
    for(int boot=0; boot<bootstrap_samples; ++boot)
    {
      for(unsigned int b=0; b<fit_n_data_sets; ++b)
      {
        bse >> bootstrap_config[boot][b];
        bootstrap_config[boot][b]-=1;
      }
    }
    bse.close();
  }
  else
  {
    bootstrap_config.resize(bootstrap_samples, bootstrap_config_tmp);
    for(int boot=0; boot<bootstrap_samples; ++boot)
    {
      for(unsigned int b=0; b<fit_n_data_sets; ++b)
      {
        bootstrap_config[boot][b]=static_cast<int>(gsl_rng_uniform(rng)*fit_n_data_sets);
      }
    }
  }

  std::vector< std::ofstream* > all_output(parameter_names.size());
  for(int p=0; p<parameter_names.size(); ++p)
  {
    std::string filename;
    filename=output_dir+xml_filename.substr(xml_filename.find_last_of( '/' )+1)+"_bootstrap_"+parameter_names[p]+".dat";
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

  std::cout << "Performing bootstrap with " << bootstrap_samples << " samples." << std::endl;
  std::cout << "Output directory is: " << output_dir << std::endl << std::endl;

  std::vector< double > bootstrap_results_tmp(parameter_names.size());
  std::vector< std::vector< double > > bootstrap_results(bootstrap_samples, bootstrap_results_tmp);

  int boot_min=0;
  int boot_max=bootstrap_samples-1;

  if(restrict_bootstrap_range)
  {
    if( (bootstrap_range_max<bootstrap_range_min) || (bootstrap_range_max>bootstrap_samples) )
    {
      std::cerr << "Error: bootstrap range incorrect"  << std::endl << std::endl;
      return false;
    }
    boot_min=bootstrap_range_min-1;
    boot_max=bootstrap_range_max-1;

    // skip random numbers for priors
    if( random_priors && bayesian)
    {
      for(int boot=0; boot<boot_min; ++boot)
      {
        for(int p=0; p<parameter_names.size(); ++p)
        {
          _gaussian_prior->set_prior(p, priors[p]+gsl_ran_gaussian(rng, sigmas[p]));
        }
      }
    }
  }


  std::vector< std::vector< std::vector< std::vector< std::string > > > > bootstrap_fit_min(bootstrap_samples, fit_min);
  std::vector< std::vector< std::vector< std::vector< std::string > > > > bootstrap_fit_max(bootstrap_samples, fit_max);

  for(unsigned int model_index=0; model_index<models.size(); ++model_index)
  {
    int n_variables=models[model_index] -> get_n_variables();
    for(int v=0; v<n_variables; ++v)
    {
      for(unsigned int range_index=0; range_index<has_range_bootstrap_file[model_index][v].size(); ++range_index)
      {
        if(has_range_bootstrap_file[model_index][v][range_index])
        {
          std::ifstream brf(range_bootstrap_file[model_index][v][range_index].c_str());
          if(!brf)
          {
            std::cerr << "Error: could not open range bootstrap file " << range_bootstrap_file[model_index][v][range_index] << std::endl << std::endl;
            return false;
          }
          for(int boot=0; boot<bootstrap_samples; ++boot)
          {
            std::string min_str, max_str;
            brf >> min_str;
            if(brf.eof())
            {
              std::cerr << "Error while reading range bootstrap file " << range_bootstrap_file[model_index][v][range_index] << std::endl << std::endl;
              return false;
            }
            brf >> max_str;
            bootstrap_fit_min[boot][model_index][v][range_index]=min_str;
            bootstrap_fit_max[boot][model_index][v][range_index]=max_str;
          }
          brf.close();
        }
      }
    }
  }


  for(int boot=boot_min; boot<=boot_max; ++boot)
  {
    
    // prepare fit data
    std::vector< std::vector< std::vector< std::vector< double > > > > all_fit_data;
    std::vector< std::vector< std::vector< double > > > all_fit_arguments;
    if(!prepare_fit_data(models, constant_values, constant_names, bin_size, restrict_data, start_n_data, stop_n_data, bootstrap_fit_min[boot], bootstrap_fit_max[boot], has_fit_step, fit_step,
                         all_file_data, all_file_arguments, all_fit_data, all_fit_arguments))
    {
      return false;
    }
    std::vector< std::vector< double > > global_fit_data;
    if(!combine_data(all_fit_data, global_fit_data))
    {
      return false;
    }

    c_model->set_all_arguments(all_fit_arguments);
    
    delete _fitter;
    _fitter=new fitter(c_model, _gaussian_prior, _chisqr_extra_term);

    _fitter->set_num_diff(num_diff);
    _fitter->set_second_deriv_minimization(second_deriv_minimization);
    _fitter->set_second_deriv_covariance(second_deriv_covariance);

    if(num_diff || second_deriv_minimization || second_deriv_covariance)
    {
      _fitter->set_num_diff_step(num_diff_step);
    }

    _fitter->set_initial_lambda(start_lambda);
    _fitter->set_lambda_factor(lambda_factor);
    _fitter->set_tolerance(chi_sqr_tolerance);
    _fitter->set_tolerance_dof(chi_sqr_tolerance_dof);
    _fitter->set_inversion_method(inv_method);
    switch(inv_method)
    {
      case simple_cut:
        _fitter->set_svd_cut(svd_fixed_cut);
        break;
      case ratio_cut:
        _fitter->set_svd_cut_ratio(svd_ratio_cut);
        break;
      case absolute_cut:
        _fitter->set_svd_cut_absolute(svd_absolute_cut);
        break;
      case off_diagonal_rescale:
        _fitter->set_off_diagonal_rescale_factor(off_diagonal_rescale_factor);
        break;
      default:
        break;
    }
    _fitter->set_cov_normalization(cn);

    for(int p=0; p<start_values.size(); ++p)
    {
      _fitter->set_starting_value(p, start_values[p]);
      _fitter->set_parameter_name(p, parameter_names[p]);
    }
    _fitter->set_n_parameters_dof(n_parameters_dof);

    std::vector< std::vector< double > > bootstrap_data=global_fit_data;
    for(unsigned int b=0; b<global_fit_data.size(); ++b)
    {
      int r=bootstrap_config[boot][b];
      bootstrap_data[b]=global_fit_data[r];
    }

    _fitter->set_data(bootstrap_data);

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
      std::cout << "WARNING: Bootstrap fit #" << boot+1 << " did not converge after " << max_steps << " iterations. " << "chi^2/dof = " << chisqr/dof << std::endl;
    }
    else
    {
      std::cout << "Bootstrap fit #" << boot+1 << " converged after " << steps_needed << " iterations. " << "chi^2/dof = " << chisqr/dof << " dof = " << dof << std::endl;
    }
    for(int p=0; p<parameter_names.size(); ++p)
    {
      bootstrap_results[boot][p] = _fitter->get_parameter(p);
      *all_output[p] << bootstrap_results[boot][p] << std::endl;
    }
  }
  gsl_rng_free(rng);
  std::cout << std::endl << "Bootstrap completed." << std::endl << std::endl;
  for(int p=0; p<parameter_names.size(); ++p)
  {
    all_output[p] -> close();
    delete all_output[p];
  }


  std::vector< double >boot_av(parameter_names.size(), 0.0);
  std::vector< double >boot_sigma(parameter_names.size(), 0.0);

  for(int p=0; p<parameter_names.size(); ++p)
  {
    std::vector< double > range_temp(boot_max-boot_min+1, 0.0);
    for(int boot=boot_min; boot<=boot_max; ++boot)
    {
      range_temp[boot-boot_min]=bootstrap_results[boot][p];
    }

    std::stable_sort(range_temp.begin(), range_temp.end());
    
    int min_pos=static_cast<int>(0.158655*(boot_max-boot_min+1));
    int max_pos=static_cast<int>(0.841345*(boot_max-boot_min+1));
    
    for(int pos=min_pos; pos<max_pos; ++pos)
    {
      boot_av[p]+=range_temp[pos];
    }
    boot_av[p]/=double(max_pos-min_pos);
    
    boot_sigma[p]=(range_temp[max_pos]-range_temp[min_pos])/2.0;
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
    std::cout << setw(16) << boot_av[p];
    std::cout << setw(16) << boot_sigma[p];
    std::cout << std::endl;
  }
  std::cout << resetiosflags(ios::left);
  std::cout << setw(0);
  std::cout << std::endl;

  return true;
}
