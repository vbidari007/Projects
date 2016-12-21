/*
This program implement 2D convolution using SPMD model but use MPI collective communication
functions wherever possible.

Default inputs are sample/1_im1 and sample/1_im2

Output_matrix will be created in working directory

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
   const char* inputfile1 = argc == 3 ? argv[1] : "sample/2_im1";
   const char* inputfile2 = argc == 3 ? argv[2] : "sample/2_im2";

   if ( my_rank==0) printf("\nMPI with collective communication such as sactter and gather\n");
   if ( my_rank==0) printf("  Total number of processors = %d\n",p);
   if ( my_rank==0) printf("  Using input image matrices %s, %s\n\n",inputfile1, inputfile2);

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
   double time_begin, time_end, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
   MPI_Status status;


   /* Read files */
   read_matrix (inputfile1, A);
   read_matrix (inputfile2, B);

   print_matrix(A, "Matrix A", SRCE);
   print_matrix(B, "Matrix B", SRCE);

   /* Starting time */
   if ( my_rank == SRCE )
      time_begin = MPI_Wtime();


   
   /* Scatter A and B to the other processes */
   MPI_Scatter( &A[0][0], chunk*N, MPI_COMPLEX, &A[chunk*my_rank][0], chunk*N, MPI_COMPLEX, SRCE, MPI_COMM_WORLD );
   MPI_Scatter( &B[0][0], chunk*N, MPI_COMPLEX, &B[chunk*my_rank][0], chunk*N, MPI_COMPLEX, SRCE, MPI_COMM_WORLD );

   if ( my_rank == SRCE ) t1 = MPI_Wtime();


   /* A and B matrices are applied by Fast fourier transform */
   for (i= chunk*my_rank ;i< chunk*(my_rank+1);i++) {
         c_fft1d(A[i], N, -1);
         c_fft1d(B[i], N, -1);
   }
   if ( my_rank == SRCE ) t2 = MPI_Wtime();



   /* Gather matrices from A and B from other procesors */
   MPI_Gather( &A[chunk*my_rank][0], chunk*N, MPI_COMPLEX, &A[0][0], chunk*N, MPI_COMPLEX, SRCE, MPI_COMM_WORLD );
   MPI_Gather( &B[chunk*my_rank][0], chunk*N, MPI_COMPLEX, &B[0][0], chunk*N, MPI_COMPLEX, SRCE, MPI_COMM_WORLD );

   if ( my_rank == SRCE ) t3 = MPI_Wtime();

   


   /* Matrices transpose*/
   if ( my_rank == SRCE ) {
      for (i=0;i<N;i++) {
         for (j=i;j<N;j++) {
            temp = A[i][j];
            A[i][j] = A[j][i];
            A[j][i] = temp;

            temp = B[i][j];
            B[i][j] = B[j][i];
            B[j][i] = temp;
         }
      }
      t4 = MPI_Wtime();
   }



   /* Scatter A and B to other processors  */
   MPI_Scatter( &A[0][0], chunk*N, MPI_COMPLEX, &A[chunk*my_rank][0], chunk*N, MPI_COMPLEX, SRCE, MPI_COMM_WORLD );
   MPI_Scatter( &B[0][0], chunk*N, MPI_COMPLEX, &B[chunk*my_rank][0], chunk*N, MPI_COMPLEX, SRCE, MPI_COMM_WORLD );

   if ( my_rank == SRCE ) t5 = MPI_Wtime();
  

   /* Fast fourier transform on A and B matrices*/
   for (i= chunk*my_rank ;i< chunk*(my_rank+1);i++) {
         c_fft1d(A[i], N, -1);
         c_fft1d(B[i], N, -1);
   }
   

   
   /* A*B point to point matrices multiplication*/
   for (i= chunk*my_rank ;i< chunk*(my_rank+1);i++) {
      for (j=0;j<N;j++) {
         C[i][j].r = A[i][j].r*B[i][j].r - A[i][j].i*B[i][j].i;
         C[i][j].i = A[i][j].r*B[i][j].i + A[i][j].i*B[i][j].r;
      }
   }
  

   /* Inverse fast fourier transform on matrix C  */
   for (i= chunk*my_rank ;i< chunk*(my_rank+1);i++) {
      c_fft1d(C[i], N, 1);
   }
   if ( my_rank == SRCE ) t6 = MPI_Wtime();
  


   /* Gather matrix C from all other processor */
   MPI_Gather( &C[chunk*my_rank][0], chunk*N, MPI_COMPLEX, &C[0][0], chunk*N, MPI_COMPLEX, SRCE, MPI_COMM_WORLD );

   if ( my_rank == SRCE ) t7 = MPI_Wtime();   

   /* C matrix is subjected to transpose*/
   if ( my_rank == SRCE ) {
      for (i=0;i<N;i++) {
         for (j=i;j<N;j++) {
            temp = C[i][j];
            C[i][j] = C[j][i];
            C[j][i] = temp;
         }
      }
      t8 = MPI_Wtime();
   }

   /* Scatter C to the other processes */
   MPI_Scatter( &C[0][0], chunk*N, MPI_COMPLEX, &C[chunk*my_rank][0], chunk*N, MPI_COMPLEX, SRCE, MPI_COMM_WORLD );

   if ( my_rank == SRCE ) t9 = MPI_Wtime();


   /* Inverse fast fourier transform on columns of C*/
   for (i= chunk*my_rank ;i< chunk*(my_rank+1);i++) {
      c_fft1d(C[i], N, 1);
   }
   if ( my_rank == SRCE ) t10 = MPI_Wtime();

   
   /* Gather matrice C from other processor to source processor*/
   MPI_Gather( &C[chunk*my_rank][0], chunk*N, MPI_COMPLEX, &C[0][0], chunk*N, MPI_COMPLEX, SRCE, MPI_COMM_WORLD );


   /* End time */
   if ( my_rank == SRCE )
      time_end = MPI_Wtime();

   print_matrix(C, "Matrix C", SRCE);
   if ( my_rank==0) printf("C[0][0].r     = %e\n", C[0][0].r);
   if ( my_rank==0) printf("C[N-1][N-1].r = %e\n", C[N-1][N-1].r);

   /* Write output file */
   write_matrix("output_matrix_MPI_Collective", C);

   if ( my_rank==0) printf("\nProgram Completed\n");
   if ( my_rank==0) {

      double tcomputation = (t2-t1) + (t4-t3) + (t6-t5) + (t8-t7) + (t10-t9);
      double tcommunication = (t1-time_begin) + (t3-t2) + (t5-t4) + (t7-t6) + (t9-t8) + (time_end-t10);

      printf("Total time spent is %f ms\n", (time_end-time_begin) * 1000 );
      printf("Time computation is  %f ms\n", tcomputation * 1000 );
      printf("Time communication is  %f ms\n", tcommunication * 1000 );
   }

   MPI_Finalize();
}






/* Read the input matrices from input files*/
/* The complex part is 0 */
int read_matrix ( const char* filename, complex matrix[N][N] ) {
   if ( my_rank == SRCE ) {
      int i, j;
      FILE *fp = fopen(filename,"r");

      if ( !fp ) {
         printf("File does not exist :Error%s\n", filename);
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

/* Print output matrix of size less than 40 */
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
