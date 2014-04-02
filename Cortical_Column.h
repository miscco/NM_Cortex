/************************************************************************************************/
/*								Header file of a cortical module								*/
/************************************************************************************************/
#pragma once
#include <cmath>
#include <vector>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include "macros.h"
using std::vector;

/****************************************************************************************************/
/*										Typedefs for RNG											*/
/****************************************************************************************************/
typedef boost::mt11213b                    	ENG;    // Mersenne Twister
typedef boost::normal_distribution<double>	DIST;   // Normal Distribution
typedef boost::variate_generator<ENG,DIST> 	GEN;    // Variate generator
/****************************************************************************************************/
/*										 		end			 										*/
/****************************************************************************************************/


/****************************************************************************************************/
/*								Implementation of the cortical module 								*/
/****************************************************************************************************/
class Cortical_Column {
public:
	// Constructors
	Cortical_Column(void)
	: Ve		(_INIT(E_L_e)),	Vi 	   		(_INIT(E_L_i)),	Na	 		(_INIT(Na_eq)),
	  Phi_ee	(_INIT(0.0)), 	Phi_ei 		(_INIT(0.0)), 	Phi_ie 		(_INIT(0.0)), 	Phi_ii	(_INIT(0.0)),
	  x_ee 		(_INIT(0.0)), 	x_ei   		(_INIT(0.0)),	x_ie   		(_INIT(0.0)), 	x_ii	(_INIT(0.0))
	{set_RNG();}

	Cortical_Column(double* Par)
	: Ve		(_INIT(E_L_e)),	Vi 	   		(_INIT(E_L_i)),	Na	 		(_INIT(Na_eq)),
	  Phi_ee	(_INIT(0.0)), 	Phi_ei 		(_INIT(0.0)), 	Phi_ie 		(_INIT(0.0)), 	Phi_ii		(_INIT(0.0)),
	  x_ee 		(_INIT(0.0)), 	x_ei   		(_INIT(0.0)),	x_ie   		(_INIT(0.0)), 	x_ii		(_INIT(0.0)),
	  tau_e		(Par[0]), 		theta_e		(Par[1]),		sigma_e 	(Par[2]),		alpha_Na 	(Par[3]),
	  tau_Na	(Par[4]),	  	g_KNa		(Par[5]),		dphi_c		(Par[6])
	{set_RNG();}

	// Initialize the RNGs
	void 	set_RNG		(void);

	// change the strength of input
	void	set_input	(double I) {input = I;}

	// Firing rates
	double 	get_Qe		(int) const;
	double 	get_Qi		(int) const;

	// Currents
	double 	I_ee		(int) const;
	double 	I_ei		(int) const;
	double 	I_ie		(int) const;
	double 	I_ii		(int) const;
	double 	I_L_e		(int) const;
	double 	I_L_i		(int) const;
	double 	I_KNa		(int) const;

	// Potassium pump
	double 	Na_pump		(int) const;

	// Noise function
	double 	noise_xRK 	(int, int) const;

	// ODE functions
	void 	set_RK		(int);
	void 	add_RK	 	(void);

	// Data storage
	friend void get_data (int, Cortical_Column&, _REPEAT(double*, 1));

private:
	// Population variables
	vector<double> 	Ve,			// excitatory membrane voltage
					Vi,			// inhibitory membrane voltage
					Na,			// Na concentration
					Phi_ee,		// PostSP from excitatory to excitatory population
					Phi_ei,		// PostSP from excitatory to inhibitory population
					Phi_ie,		// PostSP from inhibitory to excitatory population
					Phi_ii,		// PostSP from inhibitory to inhibitory population
					x_ee,		// derivative of Phi_ee
					x_ei,		// derivative of Phi_ei
					x_ie,		// derivative of Phi_ie
					x_ii;		// derivative of Phi_ii

	// Random number generators
	vector<GEN>		MTRands;

	// Container for noise
	vector<double>	Rand_vars;

	// Declaration and Initialization of parameters
	// Membrane time in ms
	const double 	tau_e 		= 30;
	const double 	tau_i 		= 30;

	// Maximum firing rate in ms^-1
	const double 	Qe_max		= 30.E-3;
	const double 	Qi_max		= 60.E-3;

	// Sigmoid threshold in mV
	const double 	theta_e		= -58.5;
	const double 	theta_i		= -58.5;

	// Sigmoid gain in mV
	const double 	sigma_e		= 4;
	const double 	sigma_i		= 6;

	// Scaling parameter for sigmoidal mapping (dimensionless)
	const double 	C1          = (3.14159265/sqrt(3));

	// parameters of the firing adaption
	const double 	alpha_Na	= 2;			// Sodium influx per spike			in mM ms
	const double 	tau_Na		= 1;			// Sodium time constant 			in ms

	const double 	R_pump   	= 0.09;        	// Na-K pump  constant              in mM/ms
	const double 	Na_eq    	= 9.5;         	// Na-eq concentration              in mM

	// PSP rise time in ms^-1
	const double 	gamma_e		= 70E-3;
	const double 	gamma_i		= 58.6E-3;

	// Conductivities in mS/cm^-2
	// Leak current
	const double 	g_L    		= 1;

	// KNa current
	const double	g_KNa		= 1.33;

	// Connectivities (dimensionless)
	const double 	N_ee		= 120;
	const double 	N_ei		= 72;
	const double 	N_ie		= 90;
	const double 	N_ii		= 90;

	// Reversal potentials in mV
	// synaptic
	const double 	E_AMPA  	= 0;
	const double 	E_GABA  	= -70;

	// Leak
	const double 	E_L_e 		= -66;
	const double 	E_L_i 		= -64;

	// Potassium
	const double 	E_K    		= -100;

	// Noise parameters in ms^-1
	const double 	mphi_c		= 0E-3;
	const double	dphi_c		= 30E-3;;
	double			input		= 0.0;

	friend class Stim;
};
/****************************************************************************************************/
/*										 		end			 										*/
/****************************************************************************************************/

