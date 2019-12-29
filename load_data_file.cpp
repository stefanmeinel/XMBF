#include "load_data_file.h"


bool load_data_file_ASCII(string file_name, int _n_functions, int _n_variables, vector< vector< vector< double > > >& file_data, vector< vector< double > >& file_arguments)
{
  ifstream input(file_name.c_str());
  if(!input)
  {
    cerr << "Error: Could not open data file " << file_name << endl;
    return false;
  }
//  cout << "Reading data file " << file_name << " (text format) ..." << endl << endl;
  int n_functions;
  int n_variables;
  int n_data_points;
  int n_data_sets;
  input >> n_functions;
  if(n_functions!=_n_functions)
  {
    cerr << "Error: data file " << file_name << " has wrong number of functions" << endl;
    return false;
  }
  input >> n_variables;
  if(n_variables!=_n_variables)
  {
    cerr << "Error: data file " << file_name << " has wrong number of variables" << endl;
    return false;
  }
  input >> n_data_points;
  input >> n_data_sets;
  file_arguments.clear();
  file_data.clear();
  vector< double > file_arg_temp(n_variables, 0.0);
  file_arguments.resize(n_data_points, file_arg_temp);
  vector< double > file_data_temp_1(n_functions, 0.0);
  vector< vector< double > > file_data_temp_2(n_data_points, file_data_temp_1);
  file_data.resize(n_data_sets, file_data_temp_2);
  int m_buf;
  for(int m=0; m<n_data_points; ++m)
  {
    input >> m_buf;
    if(m_buf!=m+1)
    {
      cerr << "Error while reading data file " << file_name << endl;
      return false;
    }
    for(int v=0; v<n_variables; ++v)
    {
      input >> file_arguments[m][v];
    }
  }
  int n_buf;
  for(int n=0; n<n_data_sets; ++n)
  {
    for(int m=0; m<n_data_points; ++m)
    {
      input >> n_buf >> m_buf;
      if( (n_buf!=n+1) || (m_buf!=m+1) )
      {
        cerr << "Error while reading data file " << file_name << endl;
        return false;
      }
      for(int f=0; f<n_functions; ++f)
      {
        input >> file_data[n][m][f];
      }
    }
  }
  input.close();
  return true;
}


bool load_data_file_binary(string file_name, int _n_functions, int _n_variables, vector< vector< vector< double > > >& file_data, vector< vector< double > >& file_arguments)
{
  ifstream input(file_name.c_str(), ios::in | ios::binary);
  if(!input)
  {
    cerr << "Error: Could not open data file " << file_name << endl;
    return false;
  }
//  cout << "Reading data file " << file_name << " (binary 32 bit little endian format) ..." << endl << endl;
  int n_functions;
  int n_variables;
  int n_data_points;
  int n_data_sets;
  float temp;
  temp=read_float(input);
  n_functions=static_cast<int>(temp);
  if(n_functions!=_n_functions)
  {
    cerr << "Error: data file " << file_name << " has wrong number of functions" << endl;
    return false;
  }
  temp=read_float(input);
  n_variables=static_cast<int>(temp);
  if(n_variables!=_n_variables)
  {
    cerr << "Error: data file " << file_name << " has wrong number of variables" << endl;
    return false;
  }
  temp=read_float(input);
  n_data_points=static_cast<int>(temp);
  temp=read_float(input);
  n_data_sets=static_cast<int>(temp);
  file_arguments.clear();
  file_data.clear();
  vector< double > file_arg_temp(n_variables, 0.0);
  file_arguments.resize(n_data_points, file_arg_temp);
  vector< double > file_data_temp_1(n_functions, 0.0);
  vector< vector< double > > file_data_temp_2(n_data_points, file_data_temp_1);
  file_data.resize(n_data_sets, file_data_temp_2);
  for(int m=0; m<n_data_points; ++m)
  {
    for(int v=0; v<n_variables; ++v)
    {
      file_arguments[m][v]=read_float(input);
    }
  }
  for(int n=0; n<n_data_sets; ++n)
  {
    for(int m=0; m<n_data_points; ++m)
    {
      for(int f=0; f<n_functions; ++f)
      {
       file_data[n][m][f]=read_float(input);
      }
    }
  }
  input.close();
  return true;
}
