#ifndef ABSTRACT_PRIOR_H
#define ABSTRACT_PRIOR_H

#include <vector>

using namespace std;

class abstract_prior
{
  public:

    virtual ~abstract_prior() { }

    virtual void set_parameters(const vector< double >& parameters) = 0;

    virtual double chi_sqr() = 0;
    virtual double beta(int parameter) = 0;
    virtual double alpha(int parameter1, int parameter2) = 0;

    virtual int no_of_errors() = 0;
};

#endif
