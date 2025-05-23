// EurpeanOption.cpp
//
//	Author: Daniel Duffy
//
// (C) Datasim Component Technology BV 2003
//

#ifndef EuropeanOption_cpp
#define EuropeanOption_cpp


#include "EuropeanOption.hpp"
#include <math.h>
#include <iostream>

//////////// Gaussian functions /////////////////////////////////

double EuropeanOption::n(double x) const
{ 

	double A = 1.0/sqrt(2.0 * 3.1415);
	return A * exp(-x*x*0.5);

}

double EuropeanOption::N(double x) const
{ // The approximation to the cumulative normal distribution


	double a1 = 0.4361836;
	double a2 = -0.1201676;
	double a3 = 0.9372980;

	double k = 1.0/(1.0 + (0.33267 * x));
	
	if (x >= 0.0)
	{
		return 1.0 - n(x)* (a1*k + (a2*k*k) + (a3*k*k*k));
	}
	else
	{
		return 1.0 - N(-x);
	}

}
double EuropeanOption::GetOptType() const{

}


// Kernel Functions (Haug)
double EuropeanOption::CallPrice() const
{

	double tmp = sig * sqrt(T);

	double d1 = ( log(U/K) + (b+ (sig*sig)*0.5 ) * T )/ tmp;
	double d2 = d1 - tmp;


	return (U * exp((b-r)*T) * N(d1)) - (K * exp(-r * T)* N(d2));

}

double EuropeanOption::PutPrice() const
{

	double tmp = sig * sqrt(T);

	double d1 = ( log(U/K) + (b+ (sig*sig)*0.5 ) * T )/ tmp;
	double d2 = d1 - tmp;

	return (K * exp(-r * T)* N(-d2)) - (U * exp((b-r)*T) * N(-d1));

}

double EuropeanOption::CallDelta() const
{
	double tmp = sig * sqrt(T);

	double d1 = ( log(U/K) + (b+ (sig*sig)*0.5 ) * T )/ tmp;


	return exp((b-r)*T) * N(d1);
}

double EuropeanOption::PutDelta() const
{
	double tmp = sig * sqrt(T);

	double d1 = ( log(U/K) + (b+ (sig*sig)*0.5 ) * T )/ tmp;

	return exp((b-r)*T) * (N(d1) - 1.0);
}



/////////////////////////////////////////////////////////////////////////////////////

// void EuropeanOption::init()
// {	// Initialise all default values
// 	// Default values
// 	r = 0.08;
// 	sig= 0.30;
// 	K = 65.0;
// 	T = 0.25;
// 	U = 60.0;		// U == stock in this case
// 	b = r;			// Black and Scholes stock option model (1973)
// 	optType = "C";		// European Call Option (this is the default type)
// }

// void EuropeanOption::copy(const EuropeanOption& o2)
// {
// 	r	= o2.r;
// 	sig = o2.sig;	
// 	K	= o2.K;
// 	T	= o2.T;
// 	U	= o2.U;
// 	b	= o2.b;
// 	optType = o2.optType;
// }

EuropeanOption::EuropeanOption() : r(0.08), sig(0.30), K(65.0), T(0.25)
{ // Default call option

	U = 60.0;		// U == stock in this case
	b = r;			// Black and Scholes stock option model (1973)

	optType = new char[2]; 
	optType[0] = 'C';
}

EuropeanOption::EuropeanOption(const EuropeanOption& o2): 
	r(o2.r), sig(o2.sig), K(o2.K), T(o2.T), U(o2.U), b(o2.b)
{ // Copy constructor
	optType = new char[2]; 
	optType[0] = o2.optType[0];
}

EuropeanOption::EuropeanOption(double r_, double sig_, double K_, double T_, double U_, double b_, char *const optType_):
	r(r_), sig(sig_), K(K_), T(T_), U(U_), b(b_)
{
	optType = new char[2]; 
	optType[0] = optType_[0];
}	

EuropeanOption::EuropeanOption (char *const optType_): r(0.08), sig(0.30), K(65.0), T(0.25), U(60.0), b(r)
{	// Create option type

	optType = new char[2]; 
	optType[0] = optType_[0];

	if (optType[0] == 'c')
		optType[0] = 'C';
	if (optType[0] == 'p')
		optType[0] = 'P';

}



EuropeanOption::~EuropeanOption()
{
	if (optType != NULL) delete [] optType;
}


EuropeanOption& EuropeanOption::operator = (const EuropeanOption& o2)
{

	if (this == &o2) return *this;

	r	= o2.r;
	sig = o2.sig;	
	K	= o2.K;
	T	= o2.T;
	U	= o2.U;
	b	= o2.b;
	optType = new char[2]; 
	optType[0] = o2.optType[0];

	return *this;
}

// Functions that calculate option price and sensitivities
double EuropeanOption::Price() const 
{

	if (optType[0] == 'C')
	{
		return CallPrice();
	}
	else
		return PutPrice();

}

double EuropeanOption::Delta() const 
{
	if (optType[0] == 'C')
		return CallDelta();
	else
		return PutDelta();

}



// Modifier functions
void EuropeanOption::toggle()
{ // Change option type (C/P, P/C)

	if (optType[0] == 'C')
		optType[0] = 'P';
	else
		optType[0] = 'C';
}

#endif

