/****************************************************************************************************/
/* 		Implementation of the simulation as MATLAB routine (mex compiler)							*/
/* 		mex command is given by:																	*/
/* 		mex CXXFLAGS="\$CXXFLAGS -std=gnu++0x -fpermissive" Cortex.cpp Cortical_Column.cpp			*/
/*		The Simulation requires the following boost libraries:	Preprocessor						*/
/*																Random								*/
/****************************************************************************************************/
#include <ctime>
#include "mex.h"
#include "matrix.h"
#include "Cortical_Column.h"
#include "Stimulation.h"
#include "saves.h"
#include "ODE.h"

/****************************************************************************************************/
/*										Fixed simulation settings									*/
/****************************************************************************************************/
extern const int onset	= 5;								// time until data is stored in  s
extern const int res 	= 1E4;								// number of iteration steps per s
extern const int red 	= res/100;							// number of iterations that is saved
extern const double dt 	= 1E3/res;							// duration of a timestep in ms
extern const double h	= sqrt(dt);							// squareroot of dt for SRK iteration
/****************************************************************************************************/
/*										 		end			 										*/
/****************************************************************************************************/


/****************************************************************************************************/
/*										Simulation routine	 										*/
/*										lhs defines outputs											*/
/*										rhs defines inputs											*/
/****************************************************************************************************/
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	// Set the seed.
	srand(time(NULL));

	// Fetch inputs
	const int T				= (int) (mxGetScalar(prhs[0]));	// duration of simulation in s
	const int Time 			= (T+onset)*res;				// total number of iteration steps
	double* Param_Cortex	= mxGetPr (prhs[1]);			// parameters of cortical module
	double* var_stim	 	= mxGetPr (prhs[2]);			// parameters of stimulation protocol

	// Initializing the populations;
	Cortical_Column Cortex(Param_Cortex);

	// Initialize the stimulation protocol
	Stim	Stimulation(Cortex, var_stim);

	// Data container in MATLAB format
	mxArray* Ve		= SetMexArray(1, T*res/red);

	// Pointer to the actual data block
	double* Pr_Ve	= mxGetPr(Ve);

	// simulation
	int count = 0;
	for (int t=0; t<(T+onset)*res; ++t) {
		ODE (Cortex);
		Stimulation.check_stim(t);
		if(t>=onset*res && t%red==0){
			get_data(count, Cortex, Pr_Ve);
			++count;
		}
	}

	plhs[0]  = Ve;
	return;
}
/****************************************************************************************************/
/*												end													*/
/****************************************************************************************************/
