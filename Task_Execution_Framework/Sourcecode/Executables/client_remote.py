import boto3
import argparse
import os

#Parse commandline for user input
parser = argparse.ArgumentParser()
parser = argparse.ArgumentParser(description='Client: Task execution framework')
parser.add_argument('-s','--Queue_Name', help='Local version client', required=True)
parser.add_argument('-t','--Total_Threads', help='NUmber of threads', required=False)
parser.add_argument('-w','--Worker_Load_File', help='Tasks work load file', required=True)
args = vars(parser.parse_args())

#Set default session
client = boto3.setup_default_session(region_name='us-west-2')
#Declare SQS resource type
client = boto3.resource('sqs')
#Get AWS created queue by name to insert tasks
queue=client.get_queue_by_name(QueueName=args['Queue_Name'])
#Get AWS created queue by name to get excution results of teasks
rqueue=client.get_queue_by_name(QueueName='RESULT')
#Declare array to save task id of each submitted tasks
tasks=[]
#Read workload file
with open(args['Worker_Load_File']) as f:
    #Read file by line by line
    lines= f.read().splitlines()
    #Insert each line to AWS queue
    for line in lines:
	#Send meassge to SQS
	response = queue.send_message(MessageBody=line)
	#Add task Id to task array
	tasks.append(response.get('MessageId'))

#Take the count of submitted tasks
count=len(tasks)

#Read all tasks excutable code from AWS queue
while count!=0:
	try:
		#Read each message from queue
		message = rqueue.receive_messages(MaxNumberOfMessages=1,MessageAttributeNames=['task_id','exit_code'])
		#Keep the track of received message
		count-=len(message)
		print message[0].message_attributes['task_id']['StringValue']
		print message[0].message_attributes['exit_code']['StringValue']
	except:
		pass
	


