#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <utility>

#include <libxml++/libxml++.h>
#include <gsl/gsl_errno.h>

#include "mpi.h"

#include "xml_tools.h"
#include "command_line_parser.h"

#include "replace_macros.h"

#include "abstract_model.h"
#include "abstract_prior.h"
#include "gaussian_prior.h"
#include "combined_model.h"
#include "chisqr_extra_term.h"
#include "fitter.h"

#include "get_fit_settings.h"
#include "create_models_mpi.h"
#include "get_constants.h"
#include "get_chi_sqr_extra_term.h"
#include "get_parameters.h"
#include "prepare_fit_data.h"
#include "fit.h"
#include "bootstrap_mpi.h"
#include "multifit_mpi.h"
#include "write_fit_results.h"
#include "plot.h"

void print_usage()
{
  std::cout << "usage: mpirun -np nprocs XMBF_mpi options input.xml" << std::endl << std::endl;
  std::cout << "options:" << std::endl;
  std::cout << "  -b directory         perform bootstrap, write output files to \"directory\"" << std::endl;
  std::cout << "  -m directory         perform multifit, write output files to \"directory\"" << std::endl;
  std::cout << std::endl;
}

int main(int argc, char *argv[])
{
  MPI::Init(argc, argv);

  int proc = MPI::COMM_WORLD.Get_rank();
  int n_procs = MPI::COMM_WORLD.Get_size();

  if(argc!=4)
  {
    if(proc==0)
    {
      print_usage();
    }
    MPI::COMM_WORLD.Abort(1);
  }

  gsl_set_error_handler_off();

  std::string inputfile=argv[argc-1];

  command_line_parser clp(1, argc-2, argv);


  if(! (clp.exists("b") || clp.exists("m")) )
  {
    if(proc==0)
    {
      print_usage();
    }
    MPI::COMM_WORLD.Abort(1);
  }


  bool perform_bootstrap=false;
  std::string bootstrap_output_dir;
  if(clp.exists("b"))
  {
    perform_bootstrap=true;
    try
    {
      bootstrap_output_dir=clp.get_value("b");
    }
    catch (int e)
    {
      if(proc==0)
      {
        print_usage();
      }
      MPI::COMM_WORLD.Abort(1);
    }
    if(*(bootstrap_output_dir.end()-1)!='/')
    {
      bootstrap_output_dir.append("/");
    }
  }

  bool perform_multifit=false;
  std::string multifit_output_dir;
  if(clp.exists("m"))
  {
    perform_multifit=true;
    try
    {
      multifit_output_dir=clp.get_value("m");
    }
    catch (int e)
    {
      if(proc==0)
      {
        print_usage();
      }
      MPI::COMM_WORLD.Abort(1);
    }
    if(*(multifit_output_dir.end()-1)!='/')
    {
      multifit_output_dir.append("/");
    }
  }

  bool perform_plot=false;

  if(proc==0)
  {
    std::cout << std::endl << "This is XMBF_mpi version 2.53" << std::endl << std::endl;
    std::cout << std::endl << "Number of processes = " << n_procs << std::endl << std::endl;
    std::cout << "Input file: "  << inputfile  << std::endl << std::endl;
  }

  #ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
  try
  {
  #endif //LIBXMLCPP_EXCEPTIONS_ENABLED
    xmlpp::DomParser parser(inputfile);
    if(parser)
    {
      xmlpp::Node* root = parser.get_document()->get_root_node(); //deleted by DomParser.
      if(root)
      {
        // replace macros
        if(!replace_macros(root))
        {
          MPI::COMM_WORLD.Abort(1);
        }

        // get fit_settings;
        bool restrict_data_range;
        int data_range_min;
        int data_range_max;
        bool bayesian;
        bool chi_sqr_extra_term;
        bool random_priors;
        bool num_diff;
        bool second_deriv_covariance;
        bool second_deriv_minimization;
        double num_diff_step;
        double start_lambda;
        double lambda_factor;
        double chi_sqr_tolerance;
        bool chi_sqr_tolerance_dof;
        int n_parameters_dof;
        inversion_method inv_method;
        int svd_fixed_cut;
        double svd_ratio_cut;
        double svd_absolute_cut;
        double off_diagonal_rescale_factor;
        int max_iterations;
        int bin_size;
        int bootstrap_samples;
        bool use_bse_file;
        std::string bse_file;
        bool restrict_bootstrap_range;
        bool bootstrap_normalization;
        int bootstrap_range_min;
        int bootstrap_range_max;

        if(!get_fit_settings(perform_bootstrap,
                             restrict_data_range,
                             data_range_min,
                             data_range_max,
                             bayesian,
                             chi_sqr_extra_term,
                             random_priors,
                             num_diff,
                             second_deriv_covariance,
                             second_deriv_minimization,
                             num_diff_step,
                             start_lambda,
                             lambda_factor,
                             chi_sqr_tolerance,
                             chi_sqr_tolerance_dof,
                             n_parameters_dof,
                             inv_method,
                             svd_fixed_cut,
                             svd_ratio_cut,
                             svd_absolute_cut,
                             off_diagonal_rescale_factor,
                             max_iterations,
                             bin_size,
                             bootstrap_samples,
                             use_bse_file,
                             bse_file,
                             restrict_bootstrap_range,
                             bootstrap_range_min,
                             bootstrap_range_max,
                             bootstrap_normalization,
                             root))
        {
          MPI::COMM_WORLD.Abort(1);
        }

        // create models
        std::vector<abstract_model*> models;
        std::vector<int> model_groups;
        std::vector< std::vector< std::vector< std::string > > > fit_min, fit_max;
        std::vector< std::vector< std::vector< bool > > > has_fit_step;
        std::vector< std::vector< std::vector< double > > > fit_step;
        std::vector< std::vector< std::vector< bool > > > has_range_bootstrap_file;
        std::vector< std::vector< std::vector< std::string > > > range_bootstrap_file;
        std::vector< std::vector< int > > plot_order;
        std::vector< std::vector< std::string > > plot_min;
        std::vector< std::vector< std::string > > plot_max;
        std::vector< std::vector< double > > plot_step;

        std::vector< std::vector< std::vector< std::vector< double > > > > all_file_data;
        std::vector< std::vector< std::vector< double > > > all_file_arguments;
        if(!create_models(models, model_groups, fit_min, fit_max, has_fit_step, fit_step, has_range_bootstrap_file, range_bootstrap_file, perform_plot, plot_order, plot_min, plot_max, plot_step, all_file_data, all_file_arguments, num_diff, root))
        {
          MPI::COMM_WORLD.Abort(1);
        }
        combined_model* c_model=new combined_model();
        
        for(unsigned int model_index=0; model_index<models.size(); ++model_index)
        {
          if(proc==0)
          {
            std::cout << "Model " << model_index+1 << ", Model group " << model_groups[model_index] << ":" << std::endl << std::endl;
          }
          for(int f=0; f<models[model_index]->get_n_functions(); ++f)
          {
            if(proc==0)
            {
              std::cout << "   Function " << f+1 << " = " << models[model_index]->get_function_name(f) << std::endl << std::endl;
            }
          }
          c_model->add_model(models[model_index], model_groups[model_index]);
        }

        // read constant values
        std::vector< double > constant_values;
        std::vector< std::string > constant_names;
        if(!get_constants(c_model, constant_names, constant_values, root))
        {
          MPI::COMM_WORLD.Abort(1);
        }

        // read parameter values
        std::vector< std::string > parameter_names(c_model->get_n_parameters());
        std::vector< double > start_values(c_model->get_n_parameters());
        std::vector< double > priors(c_model->get_n_parameters());
        std::vector< double > sigmas(c_model->get_n_parameters());
        if(!get_parameters(c_model, bayesian, parameter_names, start_values, priors, sigmas, root))
        {
          MPI::COMM_WORLD.Abort(1);
        }
        if(n_parameters_dof==-1)  // i.e. n_parameters_dof not found in xml file
        {
          if(bayesian)
          {
            n_parameters_dof=0;
          }
          else
          {
            n_parameters_dof=c_model->get_n_parameters();
          }
        }

        // prepare fit data
        std::vector< std::vector< std::vector< std::vector< double > > > > all_fit_data;
        std::vector< std::vector< std::vector< double > > > all_fit_arguments;
        if(!prepare_fit_data(models, constant_values, constant_names, bin_size, restrict_data_range, data_range_min, data_range_max, fit_min, fit_max, has_fit_step, fit_step,
                             all_file_data, all_file_arguments, all_fit_data, all_fit_arguments))
        {
          MPI::COMM_WORLD.Abort(1);
        }
        std::vector< std::vector< double > > global_fit_data;
        if(!combine_data(all_fit_data, global_fit_data))
        {
          MPI::COMM_WORLD.Abort(1);
        }

        // read chisqr_extra_term
        std::string chisqr_extra_term_function="";
        std::vector< double > chisqr_extra_term_constant_values;
        std::vector< std::string > chisqr_extra_term_constant_names;
        double chisqr_extra_term_num_diff_step=1e-08;
        bool mixed_second_derivatives_specified=false;
        std::vector< std::pair< std::string, std::string > > mixed_second_derivatives;
        if(chi_sqr_extra_term)
        {
          if(!get_chi_sqr_extra_term(chisqr_extra_term_function, chisqr_extra_term_constant_names, chisqr_extra_term_constant_values, chisqr_extra_term_num_diff_step, mixed_second_derivatives_specified, mixed_second_derivatives, root))
          {
            MPI::COMM_WORLD.Abort(1);
          }

          if( _in("{", chisqr_extra_term_function) || _in("&", chisqr_extra_term_function) || _in("&", chisqr_extra_term_function) || _in("!", chisqr_extra_term_function) || _in("}", chisqr_extra_term_function)
           || _in("%", chisqr_extra_term_function) || _in("?", chisqr_extra_term_function) || _in("[", chisqr_extra_term_function) || _in(":", chisqr_extra_term_function)
           || _in(">", chisqr_extra_term_function) || _in("#", chisqr_extra_term_function) || _in("~", chisqr_extra_term_function) || _in("$", chisqr_extra_term_function)
           || _in("]", chisqr_extra_term_function) || _in("<", chisqr_extra_term_function) )
          {
            if(proc==0)
            {
              std::cerr << "Error: forbidden character in chi^2 extra term function " << std::endl << std::endl;
            }
            MPI::COMM_WORLD.Abort(1);
          }

          for(int p=0; p<c_model->get_n_parameters(); ++p)
          {
            if(find(chisqr_extra_term_constant_names.begin(), chisqr_extra_term_constant_names.end(), c_model->get_parameter_name(p))!=chisqr_extra_term_constant_names.end())
            {
              if(proc==0)
              {
                std::cerr << "Error: constant name \"" << c_model->get_parameter_name(p) << "\" in chi^2 extra term function is equal to a fit parameter name " << std::endl << std::endl;
              }
              MPI::COMM_WORLD.Abort(1);
            }
          }

          for(int c=0; c<c_model->get_n_constants(); ++c)
          {
            if(find(chisqr_extra_term_constant_names.begin(), chisqr_extra_term_constant_names.end(), c_model->get_constant_name(c))!=chisqr_extra_term_constant_names.end())
            {
              if(proc==0)
              {
                std::cerr << "Error: constant name \"" << c_model->get_constant_name(c) << "\" in chi^2 extra term function is equal to a fit constant name " << std::endl << std::endl;
              }
              MPI::COMM_WORLD.Abort(1);
            }
          }

          for(int c=0; c<chisqr_extra_term_constant_names.size(); ++c)
          {
            std::string constant_name=chisqr_extra_term_constant_names[c];
            if( _in("{", constant_name) || _in("&", constant_name) || _in("&", constant_name) || _in("!", constant_name) || _in("}", constant_name)
             || _in("%", constant_name) || _in("?", constant_name) || _in("[", constant_name) || _in(":", constant_name)
             || _in(">", constant_name) || _in("#", constant_name) || _in("~", constant_name) || _in("$", constant_name)
             || _in("]", constant_name) || _in("<", constant_name) ||
                          _in("arcsin", constant_name) ||  _in("sinh", constant_name)   ||
                          _in("sin", constant_name)    ||  _in("arccos", constant_name) ||
                          _in("cosh", constant_name)   ||  _in("cos", constant_name)    ||
                          _in("arctan", constant_name) ||  _in("tanh", constant_name)   ||
                          _in("tan", constant_name)    ||  _in("exp", constant_name)    ||
                          _in("log", constant_name)    ||  _in("sqrt", constant_name)   ||
                          _in("sqr", constant_name)    ||  _in("alt", constant_name) )
            {
              if(proc==0)
              {
                std::cerr << "Error: forbidden character or substring in constant \"" << constant_name << "\" within chi^2 extra term " << std::endl << std::endl;
              }
              MPI::COMM_WORLD.Abort(1);
            }
          }
          if(proc==0)
          {
            std::cout << std::endl << "Additional term in chi^2 function: " << chisqr_extra_term_function << std::endl << std::endl;
          }
        }


        // create fitter
        c_model->set_constants(constant_values);
        c_model->set_all_arguments(all_fit_arguments);
        gaussian_prior* _gaussian_prior=new gaussian_prior(c_model->get_n_parameters());
        chisqr_extra_term* _chisqr_extra_term=new chisqr_extra_term(c_model->get_n_parameters(), chisqr_extra_term_constant_names.size(), chisqr_extra_term_function);
        _chisqr_extra_term->set_enabled(chi_sqr_extra_term);
        for(int p=0; p<c_model->get_n_parameters(); ++p)
        {
          _chisqr_extra_term->set_parameter_name(p, c_model->get_parameter_name(p));
        }
        _chisqr_extra_term->init_parameter_list();
        for(int c=0; c<chisqr_extra_term_constant_names.size(); ++c)
        {
          _chisqr_extra_term->set_constant_name(c, chisqr_extra_term_constant_names[c]);
        }
        _chisqr_extra_term->set_constants(chisqr_extra_term_constant_values);
        _chisqr_extra_term->set_num_diff_step(chisqr_extra_term_num_diff_step);
        _chisqr_extra_term->set_parameters(start_values);
        _chisqr_extra_term->chi_sqr();
        if(_chisqr_extra_term->no_of_errors()!=0)
        {
          if(proc==0)
          {
            std::cerr << "Error while evaluating chi^2 extra term function " << std::endl << std::endl;
          }
          MPI::COMM_WORLD.Abort(1);
        }
        if(mixed_second_derivatives_specified)
        {
          _chisqr_extra_term->specify_mixed_second_derivatives(mixed_second_derivatives);
        }

        fitter* _fitter=new fitter(c_model, _gaussian_prior, _chisqr_extra_term);

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
        _fitter->set_bootstrap_normalization(bootstrap_normalization);


        // perform bootstrap
        if(perform_bootstrap)
        {

          if(use_bse_file)  // read number of bootstrap samples from bse file
          {
            std::ifstream bse(bse_file.c_str());
            if(!bse)
            {
              if(proc==0)
              {
                std::cerr << "Error: could not open bootstrap ensemble file " << bse_file << std::endl << std::endl;
              }
              MPI::COMM_WORLD.Abort(1);
            }
            bse >> bootstrap_samples;
            bse.close();
          }

          int boot_min=0;
          int boot_max=bootstrap_samples-1;

          if(restrict_bootstrap_range)
          {
            if( (bootstrap_range_max<bootstrap_range_min) || (bootstrap_range_max>bootstrap_samples) )
            {
              if(proc==0)
              {
                std::cerr << "Error: bootstrap range incorrect"  << std::endl << std::endl;
              }
              MPI::COMM_WORLD.Abort(1);
            }
            boot_min=bootstrap_range_min-1;
            boot_max=bootstrap_range_max-1;
          }

          if( (boot_max-boot_min+1)%n_procs !=0 )
          {
            if(proc==0)
            {
              std::cerr << "Error: number of bootstrap samples not integer multiple of number of processes "  << std::endl << std::endl;
            }
            MPI::COMM_WORLD.Abort(1);
          }
          
          bool has_any_range_bootstrap_file=false;
          for(unsigned int model_index=0; model_index<models.size(); ++model_index)
          {
            int n_variables=models[model_index] -> get_n_variables();
            for(int v=0; v<n_variables; ++v)
            {
              for(unsigned int range_index=0; range_index<has_range_bootstrap_file[model_index][v].size(); ++range_index)
              {
                if(has_range_bootstrap_file[model_index][v][range_index])
                {
                  has_any_range_bootstrap_file=true;
                }
              }
            }
          }


          int samples_per_proc=(boot_max-boot_min+1)/n_procs;

          int mpi_boot_min=boot_min+proc*samples_per_proc;
          int mpi_boot_max=boot_min+(proc+1)*samples_per_proc-1;

          if(proc==0)
          {
            std::cout << "Performing bootstrap with " << samples_per_proc << " samples per process." << std::endl;
            std::cout << "Output directory is: " << bootstrap_output_dir << std::endl << std::endl;
          }

          std::vector< double > bootstrap_results_tmp(parameter_names.size());
          std::vector< std::vector< double > > bootstrap_results(bootstrap_samples, bootstrap_results_tmp);

          std::vector< double > all_chisqr(bootstrap_samples);
          std::vector< int > all_steps_needed(bootstrap_samples);

          int* bse_config = new int[bootstrap_samples*global_fit_data.size()];

          if(use_bse_file)  // read bse file on root process only, then broadcast
          {
            if(proc == 0)
            {
              std::ifstream bse(bse_file.c_str());
              if(!bse)
              {
                std::cerr << "Error: could not open bootstrap ensemble file " << bse_file << std::endl << std::endl;
                MPI::COMM_WORLD.Abort(1);
              }
              int bse_bootstrap_samples=0;
              bse >> bse_bootstrap_samples;
              if(bse_bootstrap_samples!=bootstrap_samples)
              {
                std::cerr << "Error: bootstrap ensemble file has wrong number of samples" << std::endl << std::endl;
                MPI::COMM_WORLD.Abort(1);
              }
              int bse_n_data_sets=0;
              bse >> bse_n_data_sets;
              if(bse_n_data_sets!=global_fit_data.size())
              {
                std::cerr << "Error: bootstrap ensemble file has wrong number of data sets" << std::endl << std::endl;
                MPI::COMM_WORLD.Abort(1);
              }
              int buf;
              for(int boot=0; boot<bootstrap_samples; ++boot)
              {
                for(unsigned int b=0; b<global_fit_data.size(); ++b)
                {
                  bse >> buf;
                  bse_config[boot*global_fit_data.size()+b]=buf-1;
                }
              }
              bse.close();
            }

            MPI::COMM_WORLD.Bcast(bse_config, bootstrap_samples*global_fit_data.size(), MPI::INT, 0);
          }


          if(!has_any_range_bootstrap_file)
          {

            if(!bootstrap(proc, _fitter, bayesian, _gaussian_prior, global_fit_data, parameter_names, start_values, priors, sigmas, n_parameters_dof, max_iterations, random_priors, bootstrap_samples, use_bse_file, bse_config, mpi_boot_min, mpi_boot_max, bootstrap_results, all_steps_needed, all_chisqr))
            {
              MPI::COMM_WORLD.Abort(1);
            }
          }
          else
          {
            if(!bootstrap_with_range_change(proc, c_model, models, constant_values, constant_names, bin_size, restrict_data_range, data_range_min, data_range_max, fit_min, fit_max, has_fit_step, fit_step, has_range_bootstrap_file, range_bootstrap_file, all_file_data, all_file_arguments, _fitter, num_diff, second_deriv_covariance, second_deriv_minimization, num_diff_step, start_lambda, lambda_factor, chi_sqr_tolerance, chi_sqr_tolerance_dof, inv_method, svd_fixed_cut, svd_ratio_cut, svd_absolute_cut, off_diagonal_rescale_factor, bootstrap_normalization, bayesian, _gaussian_prior, _chisqr_extra_term, parameter_names, start_values, priors, sigmas, n_parameters_dof, max_iterations, random_priors, bootstrap_samples, use_bse_file, bse_config, mpi_boot_min, mpi_boot_max, bootstrap_results, all_steps_needed, all_chisqr))
            {
              MPI::COMM_WORLD.Abort(1);
            }
          }

          delete[] bse_config;

          // gather results

          double* partial_bootstrap_results = new double[samples_per_proc*parameter_names.size()];
          for(int boot=mpi_boot_min; boot<=mpi_boot_max; ++boot)
          {
            for(int p=0; p<parameter_names.size(); ++p)
            {
              partial_bootstrap_results[(boot-mpi_boot_min)*parameter_names.size()+p]=bootstrap_results[boot][p];
            }
          }
          double* complete_bootstrap_results = new double[n_procs*samples_per_proc*parameter_names.size()];
          MPI::COMM_WORLD.Gather(partial_bootstrap_results, samples_per_proc*parameter_names.size(), MPI::DOUBLE, complete_bootstrap_results, samples_per_proc*parameter_names.size(), MPI::DOUBLE, 0);
          for(int process=0; process<n_procs; ++process)
          {
            for(int boot=0; boot<samples_per_proc; ++boot)
            {
              for(int p=0; p<parameter_names.size(); ++p)
              {
                bootstrap_results[boot_min+process*samples_per_proc+boot][p]=complete_bootstrap_results[process*samples_per_proc*parameter_names.size()+boot*parameter_names.size()+p];
              }
            }
          }

          double* partial_all_chisqr = new double[samples_per_proc];
          for(int boot=mpi_boot_min; boot<=mpi_boot_max; ++boot)
          {
            partial_all_chisqr[boot-mpi_boot_min]=all_chisqr[boot];
          }
          double* complete_all_chisqr = new double[n_procs*samples_per_proc];
          MPI::COMM_WORLD.Gather(partial_all_chisqr, samples_per_proc, MPI::DOUBLE, complete_all_chisqr, samples_per_proc, MPI::DOUBLE, 0);

          int* partial_all_steps_needed = new int[samples_per_proc];
          for(int boot=mpi_boot_min; boot<=mpi_boot_max; ++boot)
          {
            partial_all_steps_needed[boot-mpi_boot_min]=all_steps_needed[boot];
          }
          int* complete_all_steps_needed = new int[n_procs*samples_per_proc];
          MPI::COMM_WORLD.Gather(partial_all_steps_needed, samples_per_proc, MPI::INT, complete_all_steps_needed, samples_per_proc, MPI::INT, 0);

          if(proc == 0)
          {
            // information on steps needed and chi^2 to stdout
            for(int boot=boot_min; boot<=boot_max; ++boot)
            {
              if(complete_all_steps_needed[boot-boot_min]==max_iterations+1)
              {
                std::cout << "WARNING: Bootstrap fit #" << boot+1 << " did not converge after " << max_iterations << " iterations. " << "chi^2/dof = " << complete_all_chisqr[boot-boot_min] << std::endl;
              }
              else
              {
                std::cout << "Bootstrap fit #" << boot+1 << " converged after " << complete_all_steps_needed[boot-boot_min] << " iterations. " << "chi^2/dof = " << complete_all_chisqr[boot-boot_min] << std::endl;
              }
            }
            std::cout << std::endl;

            // averages and 68% widths to stdout
            std::vector< double >multifit_av(parameter_names.size(), 0.0);
            std::vector< double >multifit_sigma(parameter_names.size(), 0.0);
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


            // write bootstrap results to file
            std::vector< ofstream* > all_output(parameter_names.size());
            for(int p=0; p<parameter_names.size(); ++p)
            {
              std::string filename;
              filename=bootstrap_output_dir+inputfile.substr(inputfile.find_last_of( '/' )+1)+"_bootstrap_"+parameter_names[p]+".dat";
              std::ofstream* output = new std::ofstream(filename.c_str());
              if(!(*output))
              {
                std::cerr << "Cannot write file "  << filename << std::endl << std::endl;
                MPI::COMM_WORLD.Abort(1);
              }
              output -> precision(15);
              all_output[p]=output;
            }

            for(int boot=boot_min; boot<=boot_max; ++boot)
            {
              for(int p=0; p<parameter_names.size(); ++p)
              {
                *all_output[p] << bootstrap_results[boot][p] << std::endl;
              }
            }

            for(int p=0; p<parameter_names.size(); ++p)
            {
              all_output[p] -> close();
              delete all_output[p];
            }
          }

          delete[] partial_bootstrap_results;
          delete[] complete_bootstrap_results;

          delete[] partial_all_chisqr;
          delete[] complete_all_chisqr;

          delete[] partial_all_steps_needed;
          delete[] complete_all_steps_needed;
          
        }

        // perform multifit
        if(perform_multifit)
        {

          int multifit_min=0;
          int multifit_max=global_fit_data.size()-1;

          if( (multifit_max-multifit_min+1)%n_procs !=0 )
          {
            if(proc==0)
            {
              std::cerr << "Error: number of multifit samples not integer multiple of number of processes "  << std::endl << std::endl;
            }
            MPI::COMM_WORLD.Abort(1);
          }
          

          int samples_per_proc=(multifit_max-multifit_min+1)/n_procs;

          int mpi_multifit_min=multifit_min+proc*samples_per_proc;
          int mpi_multifit_max=multifit_min+(proc+1)*samples_per_proc-1;

          if(proc==0)
          {
            std::cout << "Performing multifit with " << samples_per_proc << " samples per process." << std::endl;
            std::cout << "Output directory is: " << multifit_output_dir << std::endl << std::endl;
          }

          std::vector< double > multifit_results_tmp(parameter_names.size());
          std::vector< std::vector< double > > multifit_results(global_fit_data.size(), multifit_results_tmp);

          std::vector< double > all_chisqr(global_fit_data.size());
          std::vector< int > all_steps_needed(global_fit_data.size());

          if(!multifit(_fitter, bayesian, _gaussian_prior, global_fit_data, parameter_names, start_values, priors, sigmas, n_parameters_dof, max_iterations, random_priors, mpi_multifit_min, mpi_multifit_max, multifit_results, all_steps_needed, all_chisqr))
          {
            MPI::COMM_WORLD.Abort(1);
          }

          // gather results

          double* partial_multifit_results = new double[samples_per_proc*parameter_names.size()];
          for(int n=mpi_multifit_min; n<=mpi_multifit_max; ++n)
          {
            for(int p=0; p<parameter_names.size(); ++p)
            {
              partial_multifit_results[(n-mpi_multifit_min)*parameter_names.size()+p]=multifit_results[n][p];
            }
          }
          double* complete_multifit_results = new double[n_procs*samples_per_proc*parameter_names.size()];
          MPI::COMM_WORLD.Gather(partial_multifit_results, samples_per_proc*parameter_names.size(), MPI::DOUBLE, complete_multifit_results, samples_per_proc*parameter_names.size(), MPI::DOUBLE, 0);
          for(int process=0; process<n_procs; ++process)
          {
            for(int n=0; n<samples_per_proc; ++n)
            {
              for(int p=0; p<parameter_names.size(); ++p)
              {
                multifit_results[multifit_min+process*samples_per_proc+n][p]=complete_multifit_results[process*samples_per_proc*parameter_names.size()+n*parameter_names.size()+p];
              }
            }
          }

          double* partial_all_chisqr = new double[samples_per_proc];
          for(int n=mpi_multifit_min; n<=mpi_multifit_max; ++n)
          {
            partial_all_chisqr[n-mpi_multifit_min]=all_chisqr[n];
          }
          double* complete_all_chisqr = new double[n_procs*samples_per_proc];
          MPI::COMM_WORLD.Gather(partial_all_chisqr, samples_per_proc, MPI::DOUBLE, complete_all_chisqr, samples_per_proc, MPI::DOUBLE, 0);

          int* partial_all_steps_needed = new int[samples_per_proc];
          for(int n=mpi_multifit_min; n<=mpi_multifit_max; ++n)
          {
            partial_all_steps_needed[n-mpi_multifit_min]=all_steps_needed[n];
          }
          int* complete_all_steps_needed = new int[n_procs*samples_per_proc];
          MPI::COMM_WORLD.Gather(partial_all_steps_needed, samples_per_proc, MPI::INT, complete_all_steps_needed, samples_per_proc, MPI::INT, 0);

          if(proc == 0)
          {
            // information on steps needed and chi^2 to stdout
            for(int n=multifit_min; n<=multifit_max; ++n)
            {
              if(complete_all_steps_needed[n-multifit_min]==max_iterations+1)
              {
                std::cout << "WARNING: multifit #" << n+1 << " did not converge after " << max_iterations << " iterations. " << "chi^2/dof = " << complete_all_chisqr[n-multifit_min] << std::endl;
              }
              else
              {
                std::cout << "multifit #" << n+1 << " converged after " << complete_all_steps_needed[n-multifit_min] << " iterations. " << "chi^2/dof = " << complete_all_chisqr[n-multifit_min] << std::endl;
              }
            }
            std::cout << std::endl;

            // averages and 68% widths to stdout
            std::vector< double >multifit_av(parameter_names.size(), 0.0);
            std::vector< double >multifit_sigma(parameter_names.size(), 0.0);
            for(int p=0; p<parameter_names.size(); ++p)
            {
              std::vector< double > range_temp(multifit_max-multifit_min+1, 0.0);
              for(int n=multifit_min; n<=multifit_max; ++n)
              {
                range_temp[n-multifit_min]=multifit_results[n][p];
              }

              std::stable_sort(range_temp.begin(), range_temp.end());

              int min_pos=static_cast<int>(0.158655*(multifit_max-multifit_min+1));
              int max_pos=static_cast<int>(0.841345*(multifit_max-multifit_min+1));

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


            // write multifit results to file
            std::vector< ofstream* > all_output(parameter_names.size());
            for(int p=0; p<parameter_names.size(); ++p)
            {
              std::string filename;
              filename=multifit_output_dir+inputfile.substr(inputfile.find_last_of( '/' )+1)+"_multifit_"+parameter_names[p]+".dat";
              std::ofstream* output = new std::ofstream(filename.c_str());
              if(!(*output))
              {
                std::cerr << "Cannot write file "  << filename << std::endl << std::endl;
                MPI::COMM_WORLD.Abort(1);
              }
              output -> precision(15);
              all_output[p]=output;
            }

            for(int n=multifit_min; n<=multifit_max; ++n)
            {
              for(int p=0; p<parameter_names.size(); ++p)
              {
                *all_output[p] << multifit_results[n][p] << std::endl;
              }
            }

            for(int p=0; p<parameter_names.size(); ++p)
            {
              all_output[p] -> close();
              delete all_output[p];
            }
          }

          delete[] partial_multifit_results;
          delete[] complete_multifit_results;

          delete[] partial_all_chisqr;
          delete[] complete_all_chisqr;

          delete[] partial_all_steps_needed;
          delete[] complete_all_steps_needed;
          
        }
        
      }
    }
  #ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
  }
  catch(const std::exception& ex)
  {
    if(proc==0)
    {
      std::cout << "XML exception caught while reading " << inputfile << " : " << ex.what() << std::endl << std::endl;
    }
    MPI::COMM_WORLD.Abort(1);
  }
  #endif //LIBXMLCPP_EXCEPTIONS_ENABLED

  MPI::Finalize();
}
