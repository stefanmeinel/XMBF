#include "create_models.h"


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
                   const xmlpp::Node* root_node)
{
  models.clear();
  model_groups.clear();
  all_file_data.clear();
  all_file_arguments.clear();
  fit_min.clear();
  fit_max.clear();
  has_fit_step.clear();
  fit_step.clear();
  plot_order.clear();
  plot_min.clear();
  plot_max.clear();
  plot_step.clear();

  xmlpp::NodeSet combined_model_nodeset = root_node->find("combined_model");
  if(combined_model_nodeset.size()!=1)
  {
    std::cerr << "Error: \"combined_model\" did not return unique node" << std::endl << std::endl;
    return false;
  }

  xmlpp::Node::NodeList model_list = (*combined_model_nodeset.begin())->get_children();
  if(model_list.empty())
  {
    std::cerr << "Error: no models found" << std::endl << std::endl;
    return false;
  }

  for(xmlpp::Node::NodeList::iterator model_list_iter = model_list.begin(); model_list_iter != model_list.end(); ++model_list_iter)
  {
    if(is_element(*model_list_iter))
    {
      std::string name=(*model_list_iter)->get_name();

      abstract_model* model = NULL;
      
      int model_group=0;
      if(name_exists(*model_list_iter, "model_group"))
      {
        try { model_group=get_int(*model_list_iter, "model_group"); } catch (int e) { return false; }
      }

      if(name=="multi_exp_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        model = new multi_exp_model(E_name, dE_name, A_name, B_name, t_name, n_exp);
      }
      else if(name=="multi_exp_expE_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        model = new multi_exp_expE_model(E_name, dE_name, A_name, B_name, t_name, n_exp);
      }
      else if(name=="multi_exp_Asqr_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        model = new multi_exp_Asqr_model(E_name, dE_name, A_name, B_name, t_name, n_exp);
      }
      else if(name=="multi_exp_Asqr_expE_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        model = new multi_exp_Asqr_expE_model(E_name, dE_name, A_name, B_name, t_name, n_exp);
      }
      else if(name=="multi_alt_exp_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_model(E_name, dE_name, A_name, B_name, t_name, n_exp, n_o_exp);
      }
      else if(name=="multi_alt_exp_expE_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_expE_model(E_name, dE_name, A_name, B_name, t_name, n_exp, n_o_exp);
      }
      else if(name=="multi_alt_exp_Asqr_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_Asqr_model(E_name, dE_name, A_name, B_name, t_name, n_exp, n_o_exp);
      }
      else if(name=="multi_alt_exp_Asqr_expE_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_Asqr_expE_model(E_name, dE_name, A_name, B_name, t_name, n_exp, n_o_exp);
      }
      else if(name=="multi_exp_vec_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_exp_vec_model(E_name, dE_name, A_name, B_name, t_name, n_exp, dim);
      }
      else if(name=="multi_exp_expE_vec_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_exp_expE_vec_model(E_name, dE_name, A_name, B_name, t_name, n_exp, dim);
      }
      else if(name=="multi_exp_Asqr_vec_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_exp_Asqr_vec_model(E_name, dE_name, A_name, B_name, t_name, n_exp, dim);
      }
      else if(name=="multi_exp_Asqr_expE_vec_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_exp_Asqr_expE_vec_model(E_name, dE_name, A_name, B_name, t_name, n_exp, dim);
      }
      else if(name=="multi_alt_exp_vec_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_alt_exp_vec_model(E_name, dE_name, A_name, B_name, t_name, n_exp, n_o_exp, dim);
      }
      else if(name=="multi_alt_exp_expE_vec_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_alt_exp_expE_vec_model(E_name, dE_name, A_name, B_name, t_name, n_exp, n_o_exp, dim);
      }
      else if(name=="multi_alt_exp_Asqr_vec_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_alt_exp_Asqr_vec_model(E_name, dE_name, A_name, B_name, t_name, n_exp, n_o_exp, dim);
      }
      else if(name=="multi_alt_exp_Asqr_expE_vec_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_alt_exp_Asqr_expE_vec_model(E_name, dE_name, A_name, B_name, t_name, n_exp, n_o_exp, dim);
      }
      else if(name=="multi_exp_BC_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_exp_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, n_exp);
      }
      else if(name=="multi_exp_expE_BC_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_exp_expE_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, n_exp);
      }
      else if(name=="multi_exp_Asqr_BC_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_exp_Asqr_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, n_exp);
      }
      else if(name=="multi_exp_Asqr_expE_BC_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_exp_Asqr_expE_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, n_exp);
      }
      else if(name=="multi_alt_exp_BC_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, n_exp, n_o_exp);
      }
      else if(name=="multi_alt_exp_expE_BC_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_expE_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, n_exp, n_o_exp);
      }
      else if(name=="multi_alt_exp_Asqr_BC_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_Asqr_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, n_exp, n_o_exp);
      }
      else if(name=="multi_alt_exp_Asqr_expE_BC_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_Asqr_expE_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, n_exp, n_o_exp);
      }
      else if(name=="multi_exp_vec_BC_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_exp_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, n_exp, dim);
      }
      else if(name=="multi_exp_expE_vec_BC_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_exp_expE_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, n_exp, dim);
      }
      else if(name=="multi_exp_Asqr_vec_BC_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_exp_Asqr_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, n_exp, dim);
      }
      else if(name=="multi_exp_Asqr_expE_vec_BC_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_exp_Asqr_expE_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, n_exp, dim);
      }
      else if(name=="multi_alt_exp_vec_BC_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, n_exp, n_o_exp, dim);
      }
      else if(name=="multi_alt_exp_expE_vec_BC_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_expE_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, n_exp, n_o_exp, dim);
      }
      else if(name=="multi_alt_exp_Asqr_vec_BC_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_Asqr_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, n_exp, n_o_exp, dim);
      }
      else if(name=="multi_alt_exp_Asqr_expE_vec_BC_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_Asqr_expE_vec_BC_model(E_name, dE_name, A_name, B_name, t_name, T_name, n_exp, n_o_exp, dim);
      }
      else if(name=="multi_exp_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        model = new multi_exp_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, n_exp);
      }
      else if(name=="multi_exp_expE_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        model = new multi_exp_expE_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, n_exp);
      }
      else if(name=="multi_exp_Asqr_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        model = new multi_exp_Asqr_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, n_exp);
      }
      else if(name=="multi_exp_Asqr_expE_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        model = new multi_exp_Asqr_expE_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, n_exp);
      }
      else if(name=="multi_alt_exp_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, n_exp, n_o_exp);
      }
      else if(name=="multi_alt_exp_expE_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_expE_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, n_exp, n_o_exp);
      }
      else if(name=="multi_alt_exp_Asqr_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_Asqr_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, n_exp, n_o_exp);
      }
      else if(name=="multi_alt_exp_Asqr_expE_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_Asqr_expE_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, n_exp, n_o_exp);
      }
      else if(name=="multi_exp_vec_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_exp_vec_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, n_exp, dim);
      }
      else if(name=="multi_exp_expE_vec_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_exp_expE_vec_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, n_exp, dim);
      }
      else if(name=="multi_exp_Asqr_vec_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_exp_Asqr_vec_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, n_exp, dim);
      }
      else if(name=="multi_exp_Asqr_expE_vec_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_exp_Asqr_expE_vec_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, n_exp, dim);
      }
      else if(name=="multi_alt_exp_vec_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_alt_exp_vec_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, n_exp, n_o_exp, dim);
      }
      else if(name=="multi_alt_exp_expE_vec_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_alt_exp_expE_vec_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, n_exp, n_o_exp, dim);
      }
      else if(name=="multi_alt_exp_Asqr_vec_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_alt_exp_Asqr_vec_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, n_exp, n_o_exp, dim);
      }
      else if(name=="multi_alt_exp_Asqr_expE_vec_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_alt_exp_Asqr_expE_vec_const_model(E_name, dE_name, A_name, B_name, t_name, C_name, n_exp, n_o_exp, dim);
      }
      else if(name=="multi_exp_BC_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_exp_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, n_exp);
      }
      else if(name=="multi_exp_expE_BC_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_exp_expE_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, n_exp);
      }
      else if(name=="multi_exp_Asqr_BC_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_exp_Asqr_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, n_exp);
      }
      else if(name=="multi_exp_Asqr_expE_BC_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_exp_Asqr_expE_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, n_exp);
      }
      else if(name=="multi_alt_exp_BC_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, n_exp, n_o_exp);
      }
      else if(name=="multi_alt_exp_expE_BC_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_expE_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, n_exp, n_o_exp);
      }
      else if(name=="multi_alt_exp_Asqr_BC_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_Asqr_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, n_exp, n_o_exp);
      }
      else if(name=="multi_alt_exp_Asqr_expE_BC_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_Asqr_expE_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, n_exp, n_o_exp);
      }
      else if(name=="multi_exp_vec_BC_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_exp_vec_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, n_exp, dim);
      }
      else if(name=="multi_exp_expE_vec_BC_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_exp_expE_vec_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, n_exp, dim);
      }
      else if(name=="multi_exp_Asqr_vec_BC_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_exp_Asqr_vec_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, n_exp, dim);
      }
      else if(name=="multi_exp_Asqr_expE_vec_BC_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_exp_Asqr_expE_vec_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, n_exp, dim);
      }
      else if(name=="multi_alt_exp_vec_BC_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_vec_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, n_exp, n_o_exp, dim);
      }
      else if(name=="multi_alt_exp_expE_vec_BC_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_expE_vec_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, n_exp, n_o_exp, dim);
      }
      else if(name=="multi_alt_exp_Asqr_vec_BC_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_Asqr_vec_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, n_exp, n_o_exp, dim);
      }
      else if(name=="multi_alt_exp_Asqr_expE_vec_BC_const_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string C_name;  try { C_name=get_string(*model_list_iter, "C_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_alt_exp_Asqr_expE_vec_BC_const_model(E_name, dE_name, A_name, B_name, t_name, T_name, C_name, n_exp, n_o_exp, dim);
      }
      else if(name=="multi_exp_mat_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim_1;           try { dim_1=get_positive_int(*model_list_iter, "dim_1");        } catch (int e) { return false; }
        int dim_2;           try { dim_2=get_positive_int(*model_list_iter, "dim_2");        } catch (int e) { return false; }
        model = new multi_exp_mat_model(E_name, dE_name, A_name, B_name, t_name, n_exp, dim_1, dim_2);
      }
      else if(name=="multi_exp_mat_upper_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_exp_mat_upper_model(E_name, dE_name, A_name, B_name, t_name, n_exp, dim);
      }
      else if(name=="multi_exp_mat_II_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim_1;           try { dim_1=get_positive_int(*model_list_iter, "dim_1");        } catch (int e) { return false; }
        int dim_2;           try { dim_2=get_positive_int(*model_list_iter, "dim_2");        } catch (int e) { return false; }
        model = new multi_exp_mat_II_model(E_name, dE_name, A_name, B_name, t_name, n_exp, dim_1, dim_2);
      }
      else if(name=="multi_exp_mat_II_upper_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_exp_mat_II_upper_model(E_name, dE_name, A_name, B_name, t_name, n_exp, dim);
      }
      else if(name=="multi_exp_expE_mat_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim_1;           try { dim_1=get_positive_int(*model_list_iter, "dim_1");        } catch (int e) { return false; }
        int dim_2;           try { dim_2=get_positive_int(*model_list_iter, "dim_2");        } catch (int e) { return false; }
        model = new multi_exp_expE_mat_model(E_name, dE_name, A_name, B_name, t_name, n_exp, dim_1, dim_2);
      }
      else if(name=="multi_exp_expE_mat_upper_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_exp_expE_mat_upper_model(E_name, dE_name, A_name, B_name, t_name, n_exp, dim);
      }
      else if(name=="multi_exp_expE_mat_II_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim_1;           try { dim_1=get_positive_int(*model_list_iter, "dim_1");        } catch (int e) { return false; }
        int dim_2;           try { dim_2=get_positive_int(*model_list_iter, "dim_2");        } catch (int e) { return false; }
        model = new multi_exp_expE_mat_II_model(E_name, dE_name, A_name, B_name, t_name, n_exp, dim_1, dim_2);
      }
      else if(name=="multi_exp_expE_mat_II_upper_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim;             try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new multi_exp_expE_mat_II_upper_model(E_name, dE_name, A_name, B_name, t_name, n_exp, dim);
      }
      else if(name=="multi_alt_exp_mat_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim_1;           try { dim_1=get_positive_int(*model_list_iter, "dim_1");        } catch (int e) { return false; }
        int dim_2;           try { dim_2=get_positive_int(*model_list_iter, "dim_2");        } catch (int e) { return false; }
        model = new multi_alt_exp_mat_model(E_name, dE_name, A_name, B_name, t_name, n_exp, n_o_exp, dim_1, dim_2);
      }
      else if(name=="multi_alt_exp_expE_mat_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim_1;           try { dim_1=get_positive_int(*model_list_iter, "dim_1");        } catch (int e) { return false; }
        int dim_2;           try { dim_2=get_positive_int(*model_list_iter, "dim_2");        } catch (int e) { return false; }
        model = new multi_alt_exp_expE_mat_model(E_name, dE_name, A_name, B_name, t_name, n_exp, n_o_exp, dim_1, dim_2);
      }
      else if(name=="multi_exp_nonsym_mat_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim_1;           try { dim_1=get_positive_int(*model_list_iter, "dim_1");        } catch (int e) { return false; }
        if(dim_1<2)
        {
          std::cerr << "Error: \"multi_exp_nonsym_mat_model/dim_1\" must be larger than or equal to 2" << std::endl << std::endl;
          return false;
        }
        int dim_2;           try { dim_2=get_positive_int(*model_list_iter, "dim_2");        } catch (int e) { return false; }
        model = new multi_exp_nonsym_mat_model(E_name, dE_name, A_name, B_name, t_name, n_exp, dim_1, dim_2);
      }
      else if(name=="multi_exp_expE_nonsym_mat_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim_1;           try { dim_1=get_positive_int(*model_list_iter, "dim_1");        } catch (int e) { return false; }
        if(dim_1<2)
        {
          std::cerr << "Error: \"multi_exp_expE_nonsym_mat_model/dim_1\" must be larger than or equal to 2" << std::endl << std::endl;
          return false;
        }
        int dim_2;           try { dim_2=get_positive_int(*model_list_iter, "dim_2");        } catch (int e) { return false; }
        model = new multi_exp_expE_nonsym_mat_model(E_name, dE_name, A_name, B_name, t_name, n_exp, dim_1, dim_2);
      }
      else if(name=="multi_alt_exp_nonsym_mat_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim_1;           try { dim_1=get_positive_int(*model_list_iter, "dim_1");        } catch (int e) { return false; }
        if(dim_1<2)
        {
          std::cerr << "Error: \"multi_alt_exp_nonsym_mat_model/dim_1\" must be larger than or equal to 2" << std::endl << std::endl;
          return false;
        }
        int dim_2;           try { dim_2=get_positive_int(*model_list_iter, "dim_2");        } catch (int e) { return false; }
        model = new multi_alt_exp_nonsym_mat_model(E_name, dE_name, A_name, B_name, t_name, n_exp, n_o_exp, dim_1, dim_2);
      }
      else if(name=="multi_alt_exp_expE_nonsym_mat_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_o_exp;         try { n_o_exp=get_positive_int(*model_list_iter, "n_o_exp");    } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        int dim_1;           try { dim_1=get_positive_int(*model_list_iter, "dim_1");        } catch (int e) { return false; }
        if(dim_1<2)
        {
          std::cerr << "Error: \"multi_alt_exp_expE_nonsym_mat_model/dim_1\" must be larger than or equal to 2" << std::endl << std::endl;
          return false;
        }
        int dim_2;           try { dim_2=get_positive_int(*model_list_iter, "dim_2");        } catch (int e) { return false; }
        model = new multi_alt_exp_expE_nonsym_mat_model(E_name, dE_name, A_name, B_name, t_name, n_exp, n_o_exp, dim_1, dim_2);
      }
      else if(name=="multi_exp_2var_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_exp_2var_model(E_name, dE_name, A_name, B_name, t_name, T_name, n_exp);
      }
      else if(name=="multi_exp_expE_2var_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_exp_expE_2var_model(E_name, dE_name, A_name, B_name, t_name, T_name, n_exp);
      }
      else if(name=="multi_part_exp_expE_model")
      {
        int n_exp;           try { n_exp=get_positive_int(*model_list_iter, "n_exp");        } catch (int e) { return false; }
        int n_part;          try { n_part=get_positive_int(*model_list_iter, "n_part");        } catch (int e) { return false; }
        std::string A_name;  try { A_name=get_string(*model_list_iter, "A_name");   } catch (int e) { return false; }
        std::string B_name;  try { B_name=get_string(*model_list_iter, "B_name");   } catch (int e) { return false; }
        std::string Z_name;  try { Z_name=get_string(*model_list_iter, "Z_name");   } catch (int e) { return false; }
        std::string E_name;  try { E_name=get_string(*model_list_iter, "E_name");   } catch (int e) { return false; }
        std::string dE_name; try { dE_name=get_string(*model_list_iter, "dE_name"); } catch (int e) { return false; }
        std::string t_name;  try { t_name=get_string(*model_list_iter, "t_name");   } catch (int e) { return false; }
        std::string T_name;  try { T_name=get_string(*model_list_iter, "T_name");   } catch (int e) { return false; }
        model = new multi_part_exp_expE_model(E_name, dE_name, Z_name, A_name, B_name, t_name, T_name, n_exp, n_part);
      }
      else if(name=="threept_constant_model")
      {
        std::string C_name;          try { C_name=get_string(*model_list_iter, "C_name");                   } catch (int e) { return false; }
        std::string t_name;          try { t_name=get_string(*model_list_iter, "t_name");                   } catch (int e) { return false; }
        std::string T_name;          try { T_name=get_string(*model_list_iter, "T_name");                   } catch (int e) { return false; }
        model = new threept_constant_model(C_name, t_name, T_name);
      }
      else if(name=="threept_constant_sqr_model")
      {
        std::string C_name;          try { C_name=get_string(*model_list_iter, "C_name");                   } catch (int e) { return false; }
        std::string t_name;          try { t_name=get_string(*model_list_iter, "t_name");                   } catch (int e) { return false; }
        std::string T_name;          try { T_name=get_string(*model_list_iter, "T_name");                   } catch (int e) { return false; }
        model = new threept_constant_sqr_model(C_name, t_name, T_name);
      }
      else if(name=="threept_multi_exp_model")
      {
        int n_exp_initial;           try { n_exp_initial=get_positive_int(*model_list_iter, "n_exp_initial");        } catch (int e) { return false; }
        int n_exp_final;             try { n_exp_final=get_positive_int(*model_list_iter, "n_exp_final");            } catch (int e) { return false; }
        std::string A_name;          try { A_name=get_string(*model_list_iter, "A_name");                   } catch (int e) { return false; }
        std::string B_name;          try { B_name=get_string(*model_list_iter, "B_name");                   } catch (int e) { return false; }
        std::string E_initial_name;  try { E_initial_name=get_string(*model_list_iter, "E_initial_name");   } catch (int e) { return false; }
        std::string dE_initial_name; try { dE_initial_name=get_string(*model_list_iter, "dE_initial_name"); } catch (int e) { return false; }
        std::string E_final_name;    try { E_final_name=get_string(*model_list_iter, "E_final_name");       } catch (int e) { return false; }
        std::string dE_final_name;   try { dE_final_name=get_string(*model_list_iter, "dE_final_name");     } catch (int e) { return false; }
        std::string t_name;          try { t_name=get_string(*model_list_iter, "t_name");                   } catch (int e) { return false; }
        std::string T_name;          try { T_name=get_string(*model_list_iter, "T_name");                   } catch (int e) { return false; }
        model = new threept_multi_exp_model(E_initial_name, dE_initial_name, E_final_name, dE_final_name, A_name, B_name, t_name, T_name, n_exp_initial, n_exp_final);
      }
      else if(name=="threept_multi_exp_expE_model")
      {
        int n_exp_initial;           try { n_exp_initial=get_positive_int(*model_list_iter, "n_exp_initial");        } catch (int e) { return false; }
        int n_exp_final;             try { n_exp_final=get_positive_int(*model_list_iter, "n_exp_final");            } catch (int e) { return false; }
        std::string A_name;          try { A_name=get_string(*model_list_iter, "A_name");                   } catch (int e) { return false; }
        std::string B_name;          try { B_name=get_string(*model_list_iter, "B_name");                   } catch (int e) { return false; }
        std::string E_initial_name;  try { E_initial_name=get_string(*model_list_iter, "E_initial_name");   } catch (int e) { return false; }
        std::string dE_initial_name; try { dE_initial_name=get_string(*model_list_iter, "dE_initial_name"); } catch (int e) { return false; }
        std::string E_final_name;    try { E_final_name=get_string(*model_list_iter, "E_final_name");       } catch (int e) { return false; }
        std::string dE_final_name;   try { dE_final_name=get_string(*model_list_iter, "dE_final_name");     } catch (int e) { return false; }
        std::string t_name;          try { t_name=get_string(*model_list_iter, "t_name");                   } catch (int e) { return false; }
        std::string T_name;          try { T_name=get_string(*model_list_iter, "T_name");                   } catch (int e) { return false; }
        model = new threept_multi_exp_expE_model(E_initial_name, dE_initial_name, E_final_name, dE_final_name, A_name, B_name, t_name, T_name, n_exp_initial, n_exp_final);
      }
      else if(name=="threept_multi_alt_exp_model")
      {
        int n_exp_initial;           try { n_exp_initial=get_positive_int(*model_list_iter, "n_exp_initial");        } catch (int e) { return false; }
        int n_exp_final;             try { n_exp_final=get_positive_int(*model_list_iter, "n_exp_final");            } catch (int e) { return false; }
        int n_o_exp_initial;         try { n_o_exp_initial=get_non_negative_int(*model_list_iter, "n_o_exp_initial");    } catch (int e) { return false; }
        int n_o_exp_final;           try { n_o_exp_final=get_non_negative_int(*model_list_iter, "n_o_exp_final");        } catch (int e) { return false; }
        std::string A_name;          try { A_name=get_string(*model_list_iter, "A_name");                   } catch (int e) { return false; }
        std::string B_name;          try { B_name=get_string(*model_list_iter, "B_name");                   } catch (int e) { return false; }
        std::string E_initial_name;  try { E_initial_name=get_string(*model_list_iter, "E_initial_name");   } catch (int e) { return false; }
        std::string dE_initial_name; try { dE_initial_name=get_string(*model_list_iter, "dE_initial_name"); } catch (int e) { return false; }
        std::string E_final_name;    try { E_final_name=get_string(*model_list_iter, "E_final_name");       } catch (int e) { return false; }
        std::string dE_final_name;   try { dE_final_name=get_string(*model_list_iter, "dE_final_name");     } catch (int e) { return false; }
        std::string t_name;          try { t_name=get_string(*model_list_iter, "t_name");                   } catch (int e) { return false; }
        std::string T_name;          try { T_name=get_string(*model_list_iter, "T_name");                   } catch (int e) { return false; }
        model = new threept_multi_alt_exp_model(E_initial_name, dE_initial_name, E_final_name, dE_final_name, A_name, B_name, t_name, T_name, n_exp_initial, n_o_exp_initial, n_exp_final, n_o_exp_final);
      }
      else if(name=="threept_multi_alt_exp_expE_model")
      {
        int n_exp_initial;           try { n_exp_initial=get_positive_int(*model_list_iter, "n_exp_initial");        } catch (int e) { return false; }
        int n_exp_final;             try { n_exp_final=get_positive_int(*model_list_iter, "n_exp_final");            } catch (int e) { return false; }
        int n_o_exp_initial;         try { n_o_exp_initial=get_non_negative_int(*model_list_iter, "n_o_exp_initial");    } catch (int e) { return false; }
        int n_o_exp_final;           try { n_o_exp_final=get_non_negative_int(*model_list_iter, "n_o_exp_final");        } catch (int e) { return false; }
        std::string A_name;          try { A_name=get_string(*model_list_iter, "A_name");                   } catch (int e) { return false; }
        std::string B_name;          try { B_name=get_string(*model_list_iter, "B_name");                   } catch (int e) { return false; }
        std::string E_initial_name;  try { E_initial_name=get_string(*model_list_iter, "E_initial_name");   } catch (int e) { return false; }
        std::string dE_initial_name; try { dE_initial_name=get_string(*model_list_iter, "dE_initial_name"); } catch (int e) { return false; }
        std::string E_final_name;    try { E_final_name=get_string(*model_list_iter, "E_final_name");       } catch (int e) { return false; }
        std::string dE_final_name;   try { dE_final_name=get_string(*model_list_iter, "dE_final_name");     } catch (int e) { return false; }
        std::string t_name;          try { t_name=get_string(*model_list_iter, "t_name");                   } catch (int e) { return false; }
        std::string T_name;          try { T_name=get_string(*model_list_iter, "T_name");                   } catch (int e) { return false; }
        model = new threept_multi_alt_exp_expE_model(E_initial_name, dE_initial_name, E_final_name, dE_final_name, A_name, B_name, t_name, T_name, n_exp_initial, n_o_exp_initial, n_exp_final, n_o_exp_final);
      }
      else if(name=="threept_multi_exp_vec_model")
      {
        int n_exp_initial;           try { n_exp_initial=get_positive_int(*model_list_iter, "n_exp_initial");        } catch (int e) { return false; }
        int n_exp_final;             try { n_exp_final=get_positive_int(*model_list_iter, "n_exp_final");            } catch (int e) { return false; }
        std::string A_name;          try { A_name=get_string(*model_list_iter, "A_name");                   } catch (int e) { return false; }
        std::string B_name;          try { B_name=get_string(*model_list_iter, "B_name");                   } catch (int e) { return false; }
        std::string E_initial_name;  try { E_initial_name=get_string(*model_list_iter, "E_initial_name");   } catch (int e) { return false; }
        std::string dE_initial_name; try { dE_initial_name=get_string(*model_list_iter, "dE_initial_name"); } catch (int e) { return false; }
        std::string E_final_name;    try { E_final_name=get_string(*model_list_iter, "E_final_name");       } catch (int e) { return false; }
        std::string dE_final_name;   try { dE_final_name=get_string(*model_list_iter, "dE_final_name");     } catch (int e) { return false; }
        std::string t_name;          try { t_name=get_string(*model_list_iter, "t_name");                   } catch (int e) { return false; }
        std::string T_name;          try { T_name=get_string(*model_list_iter, "T_name");                   } catch (int e) { return false; }
        int dim;                     try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new threept_multi_exp_vec_model(E_initial_name, dE_initial_name, E_final_name, dE_final_name, A_name, B_name, t_name, T_name, n_exp_initial, n_exp_final, dim);
      }
      else if(name=="threept_multi_exp_expE_vec_model")
      {
        int n_exp_initial;           try { n_exp_initial=get_positive_int(*model_list_iter, "n_exp_initial");        } catch (int e) { return false; }
        int n_exp_final;             try { n_exp_final=get_positive_int(*model_list_iter, "n_exp_final");            } catch (int e) { return false; }
        std::string A_name;          try { A_name=get_string(*model_list_iter, "A_name");                   } catch (int e) { return false; }
        std::string B_name;          try { B_name=get_string(*model_list_iter, "B_name");                   } catch (int e) { return false; }
        std::string E_initial_name;  try { E_initial_name=get_string(*model_list_iter, "E_initial_name");   } catch (int e) { return false; }
        std::string dE_initial_name; try { dE_initial_name=get_string(*model_list_iter, "dE_initial_name"); } catch (int e) { return false; }
        std::string E_final_name;    try { E_final_name=get_string(*model_list_iter, "E_final_name");       } catch (int e) { return false; }
        std::string dE_final_name;   try { dE_final_name=get_string(*model_list_iter, "dE_final_name");     } catch (int e) { return false; }
        std::string t_name;          try { t_name=get_string(*model_list_iter, "t_name");                   } catch (int e) { return false; }
        std::string T_name;          try { T_name=get_string(*model_list_iter, "T_name");                   } catch (int e) { return false; }
        int dim;                     try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new threept_multi_exp_expE_vec_model(E_initial_name, dE_initial_name, E_final_name, dE_final_name, A_name, B_name, t_name, T_name, n_exp_initial, n_exp_final, dim);
      }
      else if(name=="threept_multi_alt_exp_vec_model")
      {
        int n_exp_initial;           try { n_exp_initial=get_positive_int(*model_list_iter, "n_exp_initial");        } catch (int e) { return false; }
        int n_exp_final;             try { n_exp_final=get_positive_int(*model_list_iter, "n_exp_final");            } catch (int e) { return false; }
        int n_o_exp_initial;         try { n_o_exp_initial=get_non_negative_int(*model_list_iter, "n_o_exp_initial");    } catch (int e) { return false; }
        int n_o_exp_final;           try { n_o_exp_final=get_non_negative_int(*model_list_iter, "n_o_exp_final");        } catch (int e) { return false; }
        std::string A_name;          try { A_name=get_string(*model_list_iter, "A_name");                   } catch (int e) { return false; }
        std::string B_name;          try { B_name=get_string(*model_list_iter, "B_name");                   } catch (int e) { return false; }
        std::string E_initial_name;  try { E_initial_name=get_string(*model_list_iter, "E_initial_name");   } catch (int e) { return false; }
        std::string dE_initial_name; try { dE_initial_name=get_string(*model_list_iter, "dE_initial_name"); } catch (int e) { return false; }
        std::string E_final_name;    try { E_final_name=get_string(*model_list_iter, "E_final_name");       } catch (int e) { return false; }
        std::string dE_final_name;   try { dE_final_name=get_string(*model_list_iter, "dE_final_name");     } catch (int e) { return false; }
        std::string t_name;          try { t_name=get_string(*model_list_iter, "t_name");                   } catch (int e) { return false; }
        std::string T_name;          try { T_name=get_string(*model_list_iter, "T_name");                   } catch (int e) { return false; }
        int dim;                     try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new threept_multi_alt_exp_vec_model(E_initial_name, dE_initial_name, E_final_name, dE_final_name, A_name, B_name, t_name, T_name, n_exp_initial, n_o_exp_initial, n_exp_final, n_o_exp_final, dim);
      }
      else if(name=="threept_multi_alt_exp_expE_vec_model")
      {
        int n_exp_initial;           try { n_exp_initial=get_positive_int(*model_list_iter, "n_exp_initial");        } catch (int e) { return false; }
        int n_exp_final;             try { n_exp_final=get_positive_int(*model_list_iter, "n_exp_final");            } catch (int e) { return false; }
        int n_o_exp_initial;         try { n_o_exp_initial=get_non_negative_int(*model_list_iter, "n_o_exp_initial");    } catch (int e) { return false; }
        int n_o_exp_final;           try { n_o_exp_final=get_non_negative_int(*model_list_iter, "n_o_exp_final");        } catch (int e) { return false; }
        std::string A_name;          try { A_name=get_string(*model_list_iter, "A_name");                   } catch (int e) { return false; }
        std::string B_name;          try { B_name=get_string(*model_list_iter, "B_name");                   } catch (int e) { return false; }
        std::string E_initial_name;  try { E_initial_name=get_string(*model_list_iter, "E_initial_name");   } catch (int e) { return false; }
        std::string dE_initial_name; try { dE_initial_name=get_string(*model_list_iter, "dE_initial_name"); } catch (int e) { return false; }
        std::string E_final_name;    try { E_final_name=get_string(*model_list_iter, "E_final_name");       } catch (int e) { return false; }
        std::string dE_final_name;   try { dE_final_name=get_string(*model_list_iter, "dE_final_name");     } catch (int e) { return false; }
        std::string t_name;          try { t_name=get_string(*model_list_iter, "t_name");                   } catch (int e) { return false; }
        std::string T_name;          try { T_name=get_string(*model_list_iter, "T_name");                   } catch (int e) { return false; }
        int dim;                     try { dim=get_positive_int(*model_list_iter, "dim");        } catch (int e) { return false; }
        model = new threept_multi_alt_exp_expE_vec_model(E_initial_name, dE_initial_name, E_final_name, dE_final_name, A_name, B_name, t_name, T_name, n_exp_initial, n_o_exp_initial, n_exp_final, n_o_exp_final, dim);
      }
      else if(name=="parse_model")
      {
        int n_variables;      try { n_variables=get_positive_int(*model_list_iter, "n_variables");        } catch (int e) { return false; }
        std::vector< std::string > variables(n_variables);
        xmlpp::NodeSet outer_variables_nodeset = (*model_list_iter)->find("variables");
        if(outer_variables_nodeset.size()!=1)
        {
          std::cerr << "Error: \"variables\" did not return unique node" << std::endl << std::endl;
          return false;
        }
        xmlpp::NodeSet variables_nodeset=(*outer_variables_nodeset.begin())->find("variable");
        for(int v=0; v<n_variables; ++v)
        {
          xmlpp::Node* variable_node;
          try
          {
            variable_node=get_node(variables_nodeset, "number", int_to_string(v+1));
          }
          catch (int e)
          {
            std::cerr << " Error: did not find unique node for variable " << v+1 << std::endl << std::endl;
            return false;
          }
          std::string variable_name; try { variable_name=get_string(variable_node, "name"); } catch (int e) { return false; }
          if(std::find(variables.begin(), variables.end(), variable_name)!=variables.end())
          {
            std::cerr << "Error: variable name repeated" << std::endl << std::endl;
            return false;
          }
          if( _in("{", variable_name) || _in("&", variable_name) || _in("&", variable_name) || _in("!", variable_name) || _in("}", variable_name)
           || _in("%", variable_name) || _in("?", variable_name) || _in("[", variable_name) || _in(":", variable_name)
           || _in(">", variable_name) || _in("#", variable_name) || _in("~", variable_name) || _in("$", variable_name)
           || _in("]", variable_name) || _in("<", variable_name) ||
                         _in("arcsin", variable_name) ||  _in("sinh", variable_name) ||
                         _in("sin", variable_name) ||  _in("arccos", variable_name) ||
                         _in("cosh", variable_name) ||  _in("cos", variable_name) ||
                         _in("arctan", variable_name) ||  _in("tanh", variable_name) ||
                         _in("tan", variable_name) ||  _in("exp", variable_name) ||
                         _in("log", variable_name) ||  _in("sqrt", variable_name) ||
                         _in("sqr", variable_name) ||  _in("alt", variable_name) )
          {
            std::cerr << "Error: forbidden character or substring in variable " << v+1 << std::endl << std::endl;
            return false;
          }
          variables[v]=variable_name;
        }

        int n_functions;      try { n_functions=get_positive_int(*model_list_iter, "n_functions");        } catch (int e) { return false; }
        std::vector< std::string > functions(n_functions);
        xmlpp::NodeSet outer_functions_nodeset = (*model_list_iter)->find("functions");
        if(outer_functions_nodeset.size()!=1)
        {
          std::cerr << "Error: \"functions\" did not return unique node" << std::endl << std::endl;
          return false;
        }
        xmlpp::NodeSet functions_nodeset=(*outer_functions_nodeset.begin())->find("function");
        for(int f=0; f<n_functions; ++f)
        {
          xmlpp::Node* function_node;
          try
          {
            function_node=get_node(functions_nodeset, "number", int_to_string(f+1));
          }
          catch (int e)
          {
            std::cerr << " Error: did not find unique node for function " << f+1 << std::endl << std::endl;
            return false;
          }
          std::string function_definition; try { function_definition=get_string(function_node, "definition"); } catch (int e) { return false; }
          if( _in("{", function_definition) || _in("&", function_definition) || _in("&", function_definition) || _in("!", function_definition) || _in("}", function_definition)
           || _in("%", function_definition) || _in("?", function_definition) || _in("[", function_definition) || _in(":", function_definition)
           || _in(">", function_definition) || _in("#", function_definition) || _in("~", function_definition) || _in("$", function_definition)
           || _in("]", function_definition) || _in("<", function_definition) )
          {
            std::cerr << "Error: forbidden character in function " << f+1 << std::endl << std::endl;
            return false;
          }
          functions[f]=function_definition;
        }

        std::vector< std::string > constants;
        xmlpp::NodeSet outer_constants_nodeset = (*model_list_iter)->find("constants");
        if(outer_constants_nodeset.size()!=1)
        {
          std::cerr << "Error: \"constants\" did not return unique node" << std::endl << std::endl;
          return false;
        }
        xmlpp::NodeSet constants_nodeset=(*outer_constants_nodeset.begin())->find("name");
        for(xmlpp::NodeSet::iterator constants_nodeset_it=constants_nodeset.begin(); constants_nodeset_it!=constants_nodeset.end(); ++constants_nodeset_it)
        {
          xmlpp::Node::NodeList constant_name_list = keep_text_nodes((*constants_nodeset_it)->get_children());
          if(constant_name_list.size()!=1)
          {
            std::cerr << " Error: \"" << (*constants_nodeset_it)->get_path() << "\" did not return string value" << std::endl << std::endl;
            return false;
          }
          const xmlpp::TextNode* constant_name_nodeText = dynamic_cast<const xmlpp::TextNode*>(*constant_name_list.begin());
          std::string constant_name=constant_name_nodeText->get_content();
          remove_whitespaces(constant_name);
          if(constant_name.empty())
          {
            std::cerr << " Error: \"" << (*constants_nodeset_it)->get_path() << "\" did not return string value" << std::endl << std::endl;
            return false;
          }
          if(std::find(constants.begin(), constants.end(), constant_name)!=constants.end())
          {
            std::cerr << "Error: constant name repeated" << std::endl << std::endl;
            return false;
          }
          if( _in("{", constant_name) || _in("&", constant_name) || _in("&", constant_name) || _in("!", constant_name) || _in("}", constant_name)
           || _in("%", constant_name) || _in("?", constant_name) || _in("[", constant_name) || _in(":", constant_name)
           || _in(">", constant_name) || _in("#", constant_name) || _in("~", constant_name) || _in("$", constant_name)
           || _in("]", constant_name) || _in("<", constant_name) ||
                         _in("arcsin", constant_name) ||  _in("sinh", constant_name) ||
                         _in("sin", constant_name) ||  _in("arccos", constant_name) ||
                         _in("cosh", constant_name) ||  _in("cos", constant_name) ||
                         _in("arctan", constant_name) ||  _in("tanh", constant_name) ||
                         _in("tan", constant_name) ||  _in("exp", constant_name) ||
                         _in("log", constant_name) ||  _in("sqrt", constant_name) ||
                         _in("sqr", constant_name) ||  _in("alt", constant_name) )
          {
            std::cerr << "Error: forbidden character or substring in constant " << constant_name << std::endl << std::endl;
            return false;
          }
          constants.push_back(constant_name);
        }

        std::vector< std::string > parameters;
        xmlpp::NodeSet outer_parameters_nodeset = (*model_list_iter)->find("parameters");
        if(outer_parameters_nodeset.size()!=1)
        {
          std::cerr << "Error: \"parameters\" did not return unique node" << std::endl << std::endl;
          return false;
        }
        xmlpp::NodeSet parameters_nodeset=(*outer_parameters_nodeset.begin())->find("name");
        for(xmlpp::NodeSet::iterator parameters_nodeset_it=parameters_nodeset.begin(); parameters_nodeset_it!=parameters_nodeset.end(); ++parameters_nodeset_it)
        {
          xmlpp::Node::NodeList parameter_name_list = keep_text_nodes((*parameters_nodeset_it)->get_children());
          if(parameter_name_list.size()!=1)
          {
            std::cerr << " Error: \"" << (*parameters_nodeset_it)->get_path() << "\" did not return string value" << std::endl << std::endl;
            return false;
          }
          const xmlpp::TextNode* parameter_name_nodeText = dynamic_cast<const xmlpp::TextNode*>(*parameter_name_list.begin());
          std::string parameter_name=parameter_name_nodeText->get_content();
          remove_whitespaces(parameter_name);
          if(parameter_name.empty())
          {
            std::cerr << " Error: \"" << (*parameters_nodeset_it)->get_path() << "\" did not return string value" << std::endl << std::endl;
            return false;
          }
          if(std::find(parameters.begin(), parameters.end(), parameter_name)!=parameters.end())
          {
            std::cerr << "Error: parameter name repeated" << std::endl << std::endl;
            return false;
          }
          if( _in("{", parameter_name) || _in("&", parameter_name) || _in("&", parameter_name) || _in("!", parameter_name) || _in("}", parameter_name)
           || _in("%", parameter_name) || _in("?", parameter_name) || _in("[", parameter_name) || _in(":", parameter_name)
           || _in(">", parameter_name) || _in("#", parameter_name) || _in("~", parameter_name) || _in("$", parameter_name)
           || _in("]", parameter_name) || _in("<", parameter_name) ||
                         _in("arcsin", parameter_name) ||  _in("sinh", parameter_name) ||
                         _in("sin", parameter_name) ||  _in("arccos", parameter_name) ||
                         _in("cosh", parameter_name) ||  _in("cos", parameter_name) ||
                         _in("arctan", parameter_name) ||  _in("tanh", parameter_name) ||
                         _in("tan", parameter_name) ||  _in("exp", parameter_name) ||
                         _in("log", parameter_name) ||  _in("sqrt", parameter_name) ||
                         _in("sqr", parameter_name) ||  _in("alt", parameter_name) )
          {
            std::cerr << "Error: forbidden character or substring in parameter " << parameter_name << std::endl << std::endl;
            return false;
          }
          parameters.push_back(parameter_name);
        }
        int n_parameters=parameters.size();

        std::vector< std::string > der_temp_vec(n_parameters);
        std::vector< std::vector< std::string > > derivatives(n_functions, der_temp_vec);

        if(!num_diff)
        {
          xmlpp::NodeSet outer_derivatives_nodeset = (*model_list_iter)->find("derivatives");
          if(outer_derivatives_nodeset.size()!=1)
          {
            std::cerr << "Error: \"derivatives\" did not return unique node" << std::endl << std::endl;
            return false;
          }
          xmlpp::NodeSet derivatives_nodeset=(*outer_derivatives_nodeset.begin())->find("derivative");
          for(int f=0; f<n_functions; ++f)
          {
            for(int p=0; p<n_parameters; ++p)
            {
              xmlpp::Node* derivative_node;
              try
              {
                derivative_node=get_node_two_properties(derivatives_nodeset, "function_number", int_to_string(f+1), "parameter_name", parameters[p]);
              }
              catch (int e)
              {
                std::cerr << " Error: did not find unique node for derivative of function " << f+1 << " with respect to parameter " << parameters[p] << std::endl << std::endl;
                return false;
              }
              std::string derivative_definition; try { derivative_definition=get_string(derivative_node, "definition"); } catch (int e) { return false; }
              if( _in("{", derivative_definition) || _in("&", derivative_definition) || _in("&", derivative_definition) || _in("!", derivative_definition) || _in("}", derivative_definition)
               || _in("%", derivative_definition) || _in("?", derivative_definition) || _in("[", derivative_definition) || _in(":", derivative_definition)
               || _in(">", derivative_definition) || _in("#", derivative_definition) || _in("~", derivative_definition) || _in("$", derivative_definition)
               || _in("]", derivative_definition) || _in("<", derivative_definition) )
              {
                std::cerr << "Error: forbidden character derivative of function " << f+1 << " with repect to parameter " << parameters[p] << std::endl << std::endl;
                return false;
              }
              derivatives[f][p]=derivative_definition;
            }
          }
        }

        model = new parse_model(functions, variables, parameters, constants, derivatives);

      }


      else
      {
        std::cerr << "Error: unknown model \"" << name << "\"" << std::endl << std::endl;
        return false;
      }

      if(model)
      {

        // test for uniqueness of parameter names
        std::vector< std::string > all_parameter_names;
        for(int p=0; p<model->get_n_parameters(); ++p)
        {
          if(std::find(all_parameter_names.begin(), all_parameter_names.end(), model->get_parameter_name(p))!=all_parameter_names.end())
          {
            std::cerr << " Error: \"" << (*model_list_iter)->get_path() << "\": repeated parameter name: " << model->get_parameter_name(p) << std::endl << std::endl;
            return false;
          }
          all_parameter_names.push_back(model->get_parameter_name(p));
        }

        // add model
        models.push_back(model);

        // add model group
        model_groups.push_back(model_group);

        // read fit domain
        xmlpp::NodeSet fit_domain_nodeset = (*model_list_iter) -> find("fit_domain");

        std::vector< std::vector< std::string > > all_min_vector, all_max_vector;
        std::vector< std::vector< bool > > all_has_step_vector;
        std::vector< std::vector< double > > all_step_vector;
        std::vector< std::vector< bool > > all_has_bootstrap_range_file_vector;
        std::vector< std::vector< std::string > > all_bootstrap_range_file_vector;

        for(int v=0; v<model->get_n_variables(); ++v)
        {
          xmlpp::Node* domain_node;
          try
          {
            domain_node=get_node(fit_domain_nodeset, "variable_name", model->get_variable_name(v));
          }
          catch (int e)
          {
            std::cerr << " Error: \"" << (*model_list_iter)->get_path() << "\": did not find unique fit_domain for variable " <<  model->get_variable_name(v) << std::endl << std::endl;
            return false;
          }

          xmlpp::NodeSet range_nodeset =  domain_node -> find("range");
          if(range_nodeset.size()<1)
          {
            std::cerr << " Error: \"" <<  domain_node -> get_path() << "/range\" did not return any nodes" << std::endl << std::endl;
            return false;
          }
          
          std::vector<std::string> min_vector, max_vector;
          std::vector<bool> has_step_vector;
          std::vector<double> step_vector;
          std::vector<bool> has_bootstrap_range_file_vector;
          std::vector<std::string> bootstrap_range_file_vector;
          for(xmlpp::NodeSet::iterator range_iter=range_nodeset.begin(); range_iter!=range_nodeset.end(); ++range_iter)
          {
            std::string min; try { min=get_string(*range_iter, "min"); } catch (int e) { return false; }
            min_vector.push_back(min);
            std::string max; try { max=get_string(*range_iter, "max"); } catch (int e) { return false; }
            max_vector.push_back(max);
            if(name_exists(*range_iter, "step"))
            {
              double step; try { step=get_positive_double(*range_iter, "step"); } catch (int e) { return false; }
              has_step_vector.push_back(true);
              step_vector.push_back(step);
            }
            else
            {
              has_step_vector.push_back(false);
              step_vector.push_back(0.0);
            }
            
            if(name_exists(*range_iter, "range_bootstrap_file"))
            {
              std::string rbf; try { rbf=get_string(*range_iter, "range_bootstrap_file"); } catch (int e) { return false; }
              has_bootstrap_range_file_vector.push_back(true);
              bootstrap_range_file_vector.push_back(rbf);
            }
            else
            {
              has_bootstrap_range_file_vector.push_back(false);
              bootstrap_range_file_vector.push_back("");
            }
          }
          all_min_vector.push_back(min_vector);
          all_max_vector.push_back(max_vector);
          all_has_step_vector.push_back(has_step_vector);
          all_step_vector.push_back(step_vector);
          all_has_bootstrap_range_file_vector.push_back(has_bootstrap_range_file_vector);
          all_bootstrap_range_file_vector.push_back(bootstrap_range_file_vector);
        
        }
        fit_min.push_back(all_min_vector);
        fit_max.push_back(all_max_vector);
        has_fit_step.push_back(all_has_step_vector);
        fit_step.push_back(all_step_vector);
        has_range_bootstrap_file.push_back(all_has_bootstrap_range_file_vector);
        range_bootstrap_file.push_back(all_bootstrap_range_file_vector);

        // read plot domain
        if(perform_plot)
        {
          xmlpp::NodeSet plot_domain_nodeset = (*model_list_iter) -> find("plot_domain");
          std::vector< std::string > plot_min_vector, plot_max_vector;
          std::vector< int > plot_order_vector;
          std::vector< double > plot_step_vector;
          for(int v=0; v<model->get_n_variables(); ++v)
          {
            xmlpp::Node* domain_node;
            try
            {
              domain_node=get_node(plot_domain_nodeset, "variable_name", model->get_variable_name(v));
            }
            catch (int e)
            {
              std::cerr << " Error: \"" << (*model_list_iter)->get_path() << "\": did not find unique plot_domain for variable " <<  model->get_variable_name(v) << std::endl << std::endl;
              return false;
            }
            int order; try { order=get_positive_int(domain_node, "plot_order"); } catch (int e) { return false; }
            plot_order_vector.push_back(order-1);

            double step; try { step=get_positive_double(domain_node, "step"); } catch (int e) { return false; }
            plot_step_vector.push_back(step);

            xmlpp::NodeSet range_nodeset =  domain_node -> find("range");
            if(range_nodeset.size()!=1)
            {
              std::cerr << " Error: \"" <<  domain_node -> get_path() << "/range\" did not return unique node" << std::endl << std::endl;
              return false;
            }
            std::string min; try { min=get_string(*(range_nodeset.begin()), "min"); } catch (int e) { return false; }
            plot_min_vector.push_back(min);
            std::string max; try { max=get_string(*(range_nodeset.begin()), "max"); } catch (int e) { return false; }
            plot_max_vector.push_back(max);
          }

          bool plot_order_correct=true;
          for(int v=0; v<model->get_n_variables(); ++v)
          {
            if(std::count(plot_order_vector.begin(), plot_order_vector.end(), v)!=1)
            {
              plot_order_correct=false;
            }
          }
          if(!plot_order_correct)
          {
            std::cerr << "Error: plot order specification for \"" << (*model_list_iter)->get_path() << "\" incorrect" << std::endl << std::endl;
            return false;
          }

          plot_order.push_back(plot_order_vector);
          plot_step.push_back(plot_step_vector);
          plot_min.push_back(plot_min_vector);
          plot_max.push_back(plot_max_vector);
        }


        // read data file
        xmlpp::NodeSet data_file_nodeset = (*model_list_iter) -> find("data_file");
        if(data_file_nodeset.size()!=1)
        {
          std::cerr << " Error: \"" << (*model_list_iter)->get_path() << "/data_file\" did not return unique node" << std::endl << std::endl;
          return false;
        }
        std::string file_name; try { file_name=get_string(*data_file_nodeset.begin(), "file_name"); } catch (int e) { return false; }
        std::string file_type; try { file_type=get_string(*data_file_nodeset.begin(), "file_type"); } catch (int e) { return false; }
        std::vector< std::vector< std::vector< double > > > file_data;
        std::vector< std::vector< double > > file_arguments;
        if(file_type=="ASCII")
        {
          if(!load_data_file_ASCII(file_name, model->get_n_functions(), model->get_n_variables(), file_data, file_arguments))
          {
            return false;
          }
        }
        else if(file_type=="binary")
        {
          if(!load_data_file_binary(file_name, model->get_n_functions(), model->get_n_variables(), file_data, file_arguments))
          {
            return false;
          }
        }
        else
        {
          std::cerr << "Error: unknown file type \"" << file_type << "\"" << std::endl << std::endl;
          return false;
        }
        all_file_data.push_back(file_data);
        all_file_arguments.push_back(file_arguments);

      }

    }
  }
  if(models.empty())
  {
     std::cerr << "Error: no models found" << std::endl << std::endl;
     return false;
  }
  return true;
}
