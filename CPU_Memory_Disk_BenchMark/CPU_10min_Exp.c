#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SECONDS 600
int flag=0; //flag to track 10 min time
int time_count=0;
long t1_count=0,t2_count=0,t3_count=0,t4_count=0; //each thread count per second.
long t1[600]; //array to store GFLOP/GIOP sample for 600 seconds.


//function to perform Floating point operations
void *flopFunction(void *arg)
{
double a=1.23,b=2.34,c=1.24,d=2.12,e=2.3,f=4.12,g=3.45,h=1.25,i=3.45,j=2.32,k=1.25,l=2.4,m=3.12,n=2.31,o=3.21,p=5.20,q=4.12,r=2.20,s=2.22,t=2.30,u=1.20,v=2.12,w=2.34,x=1.22,y=1.32,z=2.4;
long *i_count;
int *t_thread=(int *) arg;
//Switch case to identify thread with id
switch((*t_thread))
{
    case 1:
    i_count=&t1_count;
    break;
    case 2:
    i_count=&t2_count;
    break;
    case 3:
    i_count=&t3_count;
    break;
    case 4:
    i_count=&t4_count;
    break;
}
//Run loop till end of 10 min by reading flag.
while(!flag)
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

(*i_count)++;

}
}
//Function to perform integer operations
void *iopFunction(void *arg)
{

int a=1,b=2,c=3,d=5,e=3,f=6,g=7,h=3,i=4,j=3,k=67,l=23,m=12,n=5,o=6,p=5,q=3,r=3,s=5,t=7,u=1,v=5,w=3,x=4,y=5,z=2;
long *i_count;
int *t_thread=(int *) arg;
//Function to recognize thread
switch(*t_thread)
{
    case 1:
        i_count=&t1_count;
        break;
    case 2:
        i_count=&t2_count;
        break;
    case 3:
        i_count=&t3_count;
    break;
    case 4:
        i_count=&t4_count;
    break;
}
//Run loop for 10 min
while(!flag)
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

(*i_count)++;
}

}

void *data(void *arg)
{
    //Run loop for 600 seconds.
while(time_count<SECONDS)
{
    sleep(1);

    t1[time_count]=((t1_count+t2_count+t3_count+t4_count)/4);
    t1_count=0;

    t2_count=0;

  t3_count=0;

    t4_count=0;

    time_count++;
}
flag=1;
}

void pdata()
{
    //print result to file
FILE *fp;
fp=fopen("CPU_10min_data_flops.txt","w+");
int count=0;
fprintf(fp,"\nGFLOPS:Threads count 4\n");
while(count<time_count)
{
fprintf(fp,"%lf ,",t1[count]*0.000000039);
count++;
}
return ;
}

void pdata_i()
{
FILE *fp;
fp=fopen("CPU_10min_data_iops.txt","w+");
int count=0;
fprintf(fp,"\nGIOPS:Threads count 4\n");
while(count<time_count)
{
fprintf(fp,"%lf ,",t1[count]*0.000000039);
count++;
}
return;
}

int main()
{
    printf("CPU_10min_Exp\n");
    pthread_t thread1,thread2,thread3,thread4,thread5;
    //create 4 threads to call flop/iop function
    int t1=1;
    pthread_create(&thread1,NULL,flopFunction,&t1);
    int t2=2;
     pthread_create(&thread2,NULL,flopFunction,&t2);
     int t3=3;
      pthread_create(&thread3,NULL,flopFunction,&t3);
      int t4=4;
       pthread_create(&thread4,NULL,flopFunction,&t4);
       //create 5th thread store GFLOP/GIOP in array
       pthread_create(&thread5,NULL,data,NULL);

       pthread_join(thread1,NULL);
        pthread_join(thread2,NULL);
        pthread_join(thread3,NULL);
        pthread_join(thread4,NULL);
        pthread_join(thread5,NULL);

        pdata();

        flag=0;
        time_count=0;
        t1_count=0;
        t2_count=0;
        t3_count=0;
        t4_count=0;

    t1=1;
    pthread_create(&thread1,NULL,iopFunction,&t1);
    t2=2;
     pthread_create(&thread2,NULL,iopFunction,&t2);
     t3=3;
      pthread_create(&thread3,NULL,iopFunction,&t3);
      t4=4;
       pthread_create(&thread4,NULL,iopFunction,&t4);
       pthread_create(&thread5,NULL,data,NULL);

       pthread_join(thread1,NULL);
        pthread_join(thread2,NULL);
        pthread_join(thread3,NULL);
        pthread_join(thread4,NULL);
        pthread_join(thread5,NULL);

        pdata_i();
        return 0;
}
