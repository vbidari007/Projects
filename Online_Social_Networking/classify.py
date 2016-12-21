"""
classify.py
"""
from collections import defaultdict
from io import BytesIO
from zipfile import ZipFile
from urllib.request import urlopen
import re
import pickle
from collections import Counter


def AFINN_lexicon(afinn):
    url = urlopen('http://www2.compute.dtu.dk/~faan/data/AFINN.zip')
    zipfile = ZipFile(BytesIO(url.read()))
    afinn_file = zipfile.open('AFINN/AFINN-111.txt')

    #afinn = dict()

    for line in afinn_file:
        parts = line.strip().split()
        if len(parts) == 2:
            afinn[parts[0].decode("utf-8")] = int(parts[1])

    print('read %d AFINN terms' % (len(afinn)))
    
def afinn_sentiment2(terms, afinn, verbose=False):
    pos = 0
    neg = 0
    for t in terms:
        if t in afinn:
            if verbose:
                print('\t%s=%d' % (t, afinn[t]))
            if afinn[t] > 0:
                pos += afinn[t]
            else:
                neg += -1 * afinn[t]
    return pos, neg
    
def tokenize(text):
    return re.sub('\W+', ' ', text.lower()).split()
    
def sentiment_analysis(tweets,afinn):
    tokens = [tokenize(t['text']) for t in tweets]
    positives = []
    negatives = []
    neutral=[]
    for token_list, tweet in zip(tokens, tweets):
        pos, neg = afinn_sentiment2(token_list, afinn)
        if pos > neg:
            positives.append((tweet['text'], pos, neg))
        elif neg > pos:
            negatives.append((tweet['text'], pos, neg))
        else:
            neutral.append((tweet['text'], pos, neg))
            
    
    all_counts = Counter()
    for tweet in tokens:
        all_counts.update(tweet)
    sorted_tokens = sorted(all_counts.items(), key=lambda x:x[1], reverse=True)
    i = 0
    for token, count in sorted_tokens:
        if token in afinn:
            print('%s count=%d sentiment=%d' % (token, count, afinn[token]))
            i += 1
            if i > 10:
                break
    file = open("classify.txt", "w")
    file.write('\n Number of instances for Positive class found: '+str(len(positives)))
    for tweet, pos, neg in sorted(positives, key=lambda x: x[1], reverse=True)[:1]:
        file.write('\n Positive class example tweet:'+str(tweet))
    file.write('\n Number of instances for Negative class found: '+str(len(negatives)))
    for tweet, pos, neg in sorted(negatives, key=lambda x: x[2], reverse=True)[:1]:
        file.write('\n Negative class example tweet:'+str(tweet))
    file.write('\n Number of instances for Neutral class found: '+str(len(neutral)))
    for tweet, pos, neg in sorted(neutral, key=lambda x: x[2])[:1]:
        file.write('\n Neutral class example tweet:'+str(tweet))
    
    file.close()
    
    
def main():
    afinn = dict()
    AFINN_lexicon(afinn)
    with open('tweets.pkl', 'rb') as f:
            tweets = pickle.load(f)
    sentiment_analysis(tweets,afinn)
    
            
if __name__ == '__main__':
    main()

