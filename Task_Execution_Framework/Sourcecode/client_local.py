import sys
import argparse
import os
from Queue import *
import time

# Argument parser to take input from user thriugh command line.
parser = argparse.ArgumentParser()
parser = argparse.ArgumentParser(description='Client: Task execution framework')
parser.add_argument('-s','--Queue_Name', help='Local version client', required=True)
parser.add_argument('-t','--Total_Threads', help='NUmber of threads', required=True)
parser.add_argument('-w','--Worker_Load_File', help='Tasks work load file', required=True)
args = vars(parser.parse_args())

#Declare a memory Queue to insert tasks
q =Queue(maxsize=0)

#Declare a memory queue to insert task result to queue
rq=Queue(maxsize=0)

#Start time to compute elapsed time
start_time = time.time()

#Read worker Load file
with open(args['Worker_Load_File']) as f:
    #Read file all lines
    lines= f.read().splitlines()
    count=0;
    #Insert each task to queue
    for line in lines:
	count+=1
	q.put((count,line))
	


#IMplement threadpool
from multiprocessing.dummy import Pool as ThreadPool

#Function te execute task
def func1(line):
	#EXecute task
        x=os.system(line[1])
	#Put task execute result to queue
	rq.put((line[0],line[1],x))
	
	
#Function to assign tasks to threadpool
def parallel(lines,threads):
        pool = ThreadPool(threads)
        results = pool.map(func1, lines)
        pool.close()
        pool.join()
        return results

#Call thread pool and assign tasks
parallel(list(q.queue),int(args['Total_Threads']))

#Write task result to file
file=open("Local_Result","w")
for elem in list(rq.queue):
        dan=rq.get()
        #print dan
	file.write(str(dan)+'\n')
file.close()
#Calculate elapse time
elapsed_time = time.time() - start_time
#Print elapse time
print 'Elapsed Time: '+str(elapsed_time)
