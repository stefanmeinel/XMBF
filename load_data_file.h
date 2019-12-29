#ifndef LOAD_DATA_FILE_H
#define LOAD_DATA_FILE_H

#include <iostream>
#include <fstream>
#include <vector>

#include "binary_io.h"

using namespace std;

bool load_data_file_ASCII(string file_name, int _n_functions, int _n_variables, vector< vector< vector< double > > >& file_data, vector< vector< double > >& file_arguments);

bool load_data_file_binary(string file_name, int _n_functions, int _n_variables, vector< vector< vector< double > > >& file_data, vector< vector< double > >& file_arguments);

#endif
