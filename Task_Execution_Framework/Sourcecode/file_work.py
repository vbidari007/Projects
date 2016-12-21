import sys
#Create a workload file
file = open("WorkLoadFile.txt", "w+")

#Write tasks to workloadfile
for x in range(0, 10000):
	file.write("sleep 0 \n")

#Close file
file.close()
