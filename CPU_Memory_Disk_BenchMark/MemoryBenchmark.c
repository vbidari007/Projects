#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <pthread.h>

 #define TOTAL_SIZE 10485760
 #define BYTE 1
 #define KBYTE 1024
 #define MBYTE 1048576
 char *src;

void *ByteSequentialReadWrite(void *arg)
{
    unsigned int *ITR=(unsigned int *)arg;
    int i;

   char *dest=malloc(TOTAL_SIZE*sizeof(*dest));


    for(i=0; i < *ITR;i++)
    {

   memcpy(dest,src,BYTE);

   *(src+i);
   *(dest+i);
    }

    free(dest);
}

void *ByteRandomReadWrite(void *arg)
{
    unsigned int *ITR=(unsigned int *)arg;
    int i,r;

    char *dest=malloc(TOTAL_SIZE*sizeof(*dest));

    for(i=0; i < *ITR;i++)
    {
        r=rand()%(TOTAL_SIZE-BYTE);
        memcpy(dest+r,src+r,BYTE);

    }

    free(dest);

}

void *KByteSequentialReadWrite(void *arg)
{
    int *ITR=(int *)arg;
    int i;


    char *dest=malloc(TOTAL_SIZE*sizeof(*dest));


    for(i=0; i < *ITR;i++)
    {

        memcpy(dest,src,KBYTE);
    *(src+(i*KBYTE));
    *(dest+(i*KBYTE));
    }
    //free(KBsrc);
    free(dest);
}

void *KByteRandomReadWrite(void *arg)
{
    unsigned int *ITR=(unsigned int *)arg;
    int i,r;


    char *dest=malloc(TOTAL_SIZE*sizeof(*dest));

    for(i=0; i < *ITR;i++)
    {
        r=rand()% (TOTAL_SIZE-KBYTE);
        memcpy(dest+r,src+r,KBYTE);

    }

    free(dest);

}

void *MByteSequentialReadWrite(void *arg)
{
    unsigned int *ITR=(unsigned int *)arg;
    int i,k;


    char *dest=malloc(TOTAL_SIZE*sizeof(*dest));

    for(i=0; i < *ITR;i++)
    {

   memcpy(dest,src,MBYTE);

   *(src+(i*MBYTE));
   *(dest+(i*MBYTE));
    }

    free(dest);
}

void *MByteRandomReadWrite(void *arg)
{
    unsigned int *ITR=(unsigned int *)arg;
    int i,r,k;

    char *dest=malloc(TOTAL_SIZE*sizeof(*dest));

    for(i=0; i < *ITR;i++)
    {
    r=rand()%(TOTAL_SIZE-MBYTE);
   memcpy(dest+r,src+r,(MBYTE));

    }

    free(dest);
}


int main()
{
    int Bsize,i,itr1;

    unsigned int itr,k;
    int Numthreads;
    pthread_t th[10];
    clock_t start, end;
    double elapsed,latency,throughput;
    src= (char *)malloc(TOTAL_SIZE*sizeof(char));

    for(k=0;k<TOTAL_SIZE;k++)
    {
        src[k]='w';

    }




while(1)
{


    printf("\nAssignment#1: Memory Benchmark");
     printf("\nSelect number of threads: 1 -> 1Thread  2 -> 2Thread\n");
    scanf("%d",&Numthreads);
    printf("\nSelect the Block size: 1 -> 1B  2 -> 1KB 3 -> 1MB\n");
    scanf("%d", &Bsize);

    itr=(TOTAL_SIZE)/Numthreads;
   // printf("it:%u",itr);

    switch(Bsize)
    {
        case 1:

        printf("\n------1B Sequential read+write------\n");
        printf("\nNumber of threads=%d",Numthreads);

        start=clock();
            for(i=0;i<Numthreads;i++)
            {
                pthread_create(&th[i],NULL,ByteSequentialReadWrite,&itr);
                pthread_join(th[i],NULL);
            }
        end=clock();
        elapsed= (double)(end-start)/ CLOCKS_PER_SEC;
        latency= (double)(elapsed/(2*TOTAL_SIZE));
        throughput= (double)(1/latency);
        printf("\nElapsed Time in sec:%lf\n",elapsed);
        printf("\nLatency in ms=%lf\n",(1000*latency));
        printf("\nThroughput MB/sec=%lf\n",(throughput/1000000));

        printf("------------------------------------------------");
         printf("------1B Random read+write------\n");
        //printf("\nNumber of threads=%d",Numthreads);
        start=clock();
        //printf("2");
            for(i=0;i<Numthreads;i++)
            {
                pthread_create(&th[i],NULL,ByteRandomReadWrite,&itr);
                pthread_join(th[i],NULL);
            }
          //  printf("3");
        end=clock();
        elapsed= (double)(end-start)/ CLOCKS_PER_SEC;
        latency= (double)(elapsed/(2*TOTAL_SIZE));
        throughput= (double)(1/latency);
        printf("\nElapsed Time in sec:%lf\n",elapsed);
        printf("\nLatency in ms=%.10lf\n",(1000*latency));
        printf("\nThroughput MB/sec=%lf\n",(throughput/1000000));

                break;
        case 2:

        itr1=(int)itr/KBYTE;
        printf("%u",itr);
         printf("------1KB Sequential read+write------\n");
        printf("Number of threads=%d\n",Numthreads);
        start=clock();
            for(i=0;i<Numthreads;i++)
            {
                pthread_create(&th[i],NULL,KByteSequentialReadWrite,&itr1);
                pthread_join(th[i],NULL);
            }
        end=clock();
        elapsed= (double)(end-start)/ CLOCKS_PER_SEC;
        latency= (double) ((elapsed)/(2*TOTAL_SIZE));
        throughput= (double)(1/latency);
        printf("\nElapsed Time in sec:%lf\n",elapsed);
        printf("\nLatency in ms=%.10lf\n",(1000*latency));
        printf("\nThroughput MB/sec=%lf\n",(throughput/1000000));

        printf("-----------------------------------------------------");
         printf("------1KB Random read+write------\n");
        printf("Number of threads=%d\n",Numthreads);
        start=clock();
            for(i=0;i<Numthreads;i++)
            {
                pthread_create(&th[i],NULL,KByteRandomReadWrite,&itr1);
                pthread_join(th[i],NULL);
            }
        end=clock();
        elapsed= (double)(end-start)/ CLOCKS_PER_SEC;
        latency= (double)((elapsed)/(2*TOTAL_SIZE));
        throughput= (double)(1/latency);
        printf("\nElapsed Time in sec:%lf\n",elapsed);
        printf("\nLatency in ms=%.10lf\n",(1000*latency));
        printf("\nThroughput MB/sec=%lf\n",(throughput/1000000));
                break;


        case 3:
        itr1=itr/MBYTE;
       printf("%d",itr1);
         printf("------1MB Sequential read+write------\n");
        printf("\nNumber of threads=%d",Numthreads);
        start=clock();
            for(i=0;i<Numthreads;i++)
            {
                pthread_create(&th[i],NULL,MByteSequentialReadWrite,&itr1);
                pthread_join(th[i],NULL);
            }
        end=clock();
     elapsed= (double)(end-start)/ CLOCKS_PER_SEC;
        latency= (double)((elapsed)/(2*TOTAL_SIZE));
        throughput= (double)(1/latency);
        printf("\nElapsed Time in sec:%lf\n",elapsed);
        printf("\nLatency in ms=%.10lf\n",(1000*latency));
        printf("\nThroughput MB/sec=%lf\n",(throughput/1000000));

        printf("\n-----------------------------------------------------------");

         printf("\n------1MB Random read+write------");
         start=clock();
          for(i=0;i<Numthreads;i++)
            {
                pthread_create(&th[i],NULL,MByteRandomReadWrite,&itr1);
                pthread_join(th[i],NULL);
            }
        end=clock();
       elapsed= (double)(end-start)/ CLOCKS_PER_SEC;
        latency= (double) ( (elapsed)/(2*TOTAL_SIZE));
        throughput= (double)(1/latency);
        printf("\nElapsed Time in sec:%lf\n",elapsed);
        printf("\nLatency in ms=%.10lf\n",(1000*latency));
        printf("\nThroughput MB/sec=%lf\n",(throughput/1000000));
                break;

                default:
                free(src);
                exit(0);

    }

 }
  free(src);
   return 0;
}
