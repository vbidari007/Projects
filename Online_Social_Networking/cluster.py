"""
cluster.py
"""
from collections import Counter
import matplotlib.pyplot as plt
import networkx as nx
import sys
import time
import pickle
from collections import Counter, defaultdict, deque
from itertools import chain, combinations
import copy
import io


def create_graph(users):    
    G=nx.Graph()
    #G.add_node('realDonaldTrump')
    for u in users:
        for f in users[u]['friends']:
               G.add_node(str(f))
               G.add_edge(str(u),str(f))
    return G
    pass
    
def girvan_newman(G):
    if ((G.order() >= 50) and (G.order() <= 200)):
        print("stopping for " + str(G.order()))
        return [G.nodes()]
    betweenness_map = nx.edge_betweenness_centrality(G)
    edge_list = sorted(betweenness_map.items(), key=lambda x: x[1], reverse=True)
    components = [c for c in nx.connected_component_subgraphs(G)]
    count = 0
    while len(components) == 1:
        G.remove_edge(*edge_list[count][0])
        count+=1
        components = [c for c in nx.connected_component_subgraphs(G)]
    print('removed ' + str(count) + ' edges')
    print ('component size [%d, %d]' % (len(components[0]),len(components[1])))
    result=[]   
    for c in components:
        if c.order() > 50:
            result.extend(girvan_newman(c))
    return result
    
def draw_network(graph,filename,users):
    labels = {}
    nodes =graph.nodes()
    
    for n in nodes:            
            if int(n) in users:
                labels[n]=users[int(n)]['name']
            
    pos=nx.spring_layout(graph)   #G is my graph
    
    nx.draw(graph,pos,node_color='#A0CBE2',edge_color='#BB0000',node_size=17,width=0.5,edge_cmap=plt.cm.Blues,with_labels=False)
    nx.draw_networkx_labels(graph,pos,labels,font_size=8,font_color='black')
    #plt.show()
    plt.savefig(filename, dpi=500, facecolor='w', edgecolor='w',orientation='portrait', papertype=None, format=None,transparent=True, bbox_inches=None, pad_inches=0.1)
    plt.clf()
    pass

def main():    
    users=defaultdict()
    with open('users.pkl', 'rb') as f:
            users = pickle.load(f)
    #import pdb;pdb.set_trace()
    G=create_graph(users)
    print('Graph has %d nodes and %d edges' % 
      (G.order(), G.number_of_edges()))
    file = open("cluster.txt", "w")
    draw_network(G,"Graph.png",users)
    result=girvan_newman(G)    
    file.write('\n Number of communities discovered: '+str(len(result)))
    sum=0;
    print('final communities sizes:')
    i=0
    for c in result:
        i +=1
        print('size of community '+str(i)+' is '+str(len(c)))
        g=G.subgraph(c)
        
        print('sungraph'+str(i)+' has %d nodes and %d edges' % 
        (g.order(), g.number_of_edges()))
        sum += g.order()
        print()
        draw_network(g,"community"+str(i)+".png",users)
    sum = sum /i;
    file.write('\n Average number of users per community:'+str(sum))
    file.close()

if __name__ == '__main__':
    main()