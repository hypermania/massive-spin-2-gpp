#include "context.hpp"

// load info common to all k, m params
ContextForModeEqn::ContextForModeEqn(std::string context_dir) :
  info(load_vector_from_file(context_dir + "/info.dat")),
  phi(load_quintic_interpolant_from_files(context_dir + "/phi_t.dat", context_dir + "/phi_0.dat", context_dir + "/phi_1.dat", context_dir + "/phi_2.dat")),
  H(load_cubic_interpolant_from_files(context_dir + "/phi_t.dat", context_dir + "/H_0.dat", context_dir + "/H_1.dat")),
  logaOvera0(load_cubic_interpolant_from_files(context_dir + "/logaOvera0_t.dat", context_dir + "/logaOvera0_0.dat", context_dir + "/logaOvera0_1.dat"))
{

  As = info[0];
  mphi = info[1];
  tScale = info[2];
  T = info[3];
  tEnd = info[4];
  phiT = info[5];
  DphiT = info[6];
  HT = info[7];
  aT = info[8];
  He = info[9];
  Hinf = info[10];
  phii = info[11];
  mu = info[12];
  logaEnd = info[13];
  tInitMax = info[14];

  std::cout << "Loaded context from directory ./" + context_dir + "." << std::endl;
}    


double ContextForModeEqn::compute_tInit(double k, double m) const {
  std::uintmax_t it = 20;
  int digits = std::numeric_limits<double>::digits;
  boost::math::tools::eps_tolerance<double> tol(digits - 3);
  
  std::pair<double, double> r = boost::math::tools::bracket_and_solve_root(InverseLogaFunctor(log(k/m) - 5.0, logaOvera0, logaEnd), tEnd, 1.5, true, tol, it);

  double solution = (r.first + r.second) / 2;
  
  return std::min(solution, tInitMax);
}
