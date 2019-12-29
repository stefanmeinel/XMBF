#ifndef COMBINED_MODEL_H
#define COMBINED_MODEL_H

#include <vector>
#include <string>
#include <algorithm>

#include "abstract_global_model.h"
#include "abstract_model.h"

using namespace std;

class combined_model : public abstract_global_model
{
  public:
    combined_model();

    void add_model(abstract_model* model, int model_group);

    void set_all_arguments(const vector< vector< vector< double > > >& arguments);
//-----------------------------------------------------------------------------
//    arguments[n_models][n_data_points][n_variables]
//-----------------------------------------------------------------------------

    int get_n_fit_points();
    int get_n_parameters();
    int get_n_constants();

    void set_parameters(const vector< double >& params);
    void set_constants(const vector< double >& consts);

    int model_group(int fit_point);

    double eval_function(int fit_point);
    double eval_derivative(int fit_point, int parameter);

    string get_parameter_name(int parameter);
    string get_constant_name(int constant);

    int no_of_errors();

  private:

    vector<abstract_model*> models;
    vector<int> model_groups;

    vector< string > parameter_names;
    vector< vector< int > > individual_parameters;

    vector< string > constant_names;
    vector< vector< int > > individual_constants;

    vector< int > model_number;
    vector< int > data_point;
    vector< int > function;
    vector< vector< double > > all_arguments;

    int errors;

    double tmp_d;
};

#endif
