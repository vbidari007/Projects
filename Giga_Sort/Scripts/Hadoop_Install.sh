sudo apt-get install default-jdk

sudo addgroup hadoop

sudo adduser --ingroup hadoop hduser

sudo apt-get install ssh

su hduser
ssh-keygen -t rsa -P ""

cat $HOME/.ssh/id_rsa.pub >> $HOME/.ssh/authorized_keys

wget http://mirrors.sonic.net/apache/hadoop/common/hadoop-2.7.2/hadoop-2.7.2.tar.gz

tar xvzf hadoop-2.7.2.tar.gz

sudo mv * /usr/local/hadoop

sudo mkdir -p /app/hadoop/tmp

sudo mkdir -p /usr/local/hadoop_store/hdfs/namenode

sudo mkdir -p /usr/local/hadoop_store/hdfs/datanode

hadoop namenode -format
