#ifndef MPI_MODULE_H_
#define MPI_MODULE_H_

#define ROOT_PROC 0
int startup_MPI(int *my_rank, int *size);
int finish_MPI();
int create_block_length_for_each_process(int mat_dim, int my_rank, int size);

#endif // !MPI_MODULE
