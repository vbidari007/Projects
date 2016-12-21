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

    char *buf;
    char *dest;

    void *ByteSequentialWrite(void *arg)
    {
        unsigned int *ITR=(unsigned int *)arg;
        FILE *fp;
        int i;
        fp=fopen("file1.txt","w+");
        for(i=0;i< *ITR;i++)
        {
          *(buf+i);
        fwrite(buf,1,BYTE,fp);

        }

        fclose(fp);

    }

    void *ByteRandomWrite(void *arg)
    {
        unsigned int *ITR=(unsigned int *)arg;
        FILE *fp;
        int i,r;
        fp=fopen("file2.txt","w+");
        for(i=0;i< *ITR;i++)
        {
        r=rand()% (TOTAL_SIZE-BYTE);
         fseek(fp,r,SEEK_SET);
        *(buf+i);
        fwrite(buf,1,BYTE,fp);

        }

        fclose(fp);
    }

    void *ByteSequentialRead(void *arg)
    {
        unsigned int *ITR=(unsigned int *)arg;
        FILE *fp;
        int i;

        fp=fopen("file1.txt","r+");
        for(i=0;i< *ITR;i++)
        {
         *(dest+i);
        fread(dest,1,BYTE,fp);

        }

        fclose(fp);
    }

    void *ByteRandomRead(void *arg)
    {
        unsigned int *ITR=(unsigned int *)arg;
        FILE *fp;

        int i,r;
        fp=fopen("file2.txt","r+");
        for(i=0;i< *ITR;i++)
        {
        r=rand()%(TOTAL_SIZE-BYTE);
        fseek(fp,r,SEEK_SET);
         *(dest+i);
        fread(dest,1,BYTE,fp);

        }
        fclose(fp);
    }

    void *KByteSequentialWrite(void *arg)
    {
        unsigned int *ITR=(unsigned int *)arg;
        FILE *fp;
        int i,size1=0;
        fp=fopen("file3.txt","w+");
        for(i=0;i< *ITR;i++)
        {
         size1=0;
         *(buf+(i*KBYTE));

     while(size1!=KBYTE)
     {
     size1=fwrite(buf,1,KBYTE,fp);
     }

        }

        fclose(fp);
    }


    void *KByteRandomWrite(void *arg)
    {
        unsigned int *ITR=(unsigned int *)arg;
        FILE *fp;
        int i,size1=0;
        int r;
        fp=fopen("file4.txt","w+");
        for(i=0;i< *ITR;i++)
        {
         size1=0;
        r=rand()%(TOTAL_SIZE-KBYTE);
        fseek(fp,r,SEEK_SET);
         *(buf+(i*KBYTE));
         while(size1!=KBYTE)
         {
        size1=fwrite(buf,1,KBYTE,fp);
        }


        }

        fclose(fp);
    }


    void *KByteSequentialRead(void *arg)
    {
        unsigned int *ITR=(unsigned int *)arg;
        FILE *fp;
        int i,size1=0;
        fp=fopen("file3.txt","r+");
        for(i=0;i< *ITR;i++)
        {
         size1=0;
         *(dest+ (i*KBYTE));
         while(size1!=KBYTE)
         {
       size1=fread(dest,1,KBYTE,fp);
        }

        }

        fclose(fp);

    }

    void *KByteRandomRead(void *arg)
    {   unsigned int *ITR=(unsigned int *)arg;
        FILE *fp;
        int i,r,size1=0;
        fp=fopen("file4.txt","r+");
        for(i=0;i< *ITR;i++)
        {
         size1=0;
        r=rand()%(TOTAL_SIZE-KBYTE);
        fseek(fp,r,SEEK_SET);
           *(dest+(i*KBYTE));
         while(size1!=KBYTE)
         {
       size1=fread(dest,1,KBYTE,fp);
        }


        }

        fclose(fp);

    }
    void *MByteSequentialWrite(void *arg)
    {
        unsigned int *ITR=(unsigned int *)arg;
        FILE *fp;
        int i,size1=0;

        fp=fopen("file9.txt","w+");
        for(i=0;i< *ITR;i++)
        {
         size1=0;
         *(buf+(i*MBYTE));
        while(size1!=MBYTE)
         {
        size1=fwrite(buf,1,MBYTE,fp);

        }

        }
        fclose(fp);
    }

    void *MByteRandomWrite(void *arg)
    {
        unsigned int *ITR=(unsigned int *)arg;
        FILE *fp;
        int i,r,size1=0;

        fp=fopen("file10.txt","w+");
        for(i=0;i< *ITR;i++)
        {
        size1=0;
        r=rand()%(TOTAL_SIZE-MBYTE);
        fseek(fp,r,SEEK_SET);

        *(buf+(i*MBYTE));
        while(size1!=MBYTE)
        {
        size1=fwrite(buf,1,MBYTE,fp);
        }

        }
              fclose(fp);
    }

    void *MByteSequentialRead(void *arg)
    {
        unsigned int *ITR=(unsigned int *)arg;
        FILE *fp;
        int i,size1=0;
        fp=fopen("file9.txt","w+");
        for(i=0;i< *ITR;i++)
        {
        size1=0;
        *(dest+(i*MBYTE));
        //while(size1!=MBYTE)
        {
        size1=fread(dest,1,MBYTE,fp);
        }

        }
        scanf("%d",&size1);
        fclose(fp);
    }

    void *MByteRandomRead(void *arg)
    {
        unsigned int *ITR=(unsigned int *)arg;
        FILE *fp;
        int i,r,size1=0;
        fp=fopen("file10.txt","w+");
        for(i=0;i< *ITR;i++)
        {
        size1=0;
            r=rand()% (TOTAL_SIZE-MBYTE);
            fseek(fp,r,SEEK_SET);
            *(dest+(i*MBYTE));
             while(size1!=MBYTE)
             {
            size1=fread(dest,1,MBYTE,fp);
            }

        }

        fclose(fp);
    }

    int main()
    {
        int Bsize,i,itr1;
        unsigned int itr;
        int Numthreads;
        pthread_t th[10];
        clock_t start, end;
        double elapsed,latency,throughput;
        buf=(char *) malloc(TOTAL_SIZE*sizeof(char));
        dest=(char *) malloc(TOTAL_SIZE*sizeof(char));
        for(i=0;i<TOTAL_SIZE;i++)
        {
        buf[i]='w';
        }

    while(1)
    {

        printf("\nAssignment#1: Disk Benchmark");
         printf("\nSelect number of threads: 1 -> 1Thread  2 -> 2Thread\n");
        scanf("%d",&Numthreads);
        printf("\nSelect the Block size: 1 -> 1B  2 -> 1KB 3 -> 1MB\n");
        scanf("%d", &Bsize);

        itr=(TOTAL_SIZE)/Numthreads;


        switch(Bsize)
        {
            case 1:
            printf("\n------1B Sequential write------\n");
            printf("\nNumber of threads=%d",Numthreads);
            start=clock();
                for(i=0;i<Numthreads;i++)
                {
                    pthread_create(&th[i],NULL,ByteSequentialWrite,&itr);
                    pthread_join(th[i],NULL);
                }
            end=clock();
            elapsed= (double)(end-start)/ CLOCKS_PER_SEC;
            latency= (double)(elapsed/(TOTAL_SIZE));
            throughput= (double)(1/latency);
            printf("\nElapsed Time in sec:%lf\n",elapsed);
            printf("\nLatency in ms=%lf\n",(1000*latency));
            printf("\nThroughput MB/sec=%lf\n",(throughput/1000000));

            printf("------------------------------------------------");
             printf("------1B Random write------\n");
            start=clock();

                for(i=0;i<Numthreads;i++)
                {
                    pthread_create(&th[i],NULL,ByteRandomWrite,&itr);
                    pthread_join(th[i],NULL);
                }

            end=clock();
            elapsed= (double)(end-start)/ CLOCKS_PER_SEC;
            latency= (double)(elapsed/(TOTAL_SIZE));
            throughput= (double)(1/latency);
            printf("\nElapsed Time in sec:%lf\n",elapsed);
            printf("\nLatency in ms=%.10lf\n",(1000*latency));
            printf("\nThroughput MB/sec=%lf\n",(throughput/1000000));

            printf("\n-----------------------------------------------------------");
             printf("\n------1B Sequential Read------\n");
            printf("\nNumber of threads=%d",Numthreads);
            start=clock();
                for(i=0;i<Numthreads;i++)
                {
                    pthread_create(&th[i],NULL,ByteSequentialRead,&itr);
                    pthread_join(th[i],NULL);
                }
            end=clock();
            elapsed= (double)(end-start)/ CLOCKS_PER_SEC;
            latency= (double)(elapsed/(TOTAL_SIZE));
            throughput= (double)(1/latency);
            printf("\nElapsed Time in sec:%lf\n",elapsed);
            printf("\nLatency in ms=%lf\n",(1000*latency));
            printf("\nThroughput MB/sec=%lf\n",(throughput/1000000));

            printf("------------------------------------------------");
             printf("------1B Random Read------\n");

            start=clock();

                for(i=0;i<Numthreads;i++)
                {
                    pthread_create(&th[i],NULL,ByteRandomRead,&itr);
                    pthread_join(th[i],NULL);
                }

            end=clock();
            elapsed= (double)(end-start)/ CLOCKS_PER_SEC;
            latency= (double)(elapsed/(TOTAL_SIZE));
            throughput= (double)(1/latency);
            printf("\nElapsed Time in sec:%lf\n",elapsed);
            printf("\nLatency in ms=%.10lf\n",(1000*latency));
            printf("\nThroughput MB/sec=%lf\n",(throughput/1000000));

                    break;
            case 2:

            itr1=(int)itr/KBYTE;
             printf("------1KB Sequential write------\n");
            printf("Number of threads=%d\n",Numthreads);
            start=clock();
                for(i=0;i<Numthreads;i++)
                {
                    pthread_create(&th[i],NULL,KByteSequentialWrite,&itr1);
                    pthread_join(th[i],NULL);
                }
            end=clock();
            elapsed= (double)(end-start)/ CLOCKS_PER_SEC;
            latency= (double)(elapsed/(TOTAL_SIZE));
            throughput= (double)(1/latency);
            printf("\nElapsed Time in sec:%lf\n",elapsed);
            printf("\nLatency in ms=%.10lf\n",(1000*latency));
            printf("\nThroughput MB/sec=%lf\n",(throughput/1000000));

            printf("-----------------------------------------------------");
             printf("------1KB Random write------\n");
            printf("Number of threads=%d\n",Numthreads);
            start=clock();
                for(i=0;i<Numthreads;i++)
                {
                    pthread_create(&th[i],NULL,KByteRandomWrite,&itr1);
                    pthread_join(th[i],NULL);
                }
            end=clock();
            elapsed= (double)(end-start)/ CLOCKS_PER_SEC;
            latency= (double)(elapsed/(TOTAL_SIZE));
            throughput= (double)(1/latency);
            printf("\nElapsed Time in sec:%lf\n",elapsed);
            printf("\nLatency in ms=%.10lf\n",(1000*latency));
            printf("\nThroughput MB/sec=%lf\n",(throughput/1000000));

            printf("\n-----------------------------------------------------\n");
            printf("------1KB Sequential read------\n");
           // printf("Number of threads=%d\n",Numthreads);
            start=clock();
                for(i=0;i<Numthreads;i++)
                {
                    pthread_create(&th[i],NULL,KByteSequentialRead,&itr1);
                    pthread_join(th[i],NULL);
                }
            end=clock();
            elapsed= (double)(end-start)/ CLOCKS_PER_SEC;
            latency= (double)(elapsed/(TOTAL_SIZE));
            throughput= (double)(1/latency);
            printf("\nElapsed Time in sec:%lf\n",elapsed);
            printf("\nLatency in ms=%.10lf\n",(1000*latency));
            printf("\nThroughput MB/sec=%lf\n",(throughput/1000000));

            printf("-----------------------------------------------------");
             printf("------1KB Random read------\n");
           // printf("Number of threads=%d\n",Numthreads);
            start=clock();
                for(i=0;i<Numthreads;i++)
                {
                    pthread_create(&th[i],NULL,KByteRandomRead,&itr1);
                    pthread_join(th[i],NULL);
                }
            end=clock();
            elapsed= (double)(end-start)/ CLOCKS_PER_SEC;
            latency= (double)(elapsed/(TOTAL_SIZE));
            throughput= (double)(1/latency);
            printf("\nElapsed Time in sec:%lf\n",elapsed);
            printf("\nLatency in ms=%.10lf\n",(1000*latency));
            printf("\nThroughput MB/sec=%lf\n",(throughput/1000000));
                    break;


            case 3:
            itr1=itr/(MBYTE);
           //printf("%d",itr1);
             printf("------1MB Sequential write------\n");
            printf("\nNumber of threads=%d",Numthreads);
            start=clock();
                for(i=0;i<Numthreads;i++)
                {
                    pthread_create(&th[i],NULL,MByteSequentialWrite,&itr1);
                    pthread_join(th[i],NULL);
                }
            end=clock();
         elapsed= (double)(end-start)/ CLOCKS_PER_SEC;
            latency= (double)(elapsed/(TOTAL_SIZE));
            throughput= (double)(1/latency);
            printf("\nElapsed Time in sec:%lf\n",elapsed);
            printf("\nLatency in ms=%.10lf\n",(1000*latency));
            printf("\nThroughput MB/sec=%lf\n",(throughput/1000000));

            printf("\n-----------------------------------------------------------");

             printf("\n------1MB Random write------");

            start=clock();

                for(i=0;i<Numthreads;i++)
                {
                    pthread_create(&th[i],NULL,MByteRandomWrite,&itr1);
                    pthread_join(th[i],NULL);
                }
            end=clock();
           elapsed= (double)(end-start)/ CLOCKS_PER_SEC;
            latency= (double)(elapsed/(TOTAL_SIZE));
            throughput= (double)(1/latency);
            printf("\nElapsed Time in sec:%lf\n",elapsed);
            printf("\nLatency in ms=%.10lf\n",(1000*latency));
            printf("\nThroughput MB/sec=%lf\n",(throughput/1000000));

            printf("\n--------------------------------------------------------------\n");
            printf("------1MB Sequential read------\n");
            printf("\nNumber of threads=%d",Numthreads);
            start=clock();
                for(i=0;i<Numthreads;i++)
                {
                    pthread_create(&th[i],NULL,MByteSequentialRead,&itr1);
                    pthread_join(th[i],NULL);
                }
            end=clock();
         elapsed= (double)(end-start)/ CLOCKS_PER_SEC;
            latency= (double)(elapsed/(TOTAL_SIZE));
            throughput= (double)(1/latency);
            printf("\nElapsed Time in sec:%lf\n",elapsed);
            printf("\nLatency in ms=%.10lf\n",(1000*latency));
            printf("\nThroughput MB/sec=%lf\n",(throughput/1000000));

            printf("\n-----------------------------------------------------------");

             printf("\n------1MB Random read------");

            start=clock();
                 for(i=0;i<Numthreads;i++)
                {
                    pthread_create(&th[i],NULL,MByteRandomRead,&itr1);
                    pthread_join(th[i],NULL);
                }
            end=clock();
           elapsed= (double)(end-start)/ CLOCKS_PER_SEC;
            latency= (double)(elapsed/(TOTAL_SIZE));
            throughput= (double)(1/latency);
            printf("\nElapsed Time in sec:%lf\n",elapsed);
            printf("\nLatency in ms=%.10lf\n",(1000*latency));
            printf("\nThroughput MB/sec=%lf\n",(throughput/1000000));

                    break;

                    default:
                    free(buf);
                    free(dest);
                    exit(0);

        }

     }
     free(buf);
     free(dest);
      return 0;
    }
