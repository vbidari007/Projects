/**
 * 
 */
package peers;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.Properties;
import java.util.Random;
import java.util.logging.FileHandler;
import java.util.logging.Logger;
import java.util.logging.SimpleFormatter;

import com.mongodb.BasicDBObject;
import com.mongodb.DB;
import com.mongodb.DBCollection;
import com.mongodb.DBCursor;
import com.mongodb.MongoClient;





public class peerone {

	/**
	 * @param args
	 */
	public static String choice;
	public static String[] servers = new String[16];
	private static Properties prop;
	private static InputStream inpu = null;
	// private static Socket sock, sock1;
	private static String key, value;
	// private static InputStreamReader stdin,in;
	private static BufferedReader stdin;
	//private static Socket sockets[] = new Socket[8];
	private static boolean success;
	private static int flag = 0;
	private static MongoClient[] mongoClient=new MongoClient[16];	
	private static DB[] db=new DB[16] ;
	private static DBCollection[] collection=new DBCollection[16];
	private static int noOfOpers = 10000;
	private static String[][] array = new String[noOfOpers][2]; // Array to hold
																// all the
																// random
																// numbers
	private static int Totalservers;
	private static Logger logger;
	public static void main(String[] args) throws SecurityException, IOException {
		// new peer1server();
		logger = Logger.getLogger("MongoDB"); 
		FileHandler fh = new FileHandler("MongoDB.log");  
	        logger.addHandler(fh);
	        SimpleFormatter formatter = new SimpleFormatter();  
	        fh.setFormatter(formatter);  
		String dbName = "foodb";
		stdin = new BufferedReader(new InputStreamReader(System.in));
		prop = new Properties();
		File f = new File("Config.properties");
		try {
			inpu = new FileInputStream(f);
		} catch (FileNotFoundException e1) {

			e1.printStackTrace();
		}
		try {
			prop.load(inpu);
			Totalservers = Integer.parseInt(prop.getProperty("Number_Servers"));
			int i = 0;
			while (i <= Totalservers) {
				servers[i] = prop.getProperty("peer" + (i + 1));
				//sockets[i] = null;
				i++;
			}
			// populate array to generate random key

			for (i = 0; i < noOfOpers; i++) {
				for (int j = 0; j < 2; j++) {
					char[] chars = "abcdefghijklmnopqrstuvwxyz".toCharArray();
					StringBuilder sb = new StringBuilder();
					Random random = new Random();
					if (j == 0) {
						for (int z = 0; z < 10; z++) {
							char k = chars[random.nextInt(chars.length)];
							sb.append(k);
						}
						String key = sb.toString();
						array[i][j] = key;
					}
					if (j == 1)
						for (int z = 0; z < 90; z++) {
							char k = chars[random.nextInt(chars.length)];
							sb.append(k);
						}
					String value = sb.toString();
					array[i][j] = value;
				}
			}

			// }
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		while (true) {
			System.out.println("OPERATIONS MENU");
			System.out.println("1.PUT");
			System.out.println("2.GET");
			System.out.println("3.DELETE");
			System.out.println("4.Exit");
			
			System.out.println("Enter the menu option");

			try {
				choice = stdin.readLine();
				if (flag == 0) {
					
					for (int num = 0; num < Totalservers; num++) {
						String IP = servers[num];
						mongoClient[num]=new MongoClient(IP);
						//db[num]=new DB(mongo, name)
						db[num] = mongoClient[num].getDB("TestDB"+num);
						collection[num] = db[num].getCollection("KeyValue_1"+num);
						//jedis[num] = new Jedis(IP);
						/*cluster[num] = Cluster.builder().addContactPoint("127.0.0.1").withPort(9160).build();
						sessions[num] = cluster[num].connect();*/
						
					}

					flag = 1;
				}

				

					switch (choice) {
					case "1":
						
						success = put();

						break;
					case "2":
						String val;
						val = get();

						break;
					case "3":
						success = del();

						break;
					case "4":
						
						System.exit(0);
					default:
						System.out.println("Default");
						break;
					}
				
			} catch (NumberFormatException | IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} finally {
				if (inpu != null) {
					try {

						
						inpu.close();
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			}
		}

	}

	public static int hashfunction(String key) {
		int len = key.length();
		long sum = 0;
		while (len > 0) {
			sum += (int) key.charAt((len - 1));
			len--;
		}
		// 19 is prime number
		sum = (sum * 19);
		int server = (int) (sum % Totalservers);
		return server;
	}

	public static boolean put() {
		try {
			long startTime = System.currentTimeMillis();
			for(int i=0;i< noOfOpers;i++)
			{
				int num=hashfunction(array[i][0]);
				BasicDBObject dbObj = new BasicDBObject();
				
				/*sessions[num]
						.execute(("INSERT INTO cassandra.keyvalue (key, value) VALUES ('" + array[i][0] + "','" + array[i][1] + "')"));*/
				BasicDBObject dbObjectPut = new BasicDBObject("_id",array[i][0]).append(array[i][0], array[i][1]);
				collection[num].insert(dbObjectPut);				
				dbObjectPut=null;
				
			}			
			long endTime = System.currentTimeMillis();
			//System.out.println("Total time taken to put " + noOfOpers + " operations on Redis Server :"
				//	+ (endTime - startTime) + " milliseconds\n");
			logger.info("Total time taken to put " + noOfOpers + " operations on MongoDB Server :"
					+ (endTime - startTime) + " milliseconds\n");
			return success;
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
	}

	public static String get() {
		try {
			long startTime = System.currentTimeMillis();
			for(int i=0;i< noOfOpers;i++)
			{
				int num=hashfunction(array[i][0]);
				
				//ResultSet results = sessions[num].execute(("SELECT * FROM cassandra.keyvalue WHERE key ='" + array[i][0] + "'"));
				BasicDBObject field = new BasicDBObject();
				field.put("_id", array[i][0]);
				DBCursor cursor = collection[num].find(field);
				while (cursor.hasNext()) {
				    BasicDBObject obj = (BasicDBObject) cursor.next();
				    //System.out.println(obj.getString(keyToBeRetrieved));						    
				}
				field=null;
				
				
			}
			long endTime = System.currentTimeMillis();
			//System.out.println("Total time taken to GET " + noOfOpers + " operations on Redis Server :"
					//+ (endTime - startTime) + " milliseconds\n");
			logger.info("Total time taken to GET " + noOfOpers + " operations on MongoDB Server :"
					+ (endTime - startTime) + " milliseconds\n");
			String value = "";

			
			//System.out.println(d);
			return value;
		} catch (Exception e) {
			e.printStackTrace();
			return "";
		}
	}

	public static boolean del() {
		try {
			long startTime = System.currentTimeMillis();
			for(int i=0;i< noOfOpers;i++)
			{
				int num=hashfunction(array[i][0]);
				
				//sessions[num].execute("DELETE FROM cassandra.keyvalue WHERE key='" + array[i][0] + "'");
				BasicDBObject field = new BasicDBObject();
				field.put("_id", array[i][0]);
				collection[num].remove(field);
				field=null;
			}
			long endTime = System.currentTimeMillis();
			//System.out.println("Total time taken to put " + noOfOpers + " operations on Redis Server :"
					//+ (endTime - startTime) + " milliseconds\n");
			logger.info("Total time taken to delete " + noOfOpers + " operations on MongoDB Server :"
					+ (endTime - startTime) + " milliseconds\n");
			//Document d = db[num].getDocument(key);

			// System.out.println("Document 1: " + d);
			
			
			return success;
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
	}

}
