import os
import sys
import boto3
import argparse
from threading import Thread
from time import sleep

#Function to create custom dictionary to insert task id and execute code to queue
def custom_message(tid,ecode):
		task_id = {}
		task_id["StringValue"] = str(tid)
		task_id['DataType'] = 'String'
		exit_code = {}
		exit_code["StringValue"] = str(ecode)
		exit_code['DataType'] = 'String'
		
		result_msg = {	"task_id" : task_id, 
				"exit_code" : exit_code					
			}
		#import pdb;pdb.set_trace()
		return result_msg

#Thread function to receive task from SQS and checks with Dynamo db for task duplicity and executes task and insert task execute code tp other SQS
def threaded_function():
	while True:
		try:
			#Set SQS instance region
			client = boto3.setup_default_session(region_name='us-west-2')
			#Declare SQS resource variable
		        client = boto3.resource('sqs')
			#Declare a variable of SQS resurce type by name to read tasks from AWS SQS
        		queue=client.get_queue_by_name(QueueName=args['Queue_Name'])
			#Declare a variable of SQS resource type to insert task excute code 
	        	rqueue=client.get_queue_by_name(QueueName='RESULT')
			#Receive SQS message from queue.
			message = queue.receive_messages(MaxNumberOfMessages=1)
	        	#print ('{0}'.format(message[0].
			# Get the service resource of dynamo DB
			dynamodb = boto3.resource('dynamodb')
			# Instantiate a table resource object without actually
			# creating a DynamoDB table. Note that the attributes of this table
			# are lazy-loaded: a request is not made nor are the attribute
			# values populated until the attributes
			# on the table resource are accessed or its load() method is called.
			table = dynamodb.Table('REMOTE')
			#Check the existence of task Id in Dynamodb
			flag=table.get_item(Key={'ID':message[0].message_id})
			if 'Item' in flag.keys():
				#if task id exists in Dynamodb then skip the task
				pass
			else:
				#if the task id not in dynamodb then put task id to dynamodb
				response = table.put_item(
				Item={
				'ID':message[0].message_id			}
					)

			       	# Excute task
        			x=os.system(message[0].body)
				#Insert executed task execute code to SQS queue
				rqueue.send_message(MessageBody=str(message[0].body),MessageAttributes=custom_message(message[0].message_id,str(x)))
	        		#Delete task from SQS.
				message[0].delete()
		except:
			#If SQS task queue is empty then just continue the loop
			pass

#Main function
if __name__ == "__main__":
	#Parse the command line argument
	parser = argparse.ArgumentParser()
	parser = argparse.ArgumentParser(description='Remote Back End worker: Task execution framework')
	parser.add_argument('-s','--Queue_Name', help='Local version client', required=True)
	parser.add_argument('-t','--Total_Threads', help='NUmber of threads', required=True)
	parser.add_argument('-w','--Worker_Load_File', help='Tasks work load file', required=False)
	args = vars(parser.parse_args())
	

	#Create threads	
	for i in range(int(args['Total_Threads'])):
    		thread = Thread(target = threaded_function)
    		thread.start()
	#thread.join()
    	print "thread finished...exiting"
