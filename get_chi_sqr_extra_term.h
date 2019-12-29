#ifndef GET_CHI_SQR_EXTRA_TERM_H
#define GET_CHI_SQR_EXTRA_TERM_H

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <libxml++/libxml++.h>
#include "xml_tools.h"
#include <utility>

bool get_chi_sqr_extra_term(std::string& function,
                            std::vector< std::string >& constant_names,
                            std::vector< double >& constant_values,
                            double& num_diff_step,
                            bool& mixed_second_derivatives_specified,
                            std::vector< std::pair< std::string, std::string > >& mixed_second_derivatives,
                            const xmlpp::Node* root_node);

#endif
