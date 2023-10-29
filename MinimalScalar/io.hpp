#ifndef IOH
#define IOH
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "numvec.h"

#include <boost/math/interpolators/quintic_hermite.hpp>
#include <boost/math/interpolators/cubic_hermite.hpp>


std::vector<double> load_vector_from_file(std::string filename);

boost::math::interpolators::quintic_hermite<std::vector<double>> load_quintic_interpolant_from_files(std::string file_x, std::string file_y, std::string file_dydx, std::string file_d2yd2x);
boost::math::interpolators::cubic_hermite<std::vector<double>> load_cubic_interpolant_from_files(std::string file_x, std::string file_y, std::string file_dydx);

std::vector<double> load_mList_from_file(std::string filename);
std::vector<std::vector<double>> load_kListList_from_file(std::string filename);
std::vector<std::pair<double, double>> load_kmList_from_dir(std::string dir);

void write_solutions_to_mathematica_readable_csv(const std::vector<NumVec> &solutions, const std::string &filename);

#endif
