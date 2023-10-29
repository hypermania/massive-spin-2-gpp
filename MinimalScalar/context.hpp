#ifndef CONTEXTH
#define CONTEXTH
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "io.hpp"

#include <boost/math/interpolators/quintic_hermite.hpp>
#include <boost/math/interpolators/cubic_hermite.hpp>
#include <boost/math/tools/roots.hpp>

class ContextForModeEqn{

public:
  ContextForModeEqn(std::string context_dir);
  
  double compute_tInit(double k, double m) const;

  double As;
  double mphi;
  double tScale;
  double T;
  double tEnd;
  double phiT;
  double DphiT;
  double HT;
  double aT;
  double He;
  double Hinf;
  double phii;
  double mu;
  double logaEnd;
  double tInitMax;

  std::vector<double> info;
  boost::math::interpolators::quintic_hermite<std::vector<double>> phi;
  boost::math::interpolators::cubic_hermite<std::vector<double>> H;
  boost::math::interpolators::cubic_hermite<std::vector<double>> logaOvera0;
};


class InverseLogaFunctor{
  double loga_to_invert;
  boost::math::interpolators::cubic_hermite<std::vector<double>> logaOvera0;
  double logaEnd;

public:
  InverseLogaFunctor(double loga_to_invert_,
		     boost::math::interpolators::cubic_hermite<std::vector<double>> const &logaOvera0_,
		     double logaEnd_) :
    loga_to_invert(loga_to_invert_), logaOvera0(logaOvera0_), logaEnd(logaEnd_) {}
  
  double operator()(double const& t)
  {
    return logaOvera0(t) - logaEnd - loga_to_invert;
  }
};


#endif
