#include "load_data_file_mpi.h"

// load file on root process only, then broadcast


bool load_data_file_ASCII(string file_name, int _n_functions, int _n_variables, vector< vector< vector< double > > >& file_data, vector< vector< double > >& file_arguments)
{
  int proc = MPI::COMM_WORLD.Get_rank();

  ifstream* input=NULL;
  int n_data_points;
  int n_data_sets;

  if(proc == 0) 
  {
    input = new ifstream(file_name.c_str());
    if(!(*input))
    {
      cerr << "Error: Could not open data file " << file_name << endl;
      return false;
    }
  //  cout << "Reading data file " << file_name << " (text format) ..." << endl << endl;
    int n_functions;
    int n_variables;
    *input >> n_functions;
    if(n_functions!=_n_functions)
    {
      cerr << "Error: data file " << file_name << " has wrong number of functions" << endl;
      return false;
    }
    *input >> n_variables;
    if(n_variables!=_n_variables)
    {
      cerr << "Error: data file " << file_name << " has wrong number of variables" << endl;
      return false;
    }
    *input >> n_data_points;
    *input >> n_data_sets;
  }

  MPI::COMM_WORLD.Bcast(&n_data_points, 1, MPI::INT, 0);
  MPI::COMM_WORLD.Bcast(&n_data_sets, 1, MPI::INT, 0);

  double* f_args=new double[n_data_points*_n_variables];
  double* f_data=new double[n_data_sets*n_data_points*_n_functions];

  if(proc == 0)
  {
    int m_buf; double d_buf;
    for(int m=0; m<n_data_points; ++m)
    {
      *input >> m_buf;
      if(m_buf!=m+1)
      {
        cerr << "Error while reading data file " << file_name << endl;
        return false;
      }
      for(int v=0; v<_n_variables; ++v)
      {
        *input >> d_buf;
        f_args[m*_n_variables+v]=d_buf;
      }
    }
    int n_buf;
    for(int n=0; n<n_data_sets; ++n)
    {
      for(int m=0; m<n_data_points; ++m)
      {
        *input >> n_buf >> m_buf;
        if( (n_buf!=n+1) || (m_buf!=m+1) )
        {
          cerr << "Error while reading data file " << file_name << endl;
          return false;
        }
        for(int f=0; f<_n_functions; ++f)
        {
          *input >> d_buf;
          f_data[n*n_data_points*_n_functions+m*_n_functions+f]=d_buf;
        }
      }
    }
    input -> close();
    delete input;
  }
  MPI::COMM_WORLD.Bcast(f_args, n_data_points*_n_variables, MPI::DOUBLE, 0);
  MPI::COMM_WORLD.Bcast(f_data, n_data_sets*n_data_points*_n_functions, MPI::DOUBLE, 0);

  file_arguments.clear();
  file_data.clear();
  vector< double > file_arg_temp(_n_variables, 0.0);
  file_arguments.resize(n_data_points, file_arg_temp);
  vector< double > file_data_temp_1(_n_functions, 0.0);
  vector< vector< double > > file_data_temp_2(n_data_points, file_data_temp_1);
  file_data.resize(n_data_sets, file_data_temp_2);
  for(int m=0; m<n_data_points; ++m)
  {
    for(int v=0; v<_n_variables; ++v)
    {
      file_arguments[m][v]=f_args[m*_n_variables+v];
    }
  }
  for(int n=0; n<n_data_sets; ++n)
  {
    for(int m=0; m<n_data_points; ++m)
    {
      for(int f=0; f<_n_functions; ++f)
      {
        file_data[n][m][f]=f_data[n*n_data_points*_n_functions+m*_n_functions+f];
      }
    }
  }

  delete[] f_args;
  delete[] f_data;

  return true;
}




bool load_data_file_binary(string file_name, int _n_functions, int _n_variables, vector< vector< vector< double > > >& file_data, vector< vector< double > >& file_arguments)
{
  int proc = MPI::COMM_WORLD.Get_rank();

  ifstream* input=NULL;
  int n_data_points;
  int n_data_sets;

  if(proc == 0) 
  {
    input = new ifstream(file_name.c_str(), ios::in | ios::binary);
    if(!(*input))
    {
      cerr << "Error: Could not open data file " << file_name << endl;
      return false;
    }
  //  cout << "Reading data file " << file_name << " (text format) ..." << endl << endl;
    int n_functions;
    int n_variables;

    float temp;
    temp=read_float(*input);
    n_functions=static_cast<int>(temp);
    if(n_functions!=_n_functions)
    {
      cerr << "Error: data file " << file_name << " has wrong number of functions" << endl;
      return false;
    }
    temp=read_float(*input);
    n_variables=static_cast<int>(temp);
    if(n_variables!=_n_variables)
    {
      cerr << "Error: data file " << file_name << " has wrong number of variables" << endl;
      return false;
    }
    temp=read_float(*input);
    n_data_points=static_cast<int>(temp);
    temp=read_float(*input);
    n_data_sets=static_cast<int>(temp);
  }

  MPI::COMM_WORLD.Bcast(&n_data_points, 1, MPI::INT, 0);
  MPI::COMM_WORLD.Bcast(&n_data_sets, 1, MPI::INT, 0);

  double* f_args=new double[n_data_points*_n_variables];
  double* f_data=new double[n_data_sets*n_data_points*_n_functions];

  if(proc == 0)
  {
    for(int m=0; m<n_data_points; ++m)
    {
      for(int v=0; v<_n_variables; ++v)
      {
        f_args[m*_n_variables+v]=read_float(*input);
      }
    }
    for(int n=0; n<n_data_sets; ++n)
    {
      for(int m=0; m<n_data_points; ++m)
      {
        for(int f=0; f<_n_functions; ++f)
        {
          f_data[n*n_data_points*_n_functions+m*_n_functions+f]=read_float(*input);
        }
      }
    }
    input -> close();
    delete input;
  }
  MPI::COMM_WORLD.Bcast(f_args, n_data_points*_n_variables, MPI::DOUBLE, 0);
  MPI::COMM_WORLD.Bcast(f_data, n_data_sets*n_data_points*_n_functions, MPI::DOUBLE, 0);

  file_arguments.clear();
  file_data.clear();
  vector< double > file_arg_temp(_n_variables, 0.0);
  file_arguments.resize(n_data_points, file_arg_temp);
  vector< double > file_data_temp_1(_n_functions, 0.0);
  vector< vector< double > > file_data_temp_2(n_data_points, file_data_temp_1);
  file_data.resize(n_data_sets, file_data_temp_2);
  for(int m=0; m<n_data_points; ++m)
  {
    for(int v=0; v<_n_variables; ++v)
    {
      file_arguments[m][v]=f_args[m*_n_variables+v];
    }
  }
  for(int n=0; n<n_data_sets; ++n)
  {
    for(int m=0; m<n_data_points; ++m)
    {
      for(int f=0; f<_n_functions; ++f)
      {
        file_data[n][m][f]=f_data[n*n_data_points*_n_functions+m*_n_functions+f];
      }
    }
  }

  delete[] f_args;
  delete[] f_data;

  return true;
}
