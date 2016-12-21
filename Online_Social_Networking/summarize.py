"""
sumarize.py
"""
file = open("summary.txt", "w")

with open('collect.txt') as f1:
    lines=f1.readlines()
    for line in lines:
             file.write(line)

with open('cluster.txt') as f1:
    lines=f1.readlines()
    for line in lines:
             file.write(line)

with open('classify.txt') as f1:
    lines=f1.readlines()
    for line in lines:
             file.write(line)   
             
file.close()