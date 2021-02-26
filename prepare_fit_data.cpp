#include "prepare_fit_data.h"

bool prepare_fit_data(std::vector<abstract_model*> models,
                      const std::vector< double >& constant_values,
                      const std::vector< std::string >& constant_names,
                      int bin_size, bool restrict_data, int start_n_data, int stop_n_data,
                      const std::vector< std::vector< std::vector< std::string > > >& fit_min,
                      const std::vector< std::vector< std::vector< std::string > > >& fit_max,
                      const std::vector< std::vector< std::vector< bool > > >& has_fit_step,
                      const std::vector< std::vector< std::vector< double > > >& fit_step,
                      const std::vector< std::vector< std::vector< std::vector< double > > > >& all_file_data,
                      const std::vector< std::vector< std::vector< double > > >& all_file_arguments,
                      std::vector< std::vector< std::vector< std::vector< double > > > >& all_fit_data,
                      std::vector< std::vector< std::vector< double > > >& all_fit_arguments)
{
  all_fit_data.clear();
  all_fit_arguments.clear();

  int n_data_sets=all_file_data[0].size();
  for(unsigned int model_index=1; model_index<models.size(); ++model_index)
  {
    if(all_file_data[model_index].size()!=n_data_sets)
    {
      std::cerr << "error: number of data sets not equal" << std::endl << std::endl;
      return false;
    }
  }

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

  for(unsigned int model_index=0; model_index<models.size(); ++model_index)
  {
    int n_data_points=all_file_arguments[model_index].size();
    int n_variables=models[model_index] -> get_n_variables();
    int n_functions=models[model_index] -> get_n_functions();

    // select data points in fitting range
    std::vector< std::vector< parser* > > all_fit_min_parsers;
    std::vector< std::vector< parser* > > all_fit_max_parsers;

    for(int v=0; v<n_variables; ++v)
    {
      std::vector< parser* > fit_min_parsers;
      std::vector< parser* > fit_max_parsers;
      for(unsigned int range_index=0; range_index<fit_min[model_index][v].size(); ++range_index)
      {
        fit_min_parsers.push_back(new parser(fit_min[model_index][v][range_index]));
        fit_max_parsers.push_back(new parser(fit_max[model_index][v][range_index]));
      }
      all_fit_min_parsers.push_back(fit_min_parsers);
      all_fit_max_parsers.push_back(fit_max_parsers);
    }

    std::map< std::string, double > table;
    for(int c=0; c<constant_values.size(); ++c)
    {
      table[constant_names[c]]=constant_values[c];
    }

    std::vector< std::vector< double > > fit_arguments;
    std::vector< int > indices;
    for(int m=0; m<n_data_points; ++m)
    {
      for(int v=0; v<n_variables; ++v)
      {
        table[models[model_index]->get_variable_name(v)]=all_file_arguments[model_index][m][v];
      }
      bool in_range=true;
      for(int v=0; v<n_variables; ++v)
      {
        bool v_in_range=false;
        double arg=all_file_arguments[model_index][m][v];
        for(unsigned int range_index=0; range_index<fit_min[model_index][v].size(); ++range_index)
        {
          double min=all_fit_min_parsers[v][range_index]->parse(table);
          double max=all_fit_max_parsers[v][range_index]->parse(table);
          if((all_fit_min_parsers[v][range_index]->get_no_of_errors()>0) || (all_fit_max_parsers[v][range_index]->get_no_of_errors()>0))
          {
            for(std::vector< std::vector< parser* > >::iterator it=all_fit_min_parsers.begin(); it!=all_fit_min_parsers.end(); ++it)
            {
              for(std::vector< parser* >::iterator it2=(*it).begin(); it2!=(*it).end(); ++it2)
              {
                delete *it2;
              }
            }
            for(std::vector< std::vector< parser* > >::iterator it=all_fit_max_parsers.begin(); it!=all_fit_max_parsers.end(); ++it)
            {
              for(std::vector< parser* >::iterator it2=(*it).begin(); it2!=(*it).end(); ++it2)
              {
                delete *it2;
              }
            }
            std::cerr << "Error while parsing fitting range" << std::endl << std::endl;
            return false;
          }
          if(has_fit_step[model_index][v][range_index])
          {
            for(double d=min; d<=max; d+=fit_step[model_index][v][range_index])
            {
              if(d==arg)
              {
                v_in_range=true;
                break;
              }
            }
          }
          else if( (min<=arg) && (arg<=max) )
          {
            v_in_range=true;
          }
        }
        if(!v_in_range)
        {
          in_range=false;
        }
      }
      if(in_range)
      {
        fit_arguments.push_back(all_file_arguments[model_index][m]);
        indices.push_back(m);
      }
    }
    for(std::vector< std::vector< parser* > >::iterator it=all_fit_min_parsers.begin(); it!=all_fit_min_parsers.end(); ++it)
    {
      for(std::vector< parser* >::iterator it2=(*it).begin(); it2!=(*it).end(); ++it2)
      {
        delete *it2;
      }
    }
    for(std::vector< std::vector< parser* > >::iterator it=all_fit_max_parsers.begin(); it!=all_fit_max_parsers.end(); ++it)
    {
      for(std::vector< parser* >::iterator it2=(*it).begin(); it2!=(*it).end(); ++it2)
      {
        delete *it2;
      }
    }
    // end select data points in fitting range

    int fit_n_data_points=fit_arguments.size();
    if(fit_n_data_points==0)
    {
      std::cerr << "Error: No data points in fitting range" << std::endl << std::endl;
      return false;
    }
    if(fit_n_data_sets/bin_size<5)
    {
      std::cerr << "Error: fewer than 5 data samples (after binning)" << std::endl << std::endl;
      return false;
    }

    std::vector< double > fit_data_temp_1(n_functions, 0.0);
    std::vector< std::vector< double > > fit_data_temp_2(fit_n_data_points, fit_data_temp_1);
    std::vector< std::vector< std::vector< double > > > fit_data(fit_n_data_sets/bin_size, fit_data_temp_2);

    for(int m=0; m<fit_n_data_points; ++m)
    {
      for(int f=0; f<n_functions; ++f)
      {
        for(int b=0; b<fit_n_data_sets/bin_size; ++b)
        {
          // average over entries e in bin b
          fit_data[b][m][f]=0.0;
          for(int e=0; e<bin_size; ++e)
          {
            fit_data[b][m][f]+=all_file_data[model_index][start_n_data-1+b*bin_size+e][indices[m]][f];
          }
          fit_data[b][m][f]/=bin_size;
        }
      }
    }

    all_fit_arguments.push_back(fit_arguments);
    all_fit_data.push_back(fit_data);

  }
  return true;
}



bool combine_data(const std::vector< std::vector< std::vector< std::vector< double > > > >& all_fit_data,
                  std::vector< std::vector< double > > & global_fit_data)
{
  global_fit_data.clear();
  int n_fit_points=0;
  for(unsigned int model_index=0; model_index<all_fit_data.size(); ++model_index)
  {
    int n_data_points=all_fit_data[model_index][0].size();
    int n_functions=all_fit_data[model_index][0][0].size();
    n_fit_points+=n_data_points*n_functions;
  }
  int n_data_sets=all_fit_data[0].size();
  for(unsigned int model_index=1; model_index<all_fit_data.size(); ++model_index)
  {
    if(all_fit_data[model_index].size()!=n_data_sets)
    {
      std::cerr << "error: number of data sets not equal" << std::endl << std::endl;
      return false;
    }
  }
  std::vector< double > tmp_vec(n_fit_points);
  global_fit_data.resize(n_data_sets, tmp_vec);

  for(int n=0; n<n_data_sets; ++n)
  {
    int fit_point=0;
    for(unsigned int model_index=0; model_index<all_fit_data.size(); ++model_index)
    {
      int n_data_points=all_fit_data[model_index][0].size();
      int n_functions=all_fit_data[model_index][0][0].size();
      for(int m=0; m<n_data_points; ++m)
      {
        for(int f=0; f<n_functions; ++f)
        {
          global_fit_data[n][fit_point]=all_fit_data[model_index][n][m][f];
          ++fit_point;
        }
      }
    }
  }
  return true;
}
