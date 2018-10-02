// #include "monte_carlo_search_tree.hpp"
// #include "node.hpp"

#include <iostream>
#include <memory>
#include <mpi.h>
#include <random>
#include <ctime>

int main(int argc, char** argv) {

// Shared ptr test
/*
std::vector<double> my_vec(5, 0.0);
auto it = my_vec.begin();
std::shared_ptr<double> select(std::make_shared<double>(*it));
double* my_pointer(&(*select));
std::cout << *my_pointer << std::endl;
*select = 6.0;
std::cout << *my_pointer << std::endl;
*/

// Time test
MPI_Init(nullptr, nullptr);

int rank;
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

time_t rawtime;
struct tm * ptm;
time ( &rawtime );
ptm = gmtime ( &rawtime );

int seed = ptm->tm_sec + 10*(ptm->tm_min) + 100*(ptm->tm_hour) + 1000*(rank+1);
std::cout << seed << std::endl;

MPI_Finalize();

return 0;

}
