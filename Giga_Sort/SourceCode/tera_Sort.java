
import java.util.Comparator;

import org.apache.hadoop.record.Utils;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.function.Function;
import org.apache.spark.api.java.function.PairFunction;
import org.apache.spark.SparkConf;

import scala.Serializable;
import scala.Tuple2;
public class tera_Sort implements Serializable {

	/**
	 * @param args
	 */
	
	public static void main(String[] args) {
		
		SparkConf conf = new SparkConf().setAppName("TeraSort").setMaster("local");
		
		JavaSparkContext sc = new JavaSparkContext(conf);
		//Read input file path
		JavaRDD<String> lines = sc.textFile(args[0]);
		//PairFunction to split each record into key value format
		PairFunction<String, String, String> recordData =
				  new PairFunction<String, String, String>() {
				  
				public Tuple2<String, String> call(String x) {
				    //Split record using substring 
					//first 10 character as Key, remaing 90 as value
					 return new Tuple2<String, String>(x.substring(0,10),x);
				  }
				};
				
				//Call PairFunction on record data set 
				JavaPairRDD<String, String> pairs = lines.mapToPair(recordData);
			//call sorbykey on pair recors set	
		JavaPairRDD<String, String> sort = pairs.sortByKey(true);
		// Remove paranthesis from each record
		JavaRDD<String> output= sort.map(new Function<Tuple2<String,String>,String>(){ public String call(Tuple2<String,String> t){ return t._2;}});
		//Collect all sorted files and merge
		JavaRDD<String> output2 = sc.parallelize(output.collect());
		// save output file as text file
		output2.saveAsTextFile(args[1]);
		
	    sc.stop();
		
	

	}
	

}
