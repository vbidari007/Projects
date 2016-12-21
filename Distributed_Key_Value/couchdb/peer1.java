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
import java.net.Socket;
import java.util.Properties;
import java.util.Random;
import java.util.logging.FileHandler;
import java.util.logging.Logger;
import java.util.logging.SimpleFormatter;

import com.fourspaces.couchdb.Database;
import com.fourspaces.couchdb.Document;
import com.fourspaces.couchdb.Session;

public class peer1 {

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
	static Session[] dbSession = new Session[16];
	static Database[] db = new Database[16];
	private static int noOfOpers = 10000;
	private static String[][] array = new String[noOfOpers][2]; // Array to hold
																// all the
																// random
																// numbers
	private static int Totalservers;
	private static Logger logger;

	public static void main(String[] args) throws SecurityException, IOException {
		logger = Logger.getLogger("CouchDB"); 
		FileHandler fh = new FileHandler("CouchDb.log");  
	        logger.addHandler(fh);
	        SimpleFormatter formatter = new SimpleFormatter();  
	        fh.setFormatter(formatter);
		// new peer1server();
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

						dbSession[num] = new Session(IP, 5984);
						dbSession[num].createDatabase(dbName+num);
						db[num] = dbSession[num].getDatabase(dbName+num);
					}

					flag = 1;
				}

				/*int k = 0;
				long starttime = System.nanoTime();
				while (k < noOfOpers) {
					key = array[k][0];
					value = array[k][1];
					// key=Long.toString(k);

					int server = hashfunction(key);*/

					// serversocket(server);

					switch (choice) {
					case "1":
						// =Long.toString(k);
						// System.out.println("call put");
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
						int Totalservers = Integer.parseInt(prop.getProperty("Number_Servers"));
						for (int num = 0; num < Totalservers; num++) {
							dbSession[num].deleteDatabase(dbName+num);
						}
						System.exit(0);
					default:
						System.out.println("Default");
						break;
					}
				/*	++k;
				}*/
				/*long endtime = System.nanoTime();
				long elapsed = (endtime - starttime);
				System.out.println("Elapsed Time in ns" + elapsed);*/
			} catch (NumberFormatException | IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} finally {
				if (inpu != null) {
					try {/*int Totalservers = Integer.parseInt(prop.getProperty("Number_Servers"));
					for (int num = 0; num < Totalservers; num++) {
						dbSession[num].deleteDatabase(dbName);
						
					}*/

						
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
				Document doc = new Document();
				doc.setId(array[i][0]);
				//doc.put("Key", key);
				doc.put("Value", array[i][1]);
				db[num].saveDocument(doc);
				
			}			
			long endTime = System.currentTimeMillis();
			//System.out.println("Total time taken to put " + noOfOpers + " operations on CouchDB Server :"
				//	+ (endTime - startTime) + " milliseconds\n");
			logger.info("Total time taken to put " + noOfOpers + " operations on CouchDB Server :"
					+ (endTime - startTime) + " milliseconds\n");
			return success;
		} catch (Exception e) {
			return false;
		}
	}

	public static String get() {
		try {
			long startTime = System.currentTimeMillis();
			for(int i=0;i< noOfOpers;i++)
			{
				int num=hashfunction(array[i][0]);
				Document d = db[num].getDocument(array[i][0]);
				
			}
			long endTime = System.currentTimeMillis();
			//System.out.println("Total time taken to GET " + noOfOpers + " operations on CouchDB Server :"
			//		+ (endTime - startTime) + " milliseconds\n");
			logger.info("Total time taken to GET " + noOfOpers + " operations on CouchDB Server :"
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
				db[num].deleteDocument(db[num].getDocument(array[i][0]));
				
			}
			long endTime = System.currentTimeMillis();
			//System.out.println("Total time taken to put " + noOfOpers + " operations on CouchDB Server :"
				//	+ (endTime - startTime) + " milliseconds\n");
			logger.info("Total time taken to delete " + noOfOpers + " operations on CouchDB Server :"
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
