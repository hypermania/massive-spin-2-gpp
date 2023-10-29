#ifndef MINIMAL_SCALARH
#define MINIMAL_SCALARH
// functions related to minimally coupled theory for massive spin-2

#include <cstdlib>
#include <iostream>
#include <vector>
#include "math.h"
#include <boost/math/interpolators/quintic_hermite.hpp>
#include <boost/math/interpolators/cubic_hermite.hpp>
#include <boost/numeric/odeint.hpp>
#include <boost/math/special_functions/hankel.hpp>

#include "numvec.h"
//#include "nummat.h"
#include "context.hpp"


class MinimalScalarEqn{
  const double k;
  const double m;
  const boost::math::interpolators::quintic_hermite<std::vector<double>> phi;
  const boost::math::interpolators::cubic_hermite<std::vector<double>> H;
  const boost::math::interpolators::cubic_hermite<std::vector<double>> logaOvera0;
  const double logaEnd;
  
public:
  MinimalScalarEqn(double k_, double m_, const ContextForModeEqn &context);
  void operator()(const NumVec &x, NumVec &dxdt, const double t) const;
};

//class JobSolveMinimalScalarEqn{
//}

NumVec compute_initial_condition_for_minimal_scalar_eqn(double k, double m, const ContextForModeEqn &context);
NumVec solve_minimal_scalar_eqn(double k, double m, const ContextForModeEqn &context);

/*
double computeKphi(double a, double H, double k, double m, double phiDot, double phi);
double computeKB(double a, double H, double k, double m, double phiDot, double phi);
double computeMphi(double a, double H, double k, double m, double phiDot, double phi);
double computeMB(double a, double H, double k, double m, double phiDot, double phi);
double computeL2(double a, double H, double k, double m, double phiDot, double phi);
double computeL1(double a, double H, double k, double m, double phiDot, double phi);
double computeL0(double a, double H, double k, double m, double phiDot, double phi);

double computeDKphi(double a, double H, double k, double m, double phiDot, double phi);
double computeDKB(double a, double H, double k, double m, double phiDot, double phi);
double computeDL2(double a, double H, double k, double m, double phiDot, double phi);
double computeDL1(double a, double H, double k, double m, double phiDot, double phi);
*/

#endif
