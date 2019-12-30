#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include <limits>

#include <libxml++/libxml++.h>
#include <boost/lexical_cast.hpp>

#include "settingsmap.h"
#include "string_tools.h"
#include "xml_tools.h"

using namespace std;

const double empty_double=std::numeric_limits<int>::max()-1;
const int empty_int=std::numeric_limits<int>::max()-1;

enum inversion_method
{
  LU_inversion = 0,
  simple_cut = 1,
  ratio_cut = 2,
  absolute_cut = 3,
  diagonal = 4,
  off_diagonal_rescale = 6
};


const int start_n_functions=1;
const int start_n_variables=1;
const int start_n_parameters=1;
const int start_n_constants=0;
const bool start_bayes=true;
const bool start_num_diff=false;
const bool start_second_deriv_minimization=false;
const bool start_second_deriv_covariance=false;
const double start_num_diff_step=1e-08;
const bool start_use_bse=false;
const bool start_restrict_data=false;
const bool start_bootstrap_normalization=false;
const int start_start_data=1;
const int start_stop_data=1000;
const double start_startlambda=0.001;
const double start_lambdafac=10.0;
const double start_tolerance=0.001;
const int start_svd=0;
const double start_svd_ratio=0.000001;
const double start_svd_value=0.000000000001;
const double start_rescale_value=0.99;
const int start_steps=100;
const int start_bin=1;
const int start_n_exp=1;
const int start_m_exp=1;
const int start_n_vec=2;


enum model  // keep existing entries for backward compatibility!
{
  PARSE=0,

  MULTIEXP=1,
  MULTIEXPASQR=7,
  MULTIEXPEXPA=8,
  MULTIEXPEXPE=37,
  MULTIEXPEXPAEXPE=38,
  MULTIALTEXP=4,
  MULTIALTEXPASQR=9,
  MULTIALTEXPEXPA=10,
  MULTIALTEXPEXPE=39,
  MULTIALTEXPEXPAEXPE=40,

  MULTIEXPVEC=2,
  MULTIEXPASQRVEC=17,
  MULTIEXPEXPAVEC=18,
  MULTIEXPEXPEVEC=41,
  MULTIEXPEXPAEXPEVEC=42,
  MULTIALTEXPVEC=5,
  MULTIALTEXPASQRVEC=19,
  MULTIALTEXPEXPAVEC=20,
  MULTIALTEXPEXPEVEC=43,
  MULTIALTEXPEXPAEXPEVEC=44,

  MULTIEXPMAT=3,
  MULTIEXPEXPEMAT=27,
  MULTIALTEXPMAT=6,
  MULTIALTEXPEXPEMAT=30,

  MULTIEXPBC=45,
  MULTIEXPASQRBC=46,
  MULTIEXPEXPABC=47,
  MULTIEXPEXPEBC=48,
  MULTIEXPEXPAEXPEBC=49,
  MULTIALTEXPBC=50,
  MULTIALTEXPASQRBC=51,
  MULTIALTEXPEXPABC=52,
  MULTIALTEXPEXPEBC=53,
  MULTIALTEXPEXPAEXPEBC=54,

  MULTIEXPVECBC=55,
  MULTIEXPASQRVECBC=56,
  MULTIEXPEXPAVECBC=57,
  MULTIEXPEXPEVECBC=58,
  MULTIEXPEXPAEXPEVECBC=59,
  MULTIALTEXPVECBC=60,
  MULTIALTEXPASQRVECBC=61,
  MULTIALTEXPEXPAVECBC=62,
  MULTIALTEXPEXPEVECBC=63,
  MULTIALTEXPEXPAEXPEVECBC=64,

  MULTIEXPASQREXPE=101,
  MULTIEXPASQREXPEBC=102,

  MULTIALTEXPASQREXPE=103,
  MULTIALTEXPASQREXPEBC=104,

  MULTIEXPASQREXPEVEC=105,
  MULTIEXPASQREXPEVECBC=106,

  MULTIALTEXPASQREXPEVEC=107,
  MULTIALTEXPASQREXPEVECBC=108,

  THREEPT2VARMULTIEXP=70,
  THREEPT2VARMULTIALTEXP=75,
  THREEPT2VARMULTIEXPVEC=170,
  THREEPT2VARMULTIALTEXPVEC=175,
  THREEPT2VARMULTIEXPEXPE=180,
  THREEPT2VARMULTIALTEXPEXPE=181,
  THREEPT2VARMULTIEXPEXPEVEC=182,
  THREEPT2VARMULTIALTEXPEXPEVEC=183,

  MULTIEXPNONSYMMAT=303,
  MULTIEXPEXPENONSYMMAT=304,
  MULTIALTEXPNONSYMMAT=305,
  MULTIALTEXPEXPENONSYMMAT=306,

  MULTIEXPCONST=400,
  MULTIEXPASQRCONST=401,
  MULTIEXPEXPECONST=402,
  MULTIALTEXPCONST=403,
  MULTIALTEXPASQRCONST=404,
  MULTIALTEXPEXPECONST=405,
  MULTIEXPVECCONST=406,
  MULTIEXPASQRVECCONST=407,
  MULTIEXPEXPEVECCONST=408,
  MULTIALTEXPVECCONST=409,
  MULTIALTEXPASQRVECCONST=410,
  MULTIALTEXPEXPEVECCONST=411,
  MULTIEXPBCCONST=412,
  MULTIEXPASQRBCCONST=413,
  MULTIEXPEXPEBCCONST=414,
  MULTIALTEXPBCCONST=415,
  MULTIALTEXPASQRBCCONST=416,
  MULTIALTEXPEXPEBCCONST=417,
  MULTIEXPVECBCCONST=418,
  MULTIEXPASQRVECBCCONST=419,
  MULTIEXPEXPEVECBCCONST=420,
  MULTIALTEXPVECBCCONST=421,
  MULTIALTEXPASQRVECBCCONST=422,
  MULTIALTEXPEXPEVECBCCONST=423,
  MULTIEXPASQREXPECONST=424,
  MULTIEXPASQREXPEBCCONST=425,
  MULTIALTEXPASQREXPECONST=426,
  MULTIALTEXPASQREXPEBCCONST=427,
  MULTIEXPASQREXPEVECCONST=428,
  MULTIEXPASQREXPEVECBCCONST=429,
  MULTIALTEXPASQREXPEVECCONST=430,
  MULTIALTEXPASQREXPEVECBCCONST=431,

  MULTIEXPMATII=500,
  MULTIEXPEXPEMATII=501,

  MULTIEXPMATIIUPPER=600,
  MULTIEXPEXPEMATIIUPPER=601,
  MULTIEXPMATUPPER=603,
  MULTIEXPEXPEMATUPPER=627

};

double version;

model current_model;

int n_functions;
int n_variables;
int n_parameters;
int n_constants;

vector< string > functions;
vector< string > variables;
vector< string > parameters;
vector< string > constants;
vector< vector< string > > derivatives;

vector< string > fit_min;
vector< string > fit_max;

vector< double > start_values;
vector< double > priors;
vector< double > sigmas;
vector< double > constant_values;

bool chisqr_extra_term_enabled;
string chisqr_extra_term_function;
double chisqr_extra_term_num_diff_step;
int chisqr_extra_term_n_constants;
vector< string > chisqr_extra_term_constant_names;
vector< double > chisqr_extra_term_constant_values;


bool bayesian;

bool num_diff;
bool second_deriv_minimization;
bool second_deriv_covariance;
double num_diff_step;

bool use_bse;
bool restrict_data;

bool bootstrap_normalization;

int start_n_data;
int stop_n_data;
double start_lambda;
double lambda_factor;
double chisqr_tolerance;
int svd_cut;
double svd_ratio;
double svd_value;
double rescale_factor;
inversion_method inv_method;
int max_steps;
int bin_size;
bool boot_prior;
int bssamples;
string bse_file;
int data_file_type;
string data_file;

int multiexpdialog_nexp;
bool multiexpdialog_BC;
bool multiexpdialog_constant;
int multiexpvecdialog_nexp;
int multiexpvecdialog_nvec;
bool multiexpvecdialog_BC;
bool multiexpvecdialog_constant;
int multiexpmatdialog_nexp;
int multiexpmatdialog_dim_1;
int multiexpmatdialog_dim_2;
int multiexpmatupperdialog_nexp;
int multiexpmatupperdialog_dim;
int multialtexpdialog_nexp;
int multialtexpdialog_mexp;
bool multialtexpdialog_BC;
bool multialtexpdialog_constant;
int multialtexpvecdialog_nexp;
int multialtexpvecdialog_mexp;
int multialtexpvecdialog_nvec;
bool multialtexpvecdialog_BC;
bool multialtexpvecdialog_constant;
int multialtexpmatdialog_nexp;
int multialtexpmatdialog_mexp;
int multialtexpmatdialog_dim_1;
int multialtexpmatdialog_dim_2;
int threeptmultiexpdialog_nexpinitial;
int threeptmultiexpdialog_nexpfinal;
int threeptmultiexpvecdialog_nexpinitial;
int threeptmultiexpvecdialog_nexpfinal;
int threeptmultiexpvecdialog_nvec;
int threeptmultialtexpdialog_nexpinitial;
int threeptmultialtexpdialog_nexpfinal;
int threeptmultialtexpdialog_mexpinitial;
int threeptmultialtexpdialog_mexpfinal;
int threeptmultialtexpvecdialog_nexpinitial;
int threeptmultialtexpvecdialog_nexpfinal;
int threeptmultialtexpvecdialog_mexpinitial;
int threeptmultialtexpvecdialog_mexpfinal;
int threeptmultialtexpvecdialog_nvec;

int n_parameters_dof;


void init()
{
  version=0.0;
  current_model=PARSE;
  chisqr_extra_term_enabled=false;
  chisqr_extra_term_function="";
  chisqr_extra_term_num_diff_step=start_num_diff_step;
  chisqr_extra_term_n_constants=0;
  bayesian=start_bayes;
  num_diff=start_num_diff;
  second_deriv_minimization=start_second_deriv_minimization;
  second_deriv_covariance=start_second_deriv_covariance;
  num_diff_step=start_num_diff_step;
  use_bse=start_use_bse;
  restrict_data=start_restrict_data;
  bootstrap_normalization=start_bootstrap_normalization;
  start_n_data=start_start_data;
  stop_n_data=start_stop_data;
  start_lambda=start_startlambda;
  lambda_factor=start_lambdafac;
  chisqr_tolerance=start_tolerance;
  svd_cut=start_svd;
  svd_ratio=start_svd_ratio;
  svd_value=start_svd_value;
  rescale_factor=start_rescale_value;
  max_steps=start_steps;
  bin_size=start_bin;
  bse_file="";
  boot_prior=true;
  bssamples=500;
  data_file_type=0;
  multiexpdialog_nexp=start_n_exp;
  multiexpdialog_BC=false;
  multiexpdialog_constant=false;
  multiexpvecdialog_nexp=start_n_exp;
  multiexpvecdialog_nvec=start_n_vec;
  multiexpvecdialog_BC=false;
  multiexpvecdialog_constant=false;
  multiexpmatdialog_nexp=start_n_exp;
  multiexpmatdialog_dim_1=start_n_vec;
  multiexpmatdialog_dim_2=start_n_vec;
  multiexpmatupperdialog_nexp=start_n_exp;
  multiexpmatupperdialog_dim=start_n_vec;
  multialtexpdialog_nexp=start_n_exp;
  multialtexpdialog_mexp=start_m_exp;
  multialtexpdialog_BC=false;
  multialtexpdialog_constant=false;
  multialtexpvecdialog_nexp=start_n_exp;
  multialtexpvecdialog_mexp=start_m_exp;
  multialtexpvecdialog_nvec=start_n_vec;
  multialtexpvecdialog_BC=false;
  multialtexpvecdialog_constant=false;
  multialtexpmatdialog_nexp=start_n_exp;
  multialtexpmatdialog_mexp=start_m_exp;
  multialtexpmatdialog_dim_1=start_n_vec;
  multialtexpmatdialog_dim_2=start_n_vec;
  threeptmultiexpdialog_nexpinitial=start_n_exp;
  threeptmultiexpdialog_nexpfinal=start_n_exp;
  threeptmultiexpvecdialog_nexpinitial=start_n_exp;
  threeptmultiexpvecdialog_nexpfinal=start_n_exp;
  threeptmultiexpvecdialog_nvec=start_n_vec;
  threeptmultialtexpdialog_nexpinitial=start_n_exp;
  threeptmultialtexpdialog_nexpfinal=start_n_exp;
  threeptmultialtexpdialog_mexpinitial=start_m_exp;
  threeptmultialtexpdialog_mexpfinal=start_m_exp;
  threeptmultialtexpvecdialog_nexpinitial=start_n_exp;
  threeptmultialtexpvecdialog_nexpfinal=start_n_exp;
  threeptmultialtexpvecdialog_mexpinitial=start_m_exp;
  threeptmultialtexpvecdialog_mexpfinal=start_m_exp;
  threeptmultialtexpvecdialog_mexpfinal=start_n_vec;
}


bool loadFile(const string& fileName)
{
  settingsmap m;
  if(!m.load_file(fileName))
  {
    cerr << "cannot read file " << fileName << endl;
    return false;
  }
  if(m.exists("version"))
  {
    version=m.get_double("version");
  }
  if(!m.exists("nfunctions"))
  {
    cerr << "input file does not contain number of functions" << endl;
    return false;
  }
  if(!m.exists("nvariables"))
  {
    cerr << "input file does not contain number of variables" << endl;
    return false;
  }
  if(!m.exists("nparameters"))
  {
    cerr << "input file does not contain number of parameters" << endl;
    return false;
  }
  if(!m.exists("nconstants"))
  {
    cerr << "input file does not contain number of constants" << endl;
    return false;
  }
  n_functions=m.get_int("nfunctions");
  functions.resize(n_functions);
  n_variables=m.get_int("nvariables");
  variables.resize(n_variables);
  fit_min.resize(n_variables);
  fit_max.resize(n_variables);
  n_parameters=m.get_int("nparameters");
  parameters.resize(n_parameters);
  start_values.resize(n_parameters);
  priors.resize(n_parameters);
  sigmas.resize(n_parameters);
  vector< string > der_temp_vec;
  der_temp_vec.resize(n_parameters);
  derivatives.resize(n_functions, der_temp_vec);
  n_constants=m.get_int("nconstants");
  constants.resize(n_constants);
  constant_values.resize(n_constants);
  if(m.exists("currentmodel"))
  {
    current_model=static_cast<model>(m.get_int("currentmodel"));
  }
  for(int f=0; f<n_functions; ++f)
  {
    functions[f]=m.get_string("function", f);
  }
  for(int v=0; v<n_variables; ++v)
  {
    variables[v]=m.get_string("variable", v);
    fit_min[v]=m.get_string("fitmin", v);
    fit_max[v]=m.get_string("fitmax", v);
  }
  for(int p=0; p<n_parameters; ++p)
  {
    parameters[p]=m.get_string("parameter", p);
    if(m.exists("startval", p))
    {
      double start_val=m.get_double("startval", p);
      if(start_val!=empty_double)
      {
       start_values[p]=start_val;
      }
    }
    if(m.exists("prior", p))
    {
      double prior=m.get_double("prior", p);
      if(prior!=empty_double)
      {
        priors[p]=prior;
      }
    }
    if(m.exists("sigma", p))
    {
      double sigma=m.get_double("sigma", p);
      if(sigma!=empty_double)
      {
        sigmas[p]=sigma;
      }
    }
  }
  for(int c=0; c<n_constants; ++c)
  {
    constants[c]=m.get_string("constant", c);
    if(m.exists("constantvalue", c))
    {
      double val=m.get_double("constantvalue", c);
      if(val!=empty_double)
      {
        constant_values[c]=val;
      }
    }
  }
  for(int f=0; f<n_functions; ++f)
  {
    for(int p=0; p<n_parameters; ++p)
    {
      derivatives[f][p]=m.get_string("derivative", f, p);
    }
  }
  if(m.exists("bayesian"))
  {
    bayesian=m.get_bool("bayesian");
  }

  if(m.exists("numdiff")) num_diff=m.get_bool("numdiff");
  if(m.exists("secondderivminimization")) second_deriv_minimization=m.get_bool("secondderivminimization");
  if(m.exists("secondderivcovariance")) second_deriv_covariance=m.get_bool("secondderivcovariance");

  if(m.exists("numdiffstep"))
  {
    double numdiffstep=m.get_double("numdiffstep");
    if(numdiffstep!=empty_double)
    {
      num_diff_step=numdiffstep;
    }
  }

  if(m.exists("bootprior"))
  {
    boot_prior=m.get_bool("bootprior");
  }
  else
  {
    boot_prior=true;
  }

  if(m.exists("usebse"))
  {
    use_bse=m.get_bool("usebse");
  }
  else
  {
    if(m.exists("bsefile"))
    {
      use_bse=true;
    }
    else
    {
      use_bse=false;
    }
  }

  if(m.exists("bsefile"))
  {
    bse_file=m.get_string("bsefile");
  }

  if(m.exists("bssamples"))
  {
    int _bssamples=m.get_int("bssamples");
    if(_bssamples!=empty_int)
    {
      bssamples=_bssamples;
    }
  }

  if(m.exists("restrictdatarange")) restrict_data=m.get_bool("restrictdatarange");
  if(m.exists("datarangemin")) start_n_data=m.get_int("datarangemin");
  if(m.exists("datarangemax")) stop_n_data=m.get_int("datarangemax");

  if(m.exists("startlambda"))
  {
    double startlambda=m.get_double("startlambda");
    if(startlambda!=empty_double)
    {
      start_lambda=startlambda;
    }
  }
  if(m.exists("lambdafactor"))
  {
    double lambdafactor=m.get_double("lambdafactor");
    if(lambdafactor!=empty_double)
    {
      lambda_factor=lambdafactor;
    }
  }
  if(m.exists("tolerance"))
  {
    double tolerance=m.get_double("tolerance");
    if(tolerance!=empty_double)
    {
      chisqr_tolerance=tolerance;
    }
  }
  if(m.exists("svdcut"))
  {
    int svdcut=m.get_int("svdcut");
    if(svdcut!=empty_int)
    {
      svd_cut=svdcut;
    }
  }
  if(m.exists("svdratio"))
  {
    double ratio=m.get_double("svdratio");
    if(ratio!=empty_double)
    {
      svd_ratio=ratio;
    }
  }
  if(m.exists("svdvalue"))
  {
    double value=m.get_double("svdvalue");
    if(value!=empty_double)
    {
      svd_value=value;
    }
  }
  if(m.exists("rescalevalue"))
  {
    double value=m.get_double("rescalevalue");
    if(value!=empty_double)
    {
      rescale_factor=value;
    }
  }
  if(m.exists("inversionmethod"))
  {
    inv_method=static_cast<inversion_method>(m.get_int("inversionmethod"));
  }
  else
  {
    if(svd_cut>0)
    {
      inv_method=simple_cut;
    }
    else
    {
      inv_method=LU_inversion;
    }
  }
  if(m.exists("maxsteps"))
  {
    int maxsteps=m.get_int("maxsteps");
    if(maxsteps!=empty_int)
    {
      max_steps=maxsteps;
    }
  }
  if(m.exists("binsize"))
  {
    int binsize=m.get_int("binsize");
    if(binsize!=empty_int)
    {
      bin_size=binsize;
    }
  }
  if(m.exists("bootstrapnormalization"))
  {
    bootstrap_normalization=m.get_bool("bootstrapnormalization");
    if(bootstrap_normalization)
    {
      bin_size=1;
    }
  }

  if(m.exists("datafiletype"))
  {
    data_file_type=m.get_int("datafiletype");
  }
  data_file=m.get_string("datafile");
  if(m.exists("multiexpdialog:nexp")) multiexpdialog_nexp=m.get_int("multiexpdialog:nexp");
  if(m.exists("multiexpdialog:BC")) multiexpdialog_BC=m.get_bool("multiexpdialog:BC");
  if(m.exists("multiexpdialog:constant")) multiexpdialog_BC=m.get_bool("multiexpdialog:constant");
  if(m.exists("multiexpvecdialog:nexp"))
  {
    multiexpvecdialog_nexp=m.get_int("multiexpvecdialog:nexp");
    multiexpmatdialog_nexp=m.get_int("multiexpvecdialog:nexp");
  }
  if(m.exists("multiexpvecdialog:nvec"))
  {
    multiexpvecdialog_nvec=m.get_int("multiexpvecdialog:nvec");
    multiexpmatdialog_dim_1=m.get_int("multiexpvecdialog:nvec");
    multiexpmatdialog_dim_2=m.get_int("multiexpvecdialog:nvec");
  }
  if(m.exists("multiexpvecdialog:BC")) multiexpvecdialog_BC=m.get_bool("multiexpvecdialog:BC");
  if(m.exists("multiexpvecdialog:constant")) multiexpvecdialog_BC=m.get_bool("multiexpvecdialog:constant");

  if(m.exists("multiexpmatdialog:nexp")) multiexpmatdialog_nexp=m.get_int("multiexpmatdialog:nexp");
  if(m.exists("multiexpmatdialog:dim1")) multiexpmatdialog_dim_1=m.get_int("multiexpmatdialog:dim1");
  if(m.exists("multiexpmatdialog:dim2")) multiexpmatdialog_dim_2=m.get_int("multiexpmatdialog:dim2");

  if(m.exists("multiexpmatupperdialog:nexp")) multiexpmatupperdialog_nexp=m.get_int("multiexpmatupperdialog:nexp");
  if(m.exists("multiexpmatupperdialog:dim")) multiexpmatupperdialog_dim=m.get_int("multiexpmatupperdialog:dim");

  if(m.exists("multialtexpdialog:nexp")) multialtexpdialog_nexp=m.get_int("multialtexpdialog:nexp");
  if(m.exists("multialtexpdialog:mexp")) multialtexpdialog_mexp=m.get_int("multialtexpdialog:mexp");
  if(m.exists("multialtexpdialog:BC")) multialtexpdialog_BC=m.get_bool("multialtexpdialog:BC");
  if(m.exists("multialtexpdialog:constant")) multialtexpdialog_BC=m.get_bool("multialtexpdialog:constant");
  if(m.exists("multialtexpvecdialog:nexp"))
  {
    multialtexpvecdialog_nexp=m.get_int("multialtexpvecdialog:nexp");
    multialtexpmatdialog_nexp=m.get_int("multialtexpvecdialog:nexp");
  }
  if(m.exists("multialtexpvecdialog:mexp"))
  {
    multialtexpvecdialog_mexp=m.get_int("multialtexpvecdialog:mexp");
    multialtexpmatdialog_mexp=m.get_int("multialtexpvecdialog:mexp");
  }
  if(m.exists("multialtexpvecdialog:nvec"))
  {
    multialtexpvecdialog_nvec=m.get_int("multialtexpvecdialog:nvec");
    multialtexpmatdialog_dim_1=m.get_int("multialtexpvecdialog:nvec");
    multialtexpmatdialog_dim_2=m.get_int("multialtexpvecdialog:nvec");
  }
  if(m.exists("multialtexpvecdialog:BC")) multialtexpvecdialog_BC=m.get_bool("multialtexpvecdialog:BC");
  if(m.exists("multialtexpvecdialog:constant")) multialtexpvecdialog_BC=m.get_bool("multialtexpvecdialog:constant");

  if(m.exists("multialtexpmatdialog:nexp")) multialtexpmatdialog_nexp=m.get_int("multialtexpmatdialog:nexp");
  if(m.exists("multialtexpmatdialog:mexp")) multialtexpmatdialog_mexp=m.get_int("multialtexpmatdialog:mexp");
  if(m.exists("multialtexpmatdialog:dim1")) multialtexpmatdialog_dim_1=m.get_int("multialtexpmatdialog:dim1");
  if(m.exists("multialtexpmatdialog:dim2")) multialtexpmatdialog_dim_2=m.get_int("multialtexpmatdialog:dim2");

  if(m.exists("threeptmultiexpdialog:nexpinitial")) threeptmultiexpdialog_nexpinitial=m.get_int("threeptmultiexpdialog:nexpinitial");
  if(m.exists("threeptmultiexpdialog:nexpfinal")) threeptmultiexpdialog_nexpfinal=m.get_int("threeptmultiexpdialog:nexpfinal");

  if(m.exists("threeptmultiexpvecdialog:nexpinitial")) threeptmultiexpvecdialog_nexpinitial=m.get_int("threeptmultiexpvecdialog:nexpinitial");
  if(m.exists("threeptmultiexpvecdialog:nexpfinal")) threeptmultiexpvecdialog_nexpfinal=m.get_int("threeptmultiexpvecdialog:nexpfinal");
  if(m.exists("threeptmultiexpvecdialog:nvec")) threeptmultiexpvecdialog_nvec=m.get_int("threeptmultiexpvecdialog:nvec");

  if(m.exists("threeptmultialtexpdialog:nexpinitial")) threeptmultialtexpdialog_nexpinitial=m.get_int("threeptmultialtexpdialog:nexpinitial");
  if(m.exists("threeptmultialtexpdialog:nexpfinal")) threeptmultialtexpdialog_nexpfinal=m.get_int("threeptmultialtexpdialog:nexpfinal");
  if(m.exists("threeptmultialtexpdialog:mexpinitial")) threeptmultialtexpdialog_mexpinitial=m.get_int("threeptmultialtexpdialog:mexpinitial");
  if(m.exists("threeptmultialtexpdialog:mexpfinal")) threeptmultialtexpdialog_mexpfinal=m.get_int("threeptmultialtexpdialog:mexpfinal");

  if(m.exists("threeptmultialtexpvecdialog:nexpinitial")) threeptmultialtexpvecdialog_nexpinitial=m.get_int("threeptmultialtexpvecdialog:nexpinitial");
  if(m.exists("threeptmultialtexpvecdialog:nexpfinal")) threeptmultialtexpvecdialog_nexpfinal=m.get_int("threeptmultialtexpvecdialog:nexpfinal");
  if(m.exists("threeptmultialtexpvecdialog:mexpinitial")) threeptmultialtexpvecdialog_mexpinitial=m.get_int("threeptmultialtexpvecdialog:mexpinitial");
  if(m.exists("threeptmultialtexpvecdialog:mexpfinal")) threeptmultialtexpvecdialog_mexpfinal=m.get_int("threeptmultialtexpvecdialog:mexpfinal");
  if(m.exists("threeptmultialtexpvecdialog:nvec")) threeptmultialtexpvecdialog_nvec=m.get_int("threeptmultialtexpvecdialog:nvec");

  if(m.exists("nparametersdof"))
  {
    n_parameters_dof=m.get_int("nparametersdof");
  }
  else
  {
    if(bayesian)
    {
      n_parameters_dof=0;
    }
    else
    {
      n_parameters_dof=n_parameters;
    }
  }
  if(m.exists("chisqrextraterm:enabled"))
  {
    chisqr_extra_term_enabled=m.get_bool("chisqrextraterm:enabled");
    if(m.exists("chisqrextraterm:function")) chisqr_extra_term_function=m.get_string("chisqrextraterm:function");
    if(m.exists("chisqrextraterm:numdiffstep")) chisqr_extra_term_num_diff_step=m.get_double("chisqrextraterm:numdiffstep");
    if(!m.exists("chisqrextraterm:nconstants"))
    {
      cerr << "input file does not contain number of chi^2 extra term constants" << endl;
      return false;
    }
    chisqr_extra_term_n_constants=m.get_int("chisqrextraterm:nconstants");
    chisqr_extra_term_constant_names.resize(chisqr_extra_term_n_constants);
    chisqr_extra_term_constant_values.resize(chisqr_extra_term_n_constants);
    for(int c=0; c<chisqr_extra_term_n_constants; ++c)
    {
      chisqr_extra_term_constant_names[c]=m.get_string("chisqrextraterm:constant", c);
      if(m.exists("chisqrextraterm:constantvalue", c))
      {
        double val=m.get_double("chisqrextraterm:constantvalue", c);
        if(val!=empty_double)
        {
          chisqr_extra_term_constant_values[c]=val;
        }
      }
    }
  }
  return true;
}


bool set_model(string& name, map<string, string>& properties)
{
  properties.clear();
  if (current_model==MULTIEXP)
  {
    name="multi_exp_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
  }
  else if (current_model==MULTIEXPASQR)
  {
    name="multi_exp_Asqr_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
  }
  else if (current_model==MULTIEXPASQREXPE)
  {
    name="multi_exp_Asqr_expE_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
  }
  else if (current_model==MULTIEXPEXPE)
  {
    name="multi_exp_expE_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
  }
  else if (current_model==MULTIALTEXP)
  {
    name="multi_alt_exp_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
  }
  else if (current_model==MULTIALTEXPASQR)
  {
    name="multi_alt_exp_Asqr_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
  }
  else if (current_model==MULTIALTEXPASQREXPE)
  {
    name="multi_alt_exp_Asqr_expE_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
  }
  else if (current_model==MULTIALTEXPEXPE)
  {
    name="multi_alt_exp_expE_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
  }
  else if (current_model==MULTIEXPVEC)
  {
    name="multi_exp_vec_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpvecdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpvecdialog_nvec);
  }
  else if (current_model==MULTIEXPASQRVEC)
  {
    name="multi_exp_Asqr_vec_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpvecdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpvecdialog_nvec);
  }
  else if (current_model==MULTIEXPASQREXPEVEC)
  {
    name="multi_exp_Asqr_expE_vec_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpvecdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpvecdialog_nvec);
  }
  else if (current_model==MULTIEXPEXPEVEC)
  {
    name="multi_exp_expE_vec_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpvecdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpvecdialog_nvec);
  }
  else if (current_model==MULTIALTEXPVEC)
  {
    name="multi_alt_exp_vec_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multialtexpvecdialog_nvec);
  }
  else if (current_model==MULTIALTEXPASQRVEC)
  {
    name="multi_alt_exp_Asqr_vec_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multialtexpvecdialog_nvec);
  }
  else if (current_model==MULTIALTEXPASQREXPEVEC)
  {
    name="multi_alt_exp_Asqr_expE_vec_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multialtexpvecdialog_nvec);
  }
  else if (current_model==MULTIALTEXPEXPEVEC)
  {
    name="multi_alt_exp_expE_vec_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multialtexpvecdialog_nvec);
  }
  else if (current_model==MULTIEXPMAT)
  {
    name="multi_exp_mat_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpmatdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim_1"]=boost::lexical_cast<string>(multiexpmatdialog_dim_1);
    properties["dim_2"]=boost::lexical_cast<string>(multiexpmatdialog_dim_2);
  }
  else if (current_model==MULTIEXPEXPEMAT)
  {
    name="multi_exp_expE_mat_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpmatdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim_1"]=boost::lexical_cast<string>(multiexpmatdialog_dim_1);
    properties["dim_2"]=boost::lexical_cast<string>(multiexpmatdialog_dim_2);
  }
  else if (current_model==MULTIEXPMATII)
  {
    name="multi_exp_mat_II_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpmatdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim_1"]=boost::lexical_cast<string>(multiexpmatdialog_dim_1);
    properties["dim_2"]=boost::lexical_cast<string>(multiexpmatdialog_dim_2);
  }
  else if (current_model==MULTIEXPEXPEMATII)
  {
    name="multi_exp_expE_mat_II_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpmatdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim_1"]=boost::lexical_cast<string>(multiexpmatdialog_dim_1);
    properties["dim_2"]=boost::lexical_cast<string>(multiexpmatdialog_dim_2);
  }
  else if (current_model==MULTIEXPMATUPPER)
  {
    name="multi_exp_mat_upper_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpmatupperdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpmatupperdialog_dim);
  }
  else if (current_model==MULTIEXPEXPEMATUPPER)
  {
    name="multi_exp_expE_mat_upper_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpmatupperdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpmatupperdialog_dim);
  }
  else if (current_model==MULTIEXPMATIIUPPER)
  {
    name="multi_exp_mat_II_upper_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpmatupperdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpmatupperdialog_dim);
  }
  else if (current_model==MULTIEXPEXPEMATIIUPPER)
  {
    name="multi_exp_expE_mat_II_upper_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpmatupperdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpmatupperdialog_dim);
  }
  else if (current_model==MULTIALTEXPMAT)
  {
    name="multi_alt_exp_mat_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpmatdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpmatdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim_1"]=boost::lexical_cast<string>(multialtexpmatdialog_dim_1);
    properties["dim_2"]=boost::lexical_cast<string>(multialtexpmatdialog_dim_2);
  }
  else if (current_model==MULTIALTEXPEXPEMAT)
  {
    name="multi_alt_exp_expE_mat_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpmatdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpmatdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim_1"]=boost::lexical_cast<string>(multialtexpmatdialog_dim_1);
    properties["dim_2"]=boost::lexical_cast<string>(multialtexpmatdialog_dim_2);
  }
  else if (current_model==MULTIEXPNONSYMMAT)
  {
    name="multi_exp_nonsym_mat_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpmatdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim_1"]=boost::lexical_cast<string>(multiexpmatdialog_dim_1);
    properties["dim_2"]=boost::lexical_cast<string>(multiexpmatdialog_dim_2);
  }
  else if (current_model==MULTIEXPEXPENONSYMMAT)
  {
    name="multi_exp_expE_nonsym_mat_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpmatdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim_1"]=boost::lexical_cast<string>(multiexpmatdialog_dim_1);
    properties["dim_2"]=boost::lexical_cast<string>(multiexpmatdialog_dim_2);
  }
  else if (current_model==MULTIALTEXPNONSYMMAT)
  {
    name="multi_alt_exp_nonsym_mat_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpmatdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpmatdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim_1"]=boost::lexical_cast<string>(multialtexpmatdialog_dim_1);
    properties["dim_2"]=boost::lexical_cast<string>(multialtexpmatdialog_dim_2);
  }
  else if (current_model==MULTIALTEXPEXPENONSYMMAT)
  {
    name="multi_alt_exp_expE_nonsym_mat_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpmatdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpmatdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim_1"]=boost::lexical_cast<string>(multialtexpmatdialog_dim_1);
    properties["dim_2"]=boost::lexical_cast<string>(multialtexpmatdialog_dim_2);
  }
  else if (current_model==MULTIEXPBC)
  {
    name="multi_exp_BC_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==MULTIEXPASQRBC)
  {
    name="multi_exp_Asqr_BC_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==MULTIEXPASQREXPEBC)
  {
    name="multi_exp_Asqr_expE_BC_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==MULTIEXPEXPEBC)
  {
    name="multi_exp_expE_BC_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==MULTIALTEXPBC)
  {
    name="multi_alt_exp_BC_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==MULTIALTEXPASQRBC)
  {
    name="multi_alt_exp_Asqr_BC_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==MULTIALTEXPASQREXPEBC)
  {
    name="multi_alt_exp_Asqr_expE_BC_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==MULTIALTEXPEXPEBC)
  {
    name="multi_alt_exp_expE_BC_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==MULTIEXPVECBC)
  {
    name="multi_exp_vec_BC_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpvecdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpvecdialog_nvec);
    properties["T_name"]="T";
  }
  else if (current_model==MULTIEXPASQRVECBC)
  {
    name="multi_exp_Asqr_vec_BC_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpvecdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpvecdialog_nvec);
    properties["T_name"]="T";
  }
  else if (current_model==MULTIEXPASQREXPEVECBC)
  {
    name="multi_exp_Asqr_expE_vec_BC_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpvecdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpvecdialog_nvec);
    properties["T_name"]="T";
  }
  else if (current_model==MULTIEXPEXPEVECBC)
  {
    name="multi_exp_expE_vec_BC_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpvecdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpvecdialog_nvec);
    properties["T_name"]="T";
  }
  else if (current_model==MULTIALTEXPVECBC)
  {
    name="multi_alt_exp_vec_BC_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multialtexpvecdialog_nvec);
    properties["T_name"]="T";
  }
  else if (current_model==MULTIALTEXPASQRVECBC)
  {
    name="multi_alt_exp_Asqr_vec_BC_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multialtexpvecdialog_nvec);
    properties["T_name"]="T";
  }
  else if (current_model==MULTIALTEXPASQREXPEVECBC)
  {
    name="multi_alt_exp_Asqr_expE_vec_BC_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multialtexpvecdialog_nvec);
    properties["T_name"]="T";
  }
  else if (current_model==MULTIALTEXPEXPEVECBC)
  {
    name="multi_alt_exp_expE_vec_BC_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multialtexpvecdialog_nvec);
    properties["T_name"]="T";
  }
  else if (current_model==THREEPT2VARMULTIEXP)
  {
    name="threept_multi_exp_model";
    properties["n_exp_initial"]=boost::lexical_cast<string>(threeptmultiexpdialog_nexpinitial);
    properties["n_exp_final"]=boost::lexical_cast<string>(threeptmultiexpdialog_nexpfinal);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_initial_name"]="E";
    properties["dE_initial_name"]="dE";
    properties["E_final_name"]="F";
    properties["dE_final_name"]="dF";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==THREEPT2VARMULTIEXPEXPE)
  {
    name="threept_multi_exp_expE_model";
    properties["n_exp_initial"]=boost::lexical_cast<string>(threeptmultiexpdialog_nexpinitial);
    properties["n_exp_final"]=boost::lexical_cast<string>(threeptmultiexpdialog_nexpfinal);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_initial_name"]="E";
    properties["dE_initial_name"]="dE";
    properties["E_final_name"]="F";
    properties["dE_final_name"]="dF";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==THREEPT2VARMULTIEXPVEC)
  {
    name="threept_multi_exp_vec_model";
    properties["n_exp_initial"]=boost::lexical_cast<string>(threeptmultiexpvecdialog_nexpinitial);
    properties["n_exp_final"]=boost::lexical_cast<string>(threeptmultiexpvecdialog_nexpfinal);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_initial_name"]="E";
    properties["dE_initial_name"]="dE";
    properties["E_final_name"]="F";
    properties["dE_final_name"]="dF";
    properties["t_name"]="t";
    properties["T_name"]="T";
    properties["dim"]=boost::lexical_cast<string>(threeptmultiexpvecdialog_nvec);
  }
  else if (current_model==THREEPT2VARMULTIEXPEXPEVEC)
  {
    name="threept_multi_exp_expE_vec_model";
    properties["n_exp_initial"]=boost::lexical_cast<string>(threeptmultiexpvecdialog_nexpinitial);
    properties["n_exp_final"]=boost::lexical_cast<string>(threeptmultiexpvecdialog_nexpfinal);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_initial_name"]="E";
    properties["dE_initial_name"]="dE";
    properties["E_final_name"]="F";
    properties["dE_final_name"]="dF";
    properties["t_name"]="t";
    properties["T_name"]="T";
    properties["dim"]=boost::lexical_cast<string>(threeptmultiexpvecdialog_nvec);
  }
  else if (current_model==THREEPT2VARMULTIALTEXP)
  {
    name="threept_multi_alt_exp_model";
    properties["n_exp_initial"]=boost::lexical_cast<string>(threeptmultialtexpdialog_nexpinitial);
    properties["n_o_exp_initial"]=boost::lexical_cast<string>(threeptmultialtexpdialog_mexpinitial);
    properties["n_exp_final"]=boost::lexical_cast<string>(threeptmultialtexpdialog_nexpfinal);
    properties["n_o_exp_final"]=boost::lexical_cast<string>(threeptmultialtexpdialog_mexpfinal);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_initial_name"]="E";
    properties["dE_initial_name"]="dE";
    properties["E_final_name"]="F";
    properties["dE_final_name"]="dF";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==THREEPT2VARMULTIALTEXPEXPE)
  {
    name="threept_multi_alt_exp_expE_model";
    properties["n_exp_initial"]=boost::lexical_cast<string>(threeptmultialtexpdialog_nexpinitial);
    properties["n_o_exp_initial"]=boost::lexical_cast<string>(threeptmultialtexpdialog_mexpinitial);
    properties["n_exp_final"]=boost::lexical_cast<string>(threeptmultialtexpdialog_nexpfinal);
    properties["n_o_exp_final"]=boost::lexical_cast<string>(threeptmultialtexpdialog_mexpfinal);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_initial_name"]="E";
    properties["dE_initial_name"]="dE";
    properties["E_final_name"]="F";
    properties["dE_final_name"]="dF";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==THREEPT2VARMULTIALTEXPVEC)
  {
    name="threept_multi_alt_exp_vec_model";
    properties["n_exp_initial"]=boost::lexical_cast<string>(threeptmultialtexpvecdialog_nexpinitial);
    properties["n_o_exp_initial"]=boost::lexical_cast<string>(threeptmultialtexpvecdialog_mexpinitial);
    properties["n_exp_final"]=boost::lexical_cast<string>(threeptmultialtexpvecdialog_nexpfinal);
    properties["n_o_exp_final"]=boost::lexical_cast<string>(threeptmultialtexpvecdialog_mexpfinal);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_initial_name"]="E";
    properties["dE_initial_name"]="dE";
    properties["E_final_name"]="F";
    properties["dE_final_name"]="dF";
    properties["t_name"]="t";
    properties["T_name"]="T";
    properties["dim"]=boost::lexical_cast<string>(threeptmultialtexpvecdialog_nvec);
  }
  else if (current_model==THREEPT2VARMULTIALTEXPEXPEVEC)
  {
    name="threept_multi_alt_exp_expE_vec_model";
    properties["n_exp_initial"]=boost::lexical_cast<string>(threeptmultialtexpvecdialog_nexpinitial);
    properties["n_o_exp_initial"]=boost::lexical_cast<string>(threeptmultialtexpvecdialog_mexpinitial);
    properties["n_exp_final"]=boost::lexical_cast<string>(threeptmultialtexpvecdialog_nexpfinal);
    properties["n_o_exp_final"]=boost::lexical_cast<string>(threeptmultialtexpvecdialog_mexpfinal);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["E_initial_name"]="E";
    properties["dE_initial_name"]="dE";
    properties["E_final_name"]="F";
    properties["dE_final_name"]="dF";
    properties["t_name"]="t";
    properties["T_name"]="T";
    properties["dim"]=boost::lexical_cast<string>(threeptmultialtexpvecdialog_nvec);
  }
  else if (current_model==MULTIEXPCONST)
  {
    name="multi_exp_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
  }
  else if (current_model==MULTIEXPASQRCONST)
  {
    name="multi_exp_Asqr_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
  }
  else if (current_model==MULTIEXPASQREXPECONST)
  {
    name="multi_exp_Asqr_expE_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
  }
  else if (current_model==MULTIEXPEXPECONST)
  {
    name="multi_exp_expE_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
  }
  else if (current_model==MULTIALTEXPCONST)
  {
    name="multi_alt_exp_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
  }
  else if (current_model==MULTIALTEXPASQRCONST)
  {
    name="multi_alt_exp_Asqr_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
  }
  else if (current_model==MULTIALTEXPASQREXPECONST)
  {
    name="multi_alt_exp_Asqr_expE_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
  }
  else if (current_model==MULTIALTEXPEXPECONST)
  {
    name="multi_alt_exp_expE_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
  }
  else if (current_model==MULTIEXPVECCONST)
  {
    name="multi_exp_vec_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpvecdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpvecdialog_nvec);
  }
  else if (current_model==MULTIEXPASQRVECCONST)
  {
    name="multi_exp_Asqr_vec_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpvecdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpvecdialog_nvec);
  }
  else if (current_model==MULTIEXPASQREXPEVECCONST)
  {
    name="multi_exp_Asqr_expE_vec_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpvecdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpvecdialog_nvec);
  }
  else if (current_model==MULTIEXPEXPEVECCONST)
  {
    name="multi_exp_expE_vec_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpvecdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpvecdialog_nvec);
  }
  else if (current_model==MULTIALTEXPVECCONST)
  {
    name="multi_alt_exp_vec_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multialtexpvecdialog_nvec);
  }
  else if (current_model==MULTIALTEXPASQRVECCONST)
  {
    name="multi_alt_exp_Asqr_vec_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multialtexpvecdialog_nvec);
  }
  else if (current_model==MULTIALTEXPASQREXPEVECCONST)
  {
    name="multi_alt_exp_Asqr_expE_vec_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multialtexpvecdialog_nvec);
  }
  else if (current_model==MULTIALTEXPEXPEVECCONST)
  {
    name="multi_alt_exp_expE_vec_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multialtexpvecdialog_nvec);
  }
  else if (current_model==MULTIEXPBCCONST)
  {
    name="multi_exp_BC_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==MULTIEXPASQRBCCONST)
  {
    name="multi_exp_Asqr_BC_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==MULTIEXPASQREXPEBCCONST)
  {
    name="multi_exp_Asqr_expE_BC_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==MULTIEXPEXPEBCCONST)
  {
    name="multi_exp_expE_BC_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==MULTIALTEXPBCCONST)
  {
    name="multi_alt_exp_BC_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==MULTIALTEXPASQRBCCONST)
  {
    name="multi_alt_exp_Asqr_BC_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==MULTIALTEXPASQREXPEBCCONST)
  {
    name="multi_alt_exp_Asqr_expE_BC_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==MULTIALTEXPEXPEBCCONST)
  {
    name="multi_alt_exp_expE_BC_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["T_name"]="T";
  }
  else if (current_model==MULTIEXPVECBCCONST)
  {
    name="multi_exp_vec_BC_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpvecdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpvecdialog_nvec);
    properties["T_name"]="T";
  }
  else if (current_model==MULTIEXPASQRVECBCCONST)
  {
    name="multi_exp_Asqr_vec_BC_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpvecdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpvecdialog_nvec);
    properties["T_name"]="T";
  }
  else if (current_model==MULTIEXPASQREXPEVECBCCONST)
  {
    name="multi_exp_Asqr_expE_vec_BC_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpvecdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpvecdialog_nvec);
    properties["T_name"]="T";
  }
  else if (current_model==MULTIEXPEXPEVECBCCONST)
  {
    name="multi_exp_expE_vec_BC_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multiexpvecdialog_nexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multiexpvecdialog_nvec);
    properties["T_name"]="T";
  }
  else if (current_model==MULTIALTEXPVECBCCONST)
  {
    name="multi_alt_exp_vec_BC_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multialtexpvecdialog_nvec);
    properties["T_name"]="T";
  }
  else if (current_model==MULTIALTEXPASQRVECBCCONST)
  {
    name="multi_alt_exp_Asqr_vec_BC_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multialtexpvecdialog_nvec);
    properties["T_name"]="T";
  }
  else if (current_model==MULTIALTEXPASQREXPEVECBCCONST)
  {
    name="multi_alt_exp_Asqr_expE_vec_BC_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multialtexpvecdialog_nvec);
    properties["T_name"]="T";
  }
  else if (current_model==MULTIALTEXPEXPEVECBCCONST)
  {
    name="multi_alt_exp_expE_vec_BC_const_model";
    properties["n_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_nexp);
    properties["n_o_exp"]=boost::lexical_cast<string>(multialtexpvecdialog_mexp);
    properties["A_name"]="A";
    properties["B_name"]="B";
    properties["C_name"]="C";
    properties["E_name"]="E";
    properties["dE_name"]="dE";
    properties["t_name"]="t";
    properties["dim"]=boost::lexical_cast<string>(multialtexpvecdialog_nvec);
    properties["T_name"]="T";
  }
  else if (current_model==PARSE)
  {
    name="parse_model";
    properties["n_variables"]=boost::lexical_cast<string>(n_variables);
    properties["n_functions"]=boost::lexical_cast<string>(n_functions);
  }
  else 
  {
    cerr << "Error: unknown model" << endl;
    return false;
  }
  return true;
}


void print_usage()
{
  std::cout << "usage: MBF_to_XMBF input.mbf output.xml" << std::endl << std::endl;
  std::cout << std::endl;
}



int main(int argc, char *argv[])
{
  if(argc!=3)
  {
    print_usage();
    return 1;
  }
  std::string inputfile=argv[1];
  std::string outputfile=argv[2];

  init();
  if(!loadFile(inputfile))
  {
    return 1;
  }

  string model_name;
  map<string, string> properties;

  if(!set_model(model_name, properties))
  {
    return 1;
  }

  #ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
  try
  {
  #endif //LIBXMLCPP_EXCEPTIONS_ENABLED
    xmlpp::Document document;

    xmlpp::Element* root = document.create_root_node("fit");

    // combined_model node
    xmlpp::Element* combined_model_node = root->add_child("combined_model");

    xmlpp::Element* model_node = combined_model_node->add_child(model_name);

    for(map<string, string>::iterator it=properties.begin(); it!=properties.end(); ++it)
    {
      addchild(model_node, it->first, it->second);
    }


    if(model_name=="parse_model")
    {
      // variables node
      xmlpp::Element* variables_node = model_node->add_child("variables");
      for(int v=0; v<n_variables; ++v)
      {
        xmlpp::Element* variable_node = variables_node->add_child("variable");
        addchild(variable_node, "number", int_to_string(v+1));
        addchild(variable_node, "name", variables[v]);
      }

      // functions node
      xmlpp::Element* functions_node = model_node->add_child("functions");
      for(int f=0; f<n_functions; ++f)
      {
        xmlpp::Element* function_node = functions_node->add_child("function");
        addchild(function_node, "number", int_to_string(f+1));
        addchild(function_node, "definition", functions[f]);
      }

      // derivatives node
      xmlpp::Element* derivatives_node = model_node->add_child("derivatives");
      for(int f=0; f<n_functions; ++f)
      {
        for(int p=0; p<n_parameters; ++p)
        {
          xmlpp::Element* derivative_node = derivatives_node->add_child("derivative");
          addchild(derivative_node, "function_number", int_to_string(f+1));
          addchild(derivative_node, "parameter_name", parameters[p]);
          addchild(derivative_node, "definition", derivatives[f][p]);
        }
      }

      // constants node
      xmlpp::Element* constants_node = model_node->add_child("constants");
      for(int c=0; c<n_constants; ++c)
      {
        addchild(constants_node, "name", constants[c]);
      }

      // parameters node
      xmlpp::Element* parameters_node = model_node->add_child("parameters");
      for(int p=0; p<n_parameters; ++p)
      {
        addchild(parameters_node, "name", parameters[p]);
      }

    }


    for(int v=0; v<n_variables; ++v)
    {
      xmlpp::Element* fit_domain_node = model_node->add_child("fit_domain");

      addchild(fit_domain_node, "variable_name", variables[v]);

      xmlpp::Element* range_node=fit_domain_node-> add_child("range");

      addchild(range_node, "min", fit_min[v]);
      addchild(range_node, "max", fit_max[v]);
    }

    xmlpp::Element* datafile_node = model_node->add_child("data_file");
    if(data_file_type==0)
    {
      addchild(datafile_node, "file_type", "ASCII");
    }
    else
    {
      addchild(datafile_node, "file_type", "binary");
    }
    addchild(datafile_node, "file_name", data_file);


    // chi_sqr_extra_term node
    if(chisqr_extra_term_enabled)
    {
      xmlpp::Element* chisqr_extra_term_node = root->add_child("chi_sqr_extra_term");
      addchild(chisqr_extra_term_node, "function", chisqr_extra_term_function);
      for(int c=0; c<chisqr_extra_term_n_constants; ++c)
      {
        xmlpp::Element* constant_node = chisqr_extra_term_node->add_child("constant");
        addchild(constant_node, "name", chisqr_extra_term_constant_names[c]);
        addchild(constant_node, "value", double_to_string(chisqr_extra_term_constant_values[c], 14));
      }
      addchild(chisqr_extra_term_node, "num_diff_step", chisqr_extra_term_num_diff_step);
    }
 
    // fit_settings node
    xmlpp::Element* fit_settings_node = root->add_child("fit_settings");
    addchild(fit_settings_node, "restrict_data_range", bool_to_string(restrict_data));
    addchild(fit_settings_node, "data_range_min", start_n_data);
    addchild(fit_settings_node, "data_range_max", stop_n_data);
    addchild(fit_settings_node, "chi_sqr_extra_term_enabled",  bool_to_string(chisqr_extra_term_enabled));
    addchild(fit_settings_node, "bayesian", bool_to_string(bayesian));

    addchild(fit_settings_node, "num_diff_first_order", bool_to_string(num_diff));
    addchild(fit_settings_node, "second_deriv_covariance", bool_to_string(second_deriv_covariance));
    addchild(fit_settings_node, "second_deriv_minimization", bool_to_string(second_deriv_minimization));
    addchild(fit_settings_node, "num_diff_step", double_to_string(num_diff_step, 14));

    addchild(fit_settings_node, "random_priors", bool_to_string(boot_prior));
    addchild(fit_settings_node, "start_lambda",  double_to_string(start_lambda, 14));
    addchild(fit_settings_node, "lambda_factor",  double_to_string(lambda_factor, 14));
    addchild(fit_settings_node, "chi_sqr_tolerance",  double_to_string(chisqr_tolerance, 14));
    addchild(fit_settings_node, "chi_sqr_per_dof_tolerance",  bool_to_string(false));
    addchild(fit_settings_node, "n_parameters_dof",  n_parameters_dof);
    addchild(fit_settings_node, "bootstrap_normalization", bool_to_string(bootstrap_normalization));

    switch(inv_method)
    {
      case LU_inversion:
        addchild(fit_settings_node, "inversion_method", "LU");
        break;
      case simple_cut:
        addchild(fit_settings_node, "inversion_method", "svd_fixed_cut");
        break;
      case ratio_cut:
        addchild(fit_settings_node, "inversion_method", "svd_ratio_cut");
        break;
      case absolute_cut:
        addchild(fit_settings_node, "inversion_method", "svd_absolute_cut");
        break;
      case diagonal:
        addchild(fit_settings_node, "inversion_method", "diagonal");
        break;
      case off_diagonal_rescale:
        addchild(fit_settings_node, "inversion_method", "off_diagonal_rescale");
        break;
      default:
        break;
    }
    addchild(fit_settings_node, "svd_fixed_cut", svd_cut);
    addchild(fit_settings_node, "svd_ratio_cut",  double_to_string(svd_ratio, 14));
    addchild(fit_settings_node, "svd_absolute_cut", double_to_string(svd_value, 14));
    addchild(fit_settings_node, "off_diagonal_rescale_factor", double_to_string(rescale_factor, 14));
    addchild(fit_settings_node, "max_iterations", max_steps);
    addchild(fit_settings_node, "bin_size", bin_size);
    addchild(fit_settings_node, "bootstrap_samples", bssamples);
    addchild(fit_settings_node, "use_bse_file", bool_to_string(use_bse));
    addchild(fit_settings_node, "bse_file", bse_file);

    addchild(fit_settings_node, "restrict_bootstrap_range",  bool_to_string(false));
    addchild(fit_settings_node, "bootstrap_range_min",  1);
    addchild(fit_settings_node, "bootstrap_range_max",  50);

    if((version < 5.0) && (model_name!="parse_model"))  // change parameter names to XMBF style (which is identical to QMBF 5.0 style)
    {
      for(int p=0; p<n_parameters; ++p)
      {
        // the order of instructions here is crucial

        // these must remain unchanged
        if (parameters[p].find("Aee")!=std::string::npos)  goto end_of_loop;
        if (parameters[p].find("Aeo")!=std::string::npos)  goto end_of_loop;
        if (parameters[p].find("Aoe")!=std::string::npos)  goto end_of_loop;
        if (parameters[p].find("Aoo")!=std::string::npos)  goto end_of_loop;
        if (parameters[p].find("Bee")!=std::string::npos)  goto end_of_loop;
        if (parameters[p].find("Beo")!=std::string::npos)  goto end_of_loop;
        if (parameters[p].find("Boe")!=std::string::npos)  goto end_of_loop;
        if (parameters[p].find("Boo")!=std::string::npos)  goto end_of_loop;

        // energy parameters
        if (string_replace(parameters[p], "dEo", "dEo_")) goto end_of_loop;
        if (string_replace(parameters[p], "dE",  "dE_")) goto end_of_loop;
        if (string_replace(parameters[p], "dFo", "dFo_")) goto end_of_loop;
        if (string_replace(parameters[p], "dF",  "dF_")) goto end_of_loop;
        if (string_replace(parameters[p], "Eo1", "Eo")) goto end_of_loop;
        if (string_replace(parameters[p], "E1",  "E")) goto end_of_loop;
        if (string_replace(parameters[p], "Fo1", "Fo")) goto end_of_loop;
        if (string_replace(parameters[p], "F1",  "F")) goto end_of_loop;

        // all underscores now correspond to vector/matrix components
        string_replace(parameters[p], "_", "__");

        if (string_replace(parameters[p], "Aox1", "Aox")) goto end_of_loop;
        if (string_replace(parameters[p], "Ax1",  "Ax")) goto end_of_loop;

        if (string_replace(parameters[p], "Box", "Box_")) goto end_of_loop;
        if (string_replace(parameters[p], "Bx",  "Bx_")) goto end_of_loop;

        if (string_replace(parameters[p], "Aoy1", "Aoy")) goto end_of_loop;
        if (string_replace(parameters[p], "Ay1",  "Ay")) goto end_of_loop;

        if (string_replace(parameters[p], "Boy", "Boy_")) goto end_of_loop;
        if (string_replace(parameters[p], "By",  "By_")) goto end_of_loop;

        if (string_replace(parameters[p], "Ao1", "Ao")) goto end_of_loop;
        if (string_replace(parameters[p], "A1",  "A")) goto end_of_loop;

        if (string_replace(parameters[p], "Bo", "Bo_")) goto end_of_loop;
        if (string_replace(parameters[p], "B",  "B_")) goto end_of_loop;

        end_of_loop: ;
      }
    }

    // parameter_values node
    xmlpp::Element* parameter_values_node = root->add_child("parameter_values");

    for(int p=0; p<n_parameters; ++p)
    {
      xmlpp::Element* parameter_node = parameter_values_node->add_child("parameter");
      addchild(parameter_node, "name", parameters[p]);
      addchild(parameter_node, "start_value", double_to_string(start_values[p], 14));
      if(bayesian)
      {
        addchild(parameter_node, "prior", double_to_string(priors[p], 14));
        addchild(parameter_node, "prior_width", double_to_string(sigmas[p], 14));
      }
    }

    // constant_values node
    xmlpp::Element* constant_values_node = root->add_child("constant_values");
    for(int c=0; c<n_constants; ++c)
    {
      xmlpp::Element* constant_node = constant_values_node->add_child("constant");
      addchild(constant_node, "name", constants[c]);
      addchild(constant_node, "value", double_to_string(constant_values[c], 14));
    }

    document.write_to_file_formatted(outputfile);

  #ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
  }
  catch(const std::exception& ex)
  {
    std::cout << "Exception caught: " << ex.what() << std::endl;
    return 1;
  }
  #endif //LIBXMLCPP_EXCEPTIONS_ENABLED

  return 0;
}
