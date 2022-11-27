#include <mpi.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
  int rank, size;
  MPI_Init( &argc, &argv );
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );

  long max = 100000;
  double suma = 0.0;
  for (long n = rank; n<max; n+=size){
    double tmp = 1.0 / (2.0 * (double)n + 1);
    if (n % 2 == 0){suma += tmp;
      } else { suma -= tmp;
    }
  }
  double total_sum;
  MPI_Reduce(&suma, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == 0){
    printf("Obliczone PI: %f", 4.0*total_sum);
  }
  MPI_Finalize();
  return 0;
}
