%%sh
cat > pi-mpi.c << EOF
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <sys/time.h>
#define N 4

MPI_Status status;
double A[N][N];
double B[N], C[N];

int main(int argc, char **argv) {
int processCount, processId, slaveTaskCount, source, dest, rows, offset;

struct timeval;
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &processId);
MPI_Comm_size(MPI_COMM_WORLD, &processCount);

slaveTaskCount = processCount - 1;
if (processId == 0) {

srand ( time(NULL) );
for (int i = 0; i<N; i++) {
  for (int j = 0; j<N; j++) {
    A[i][j]= rand()%10;
  }
  B[i]= rand()%10;
}
printf("\nMacierz \n\n");
for (int i = 0; i<N; i++) {
  for (int j = 0; j<N; j++) {
    printf("%.0f\t", A[i][j]);
  }
  printf("\n");
}
printf("\nWektor \n\n");
for (int i = 0; i<N; i++) {
  printf("%.0f\t", B[i]);
  printf("\n");
}

rows = N/slaveTaskCount;

offset = 0;

for (dest=1; dest <= slaveTaskCount; dest++)
{
  MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
  MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
  MPI_Send(&A[offset][0], rows*N, MPI_DOUBLE,dest,1, MPI_COMM_WORLD);
  MPI_Send(&B, N, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
  offset = offset + rows;
}

for (int i = 1; i <= slaveTaskCount; i++)
{
  source = i;
  MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
  MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
  MPI_Recv(&C[offset], rows*N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
}

printf("\nWynikowy wektor\n\n");
for (int i = 0; i<N; i++) {
  printf("%.0f\t", C[i]);
  printf ("\n");
}
printf ("\n");

}
if (processId > 0) {
  source = 0;
    MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
  
  MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
  MPI_Recv(&A, rows*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
  MPI_Recv(&B, N*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
  for (int i = 0; i<rows; i++) {

    for (int j = 0; j<N; j++)
      C[i] = C[i] + A[i][j] * B[j];
  }

  MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
  MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
  MPI_Send(&C, rows*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
}
MPI_Finalize();
}
