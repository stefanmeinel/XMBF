#ifndef GET_CONSTANTS_H
#define GET_CONSTANTS_H

#include <vector>
#include <string>
#include <iostream>
#include <libxml++/libxml++.h>
#include "xml_tools.h"
#include "combined_model.h"

bool get_constants(combined_model* c_model,
                   std::vector< std::string >& constant_names,
                   std::vector< double >& constant_values,
                   const xmlpp::Node* root_node);

#endif
