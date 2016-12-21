"""
collect.py
"""
# Imports you'll need.
from collections import Counter
import matplotlib.pyplot as plt
import networkx as nx
import sys
import time
from TwitterAPI import TwitterAPI
import pickle
from twitter import *
from collections import Counter, defaultdict, deque
from itertools import chain, combinations
import io

consumer_key = 'R1yihLNelVbISKts6BjvImFTh'
consumer_secret = 'RjafZNMmhnNK9RCipcSNhMhLF4QT6i54egHgHZXKfHJLiJK6na'
access_token = '772477688787378176-p0MMnCrkYP536eGYzaI7p5OYVK8B3lB'
access_token_secret = 'Uf4BytSHOMEu2cID8BaDkQsYkRLuCjByKmDuDc7LkWfeg'


def get_twitter():
    """ Construct an instance of TwitterAPI using the tokens you entered above.
    Returns:
      An instance of TwitterAPI.
    """
    return TwitterAPI(consumer_key, consumer_secret, access_token, access_token_secret)
  
  

def robust_request(twitter, resource, params, max_tries=5):
    """ If a Twitter request fails, sleep for 15 minutes.
    Do this at most max_tries times before quitting.
    Args:
      twitter .... A TwitterAPI object.
      resource ... A resource string to request; e.g., "friends/ids"
      params ..... A parameter dict for the request, e.g., to specify
                   parameters like screen_name or count.
      max_tries .. The maximum number of tries to attempt.
    Returns:
      A TwitterResponse object, or None if failed.
    """
    for i in range(max_tries):
        request = twitter.request(resource, params)
        if request.status_code == 200:
            return request
        else:
            print('Got error %s \nsleeping for 15 minutes.' % request.text)
            sys.stderr.flush()
            time.sleep(61 * 15)

def srch(twitter):
        tweets = []
        n_tweets=1000
        for r in robust_request(twitter,'statuses/filter', {'track': 'donald trump'},5):
            tweets.append(r)
            #user = r['user']
            #wr.writerow([r['created_at'],user['name'],user['screen_name'],smart_str(user['description']),smart_str(r['text']),r['lang'],user['favourites_count'],user['followers_count'],user['friends_count'],user['location'],user['listed_count']])
            if len(tweets) % 100 == 0:
                print('%d tweets' % len(tweets))
            if len(tweets) >= n_tweets:
                break
        tweets = [t for t in tweets if 'user' in t]
        print('fetched %d tweets' % len(tweets))
        pickle.dump(tweets, open('tweets.pkl', 'wb'))
        
        return tweets

def get_friends(twitter, screen_name):
        users=defaultdict(dict)
        frnds = robust_request(twitter,'friends/ids',{'screen_name':screen_name,'count':14},5)
        for f in frnds.json()["ids"]:
            users[f]['friends']=[]
            users[f]['name']=None
    
        pickle.dump(users, open('users.pkl', 'wb'))
        return users
        
def get_names(twitter,users):
    for id in users:
        response=robust_request(twitter,'users/lookup',{'user_id':id},5)
        users[id]['name']=response.json()[0]['screen_name']
    pickle.dump(users, open('users.pkl', 'wb'))
        

def get_allfriends(twitter,users):
        for id in users:
            frnds = robust_request(twitter,'friends/ids',{'screen_name':users[id]['name'],'count':100},5)
            for f in frnds.json()["ids"]:
                users[id]['friends'].append(f)
        pickle.dump(users, open('users.pkl', 'wb'))  
        
           


def main():
    twitter = get_twitter()
    users=defaultdict(dict)
    print('Established Twitter connection.\n')
    TwitterData=False;
    
    if TwitterData==True:
        print("Getting data from twitter\n")
        print("To use stored data from disk then set 'TwitterData' variable to False in main()")
        candidate=robust_request(twitter,'users/lookup',{'screen_name':'realDonaldTrump'},5)
        id=candidate.json()[0]['id']
        users=get_friends(twitter,'realDonaldTrump')
        users[id]['friends']=[]
        users[id]['name']=None
        get_names(twitter,users)
        get_allfriends(twitter,users)
        tweets=srch(twitter)
    else:
        print("Using stored data from disk\n")
        print("To get new data then set 'TwitterData' variable to True in main()\n")
        
    count= (14 *100) +1
    file = open("collect.txt", "w")
    file.write('\n Number of users collected: '+str(count))
    file.write('\n Number of messages collected: '+ '1000')
    file.close()
    print("Data collection is done!")
    
    
    
if __name__ == '__main__':
    main()







    
