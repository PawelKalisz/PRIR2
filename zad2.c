#include <mpi.h>
#include <stdio.h>

double fun(double x) {
  return x*x;
}

int main(int argc, char *argv[]) {

  int np,lp;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &np);
  MPI_Comm_size(MPI_COMM_WORLD, &lp);
  double xp = 0.0;
  double xk = 1.0;
  int n = 111;
  double clk = 0.0;
  double h = (xk-xp)/n;
  double xi;
  for (long nn = np; nn<n; nn+=lp){
    xi = xp + nn * h;
    clk += fun(xi);
  }
  double wynik;
  MPI_Reduce(&clk, &wynik, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if (np == 0){
    wynik += fun(xp)/2;
    wynik += fun(xk)/2;
    wynik *= h;
    printf("Obliczona calka: %f", wynik);
  }
  MPI_Finalize();
  return 0;
}
