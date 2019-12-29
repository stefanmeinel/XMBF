#ifndef CREATE_MODELS_H
#define CREATE_MODELS_H

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <libxml++/libxml++.h>

#include "xml_tools.h"

#include "load_data_file.h"

#include "abstract_model.h"

#include "parse_model.h"
#include "multi_exp_model.h"
#include "multi_exp_expE_model.h"
#include "multi_exp_Asqr_model.h"
#include "multi_exp_Asqr_expE_model.h"
#include "multi_alt_exp_model.h"
#include "multi_alt_exp_expE_model.h"
#include "multi_alt_exp_Asqr_model.h"
#include "multi_alt_exp_Asqr_expE_model.h"
#include "multi_exp_vec_model.h"
#include "multi_exp_expE_vec_model.h"
#include "multi_exp_Asqr_vec_model.h"
#include "multi_exp_Asqr_expE_vec_model.h"
#include "multi_alt_exp_vec_model.h"
#include "multi_alt_exp_expE_vec_model.h"
#include "multi_alt_exp_Asqr_vec_model.h"
#include "multi_alt_exp_Asqr_expE_vec_model.h"
#include "multi_exp_BC_model.h"
#include "multi_exp_expE_BC_model.h"
#include "multi_exp_Asqr_BC_model.h"
#include "multi_exp_Asqr_expE_BC_model.h"
#include "multi_alt_exp_BC_model.h"
#include "multi_alt_exp_expE_BC_model.h"
#include "multi_alt_exp_Asqr_BC_model.h"
#include "multi_alt_exp_Asqr_expE_BC_model.h"
#include "multi_exp_vec_BC_model.h"
#include "multi_exp_expE_vec_BC_model.h"
#include "multi_exp_Asqr_vec_BC_model.h"
#include "multi_exp_Asqr_expE_vec_BC_model.h"
#include "multi_alt_exp_vec_BC_model.h"
#include "multi_alt_exp_expE_vec_BC_model.h"
#include "multi_alt_exp_Asqr_vec_BC_model.h"
#include "multi_alt_exp_Asqr_expE_vec_BC_model.h"
#include "multi_exp_const_model.h"
#include "multi_exp_expE_const_model.h"
#include "multi_exp_Asqr_const_model.h"
#include "multi_exp_Asqr_expE_const_model.h"
#include "multi_alt_exp_const_model.h"
#include "multi_alt_exp_expE_const_model.h"
#include "multi_alt_exp_Asqr_const_model.h"
#include "multi_alt_exp_Asqr_expE_const_model.h"
#include "multi_exp_vec_const_model.h"
#include "multi_exp_expE_vec_const_model.h"
#include "multi_exp_Asqr_vec_const_model.h"
#include "multi_exp_Asqr_expE_vec_const_model.h"
#include "multi_alt_exp_vec_const_model.h"
#include "multi_alt_exp_expE_vec_const_model.h"
#include "multi_alt_exp_Asqr_vec_const_model.h"
#include "multi_alt_exp_Asqr_expE_vec_const_model.h"
#include "multi_exp_BC_const_model.h"
#include "multi_exp_expE_BC_const_model.h"
#include "multi_exp_Asqr_BC_const_model.h"
#include "multi_exp_Asqr_expE_BC_const_model.h"
#include "multi_alt_exp_BC_const_model.h"
#include "multi_alt_exp_expE_BC_const_model.h"
#include "multi_alt_exp_Asqr_BC_const_model.h"
#include "multi_alt_exp_Asqr_expE_BC_const_model.h"
#include "multi_exp_vec_BC_const_model.h"
#include "multi_exp_expE_vec_BC_const_model.h"
#include "multi_exp_Asqr_vec_BC_const_model.h"
#include "multi_exp_Asqr_expE_vec_BC_const_model.h"
#include "multi_alt_exp_vec_BC_const_model.h"
#include "multi_alt_exp_expE_vec_BC_const_model.h"
#include "multi_alt_exp_Asqr_vec_BC_const_model.h"
#include "multi_alt_exp_Asqr_expE_vec_BC_const_model.h"
#include "multi_exp_mat_model.h"
#include "multi_exp_mat_upper_model.h"
#include "multi_exp_mat_II_model.h"
#include "multi_exp_mat_II_upper_model.h"
#include "multi_exp_expE_mat_model.h"
#include "multi_exp_expE_mat_upper_model.h"
#include "multi_exp_expE_mat_II_model.h"
#include "multi_exp_expE_mat_II_upper_model.h"
#include "multi_alt_exp_mat_model.h"
#include "multi_alt_exp_expE_mat_model.h"
#include "multi_exp_nonsym_mat_model.h"
#include "multi_exp_expE_nonsym_mat_model.h"
#include "multi_alt_exp_nonsym_mat_model.h"
#include "multi_alt_exp_expE_nonsym_mat_model.h"
#include "multi_exp_2var_model.h"
#include "multi_exp_expE_2var_model.h"
#include "threept_constant_model.h"
#include "threept_constant_sqr_model.h"
#include "threept_multi_exp_model.h"
#include "threept_multi_alt_exp_model.h"
#include "threept_multi_exp_expE_model.h"
#include "threept_multi_alt_exp_expE_model.h"
#include "threept_multi_exp_vec_model.h"
#include "threept_multi_exp_expE_vec_model.h"
#include "threept_multi_alt_exp_vec_model.h"
#include "threept_multi_alt_exp_expE_vec_model.h"
#include "multi_part_exp_expE_model.h"

bool create_models(std::vector<abstract_model*>& models,
                   std::vector<int>& model_groups,
                   std::vector< std::vector< std::vector< std::string > > >& fit_min,
                   std::vector< std::vector< std::vector< std::string > > >& fit_max,
                   std::vector< std::vector< std::vector< bool > > >& has_fit_step,
                   std::vector< std::vector< std::vector< double > > >& fit_step,
                   std::vector< std::vector< std::vector< bool > > >& has_range_bootstrap_file,
                   std::vector< std::vector< std::vector< std::string > > >& range_bootstrap_file,
                   bool perform_plot,
                   std::vector< std::vector< int > >& plot_order,
                   std::vector< std::vector< std::string > >& plot_min,
                   std::vector< std::vector< std::string > >& plot_max,
                   std::vector< std::vector< double > >& plot_step,
                   std::vector< std::vector< std::vector< std::vector< double > > > >& all_file_data,
                   std::vector< std::vector< std::vector< double > > >& all_file_arguments,
                   bool num_diff,
                   const xmlpp::Node* root_node);

#endif
