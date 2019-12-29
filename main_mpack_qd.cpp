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

#include "xml_tools.h"
#include "command_line_parser.h"

#include "replace_macros.h"

#include "abstract_model.h"
#include "abstract_prior.h"
#include "gaussian_prior.h"
#include "combined_model.h"
#include "chisqr_extra_term.h"
#include "fitter_mpack_qd.h"

#include "get_fit_settings_mpack_qd.h"
#include "create_models.h"
#include "get_constants.h"
#include "get_chi_sqr_extra_term.h"
#include "get_parameters.h"
#include "prepare_fit_data.h"
#include "fit_mpack_qd.h"
#include "bootstrap_mpack_qd.h"
#include "multifit_mpack_qd.h"
#include "write_fit_results_mpack_qd.h"
#include "plot.h"

void print_usage()
{
  std::cout << "usage: XMBF_mpack_qd [options] input.xml" << std::endl << std::endl;
  std::cout << "options:" << std::endl;
  std::cout << "  -o output.xml        write fit results to \"output.xml\"" << std::endl;
  std::cout << "  -c cov.dat           write covariance matrix to \"cov.dat\"" << std::endl;
  std::cout << "  -r res.dat           write results to \"res.dat\"" << std::endl;
  std::cout << "  -re res_err.dat      write results with errors to \"res_err.dat\"" << std::endl;
  std::cout << "  -p directory         plot data and fit functions, write output files to \"directory\"" << std::endl;
  std::cout << "  -b directory         perform bootstrap, write output files to \"directory\"" << std::endl;
  std::cout << "  -m directory         perform multifit, write output files to \"directory\"" << std::endl;
  std::cout << "  -v level             verbose level [0,1,2]" << std::endl;
  std::cout << std::endl;
}

int main(int argc, char *argv[])
{
  std::cout << std::endl << "This is XMBF_mpack_qd version 2.52" << std::endl << std::endl;
  if(argc<2)
  {
    print_usage();
    return 1;
  }

  gsl_set_error_handler_off();

  std::string inputfile=argv[argc-1];

  command_line_parser clp(1, argc-2, argv);

  bool write_output=false;
  std::string outputfile;
  if(clp.exists("o"))
  {
    write_output=true;
    try
    {
      outputfile=clp.get_value("o");
    }
    catch (int e)
    {
      print_usage();
      return 1;
    }
  }

  bool write_cov=false;
  std::string covariancefile;
  if(clp.exists("c"))
  {
    write_cov=true;
    try
    {
      covariancefile=clp.get_value("c");
    }
    catch (int e)
    {
      print_usage();
      return 1;
    }
  }

  bool write_res=false;
  std::string resultsfile;
  if(clp.exists("r"))
  {
    write_res=true;
    try
    {
      resultsfile=clp.get_value("r");
    }
    catch (int e)
    {
      print_usage();
      return 1;
    }
  }

  bool write_res_err=false;
  std::string resultserrorsfile;
  if(clp.exists("re"))
  {
    write_res_err=true;
    try
    {
      resultserrorsfile=clp.get_value("re");
    }
    catch (int e)
    {
      print_usage();
      return 1;
    }
  }

  bool perform_plot=false;
  std::string plot_output_dir;
  if(clp.exists("p"))
  {
    perform_plot=true;
    try
    {
      plot_output_dir=clp.get_value("p");
    }
    catch (int e)
    {
      print_usage();
      return 1;
    }
    if(*(plot_output_dir.end()-1)!='/')
    {
      plot_output_dir.append("/");
    }
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
      print_usage();
      return 1;
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
      print_usage();
      return 1;
    }
    if(*(multifit_output_dir.end()-1)!='/')
    {
      multifit_output_dir.append("/");
    }
  }

  int verbose_level=0;
  if(clp.exists("v"))
  {
    try
    {
      verbose_level=boost::lexical_cast<int>(clp.get_value("v"));
    }
    catch (int e)
    {
      print_usage();
      return 1;
    }
    if( !( (verbose_level==0) || (verbose_level==1) || (verbose_level==2) )  )
    {
      print_usage();
      return 1;
    }
  }


  std::cout << "Input file: "  << inputfile  << std::endl << std::endl;

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
          return 1;
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
          return 1;
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
          return 1;
        }
        combined_model* c_model=new combined_model();
        
        for(unsigned int model_index=0; model_index<models.size(); ++model_index)
        {
          std::cout << "Model " << model_index+1 << ", Model group " << model_groups[model_index] << ":" << std::endl << std::endl;
          for(int f=0; f<models[model_index]->get_n_functions(); ++f)
          {
            std::cout << "   Function " << f+1 << " = " << models[model_index]->get_function_name(f) << std::endl << std::endl;
          }
          c_model->add_model(models[model_index], model_groups[model_index]);
        }

        // read constant values
        std::vector< double > constant_values;
        std::vector< std::string > constant_names;
        if(!get_constants(c_model, constant_names, constant_values, root))
        {
          return 1;
        }

        // read parameter values
        std::vector< std::string > parameter_names(c_model->get_n_parameters());
        std::vector< double > start_values(c_model->get_n_parameters());
        std::vector< double > priors(c_model->get_n_parameters());
        std::vector< double > sigmas(c_model->get_n_parameters());
        if(!get_parameters(c_model, bayesian, parameter_names, start_values, priors, sigmas, root))
        {
          return 1;
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
          return 1;
        }
        std::vector< std::vector< double > > global_fit_data;
        if(!combine_data(all_fit_data, global_fit_data))
        {
          return 1;
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
            return 1;
          }

          if( _in("{", chisqr_extra_term_function) || _in("&", chisqr_extra_term_function) || _in("&", chisqr_extra_term_function) || _in("!", chisqr_extra_term_function) || _in("}", chisqr_extra_term_function)
           || _in("%", chisqr_extra_term_function) || _in("?", chisqr_extra_term_function) || _in("[", chisqr_extra_term_function) || _in(":", chisqr_extra_term_function)
           || _in(">", chisqr_extra_term_function) || _in("#", chisqr_extra_term_function) || _in("~", chisqr_extra_term_function) || _in("$", chisqr_extra_term_function)
           || _in("]", chisqr_extra_term_function) || _in("<", chisqr_extra_term_function) )
          {
            std::cerr << "Error: forbidden character in chi^2 extra term function " << std::endl << std::endl;
            return 1;
          }

          for(int p=0; p<c_model->get_n_parameters(); ++p)
          {
            if(find(chisqr_extra_term_constant_names.begin(), chisqr_extra_term_constant_names.end(), c_model->get_parameter_name(p))!=chisqr_extra_term_constant_names.end())
            {
              std::cerr << "Error: constant name \"" << c_model->get_parameter_name(p) << "\" in chi^2 extra term function is equal to a fit parameter name " << std::endl << std::endl;
              return 1;
            }
          }

          for(int c=0; c<c_model->get_n_constants(); ++c)
          {
            if(find(chisqr_extra_term_constant_names.begin(), chisqr_extra_term_constant_names.end(), c_model->get_constant_name(c))!=chisqr_extra_term_constant_names.end())
            {
              std::cerr << "Error: constant name \"" << c_model->get_constant_name(c) << "\" in chi^2 extra term function is equal to a fit constant name " << std::endl << std::endl;
              return 1;
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
              std::cerr << "Error: forbidden character or substring in constant \"" << constant_name << "\" within chi^2 extra term " << std::endl << std::endl;
              return false;
            }
          }
          std::cout << std::endl << "Additional term in chi^2 function: " << chisqr_extra_term_function << std::endl << std::endl;

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
          std::cerr << "Error while evaluating chi^2 extra term function " << std::endl << std::endl;
          return 1;
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
          default:
            break;
        }
        _fitter->set_bootstrap_normalization(bootstrap_normalization);

        // perform fit
        int steps_needed;
        bool converged=true;
        if(!fit(_fitter, bayesian, _gaussian_prior, global_fit_data, parameter_names, start_values, priors, sigmas, n_parameters_dof, max_iterations, verbose_level, steps_needed))
        {
          return 1;
        }
        if(steps_needed==max_iterations+1)
        {
          print_fit_results(_fitter, bayesian, parameter_names, start_values, priors, sigmas, n_parameters_dof);
          std::cout << std::endl << "WARNING: Fit did not converge after " << max_iterations << " iterations" << std::endl << std::endl;
          converged=false;
        }
        else
        {
          print_fit_results(_fitter, bayesian, parameter_names, start_values, priors, sigmas, n_parameters_dof);
          std::cout << std::endl << "Fit converged after " << steps_needed << " iterations." << std::endl << std::endl;
        }

        if(!(_fitter->covariance_positive_definite()))
        {
          std::cout << std::endl << "WARNING: Hessian of chi^2 is not positive definite" << std::endl << std::endl;
          converged=false;
        }

        if(write_cov)
        {
          write_covariance(covariancefile, _fitter, parameter_names);
        }

        if(write_res)
        {
          write_results(resultsfile, _fitter, parameter_names);
        }

        if(write_res_err)
        {
          write_results_errors(resultserrorsfile, _fitter, parameter_names);
        }

        // write fit results
        if(write_output)
        {
          std::cout << "Output file: "  << outputfile  << std::endl << std::endl;
          if(!write_fit_results(_fitter, bayesian, converged, n_parameters_dof, steps_needed, parameter_names, inputfile, outputfile))
          {
            return 1;
          }
        }

        // perform plot
        if(perform_plot)
        {
          if(!plot(models,
                   constant_values,
                   constant_names,
                   plot_order,
                   plot_min,
                   plot_max,
                   plot_step,
                   bin_size, restrict_data_range, data_range_min, data_range_max,
                   bootstrap_normalization,
                   all_file_data,
                   all_file_arguments,
                   plot_output_dir,
                   inputfile))
          {
            return 1;
          }
        }

        // perform bootstrap
        if(perform_bootstrap)
        {

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

          if(!has_any_range_bootstrap_file)
          {

            if(!bootstrap(_fitter, bayesian, _gaussian_prior, global_fit_data, parameter_names, start_values, priors, sigmas, n_parameters_dof, max_iterations, random_priors, bootstrap_samples, use_bse_file, bse_file, restrict_bootstrap_range, bootstrap_range_min, bootstrap_range_max, bootstrap_output_dir, inputfile))
            {
              return 1;
            }
          }
          else
          {
            if(!bootstrap_with_range_change(c_model, models, constant_values, constant_names, bin_size, restrict_data_range, data_range_min, data_range_max, fit_min, fit_max, has_fit_step, fit_step, has_range_bootstrap_file, range_bootstrap_file, all_file_data, all_file_arguments, _fitter, num_diff, second_deriv_covariance, second_deriv_minimization, num_diff_step, start_lambda, lambda_factor, chi_sqr_tolerance, chi_sqr_tolerance_dof, inv_method, svd_fixed_cut, svd_ratio_cut, svd_absolute_cut, bootstrap_normalization, bayesian, _gaussian_prior, _chisqr_extra_term, parameter_names, start_values, priors, sigmas, n_parameters_dof, max_iterations, random_priors, bootstrap_samples, use_bse_file, bse_file, restrict_bootstrap_range, bootstrap_range_min, bootstrap_range_max, bootstrap_output_dir, inputfile))
            {
              return 1;
            }
          }

        }

        // perform multifit
        if(perform_multifit)
        {
          if(!multifit(_fitter, bayesian, _gaussian_prior, global_fit_data, parameter_names, start_values, priors, sigmas, n_parameters_dof, max_iterations, random_priors, multifit_output_dir, inputfile))
          {
            return 1;
          }
        }

        return 0;
      }
    }
  #ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
  }
  catch(const std::exception& ex)
  {
    std::cout << "XML exception caught while reading " << inputfile << " : " << ex.what() << std::endl << std::endl;
    return 1;
  }
  #endif //LIBXMLCPP_EXCEPTIONS_ENABLED
}
