/*
This program implemets the 2D convolution using SPMD model and use MPI send and receive
operations to perform communication.
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

#define LOGFILE	"MPI_Send_Recieve.log"     // all Log(); messages will be appended to this file
 
bool LogCreated =false;      // keeps track whether the log file is created or not
 
void Log (char *message);    // logs a message to LOGFILE
void LogErr (char *message); // logs a message; execution is interrupted
/* Size of matrix (NxN) */
const int N = 512;


int p, my_rank;
#define SOURCE 0

int main (int argc, char **argv) {

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &p);

   /* Input files */
   const char* inputfile1 = argc == 3 ? argv[1] : "sample/1_im1";
   const char* inputfile2 = argc == 3 ? argv[2] : "sample/1_im2";

   if ( my_rank==0) printf("\nMPI with Send + Recv\n");
   if ( my_rank==0) printf("Number of processors = %d\n",p);
   if ( my_rank==0) printf("Using input images %s, %s\n\n",inputfile1, inputfile2);

   /*  functions Prototype*/
   int read_matrix ( const char* file, complex matrix[N][N] );
   int write_matrix ( const char* file, complex matrix[N][N] );
  // int write_log( const char* file, char* message );
   void c_fft1d(complex *r, int n, int isign);
   void print_matrix ( complex matrix[N][N], const char* matrixname, int rank );


   /* Variable init */
   int chunk = N / p; /* number of rows for each process */
   complex A[N][N], B[N][N], C[N][N];
   int i, j;
   complex temp;
   double time_start, time_end, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;
   MPI_Status status;

Log("Read files");
   /* Read files */
   read_matrix (inputfile1, A);
   read_matrix (inputfile2, B);

   print_matrix(A, "Matrix A", SOURCE);
   print_matrix(B, "Matrix B", SOURCE);

   /* Initial time */
   if ( my_rank == SOURCE )
      time_start = MPI_Wtime();


   
   /* Scatter A and B to the other processes. We supose N is divisible by p */
   if ( my_rank == SOURCE ){
	   
      for ( i=0; i<p; i++ ) {
         if ( i==SOURCE ) continue; /* Source process doesn't send to itself */
Log("Scatter A and B to the other processes. We supose N is divisible by p");
         MPI_Send( &A[chunk*i][0], chunk*N, MPI_COMPLEX, i, 0, MPI_COMM_WORLD );
         MPI_Send( &B[chunk*i][0], chunk*N, MPI_COMPLEX, i, 0, MPI_COMM_WORLD );
		 
      }
   }
   else {
	   Log("Recieve A and B from SOURCE");
      MPI_Recv( &A[chunk*my_rank][0], chunk*N, MPI_COMPLEX, SOURCE, 0, MPI_COMM_WORLD, &status );
      MPI_Recv( &B[chunk*my_rank][0], chunk*N, MPI_COMPLEX, SOURCE, 0, MPI_COMM_WORLD, &status );
   }
   if ( my_rank == SOURCE ) t1 = MPI_Wtime();



   /* Apply 1D FFT in all rows of A and B */
   for (i= chunk*my_rank ;i< chunk*(my_rank+1);i++) {
	   Log("Apply 1D FFT in all rows of A and B");
         c_fft1d(A[i], N, -1);
         c_fft1d(B[i], N, -1);
   }
   if ( my_rank == SOURCE ) t2 = MPI_Wtime();



   /* Gather A and B to the source processor */
   if ( my_rank == SOURCE ){
      for ( i=0; i<p; i++ ) {
         if ( i==SOURCE ) continue; /* Source process doesn't send to itself */
Log("Gather A and B from other processor to SOURCE");
         MPI_Recv( &A[chunk*i][0], chunk*N, MPI_COMPLEX, i, 0, MPI_COMM_WORLD, &status );
         MPI_Recv( &B[chunk*i][0], chunk*N, MPI_COMPLEX, i, 0, MPI_COMM_WORLD, &status );
      }
   }
   else {
	   Log("Send A and B to SOURCE");
      MPI_Send( &A[chunk*my_rank][0], chunk*N, MPI_COMPLEX, SOURCE, 0, MPI_COMM_WORLD );
      MPI_Send( &B[chunk*my_rank][0], chunk*N, MPI_COMPLEX, SOURCE, 0, MPI_COMM_WORLD );
   }
   if ( my_rank == SOURCE ) t3 = MPI_Wtime();

   

   /* Transpose matrixes sequentially */
   if ( my_rank == SOURCE ) {
	   Log("Transpose matrixes sequentially");
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
  




   /* Scatter A and B to the other processes. We supose N is divisible by p */
   if ( my_rank == SOURCE ){
      for ( i=0; i<p; i++ ) {
         if ( i==SOURCE ) continue; /* Source process doesn't send to itself */
Log("Scatter A and B to the other processes. We supose N is divisible by p");
         MPI_Send( &A[chunk*i][0], chunk*N, MPI_COMPLEX, i, 0, MPI_COMM_WORLD );
         MPI_Send( &B[chunk*i][0], chunk*N, MPI_COMPLEX, i, 0, MPI_COMM_WORLD );
      }
   }
   else {
	   Log("Gather A and B from other processor to source");
      MPI_Recv( &A[chunk*my_rank][0], chunk*N, MPI_COMPLEX, SOURCE, 0, MPI_COMM_WORLD, &status );
      MPI_Recv( &B[chunk*my_rank][0], chunk*N, MPI_COMPLEX, SOURCE, 0, MPI_COMM_WORLD, &status );
   }
   if ( my_rank == SOURCE ) t5 = MPI_Wtime();



   /* Apply 1D FFT in all rows of A and B */
   for (i= chunk*my_rank ;i< chunk*(my_rank+1);i++) {
	   Log("Apply 1D FFT in all rows of A and B");
         c_fft1d(A[i], N, -1);
         c_fft1d(B[i], N, -1);
   }

   
   /* Point to point multiplication */
   for (i= chunk*my_rank ;i< chunk*(my_rank+1);i++) {
	   Log("Point to point multiplication");
      for (j=0;j<N;j++) {
         C[i][j].r = A[i][j].r*B[i][j].r - A[i][j].i*B[i][j].i;
         C[i][j].i = A[i][j].r*B[i][j].i + A[i][j].i*B[i][j].r;
      }
   }

  
   /* Inverse 1D FFT in all rows of C */
   for (i= chunk*my_rank ;i< chunk*(my_rank+1);i++) {
	   Log("Inverse 1D FFT in all rows of C");
      c_fft1d(C[i], N, 1);
   }
   if ( my_rank == SOURCE ) t6 = MPI_Wtime();

  
   /* Gather the fragments of C to the source processor */
   if ( my_rank == SOURCE ){
      for ( i=0; i<p; i++ ) {
         if ( i==SOURCE ) continue; /* Source process doesn't receive from itself */
Log("Gather the fragments of C to the source processor");
         MPI_Recv( &C[chunk*i][0], chunk*N, MPI_COMPLEX, i, 0, MPI_COMM_WORLD, &status );
      }
   }
   else
   {
	   Log("Send fragaments of C to source processor");
      MPI_Send( &C[chunk*my_rank][0], chunk*N, MPI_COMPLEX, SOURCE, 0, MPI_COMM_WORLD );
   }
   if ( my_rank == SOURCE ) t7 = MPI_Wtime();

   
   /* Transpose C sequentially */
   if ( my_rank == SOURCE ) {
	   Log("Transpose C sequentially");
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
   if ( my_rank == SOURCE ){
	   Log("Scatter C to the other processes");
      for ( i=0; i<p; i++ ) {
         if ( i==SOURCE ) continue; /* Source process doesn't receive from itself */

         MPI_Send( &C[chunk*i][0], chunk*N, MPI_COMPLEX, i, 0, MPI_COMM_WORLD );
      }
   }
   else
   {
	   Log("Gather C from other processor");
      MPI_Recv( &C[chunk*my_rank][0], chunk*N, MPI_COMPLEX, SOURCE, 0, MPI_COMM_WORLD, &status );
   }
   if ( my_rank == SOURCE ) t9 = MPI_Wtime();


   /* Inverse 1D FFT in all columns of C */
   log("Inverse 1D FFT in all columns of C");
   for (i= chunk*my_rank ;i< chunk*(my_rank+1);i++) {
      c_fft1d(C[i], N, 1);
   }
   if ( my_rank == SOURCE ) t10 = MPI_Wtime();

   /* Transpose C */
   /* It is not necessary if we remove the other traspose */


   /* Gather the fragments of C to the source processor */
   if ( my_rank == SOURCE ){
      for ( i=0; i<p; i++ ) {
         if ( i==SOURCE ) continue; /* Source process doesn't receive from itself */
Log("Gather the fragments of c to the source processor");
         MPI_Recv( &C[chunk*i][0], chunk*N, MPI_COMPLEX, i, 0, MPI_COMM_WORLD, &status );
      }
   }
   else
   {
	   Log("scatter the fragments of c to the source processor");
      MPI_Send( &C[chunk*my_rank][0], chunk*N, MPI_COMPLEX, SOURCE, 0, MPI_COMM_WORLD );
   }

/*-------------------------------------------------------------------------------------------------------*/
   /* Final time */
   if ( my_rank == SOURCE )
      time_end = MPI_Wtime();
Log("done");
   print_matrix(C, "Matrix C", SOURCE);
   if ( my_rank==0) printf("C[0][0].r     = %e\n", C[0][0].r);
   if ( my_rank==0) printf("C[N-1][N-1].r = %e\n", C[N-1][N-1].r);

   /* Write output file */
   write_matrix("output_matrix_MPI_Send_Recieve", C);

   if ( my_rank==0) printf("\nCS 546 Project: done\n");
   if ( my_rank==0) {

      double tcomputation = (t2-t1) + (t4-t3) + (t6-t5) + (t8-t7) + (t10-t9);
      double tcommunication = (t1-time_start) + (t3-t2) + (t5-t4) + (t7-t6) + (t9-t8) + (time_end-t10);

      printf("Total time spent is %f ms\n", (time_end-time_start) * 1000 );
      printf("Time computation is  %f ms\n", tcomputation * 1000 );
      printf("Time communication is  %f ms\n", tcommunication * 1000 );
   }

   MPI_Finalize();
}




/*-------------------------------------------------------------------------------------------------------*/

/* Reads the matrix from tha file and inserts the values in the real part */
/* The complex part is left to 0 */
int read_matrix ( const char* filename, complex matrix[N][N] ) {
   if ( my_rank == SOURCE ) {
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
   if ( my_rank == SOURCE ) {
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

/* Write log file  
int write_log ( const char* filename, char* message ) {
         int i, j;
      FILE *fp = fopen(filename,"w");
      
            fprintf(fp,"   %e",message);
         fprintf(fp,"\n");
     

      fclose(fp);
   }
} */
/* Print the matrix if its size is no more than 32x32 */
/* Rank is the processor that should print this */
void print_matrix ( complex matrix[N][N], const char* matrixname, int rank ) {
   if ( my_rank == rank ) {
      if ( N<33 ) {
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

/* log functions */
void Log (char *message)
{
	FILE *file;
 
	if (!LogCreated) {
		file = fopen(LOGFILE, "w");
		LogCreated = true;
	}
	else		
		file = fopen(LOGFILE, "a");
		
	if (file == NULL) {
		if (LogCreated)
			LogCreated = false;
		return;
	}
	else
	{
		fputs(message, file);
		fclose(file);
	}
 
	if (file)
		fclose(file);
}
 
void LogErr (char *message)
{
	Log(message);
	Log("\n");
	//SysShutdown();
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
