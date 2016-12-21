This manual instructs user to excute this programming assignment step by step.
Since we are executing this programming assignement in Amazon ex2 instances using t2.micro instances.

Follow below steps to connect Amazon AWS.

	1. Open terminal in Linux OS.

	2. Change terminal working directory to amazon aws ".pem" file directory using below command
		cd path/to/pem/file

	3. Enter below command to connect amazon ec2 instance
		ssh -i "user.pem" username@public-domain-name
	
       for example:

		ssh -i vishwanath.pem ubuntu@ec2-54-201-89-31.us-west-2.compute.amazonaws.com
	//user.pem : is security file to connect amazon ec2 instance
	//username: host username
	//public-domain-name: public host name with IP.

	
	4. You will get into amazon ec2 instance terminal.
----------------------------------------------------------------------------------------------------------------------------------------------

CPUBenchmark.c : This file contains program to measure CPU benchmark.

Steps to excute program:

	1. Open terminal in Linux OS.

	2. Copy source code folder to amzon ec2 instance using below command
			scp -i user.pem -r  folder/path/copy  host_username@public-domain-name:/path/to/directory

		example: scp -i vishwanath.pem -r  Assignment#1/PA#1/ ubuntu@ec2-54-201-89-31.us-west-2.compute.amazonaws.com:/home/ubuntu
	
	3. Connect to Amazon aws ec2 instance following above steps.

	4. Change terminal working directory to source code directory using below command
		cd path/to/file

	5. Enter below command to excute CPUBenchmark program. 
 		gcc -pthread CPUBenchmark.c -o CPUBenchmark.out
	
   		//pthread is library to excute pthread functions in program.

	6. Excute output file using below command.
   		./CPUBenchmark.out

	7. Enter the number of threads in console as below.
		Enter number of threads (1 thread 2 thread): 1
		//1 for 1 thread
		//2 for 2 thread

	8. Result will be diaplayed in terms of GFLOPS and GIOPS as below
		GFLOPS:result, GIOPS: result.


--------------------------------------------------------------------------------------------------------------------------------------------

MemoryBenchmark.c : This file contain program to measure Memory benchmark.

Steps to execute program.

	1. Open terminal in Linux OS.

	2. Copy source code folder to amzon ec2 instance using below command
			scp -i user.pem -r  folder/path/copy  host_username@public-domain-name:/path/to/directory

		example: scp -i vishwanath.pem -r  Assignment#1/PA#1/ ubuntu@ec2-54-201-89-31.us-west-2.compute.amazonaws.com:/home/ubuntu
	
	3. Connect to Amazon aws ec2 instance following above steps.

	4. Change terminal working directory to source code directory using below command
		cd path/to/file

	5. Enter below command to excute CPUBenchmark program. 
 		gcc -pthread MemoryBenchmark.c -o MemoryBenchmark.out
	
   		//pthread is library to excute pthread functions in program.

	6. Excute output file using below command.
   		./MemoryBenchmark.out

	7. Enter the number of threads in console as below.
		Enter number of threads (1 thread 2 thread): 1
		//1 for 1 thread
		//2 for 2 thread

	8. Enter block size: 1. 1 B  2. 1 KB 3. 1 MB.
	
	9. Result displayed on console in terms of Throughput: Mb/sec, Latency: ms.

--------------------------------------------------------------------------------------------------------------------------------------------------

DiskBenchmark.c : This contain program to measure Disk performance

Steps to execute program

	1. Open terminal in Linux OS.

	2. Copy source code folder to amzon ec2 instance using below command
			scp -i user.pem -r  folder/path/copy  host_username@public-domain-name:/path/to/directory

		example: scp -i vishwanath.pem -r  Assignment#1/PA#1/ ubuntu@ec2-54-201-89-31.us-west-2.compute.amazonaws.com:/home/ubuntu
	
	3. Connect to Amazon aws ec2 instance following above steps.

	4. Change terminal working directory to source code directory using below command
		cd path/to/file

	5. Enter below command to excute CPUBenchmark program. 
 		gcc -pthread DiskBenchmark.c -o DiskBenchmark.out
	
   		//pthread is library to excute pthread functions in program.

	6. Excute output file using below command.
   		DiskBenchmark.out

	7. Enter the number of threads in console as below.
		Enter number of threads (1 thread 2 thread): 1
		//1 for 1 thread
		//2 for 2 thread

	8. Enter block size: 1. 1 B  2. 1 KB 3. 1 MB.
	
	9. Result displayed on console in terms of Throughput: Mb/sec, Latency: ms.

----------------------------------------------------------------------------------------------------------------------------------------------------

CPU_10min_Exp.c : This contain program to conduct CPU 10 minute experiment.

Steps to execute program.

	1. Open terminal in Linux OS.

	2. Copy source code folder to amzon ec2 instance using below command
			scp -i user.pem -r  folder/path/copy  host_username@public-domain-name:/path/to/directory

		example: scp -i vishwanath.pem -r  Assignment#1/PA#1/ ubuntu@ec2-54-201-89-31.us-west-2.compute.amazonaws.com:/home/ubuntu
	
	3. Connect to Amazon aws ec2 instance following above steps.

	4. Change terminal working directory to source code directory using below command
		cd path/to/file

	5. Enter below command to excute CPUBenchmark program. 
 		gcc -pthread CPU_10min_Exp.c -o CPU_10min_Exp.out
	
   		//pthread is library to excute pthread functions in program.

	6. Excute output file using below command.
   		CPU_10min_Exp.out

	7. Two files with sample data created in working directory.
