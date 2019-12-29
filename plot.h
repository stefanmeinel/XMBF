#ifndef PLOT_H
#define PLOT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <boost/lexical_cast.hpp>

#include "abstract_model.h"
#include "parser.h"


// this assumes all models have parameters set
bool plot(std::vector<abstract_model*> models,
          const std::vector< double >& constant_values,
          const std::vector< std::string >& constant_names,
          const std::vector< std::vector< int > >& plot_order,
          const std::vector< std::vector< std::string > >& plot_min,
          const std::vector< std::vector< std::string > >& plot_max,
          const std::vector< std::vector< double > >& plot_step,
          int bin_size, bool restrict_data, int start_n_data, int stop_n_data,
          bool bootstrap_normalization,
          const std::vector< std::vector< std::vector< std::vector< double > > > >& all_file_data,
          const std::vector< std::vector< std::vector< double > > >& all_file_arguments,
          const std::string& plot_output_dir,
          const std::string& xml_filename);

#endif
