

import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.KeyValueTextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class Hadoop_Sort {

  public static class TokenizerMapper
       extends Mapper<Object, Text, Text, Text>{

//map function to read key and value string input
    public void map(Text key, Text value, Context context
                    ) throws IOException, InterruptedException {
    
      context.write(key,value);
      
     
    }
  }

  public static class SortReducer
       extends Reducer<Text,Text,Text,Text> {
  
//Reduce function to read key and list of values from map
    public void reduce(Text key, Iterable<Text> values,
                       Context context
                       ) throws IOException, InterruptedException {
    
      for (Text val : values) {
    	  context.write(key, val);
   
      }
    
     
    }
  }

  public static void main(String[] args) throws Exception {
    Configuration conf = new Configuration();
    Job job = Job.getInstance(conf, "Hadoop_Sort"); 
//KeyValueTextInputFormat splits each line to key and value format	
    job.setInputFormatClass(KeyValueTextInputFormat.class);
    job.setJarByClass(Hadoop_Sort.class);
	//Set map class
    job.setMapperClass(TokenizerMapper.class);
	//Set reduce class
    job.setReducerClass(SortReducer.class);
    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(Text.class);
	//Set input file path
    FileInputFormat.addInputPath(job, new Path(args[0]));
	//Set Output file path
    FileOutputFormat.setOutputPath(job, new Path(args[1]));
    System.exit(job.waitForCompletion(true) ? 0 : 1);
  }
}
