#ifndef GET_PARAMETERS_H
#define GET_PARAMETERS_H

#include <vector>
#include <string>
#include <iostream>
#include <libxml++/libxml++.h>
#include "xml_tools.h"
#include "combined_model.h"

bool get_parameters(combined_model* c_model,
                    bool bayesian,
                    std::vector< std::string >& parameter_names,
                    std::vector< double >& start_values,
                    std::vector< double >& priors,
                    std::vector< double >& sigmas,
                    const xmlpp::Node* root_node);

#endif
