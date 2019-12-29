#ifndef PREPARE_FIT_DATA_H
#define PREPARE_FIT_DATA_H

#include <vector>
#include <string>
#include <iostream>
#include <map>

#include "abstract_model.h"
#include "parser.h"

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
                      std::vector< std::vector< std::vector< double > > >& all_fit_arguments);

bool combine_data(const std::vector< std::vector< std::vector< std::vector< double > > > >& all_fit_data,
                  std::vector< std::vector< double > > & global_fit_data);

#endif
