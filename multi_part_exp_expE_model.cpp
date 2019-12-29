#include "multi_part_exp_expE_model.h"

// this model was added by William Detmold

multi_part_exp_expE_model::multi_part_exp_expE_model(string _E_str, string _dE_str, string _Z_str,
                                                     string _A_str, string _B_str, string _t_str,
                                                     string _T_str, int n_exp, int n_part)
{
    E_str=_E_str;
    dE_str=_dE_str;
    Z_str=_Z_str;
    A_str=_A_str;
    B_str=_B_str;
    nexp=n_exp;
    npart=n_part;
    t_str=_t_str;
    T_str=_T_str;
    if (npart%2==0)
        nmax = npart/2;
    else
        nmax = (npart-1)/2;
    nparam = npart+2*nexp-1+nmax;
    cout << "Nparams=" << nparam <<endl;
    cout << "Nmax=" << nmax <<endl;
    params.resize(nparam);
}


int multi_part_exp_expE_model::get_n_functions()
{
    return 1;
}


int multi_part_exp_expE_model::get_n_variables()
{
    return 1;
}


int multi_part_exp_expE_model::get_n_parameters()
{
    return nparam;
}

int multi_part_exp_expE_model::get_n_constants()
{
    return 1;
}


void multi_part_exp_expE_model::set_parameters(const vector< double >& parameters)
{
    params=parameters;
}


void multi_part_exp_expE_model::set_constants(const vector< double >& constants)
{
    T=constants[0];
}


double multi_part_exp_expE_model::eval_function(int function, const vector< double >& arguments)
{
    double t=arguments[0];

 //   cout << params[0] << " " << params[1] << endl;
 //   return params[npart]*exp(-exp(params[npart-1])*T/2)*  cosh(exp(params[npart-1])*(t-T/2));

 //     return params[2] * (exp(-exp(params[0])*T/2) * cosh(exp(params[0])*(t-T/2))+ params[3]*exp(-(exp(params[0])+exp(params[1]))*T/2) * cosh((exp(params[0])+exp(params[3]))*(t-T/2)));

    // pseudonyms for parameters
    double Z_N;
    vector<double> E,dE;
    vector<double> A_N,B_N;
    E.resize(npart+1);
    dE.resize(nexp);
    A_N.resize(nmax+1);
    B_N.resize(nexp);
    for(int n=1; n<= npart; ++n)
        E[n]=params[n-1];
    for(int n=1; n < nexp; ++n)
        dE[n]=params[npart+n-1];
    Z_N = params[npart+nexp-1];
    for(int n=1; n<= nmax; ++n)
        A_N[n]=params[npart+nexp-1+n];
    for(int n=1; n < nexp; ++n)
        B_N[n]=params[npart+nexp+nmax+n-1];

 //   cout << params[0] << " " << E[1] << endl;
 //   cout << params[1] << " " << dE[1] << endl;
 //   cout << params[2] << " " << Z_N << endl;
 //   cout << params[3] << " " << B_N[1] << endl;
    double sum=0.0;
    double tmp=0;	

    // ground state contribution
    sum += Z_N * exp(-exp(E[npart])*T/2) * cosh(exp(E[npart])*(t-T/2));

    // thermal state
    for(int n=1; n<= nmax; ++n)
    {   
        tmp = 2.0 * Z_N * gsl_sf_choose(npart,n) * A_N[n] * exp(-(exp(E[npart-n])+exp(E[n]))*T/2);
	if (n != npart - n) tmp *= cosh((exp(E[npart-n])-exp(E[n]))*(t-T/2));
	sum += tmp;
     } 
    //excited states
    double E0 = exp(E[npart]);
    for(int n=1; n<nexp; ++n)
    {
        E0+=exp(dE[n]);
        sum += Z_N * B_N[n] * exp(-E0*T/2) * cosh( E0 * (t-T/2));
    }
    return sum;
}


double multi_part_exp_expE_model::eval_derivative(int function, int parameter, const vector< double >& arguments)
{
	cout << "In undefined eval_derivative" << endl;
    if( (parameter >= 0) && (parameter < nexp) )
    {
        return 0.0;
    }
    else if( (parameter >= nexp) && (parameter < nexp+1) )
    {
        return 0.0;
    }
    else
    {
        return 0.0;
    }
}


int multi_part_exp_expE_model::no_of_errors()
{
    return 0;
}


string multi_part_exp_expE_model::get_function_name(int function, string variable)
{
    if(function!=0)
    {
        return "";
    }


    stringstream function_st;
    // Construct ground state part of function
    function_st << Z_str << "_" << npart << " * exp(-Exp(" << E_str << "_" << npart << ")*" << T_str <<
            "/2) * cosh(Exp(" << E_str << "_" << npart << ")*("<< variable << "-" << T_str << "/2))";

    // Construct thermal state part of function
    for(int n=1; n<=nmax; ++n)
    {
        function_st << "+ 2 * binomial(" << npart << "," << n << ") * " << Z_str <<"_" << npart
                << "*" << A_str << "_" << npart << "_" << n
                << " * exp(-(Exp("    << E_str << "_" << npart - n << ") + Exp(" << E_str << "_" <<  n << "))*" << T_str << "/2)"
                << " * cosh((Exp(" << E_str << "_" << npart - n << ") - Exp(" << E_str << "_" << n << "))*("
                << variable << "-" << T_str << "/2))";
    }
    // Construct excited state on non-thermal states
    for(int m=1; m<nexp; ++m)
    {
        function_st << "+" << Z_str << "_" << npart << " * " << B_str << "_" << npart << "_" << m
                << "*exp(-(exp(" << E_str << "_" << npart << ")";
        for(int mm=1; mm<=m; ++mm)
        {
            function_st << "+exp(" << dE_str << "_" << npart << "_" << mm << ")";
        }
        function_st << ")*" << T_str << "/2) * cosh((exp(" << E_str << "_" << npart << ")";
        for(int mm=1; mm<=m; ++mm)
        {
            function_st << "+exp(" << dE_str << "_" << npart << "_" << mm << ")";
        }
        function_st << ")*(" << variable << "-" << T_str << "/2))";
    }
    return function_st.str();
}


string multi_part_exp_expE_model::get_function_name(int function)
{
    return get_function_name(function, t_str);
}


string multi_part_exp_expE_model::get_parameter_name(int parameter)
{
    vector<string> names(nparam, "");
    for(int n=0; n<npart; ++n)
    {
        stringstream param_st;
        param_st << E_str << "_" << n+1;
        names[n]=param_st.str();
    }
    for(int n=1; n<nexp; ++n)
    {
        stringstream param_st;
        param_st << dE_str << "_" << n;
        names[npart+n-1]=param_st.str();
    }
    for(int n=1; n<=nmax; ++n)
    {
        stringstream param_st;
        param_st << A_str << "_" << npart << "_" << n;
        names[npart+nexp-1+n]=param_st.str();
    }
    for(int n=1; n<nexp; ++n)
    {
        stringstream param_st;
        param_st << B_str << "_" << npart << "_" << n;
        names[npart+nexp+nmax+n-1]=param_st.str();
    }
    {
        stringstream param_st;
        param_st << Z_str << "_" << npart;
        names[npart+nexp-1]= param_st.str();
    }

    return names[parameter];
}


string multi_part_exp_expE_model::get_variable_name(int variable)
{
    if(variable!=0)
    {
        return "";
    }
    return t_str;
}

string multi_part_exp_expE_model::get_constant_name(int constant)
{
    if(constant==0)
    {
        return T_str;
    }
    return "";
}



