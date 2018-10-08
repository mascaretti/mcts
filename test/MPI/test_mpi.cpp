#include <iostream>
#include <memory>
#include <random>
#include <mpi.h>
#include <ctime>

int main(int argc, char** argv) {

int is_init = 0;
MPI_Initialized( &is_init );
std::cout << is_init << std::endl;

// Time test
MPI_Init(nullptr, nullptr);

MPI_Initialized( &is_init );
std::cout << is_init << std::endl;

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
