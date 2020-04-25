#include "plot_mpack_qd.h"

bool plot(std::vector<abstract_model*> models,
          const std::vector< double >& constant_values,
          const std::vector< std::string >& constant_names,
          const std::vector< std::vector< int > >& plot_order,
          const std::vector< std::vector< std::string > >& plot_min,
          const std::vector< std::vector< std::string > >& plot_max,
          const std::vector< std::vector< double > >& plot_step,
          int bin_size, bool restrict_data, int start_n_data, int stop_n_data,
          const cov_normalization cn,
          const std::vector< std::vector< std::vector< std::vector< double > > > >& all_file_data,
          const std::vector< std::vector< std::vector< double > > >& all_file_arguments,
          const std::string& plot_output_dir,
          const std::string& xml_filename)
{
  int n_data_sets=all_file_data[0].size();
  int plot_n_data_sets;
  if(restrict_data)
  {
    plot_n_data_sets=stop_n_data-start_n_data+1;
  }
  else
  {
    start_n_data=1;
    stop_n_data=n_data_sets;
    plot_n_data_sets=n_data_sets;
  }

  for(unsigned int model_index=0; model_index<models.size(); ++model_index)
  {

    int n_data_points=all_file_arguments[model_index].size();
    int n_variables=models[model_index] -> get_n_variables();
    int n_functions=models[model_index] -> get_n_functions();

    // select data points in plotting range
    std::vector< parser* > all_plot_min_parsers;
    std::vector< parser* > all_plot_max_parsers;

    for(int v=0; v<n_variables; ++v)
    {

      all_plot_min_parsers.push_back(new parser(plot_min[model_index][v]));
      all_plot_max_parsers.push_back(new parser(plot_max[model_index][v]));
    }

    std::vector< std::vector< double > > plot_arguments;
    std::vector< int > indices;
    for(int m=0; m<n_data_points; ++m)
    {

      std::map< std::string, double > table;
      for(int c=0; c<constant_values.size(); ++c)
      {
        table[constant_names[c]]=constant_values[c];
      }

      for(int v=0; v<n_variables; ++v)
      {
        table[models[model_index]->get_variable_name(v)]=all_file_arguments[model_index][m][v];
      }
      bool in_range=true;
      for(int v=0; v<n_variables; ++v)
      {
        bool v_in_range=false;
        double arg=all_file_arguments[model_index][m][v];
        double min=all_plot_min_parsers[v]->parse(table);
        double max=all_plot_max_parsers[v]->parse(table);
        if((all_plot_min_parsers[v]->get_no_of_errors()>0) || (all_plot_max_parsers[v]->get_no_of_errors()>0))
        {
          for(std::vector< parser* >::iterator it=all_plot_min_parsers.begin(); it!=all_plot_min_parsers.end(); ++it)
          {
            delete *it;
          }
          for(std::vector< parser* >::iterator it=all_plot_max_parsers.begin(); it!=all_plot_max_parsers.end(); ++it)
          {
            delete *it;
          }
          std::cerr << "Error while parsing plotting range" << std::endl << std::endl;
          return false;
        }
        if( (min<=arg) && (arg<=max) )
        {
          v_in_range=true;
        }
        if(!v_in_range)
        {
          in_range=false;
        }
      }
      if(in_range)
      {
        plot_arguments.push_back(all_file_arguments[model_index][m]);
        indices.push_back(m);
      }
    }
    for(std::vector< parser* >::iterator it=all_plot_min_parsers.begin(); it!=all_plot_min_parsers.end(); ++it)
    {
      delete *it;
    }
    for(std::vector< parser* >::iterator it=all_plot_max_parsers.begin(); it!=all_plot_max_parsers.end(); ++it)
    {
      delete *it;
    }
    // end select data points in plotting range

    int plot_n_data_points=plot_arguments.size();
    if(plot_n_data_points==0)
    {
      std::cerr << "Error: No data points in plotting range" << std::endl << std::endl;
      return false;
    }

    std::vector< double > plot_data_temp_1(n_functions, 0.0);
    std::vector< std::vector< double > > plot_data_temp_2(plot_n_data_points, plot_data_temp_1);
    std::vector< std::vector< std::vector< double > > > plot_data(plot_n_data_sets/bin_size, plot_data_temp_2);

    for(int m=0; m<plot_n_data_points; ++m)
    {
      for(int f=0; f<n_functions; ++f)
      {
        for(int b=0; b<plot_n_data_sets/bin_size; ++b)
        {
          // average over entries e in bin b
          plot_data[b][m][f]=0.0;
          for(int e=0; e<bin_size; ++e)
          {
            plot_data[b][m][f]+=all_file_data[model_index][start_n_data-1+b*bin_size+e][indices[m]][f];
          }
          plot_data[b][m][f]/=bin_size;
        }
      }
    }
    plot_n_data_sets/=bin_size;


    // compute average and sigma
    std::vector< double > zero_temp(n_functions, 0.0);
    std::vector< std::vector< double > > average(plot_n_data_points, zero_temp);

    for(int n=0; n<plot_n_data_sets; ++n)
    {
      for(int m=0; m<plot_n_data_points; ++m)
      {
        for(int f=0; f<n_functions; ++f)
        {
          average[m][f]+=plot_data[n][m][f];
        }
      }
    }
    for(int m=0; m<plot_n_data_points; ++m)
    {
      for(int f=0; f<n_functions; ++f)
      {
        average[m][f]/=plot_n_data_sets;
      }
    }
    std::vector< std::vector< double > > sigma(plot_n_data_points, zero_temp);

    double normalization=1.0;
    if(cn==standard_normalization)
    {
      normalization=double(plot_n_data_sets)*double(plot_n_data_sets-1);
    }
    else if(cn==bootstrap_normalization)
    {
      normalization=double(plot_n_data_sets-1);
    }
    else if(cn==jackknife_normalization)
    {
      normalization=double(plot_n_data_sets)/double(plot_n_data_sets-1);
    }


    for(int m=0; m<plot_n_data_points; ++m)
    {
      for(int f=0; f<n_functions; ++f)
      {
        double temp=0.0;
        for(int n=0; n<plot_n_data_sets; ++n)
        {
          temp+=(plot_data[n][m][f]-average[m][f])*(plot_data[n][m][f]-average[m][f]);
        }
        sigma[m][f]=sqrt(temp/normalization);
      }
    }


    // variable as a function of plot_order_index
    std::vector< int > variable_number(n_variables);
    for(unsigned int plot_order_index=0; plot_order_index<n_variables; ++plot_order_index)
    {
      std::vector< int >::const_iterator pos=std::find(plot_order[model_index].begin(), plot_order[model_index].end(), plot_order_index);
      variable_number[plot_order_index]=pos-plot_order[model_index].begin();
    }

    std::cout << "Writing plot files ... " << std::endl;
    std::cout << "Output directory is: " << plot_output_dir << std::endl << std::endl;


    // plot data
    for(int f=0; f<n_functions; ++f)
    {
      std::string filename=plot_output_dir+xml_filename.substr(xml_filename.find_last_of( '/' )+1)+"_plot_data_model_"+boost::lexical_cast<std::string>(model_index+1)+"_function_"+boost::lexical_cast<std::string>(f+1)+".dat";
      std::ofstream output(filename.c_str());
      if(!output)
      {
        std::cerr << "Error: could not write to \"" << filename << "\"" << std::endl << std::endl;
        return false;
      }
      for(int m=0; m<plot_n_data_points; ++m)
      {
        for(unsigned int plot_order_index=0; plot_order_index<n_variables; ++plot_order_index)
        {
          output << plot_arguments[m][variable_number[plot_order_index]] << "   ";
        }
        output << average[m][f] << "   " << sigma[m][f] << std::endl;
      }
      output.close();
    }

    // plot fit functions
    for(int f=0; f<n_functions; ++f)
    {
      std::string filename=plot_output_dir+xml_filename.substr(xml_filename.find_last_of( '/' )+1)+"_plot_fit_model_"+boost::lexical_cast<std::string>(model_index+1)+"_function_"+boost::lexical_cast<std::string>(f+1)+".dat";
      std::ofstream output(filename.c_str());
      if(!output)
      {
        std::cerr << "Error: could not write to \"" << filename << "\"" << std::endl << std::endl;
        return false;
      }
      if(n_variables==1)
      {
        std::map< std::string, double > table;
        for(int c=0; c<constant_values.size(); ++c)
        {
          table[constant_names[c]]=constant_values[c];
        }
        parser plot_min_parser_0(plot_min[model_index][variable_number[0]]);
        parser plot_max_parser_0(plot_max[model_index][variable_number[0]]);
        double x_min=plot_min_parser_0.parse(table);
        if(plot_min_parser_0.get_no_of_errors()>0)
        {
          std::cerr << "Error while parsing plotting range" << std::endl << std::endl;
          return false;
        }
        double x_max=plot_max_parser_0.parse(table);
        if(plot_max_parser_0.get_no_of_errors()>0)
        {
          std::cerr << "Error while parsing plotting range" << std::endl << std::endl;
          return false;
        }
        std::vector< double > arg(1);
        for(double x=x_min; x<=x_max; x+=plot_step[model_index][variable_number[0]])
        {
          arg[variable_number[0]]=x;
          output << x << "   " << models[model_index]->eval_function(f, arg) << std::endl;
        }
      }
      else if(n_variables==2)
      {
        std::map< std::string, double > table;
        for(int c=0; c<constant_values.size(); ++c)
        {
          table[constant_names[c]]=constant_values[c];
        }
        parser plot_min_parser_0(plot_min[model_index][variable_number[0]]);
        parser plot_max_parser_0(plot_max[model_index][variable_number[0]]);
        parser plot_min_parser_1(plot_min[model_index][variable_number[1]]);
        parser plot_max_parser_1(plot_max[model_index][variable_number[1]]);
        double x_min=plot_min_parser_0.parse(table);
        if(plot_min_parser_0.get_no_of_errors()>0)
        {
          std::cerr << "Error while parsing plotting range" << std::endl << std::endl;
          return false;
        }
        double x_max=plot_max_parser_0.parse(table);
        if(plot_max_parser_0.get_no_of_errors()>0)
        {
          std::cerr << "Error while parsing plotting range" << std::endl << std::endl;
          return false;
        }
        std::vector< double > arg(2);
        for(double x=x_min; x<=x_max; x+=plot_step[model_index][variable_number[0]])
        {
          arg[variable_number[0]]=x;
          table[models[model_index]->get_variable_name(variable_number[0])]=x;
          double y_min=plot_min_parser_1.parse(table);
          if(plot_min_parser_1.get_no_of_errors()>0)
          {
            std::cerr << "Error while parsing plotting range" << std::endl << std::endl;
            return false;
          }
          double y_max=plot_max_parser_1.parse(table);
          if(plot_max_parser_1.get_no_of_errors()>0)
          {
            std::cerr << "Error while parsing plotting range" << std::endl << std::endl;
            return false;
          }
          for(double y=y_min; y<=y_max; y+=plot_step[model_index][variable_number[1]])
          {
            arg[variable_number[1]]=y;
            output << x << "   " << y << "   " << models[model_index]->eval_function(f, arg) << std::endl;
          }
          output << std::endl;
        }
      }
      else if(n_variables==3)
      {
        std::map< std::string, double > table;
        for(int c=0; c<constant_values.size(); ++c)
        {
          table[constant_names[c]]=constant_values[c];
        }
        parser plot_min_parser_0(plot_min[model_index][variable_number[0]]);
        parser plot_max_parser_0(plot_max[model_index][variable_number[0]]);
        parser plot_min_parser_1(plot_min[model_index][variable_number[1]]);
        parser plot_max_parser_1(plot_max[model_index][variable_number[1]]);
        parser plot_min_parser_2(plot_min[model_index][variable_number[2]]);
        parser plot_max_parser_2(plot_max[model_index][variable_number[2]]);
        double x_min=plot_min_parser_0.parse(table);
        if(plot_min_parser_0.get_no_of_errors()>0)
        {
          std::cerr << "Error while parsing plotting range" << std::endl << std::endl;
          return false;
        }
        double x_max=plot_max_parser_0.parse(table);
        if(plot_max_parser_0.get_no_of_errors()>0)
        {
          std::cerr << "Error while parsing plotting range" << std::endl << std::endl;
          return false;
        }
        std::vector< double > arg(3);
        for(double x=x_min; x<=x_max; x+=plot_step[model_index][variable_number[0]])
        {
          arg[variable_number[0]]=x;
          table[models[model_index]->get_variable_name(variable_number[0])]=x;
          double y_min=plot_min_parser_1.parse(table);
          if(plot_min_parser_1.get_no_of_errors()>0)
          {
            std::cerr << "Error while parsing plotting range" << std::endl << std::endl;
            return false;
          }
          double y_max=plot_max_parser_1.parse(table);
          if(plot_max_parser_1.get_no_of_errors()>0)
          {
            std::cerr << "Error while parsing plotting range" << std::endl << std::endl;
            return false;
          }
          for(double y=y_min; y<=y_max; y+=plot_step[model_index][variable_number[1]])
          {
            arg[variable_number[1]]=y;
            table[models[model_index]->get_variable_name(variable_number[1])]=y;
            double z_min=plot_min_parser_2.parse(table);
            if(plot_min_parser_2.get_no_of_errors()>0)
            {
              std::cerr << "Error while parsing plotting range" << std::endl << std::endl;
              return false;
            }
            double z_max=plot_max_parser_2.parse(table);
            if(plot_max_parser_2.get_no_of_errors()>0)
            {
              std::cerr << "Error while parsing plotting range" << std::endl << std::endl;
              return false;
            }
            for(double z=z_min; z<=z_max; z+=plot_step[model_index][variable_number[2]])
            {
              arg[variable_number[2]]=z;
              output << x << "   " << y << "   " << z << "   " << models[model_index]->eval_function(f, arg) << std::endl;
            }
            output << std::endl;
          }
          output << std::endl;
        }
      }
      else
      {
        std::cerr << "Error: plotting does not support more than 3 variables" << std::endl << std::endl;
        return false;
      }
      output.close();
    }
  }
  return true;
}
