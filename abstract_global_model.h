#ifndef ABSTRACT_GLOBAL_MODEL_H
#define ABSTRACT_GLOBAL_MODEL_H

#include <vector>
#include <string>

class abstract_global_model
{
  public:

    virtual ~abstract_global_model() { }

    virtual int get_n_fit_points() = 0;
    virtual int get_n_parameters() = 0;
    virtual int get_n_constants() = 0;

    virtual void set_parameters(const std::vector< double >& parameters) = 0;
    virtual void set_constants(const std::vector< double >& constants) = 0;

    virtual int model_group(int fit_point) = 0;

    virtual double eval_function(int fit_point) = 0;
    virtual double eval_derivative(int fit_point, int parameter) = 0;

    virtual std::string get_parameter_name(int parameter) = 0;
    virtual std::string get_constant_name(int constant) = 0;

    virtual int no_of_errors() = 0;
};

#endif
