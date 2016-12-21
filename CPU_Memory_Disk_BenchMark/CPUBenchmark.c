                #include <stdio.h>
                #include <stdlib.h>
                #include <time.h>
                #include <pthread.h>
                #include <sys/time.h>
                #include <string.h>
                #include <sys/times.h>
                #include <omp.h>

                #define ITERATION_SIZE 300000000

                void *flopFunction(void *arg)
                {
                    double a=1.23,b=2.34,c=1.24,d=2.12,e=2.3,f=4.12,g=3.45,h=1.25,i=3.45,j=2.32,k=1.25,l=2.4,m=3.12,n=2.31,o=3.21,p=5.20,q=4.12,r=2.20,s=2.22,t=2.30,u=1.20,v=2.12,w=2.34,x=1.22,y=1.32,z=2.4;
                    int i_c;
                   // int N;
                    int *N=(int*) arg;
                    for( i_c=0;i_c< *N;i_c++)
                        {
                            a=b/c;
                            d=e+f;
                            g=h*i;
                            j=k+l;
                            m=n+o;
                            p=q+r;
                            s=t*u;
                            v=w+x;
                            y=z+a;
                            b=c*d;
                            e=f+g;
                            h=i*j;
                            k=l+m;
                            n=o/p;
                            q=r+s;
                            t=u+v;
                            w=x*y;
                            z=a+b;

                        }
                        return NULL ;

                }

                void *iopFunction(void *arg)
                {
                    int a=1,b=2,c=3,d=5,e=3,f=6,g=7,h=3,i=4,j=3,k=67,l=23,m=12,n=5,o=6,p=5,q=3,r=3,s=5,t=7,u=1,v=5,w=3,x=4,y=5,z=2;
                    int i_c;
                    //int *N;
                   int *N=(int*) arg;
                    for(i_c=0;i_c< *N;i_c++)
                        {
                           a=b/c;
                            d=e+f;
                            g=h*i;
                            j=k+l;
                            m=n+o;
                            p=q+r;
                            s=t*u;
                            v=w+x;
                            y=z+a;
                            b=c*d;
                            e=f+g;
                            h=i*j;
                            k=l+m;
                            n=o/p;
                            q=r+s;
                            t=u+v;
                            w=x*y;
                            z=a+b;
                        }
                        return NULL;
                }



                int main()
                {
                    //clock_t start, end;
                    double  g_flops, g_iops,cpu_time;
                   // struct tms cputstart, cputstop;
                    struct timeval etstart, etstop;  /* Elapsed times using gettimeofday() */
                    struct timezone tzdummy;
                    unsigned long long usecstart, usecstop;
                    int i,num_threads;

                    pthread_t threads[10];

                            while(1)
                            {
                                    printf("\nEnter the number of Threads: 1, 2, 4\n");
                                    scanf("%d",&num_threads);
                                    if(num_threads==1 || num_threads==2 || num_threads==4)
                                    {
                                            int iteration= ITERATION_SIZE/num_threads;
                                            gettimeofday(&etstart, &tzdummy);
                                            for(i=0;i< num_threads;i++)
                                            {
                                                pthread_create(&threads[i],NULL,flopFunction,&iteration);
                                                pthread_join(threads[i],NULL);
                                            }

                                        gettimeofday(&etstop, &tzdummy);


                                    usecstart = (unsigned long long)etstart.tv_sec * 1000000 + etstart.tv_usec;
                                    usecstop = (unsigned long long)etstop.tv_sec * 1000000 + etstop.tv_usec;


                                     printf("\nElapsed time = %lf ms.\n",	 (double)(usecstop - usecstart)/(double)1000);


                                    cpu_time=(double)(usecstop - usecstart)/(double)1000000;

                                    g_flops=(double) (12)/cpu_time;

                                    printf("GFLOPS= %lf\n",g_flops);


                                            gettimeofday(&etstart, &tzdummy);

                                            for(i=0;i< num_threads;i++)
                                            {
                                                pthread_create(&threads[i],NULL,iopFunction,&iteration);
                                                pthread_join(threads[i],NULL);
                                            }

                                            gettimeofday(&etstop, &tzdummy);
                                   // end=clock();

                                    usecstart = (unsigned long long)etstart.tv_sec * 1000000 + etstart.tv_usec;
                                    usecstop = (unsigned long long)etstop.tv_sec * 1000000 + etstop.tv_usec;


                                    printf("\nElapsed time = %lf ms.\n",	 (double)(usecstop - usecstart)/(double)1000);

                //
                                    cpu_time=(double)(usecstop - usecstart)/(float)1000000;
                                    //cpu_time=cpu_time/num_threads;
                                                    g_iops= (double) (12)/cpu_time;
                                       // g_iops=g_iops/1000000000;

                                        printf("GIOPS:%lf\n",g_iops);


                                    }
                                    else
                                    {
                                    exit(0);
                                    }
                            }

                    return 0;
                }
