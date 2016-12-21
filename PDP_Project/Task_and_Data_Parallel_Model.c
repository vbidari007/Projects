/*
This program implement 2D convolution model using a Task and Data Parallel Model. You also need to
show the use of communicators in MPI. Divide the P processors into four groups: P1, P2, P3, and P4. 
Run Task 1 on P1 processors, Task 2 on P2 processors, Task 3 on P3 processors, and Task 4 on P4 processors.

Default inputs are sample/1_im1 and sample/1_im2

*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <mpi.h>

/* Given with the fft function in the assignment */
typedef struct {float r; float i;} complex;
static complex ctemp;
#define C_SWAP(x,y) {ctemp=(x);(x)=(y);(y)=ctemp;}


/* Size of matrix (NxN) */
const int N = 512;


int p, my_rank;
#define SRCE 0

int main (int argc, char **argv) {

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   /* READ input N* N matrices from input files */
   const char* inputfile1 = argc == 3 ? argv[1] : "sample/1_im1";
   const char* inputfile2 = argc == 3 ? argv[2] : "sample/1_im2";

   if ( my_rank==0) printf("\n Task and data parallel models\n");
   if ( my_rank==0) printf(" Number of processors = %d\n",p);
   if ( my_rank==0) printf(" using input images files %s, %s\n\n",inputfile1, inputfile2);

   /* Prototype functions */
   int read_matrix ( const char* file, complex matrix[N][N] );
   int write_matrix ( const char* file, complex matrix[N][N] );
   void c_fft1d(complex *r, int n, int isign);
   void print_matrix ( complex matrix[N][N], const char* matrixname, int rank );


   /* Variable init */
   int chunk = N / p; /* Diveide matrix to share among processor*/
   complex A[N][N], B[N][N], C[N][N];
   int i, j;
   complex temp;
   double time_begin, time_end, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14;
   MPI_Status status;


   /* Read files */
   read_matrix (inputfile1, A);
   read_matrix (inputfile2, B);

   print_matrix(A, "Matrix A initial", SRCE);
   print_matrix(B, "Matrix B initial", SRCE);

   /* Starting time */
   if ( my_rank == SRCE )
      time_begin = MPI_Wtime();


   
   if ( my_rank != SRCE )
   for (i=0;i<N;i++)
      for (j=0;j<N;j++) { A[i][j].r = 0; A[i][j].i = 0; B[i][j].r = 0; B[i][j].i = 0; C[i][j].r = 0; C[i][j].i = 0;}
   

/* Processors are devided into 4 groups. so each task runs with one process group*/

   int grp_size = p / 4;
   int my_grp_rank;
   int Proc1_array[grp_size], Proc2_array[grp_size], Proc3_array[grp_size], Proc4_array[grp_size];

   for(i=0; i<p; i++) {
      int processor_group = i / grp_size;
      switch(processor_group){
      case 0:
         Proc1_array[ i%grp_size ] = i;
         break;
      case 1:
         Proc2_array[ i%grp_size ] = i;
         break;
      case 2:
         Proc3_array[ i%grp_size ] = i;
         break;
      case 3:
         Proc4_array[ i%grp_size ] = i;
         break;
      }
   }
   
   MPI_Group world_group, P1, P2, P3, P4; 
   MPI_Comm P1_comm, P2_comm, P3_comm, P4_comm;

   /* Extract the original group handle */ 
   MPI_Comm_group(MPI_COMM_WORLD, &world_group); 

   /* creation of 4 groups */
   int my_group = my_rank / grp_size;

   if ( my_group == 0 )      { 
      
      MPI_Group_incl(world_group, p/4, Proc1_array, &P1);
      MPI_Comm_create( MPI_COMM_WORLD, P1, &P1_comm);
      MPI_Group_rank(P1, &my_grp_rank);
   } 
   else if ( my_group == 1 ) { 

      MPI_Group_incl(world_group, p/4, Proc2_array, &P2); 
      MPI_Comm_create( MPI_COMM_WORLD, P2, &P2_comm);
      MPI_Group_rank(P2, &my_grp_rank);
   } 
   else if ( my_group == 2 ) { 
      MPI_Group_incl(world_group, p/4, Proc3_array, &P3); 
      MPI_Comm_create( MPI_COMM_WORLD, P3, &P3_comm);
      MPI_Group_rank(P3, &my_grp_rank);
   } 
   else if ( my_group == 3 ) { 
      MPI_Group_incl(world_group, p/4, Proc4_array, &P4); 
      MPI_Comm_create( MPI_COMM_WORLD, P4, &P4_comm);
      MPI_Group_rank(P4, &my_grp_rank);
   } 

   
/* Send matrices A and B to all other processors*/

   chunk = N / grp_size;

   if ( my_rank == SRCE ){

      // Send A to the P1 processors
      for ( i=0; i<grp_size; i++ ) {
         if ( Proc1_array[i]==SRCE ) continue;
         MPI_Send( &A[chunk*i][0], chunk*N, MPI_COMPLEX, Proc1_array[i], 0, MPI_COMM_WORLD );
      }
      // Send B to the P2 processors
      for ( i=0; i<grp_size; i++ ) {
         if ( Proc2_array[i]==SRCE ) continue;
         MPI_Send( &B[chunk*i][0], chunk*N, MPI_COMPLEX, Proc2_array[i], 0, MPI_COMM_WORLD );
      }
   }
   else {

      // Receive A because this is group P1
      if ( my_group == 0 )
         MPI_Recv( &A[chunk*my_grp_rank][0], chunk*N, MPI_COMPLEX, SRCE, 0, MPI_COMM_WORLD, &status );
      // Receive B because this is group P2
      if ( my_group == 1 )
         MPI_Recv( &B[chunk*my_grp_rank][0], chunk*N, MPI_COMPLEX, SRCE, 0, MPI_COMM_WORLD, &status );
   }

   MPI_Barrier(MPI_COMM_WORLD); // for time meassuring
   if ( my_rank == SRCE ) t1 = MPI_Wtime();


   /*Fast fourier transform on rows of A in group P1 */

   if ( my_group == 0 )
      for ( i=chunk*my_grp_rank; i<chunk*(my_grp_rank+1); i++ )
         c_fft1d(A[i], N, -1);

   /*Fast fourier transform on rows of B*/ in group P2 */
   
   if ( my_group == 1 )
      for ( i=chunk*my_grp_rank; i<chunk*(my_grp_rank+1); i++ )
         c_fft1d(B[i], N, -1);

   MPI_Barrier(MPI_COMM_WORLD); // for time meassuring
   if ( my_rank == SRCE ) t2 = MPI_Wtime();

/* Receive matrix A*/

   if ( my_group == 0 ) {
      if ( my_grp_rank == 0 ) {
         for ( i=1; i<grp_size; i++ ) {
            MPI_Recv( &A[chunk*i][0], chunk*N, MPI_COMPLEX, i, 0, P1_comm, &status );
         }
         
      }
      else 
         MPI_Send( &A[chunk*my_grp_rank][0], chunk*N, MPI_COMPLEX, 0, 0, P1_comm );
   }


   /* Recieve matrix B */

   if ( my_group == 1 ) {
      if ( my_grp_rank == 0 ) {
         for ( i=1; i<grp_size; i++ ) {
            MPI_Recv( &B[chunk*i][0], chunk*N, MPI_COMPLEX, i, 0, P2_comm, &status );
         }
         
      }
      else 
         MPI_Send( &B[chunk*my_grp_rank][0], chunk*N, MPI_COMPLEX, 0, 0, P2_comm );
   }
   MPI_Barrier(MPI_COMM_WORLD); // for time meassuring
   if ( my_rank == SRCE ) t3 = MPI_Wtime();

   /* matrix A transpose */

   if ( my_group == 0 && my_grp_rank == 0 ) {
      for (i=0;i<N;i++) {
         for (j=i;j<N;j++) {
            temp = A[i][j];
            A[i][j] = A[j][i];
            A[j][i] = temp;
         }
      }
   }

   


   /* Transpose matrix B in P2 */
   
   if ( my_group == 1 && my_grp_rank == 0 ) {
      for (i=0;i<N;i++) {
         for (j=i;j<N;j++) {
            temp = B[i][j];
            B[i][j] = B[j][i];
            B[j][i] = temp;
         }
      }
   }
   MPI_Barrier(MPI_COMM_WORLD); // for time meassuring
   if ( my_rank == SRCE ) t4 = MPI_Wtime();


   /* Send A (transposed) to P1 */

   if ( my_group == 0 ) {
      if ( my_grp_rank == 0 ) {
         for ( i=1; i<grp_size; i++ ) {
            MPI_Send( &A[chunk*i][0], chunk*N, MPI_COMPLEX, i, 0, P1_comm );
         }
         
      }
      else 
         MPI_Recv( &A[chunk*my_grp_rank][0], chunk*N, MPI_COMPLEX, 0, 0, P1_comm, &status );
   }

   
   /* Scatter the transposed B in  P2 */

   if ( my_group == 1 ) {
      if ( my_grp_rank == 0 ) {
         for ( i=1; i<grp_size; i++ ) {
            MPI_Send( &B[chunk*i][0], chunk*N, MPI_COMPLEX, i, 0, P2_comm );
         }
         
      }
      else 
         MPI_Recv( &B[chunk*my_grp_rank][0], chunk*N, MPI_COMPLEX, 0, 0, P2_comm, &status );
   }
   MPI_Barrier(MPI_COMM_WORLD); // for time meassuring
   if ( my_rank == SRCE ) t5 = MPI_Wtime();

/* Appli fast fourier transform on matrix A*/

   if ( my_group == 0 )
      for ( i=chunk*my_grp_rank; i<chunk*(my_grp_rank+1); i++ )
         c_fft1d(A[i], N, -1);

   /* Apply FFT on matrix B*/

   if ( my_group == 1 )
      for ( i=chunk*my_grp_rank; i<chunk*(my_grp_rank+1); i++ )
         c_fft1d(B[i], N, -1);


   
   MPI_Barrier(MPI_COMM_WORLD); // for time meassuring
   if ( my_rank == SRCE ) t6 = MPI_Wtime();


   /* Gather A and B into the P3 processors */
   /* All the processors in P1 and P2 will send it to the first processor in P3 using the global communicator */

   if ( my_group == 0 )
      MPI_Send ( &A[chunk*my_grp_rank][0], chunk*N, MPI_COMPLEX, Proc3_array[my_grp_rank], 0, MPI_COMM_WORLD );
   else if ( my_group == 1 )
      MPI_Send ( &B[chunk*my_grp_rank][0], chunk*N, MPI_COMPLEX, Proc3_array[my_grp_rank], 0, MPI_COMM_WORLD );

   else if ( my_group == 2 ) {
      MPI_Recv( &A[chunk*my_grp_rank][0], chunk*N, MPI_COMPLEX, Proc1_array[my_grp_rank], 0, MPI_COMM_WORLD, &status );
      MPI_Recv( &B[chunk*my_grp_rank][0], chunk*N, MPI_COMPLEX, Proc2_array[my_grp_rank], 0, MPI_COMM_WORLD, &status );
   }
   
   MPI_Barrier(MPI_COMM_WORLD); // for time meassuring
   if ( my_rank == SRCE ) t7 = MPI_Wtime();




   /* Calculation of matrix c .i.e. point to point multiplication of A and B*/

   if ( my_group == 2 ) {
      for (i= chunk*my_grp_rank ;i< chunk*(my_grp_rank+1);i++) {
         for (j=0;j<N;j++) {
            C[i][j].r = A[i][j].r*B[i][j].r - A[i][j].i*B[i][j].i;
            C[i][j].i = A[i][j].r*B[i][j].i + A[i][j].i*B[i][j].r;
         }
      }
   }

   MPI_Barrier(MPI_COMM_WORLD); // for time meassuring
   if ( my_rank == SRCE ) t8 = MPI_Wtime();



   /* Send the result, which is among the processes of P3, to P4 */

   if ( my_group == 2 ) {
      MPI_Send ( &C[chunk*my_grp_rank][0], chunk*N, MPI_COMPLEX, Proc4_array[my_grp_rank], 0, MPI_COMM_WORLD );
   }
   else if ( my_group == 3 ) {
      MPI_Recv( &C[chunk*my_grp_rank][0], chunk*N, MPI_COMPLEX, Proc3_array[my_grp_rank], 0, MPI_COMM_WORLD, &status );
   }
   
   MPI_Barrier(MPI_COMM_WORLD); // for time meassuring
   if ( my_rank == SRCE ) t9 = MPI_Wtime();


   /* Inverse 1D FFT in all rows of C, made by P3. Each processor in P3 will do a part */

   if ( my_group == 3 )
      for ( i=chunk*my_grp_rank; i<chunk*(my_grp_rank+1); i++ )
         c_fft1d(C[i], N, 1);

   
   MPI_Barrier(MPI_COMM_WORLD); // for time meassuring
   if ( my_rank == SRCE ) t10 = MPI_Wtime();


   /* Gather the row FFTs from A into the first processor of P1 for sequential trasposition */

   if ( my_group == 3 ) {
      if ( my_grp_rank == 0 ) {
         for ( i=1; i<grp_size; i++ ) {
            MPI_Recv( &C[chunk*i][0], chunk*N, MPI_COMPLEX, i, 0, P4_comm, &status );
         }
         
      }
      else 
         MPI_Send( &C[chunk*my_grp_rank][0], chunk*N, MPI_COMPLEX, 0, 0, P4_comm );
   }
   MPI_Barrier(MPI_COMM_WORLD); // for time meassuring
   if ( my_rank == SRCE ) t11 = MPI_Wtime();





   /* Transpose C sequentially */
   if ( my_group == 3 && my_grp_rank == 0 ) {
      for (i=0;i<N;i++) {
         for (j=i;j<N;j++) {
            temp = C[i][j];
            C[i][j] = C[j][i];
            C[j][i] = temp;
         }
      }
      
   }
   MPI_Barrier(MPI_COMM_WORLD); // for time meassuring
   if ( my_rank == SRCE ) t12 = MPI_Wtime();
   
   

   /* Scatter the transposed C in the group P3 */

   if ( my_group == 3 ) {
      if ( my_grp_rank == 0 ) {
         for ( i=1; i<grp_size; i++ ) {
            MPI_Send( &C[chunk*i][0], chunk*N, MPI_COMPLEX, i, 0, P4_comm );
         }
         
      }
      else 
         MPI_Recv( &C[chunk*my_grp_rank][0], chunk*N, MPI_COMPLEX, 0, 0, P4_comm, &status );
   }
   MPI_Barrier(MPI_COMM_WORLD); // for time meassuring
   if ( my_rank == SRCE ) t13 = MPI_Wtime();


   /* Inverse 1D FFT in all rows of C, made by P3. Each processor in P3 will do a part */

   if ( my_group == 3 )
      for ( i=chunk*my_grp_rank; i<chunk*(my_grp_rank+1); i++ )
         c_fft1d(C[i], N, 1);

   
   MPI_Barrier(MPI_COMM_WORLD); // for time meassuring
   if ( my_rank == SRCE ) t14 = MPI_Wtime();

   
   /* Gather the fragments of C to the SRCE process, which will be in charge of computing time and writing file */

   if ( my_rank == SRCE ){

      for ( i=0; i<grp_size; i++ ) {
         if ( Proc4_array[i]==SRCE ) continue; /* SRCE process doesn't receive from itself */

         MPI_Recv( &C[chunk*i][0], chunk*N, MPI_COMPLEX, Proc4_array[i], 0, MPI_COMM_WORLD, &status );
      }
   }
   else if ( my_group == 3 )
      MPI_Send( &C[chunk*my_grp_rank][0], chunk*N, MPI_COMPLEX, SRCE, 0, MPI_COMM_WORLD );



   MPI_Barrier(MPI_COMM_WORLD); // for time meassuring

   /* Final time */
   if ( my_rank == SRCE )
      time_end = MPI_Wtime();

   print_matrix(C, "Matrix C final", SRCE);
   if ( my_rank==0) printf("C[0][0].r     = %e\n", C[0][0].r);
   if ( my_rank==0) printf("C[N-1][N-1].r = %e\n", C[N-1][N-1].r);

   /* Write output file */
   write_matrix("output_matrix_Task_and_Data_Parallel", C);

   if ( my_rank==0) printf("\n done\n");
   if ( my_rank==0) {

      double tcomputation = (t2-t1) + (t4-t3) + (t6-t5) + (t8-t7) + (t10-t9) + (t12-t11) + (t14-t13);
      double tcommunication = (t1-time_begin) + (t3-t2) + (t5-t4) + (t7-t6) + (t9-t8) + (t11-t10) + (t13-t12) + (time_end-t14);

      printf(" total time spent is %f ms\n", (time_end-time_begin) * 1000 );
      printf(" time computation is  %f ms\n", tcomputation * 1000 );
      printf(" time communication is  %f ms\n", tcommunication * 1000 );
   }

   MPI_Finalize();
}




/*-------------------------------------------------------------------------------------------------------*/

/* Reads the matrix from tha file and inserts the values in the real part */
/* The complex part is left to 0 */
int read_matrix ( const char* filename, complex matrix[N][N] ) {
   if ( my_rank == SRCE ) {
      int i, j;
      FILE *fp = fopen(filename,"r");

      if ( !fp ) {
         printf("Error. This file couldn't be read because it doesn't exist: %s\n", filename);
         exit(1);
      }

      for (i=0;i<N;i++)
         for (j=0;j<N;j++) {
            fscanf(fp,"%g",&matrix[i][j].r);
            matrix[i][j].i = 0;
         }
      fclose(fp);
   }
}

/* Write the real part of the result matrix */
int write_matrix ( const char* filename, complex matrix[N][N] ) {
   if ( my_rank == SRCE ) {
      int i, j;
      FILE *fp = fopen(filename,"w");

      for (i=0;i<N;i++) {
         for (j=0;j<N;j++)
            fprintf(fp,"   %e",matrix[i][j].r);
         fprintf(fp,"\n");
      };

      fclose(fp);
   }
}

/* Print the matrix if its size is no more than 40x40 */
/* Rank is the processor that should print this */
void print_matrix ( complex matrix[N][N], const char* matrixname, int rank ) {
   if ( my_rank == rank ) {
      if ( N<40 ) {
         int i, j;
         printf("%s by process #%d\n",matrixname, rank);
         for (i=0;i<N;i++){
            for (j=0;j<N;j++) {
              printf("(%.1f,%.1f) ", matrix[i][j].r,matrix[i][j].i);
           }printf("\n");
         }printf("\n");
      }
   }
}



/*
 ------------------------------------------------------------------------
 FFT1D            c_fft1d(r,i,-1)
 Inverse FFT1D    c_fft1d(r,i,+1)
 ------------------------------------------------------------------------
*/
/* ---------- FFT 1D
   This computes an in-place complex-to-complex FFT
   r is the real and imaginary arrays of n=2^m points.
   isign = -1 gives forward transform
   isign =  1 gives inverse transform
*/

void c_fft1d(complex *r, int      n, int      isign)
{
   int     m,i,i1,j,k,i2,l,l1,l2;
   float   c1,c2,z;
   complex t, u;

   if (isign == 0) return;

   /* Do the bit reversal */
   i2 = n >> 1;
   j = 0;
   for (i=0;i<n-1;i++) {
      if (i < j)
         C_SWAP(r[i], r[j]);
      k = i2;
      while (k <= j) {
         j -= k;
         k >>= 1;
      }
      j += k;
   }

   /* m = (int) log2((double)n); */
   for (i=n,m=0; i>1; m++,i/=2);

   /* Compute the FFT */
   c1 = -1.0;
   c2 =  0.0;
   l2 =  1;
   for (l=0;l<m;l++) {
      l1   = l2;
      l2 <<= 1;
      u.r = 1.0;
      u.i = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<n;i+=l2) {
            i1 = i + l1;

            /* t = u * r[i1] */
            t.r = u.r * r[i1].r - u.i * r[i1].i;
            t.i = u.r * r[i1].i + u.i * r[i1].r;

            /* r[i1] = r[i] - t */
            r[i1].r = r[i].r - t.r;
            r[i1].i = r[i].i - t.i;

            /* r[i] = r[i] + t */
            r[i].r += t.r;
            r[i].i += t.i;
         }
         z =  u.r * c1 - u.i * c2;

         u.i = u.r * c2 + u.i * c1;
         u.r = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      if (isign == -1) /* FWD FFT */
         c2 = -c2;
      c1 = sqrt((1.0 + c1) / 2.0);
   }

   /* Scaling for inverse transform */
   if (isign == 1) {       /* IFFT*/
      for (i=0;i<n;i++) {
         r[i].r /= n;
         r[i].i /= n;
      }
   }
}
