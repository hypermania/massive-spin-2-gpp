#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <complex>
#include <thread>
#include <future>
#include <chrono>
#include <filesystem>
#include <random>
#include "math.h"
#include "time.h"
#include "stdio.h"
#include <boost/math/interpolators/quintic_hermite.hpp>
#include <boost/math/interpolators/cubic_hermite.hpp>
#include <boost/math/tools/roots.hpp>
#include <boost/numeric/odeint.hpp>
#include <boost/lockfree/queue.hpp>


#include "ctpl.h"

#include "numvec.h"
#include "utility.h"
#include "io.hpp"
#include "context.hpp"
#include "minimal_scalar.hpp"


// All threads use the same Context
// Use less memory but slower
int run_main_program(int argc, char **argv){
  
  if(argc == 1){
    std::cout << "No input directory." << std::endl;
    return 0;
  }

  std::string input_str(argv[1]);
  
  if(argc == 2){
    auto path_to_dir = std::filesystem::path(input_str);
    if(!std::filesystem::is_directory(path_to_dir)){
      std::cout << "Not a path to directory." << std::endl;
      return 0;
    }
    if(!std::filesystem::is_regular_file(path_to_dir / "mList.csv")){
      std::cout << "No mList.csv." << std::endl;
      return 0;
    }
    if(!std::filesystem::is_regular_file(path_to_dir / "kListList.csv")){
      std::cout << "No kListList.csv." << std::endl;
      return 0;
    }
  }
  
  auto kmList = load_kmList_from_dir(input_str);
  int n = kmList.size();
  auto context = ContextForModeEqn("bgSol_data_2");
  
  ctpl::thread_pool p(6);
  std::vector<std::future<void>> results(n);
  std::vector<NumVec> solutions(n);

  auto start = std::chrono::system_clock::now();
  
  for(int i = 0; i < n; i++){
    results[i] = p.push([&solutions, i, &kmList, &context](int)
			{ solutions[i] = solve_minimal_scalar_eqn(kmList[i].first, kmList[i].second, context); } );
  }
  for(int i = 0; i < n; i++){
    results[i].get();
  }
  
  auto end = std::chrono::system_clock::now();  
  std::chrono::duration<double> diff = end - start;
  std::cout << std::fixed << std::setprecision(9) << std::left;
  std::cout << std::setw(9) << "duration = " << diff.count() << std::endl;

  write_solutions_to_mathematica_readable_csv(solutions, input_str + "/SSectorFinal.csv");
    
  return 0;
  
}

// Each thread loads its own Context
// Uses more memory, achieves speedup proportional to number of cores
int run_main_program_optimized(int argc, char **argv){
  
  if(argc == 1){
    std::cout << "No input directory." << std::endl;
    return 0;
  }

  std::string input_str(argv[1]);
  
  if(argc == 2){
    auto path_to_dir = std::filesystem::path(input_str);
    if(!std::filesystem::is_directory(path_to_dir)){
      std::cout << "Not a path to directory." << std::endl;
      return 0;
    }
    if(!std::filesystem::is_regular_file(path_to_dir / "mList.csv")){
      std::cout << "No mList.csv." << std::endl;
      return 0;
    }
    if(!std::filesystem::is_regular_file(path_to_dir / "kListList.csv")){
      std::cout << "No kListList.csv." << std::endl;
      return 0;
    }
  }
  
  auto kmList = load_kmList_from_dir(input_str);
  int n = kmList.size();
  std::vector<NumVec> solutions(n);

  boost::lockfree::queue<int> q(10);
  for(int i = 0; i < n; i++){
    q.push(i);
  }
  
  int num_threads = std::thread::hardware_concurrency() / 2;
  //int num_threads = 6;
  auto threads = std::vector<std::thread>(0);
  for(int i = 0; i < num_threads; i++){
    threads.push_back(std::thread([&q, &kmList, &solutions](void){
				    auto context = ContextForModeEqn("bgSol_data");
				    int idx;
				    while(q.pop(idx)){
				      solutions[idx] = solve_minimal_scalar_eqn(kmList[idx].first, kmList[idx].second, context);
				    }
				  }));
  }

  for(int i = 0; i < num_threads; i++){
    if(threads[i].joinable()){
      threads[i].join();
    }
  }
  
  write_solutions_to_mathematica_readable_csv(solutions, input_str + "/SSectorFinal.csv");
    
  return 0;
  
}


int main(int argc, char **argv){
  
  run_main_program_optimized(argc, argv);
  //run_main_program(argc, argv);

  /*
  int n = 7;
  std::vector<std::pair<double, double>> kmList(n);
  std::vector<NumVec> solutions(n);
  for(int i = 0; i < n; i++){
    kmList[i] = std::pair(100 * 5.427e-8, 10 * 1.635e-7);
  }

  boost::lockfree::queue<int> q(10);
  for(int i = 0; i < n; i++){
    q.push(i);
  }
  
  int num_threads = 6;
  auto threads = std::vector<std::thread>(0);
  for(int i = 0; i < num_threads; i++){
    threads.push_back(std::thread([&q, &kmList, &solutions](void){
				    auto context = ContextForModeEqn("bgSol_data");
				    int idx;
				    while(q.pop(idx)){
				      solutions[idx] = solve_minimal_scalar_eqn(kmList[idx].first, kmList[idx].second, context);
				    }
				  }));
  }

  for(int i = 0; i < num_threads; i++){
    if(threads[i].joinable()){
      threads[i].join();
    }
  }

  std::cout << solutions[0] << std::endl;
  */



  /*
  auto context = ContextForModeEqn("bgSol_data");
  double k = 1.0 * context.He;
  double m = 1.5 * sqrt(2) * context.Hinf;
  //solve_minimal_scalar_eqn(k, m, context);

  auto spline = boost::math::interpolators::cubic_hermite(context.H);
  auto ptr = &spline;

  std::cout << (ptr == (&context.H)) << std::endl;
  */

  return 0;
}





