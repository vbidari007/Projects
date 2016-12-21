import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.InputMismatchException;
import java.util.Scanner;

/**
 * <h1>Link-state routing algorithm</h1>
* The Project_CS542 program implements an application that
* simulate Link-state routing algorithm.
*
* @author  Vishwanath Bidari
* @version 1.0
* @since   2016-11-20 
*/
public class Project_CS542 {
	/*
	 * Global variable declaration, which are used across all functions.
	 * graph: 2D-array to store topology matrix
	 * shortDist: Array to store shortest distance to all routers from source router
	 * preDsr: Array to store predecessor for each router in topology from source router.
	 * source: Source router
	 * dest: Destination router
	 */
	static int[][] graph=null;
	static int shortDist[]=null;
	static int preDsr[]=null;
	static int source=-1;
	static int dest=-1;

	/**
	   * This is the main method to display menu items to user. This method accepts 
	   * user input from console and calls proper methods in switch case that matches with user input.
	   * @param args Unused.
	   * @return Nothing.
	   * @exception IOException On input error.
	   * @see IOException
	   */
	public static void main(String[] args) throws IOException {		
		try
		{
		//do-while loop to display menu items to user
		do{
			System.out.println("======================================================================================");
			//Display menu items to user
			System.out.println("CS542 Link State Routing Simulator \n (1) Create a Network Topology \n (2) Build a Connection Table \n " +
					"(3) Shortest Path to Destination Router \n "+"(4) Modify a Topology \n " +"(5) Best Router for Broadcast \n "+"(6) Exit \n ");
			System.out.println("======================================================================================");
			//Create input instance to read input from user
			Scanner in = new Scanner(System.in);
			
			//Prompt to accepts input
			System.out.print("Master Command:");
			
			//Read input from user console
			int option = in.nextInt();
			
	     
			//switch case to call different methods based on input provided by user from menu items
	        switch (option) {
	            case 1: //Initialize graph, source and destination router invalid state
	            		graph=null;
	        	        source=-1;
	        	        dest=-1;
	            	    //Prompt user provide input file
	        		    System.out.println("Input original network topology matrix data file:");
	        		    //Read file name
	        		    String filepath=in.next();
	        		    //Call create_topology method to read input file
	            	    create_topology(filepath);            
	                    break;
	                     
	            case 2: //Check topology matrix exists
	            		if(graph==null)
	            		{
	            			System.out.println("Please create topology matrix using option 1");
	            			break;
	            		}
	            	   //Prompt user to select source router
	            		System.out.println("Select a source router:");
	            		//Read source router
	    				source=in.nextInt();
	    				//Matrix is 0-indexed 
	    				source--;
	    				if(source >=0 && source <= (graph.length-1))
	    				//Call build_connection_tab method to display connection table
	    				build_connection_tab();
	    				else
	    					System.out.println("Enter valid source");
	            		break;
	            		
	            case 3:	//Check topology matrix exists
	            		if(graph==null)
        				{
        					System.out.println("Please create topology matrix using option 1");
        					break;
        				}
	            		//Check source router is selected
	             		if(source==-1)
	             		{
	             			System.out.println("Source is not selected!");
	             			break;
	             		}
	            	   //Prompt user to select destination router
	            		System.out.println("Select the destination router:");
	            		//Read destination router
	    				dest=in.nextInt();
	    				//Matrix is 0-indexed
	    				dest--;
	    				if(dest >=0 && dest <= (graph.length-1))
	    				//Call shortest_path_destination method to calculate shortest distance
	    				//between source and destination
	            	    shortest_path_destination();
	    				else
	    					System.out.println("Enter valid destination");
	            		break;
	            		
	            case 4: //Check topology matrix exists
	            		if(graph==null)
        				{
        					System.out.println("Please create topology matrix using option 1");
        					break;
        				}
	            		//Prompt user to select router to be removed from network
	            	 	System.out.println("Select a router to be removed:");
	            	 	//Read router number
	            	 	int router =in.nextInt();
	            	 	//Matrix is 0-indexed
	            	 	router--;
	            		if(router >=0 && router <= (graph.length-1))
	            	 	//Call remove_router method to remove router
	            	 	remove_router(router);
	            		else 
	            			System.out.println("Enter valid router");
	            	 	break;
	            		
	            case 5:	//Check topology matrix exists
	            		if(graph==null)
        				{
        				System.out.println("Please create topology matrix using option 1");
        				break;
        				}
	            	   //Call best_router method to find router which connects all routers with minimum cost
	            		best_router();
	            	   	break;
	            		
	            case 6: // Prompt user to say termination of application
	            		System.out.println("Exit CS542-04 2016 Fall project. Good Bye!");
	            		return;
	            default: //Display for any invalid input from user
	            		System.out.println("Please enter valid command option");
	            		 break;
	            
	        }
		}while(true);
		}catch(InputMismatchException exception)
		{
			//Display warning message for not integer input
			System.out.println("This is not an integer");
			main(null);
		}		

	}

	/**
	   * This create_topology method is used to read network topology from user input file.
	   * Network topology is given in matrix form, which indicates costs of links between directly connected
	   * routers. 	   * 
	   * @param String path : Name of input file.
	   * @return Nothing.
	   * @exception IOException On input error.
	   * @see IOException
	   */	
	public static  void create_topology(String path) throws IOException
	{
		try{
						
		//Create instance of FileInputStream library to read file on computer disk
		FileInputStream fis = new FileInputStream(path);
		
		//Create instance of BufferedReader library to store file content to temporary buffer
		BufferedReader br = new BufferedReader(new InputStreamReader(fis));
		
		 
		//Variable to store line from file
		String line = null;
		
		//Variable to store array of strings
		String[] splited=null;
		
		//Initialize number of rows and columns in topology matrix to zero
		int row=0;
		int col=0;
		
		//Read each line from input file till end of file
		while ((line = br.readLine()) != null) {
			
			//split line to get number of entries in each row of topology matrix
			splited = line.split(" ");
			
			//Calculate number of rows in topology matrix
			row++;
		}
		// Close buffered connection to file
		br.close();
		
		// NUmber of columns in topology matrix
		col=splited.length;	
		//Check validity of matrix because matrix should be square matrix
		if(row!=col)
		{
			System.out.println("Topology matrix is invalid!");
			return;
		}
	
		//Create 2D-array to store topology matrix
		graph = new int[row][col];
	
		//Create instance of FileInputStream library to read file on computer disk
		FileInputStream fis1 = new FileInputStream(path);
		
		//Create instance of BufferedReader library to store file content to temporary buffer
		BufferedReader br1 = new BufferedReader(new InputStreamReader(fis1));
		//Initialize row value to zero
		int i=0;
		//Read each line from input file till end of file
		while ((line = br1.readLine()) != null) {
			//split line to get number of entries in each row of topology matrix
			splited = line.split(" ");

			   //Loop through all entries in a row 
				for(int j=0;j<col;j++)
				{
					//Store each entry in row to topology matrix
					graph[i][j]=Integer.parseInt(splited[j]);
				}
			//Incremant row number
			i++;
			
		}
		// Close buffered connection to file
		br1.close();
		System.out.println("======================================================================================");	
		// Print topology matrix to console
		 for (int r = 0; r < row; r++){

		        for (int c = 0; c < col; c++)
		        {

		        	 System.out.print(graph[r][c] + "\t");
		        }

		        System.out.println("\n");

		      }
		 
		 System.out.println("=======================================================================================");
		
		 return;
		}
		catch(Exception e){
			System.out.println("Please enter valid file name ");
			return;
		}
		
	}
	
	/**
	   * This build_connection_tab method is used to build connection table for source router.
	   * Connection table contain interface value for each router in network topology from source router.
	   * interface is router through which source can connect to destination router.
	   * @param Nothong.
	   * @return Nothing.
	   * @exception IOException On input error.
	   * @see IOException
	   */
	public static void build_connection_tab() throws IOException
	{		
		//Call dikjstra method to calculate predecessor array which contains array of routers to reach destination 
		// from source router.
		dikjstra(source);
		
		//Number of Vertices
		int V=graph.length;
		
		//Display connection table to user
		System.out.println("Router "+(source+1)+" Connection Table");
		System.out.println("Destination\t"+"Interface");
		System.out.println("==============================\n");
		
		//Calculate connection table using predecessor array calculated in dikjstra method.
		for(int i=0; i< V;i++)
		{ 	//Initialize pred variable for each vertices
			int pred=i;	
			
			//Calculate interface router for each router
			while(preDsr[pred]!=source)
			{
				//Check if router is not reachable from source
				if(preDsr[pred]==-1)
					break;
				//access predecessor router value
				pred=preDsr[pred];
				
			}
			
			//if source and destination router are same then display nothing
			if(i==source )
			{
				System.out.println((i+1)+"\t\t"+"-");
			}
			//if destination router is not reachable from source router then display -1
			else if(preDsr[i]==-1)
			{
				System.out.println((i+1)+"\t\t"+"-1");
			}
			// Display interface value for each router in topology from source router
			else 
			{								
				System.out.println((i+1)+"\t\t"+(pred+1));
				
			}
		}	
		return;
	}
	
	/**
	   * This dikjstra method is used to implement Dijkstra algorithm.
	   * Dijkstra algorithm calculates shortest distance to all routers in topology from source router.
	   * This method also calculates predecessor for each router in topology from source router.
	   *
	   * @param int src: Source router number.
	   * @return Nothing.
	   * @exception IOException On input error.
	   * @see IOException
	   */
	public static void dikjstra(int src)
	{	
	 //Number of vertices in graph	
	 int V=graph.length;
	 
	 //Index of shorted distance router in topology matrix from adjacent router.
	 //Initialize with -1
	 int min_index=-1;
	 
	 //Value of shortest distance router in topology matrix from adjacent router
	 //Initialize with max value
	 int min_value=Integer.MAX_VALUE;
	 
	 //Array to store shortest distance to all routers in topology from source router
	 shortDist=new int[V];
	 
	 //Array to store predecessor to all routers in topology from source router.
	 preDsr=new int[V];
	 
	 //Array to store visited routes in topology from source router
	 Boolean visited[]=new Boolean[V];
	 
	 //Initialize arrays to default values
	 for(int i=0;i<V;i++)
	 {
		 //Initialize with max distance to all routers from source router
		 shortDist[i]=Integer.MAX_VALUE;
		 
		 //Initialize all routers with not visited state.
		 visited[i]=false;
		 
		 // Initialize all router predecessor as not reachable from source router
		 preDsr[i]=-1;
	 }
	 //Initialize distance to source router from itself as zero.
	 shortDist[src]=0;
	 
	 //Initialize predecessor of source router as itself
	 preDsr[src]=src;
	 
	//Calculate shortest distance and predecessor to all routers in topology from source router.
	 for(int v=0; v<V-1;v++)
	 {
		 min_index=-1;
		 min_value=Integer.MAX_VALUE;
		 // Find router in topology matrix which is at shortest distance to adjacent source router.
		 //Router should be not visited
		 for(int j=0;j<V;j++)
		 {
			 if(visited[j]==false && shortDist[j] <=min_value)
			 {
				 min_value=shortDist[j];
				 min_index=j;
			 }
		 }
		 
		 //Mark shortest router as visited
		 visited[min_index]=true;
		 
		 //Find adjacent routers to router which shortest from source router to find shortest to
		 //all routers in topology from source router.
		 // Also calculates predecessor for each router in topology from source router.
		 for(int k=0;k< V;k++)
		 {
			 if(!visited[k] &&  graph[min_index][k]!= 0 &&  graph[min_index][k]!= -1 && shortDist[min_index]!=Integer.MAX_VALUE && shortDist[min_index]+(graph[min_index][k]==-1?0:graph[min_index][k]) < shortDist[k])
			 {
				 shortDist[k]=shortDist[min_index]+(graph[min_index][k]==-1?0:graph[min_index][k]);
				
				 preDsr[k]=min_index;
				
			 }			 
		 } 				 
	 }
	 
	 return ;	 
		
	}

	/**
	   * This shortest_path_destination method is used to fine shortest distance and shortest path from source 
	   * to destination router in topology.	   *
	   * @param Nothing
	   * @return Nothing.
	   * @exception IOException On input error.
	   * @see IOException
	   */
	public static void shortest_path_destination() throws IOException
	{
		//Call dikjstra
		dikjstra(source);
		//Destination router
		int node=dest;
		// if destination is not selected
		if(dest==-1)
		{
			System.out.println("Destination router not selected/removed");
			return;
		}
		//Path to destination router from source router
		String path=String.valueOf(dest+1);
		
		//Calculate shortest distance and path from source router using preDsr and shortDist array.
		while(node!=source)
		{
			//if Predecessor is -1 then it is not reachable form source router.
			if(node==-1)
			{
				System.out.println("No short path");
				return;
			}
			//Find predecessor of router
			node=preDsr[node];
			path = (node+1) +"-"+ path;
		}
		//Display shortest path from source to destination router.
		System.out.println("The shortest path from router "+(source+1)+" to router "+(dest+1)+" is ");
		System.out.print(path);
		//Display minimum cost from source router to destination router.
		System.out.println(", the total cost value is "+shortDist[dest]);
		return ;
		
	}
	
	/**
	   * This remove_router method is used to remove router from topology matrix and 
	   * calls build_connection_tab and shortest_path_destination methods to recalculate
	   * connection table and shortest path from source to destination router.
	   * @param int router: Router to be removed from matrix.
	   * @return Nothing.
	   * @exception IOException On input error.
	   * @see IOException
	   */
	public static void remove_router(int router) throws IOException
	{
		//set source to -1 if router to be removed is source
		if(router==source)
		{
			source=-1;
		}
		//set destination to -1 if router to be removed is destination
		if(router==dest)
		{
			dest=-1;
		}
		//Remove router from topology matrix so assign -1 value to row and column of router to be removed 
		//in matrix.
		for(int i=0;i<graph.length;i++)
		{
		 graph[router][i]=-1;
		 graph[i][router]=-1;
		}
		
		 //Call build_connection_tab to updated connection table
		build_connection_tab();
		//Call shortest_path_destination to calculate shorted distance and path from source to destination.
		shortest_path_destination();
		return;
		 
	}

	/**
	   * This best_router method is used to find router which is connected to all or maximum number of routers
	   * in topology with minimum cost.
	   * @param Nothing.
	   * @return Nothing.
	   * @exception IOException On input error.
	   * @see IOException
	   */
	public static void best_router()
	{
		//Minimum cost value
		int min=Integer.MAX_VALUE;
		
		//Best router index.
		int best=0;
		
		//Number of connected routers.
		int max_count=0;
		
		//Number of vertices
		int V=graph.length;
		
		//Calculate number of connected routers and minimum cost for each router in topology.
		for(int i=0; i< V;i++)
		{
			//Initial cost
			int val=0;
			
			//Initial number of routers
			int count=0;
			//Call dikjstra for each router in topology
			dikjstra(i);
			// Calculate minimum cost for selected router as source
			for(int v:shortDist)
			{
				//If distance is Integer.MAX_VALUE then it is not reachable from selected source router.
				if(v==Integer.MAX_VALUE)
					continue;
				//Calculate minimum cost for router
				else
				{
				val +=v;
				count++;
				}
			}
			
			//if source router has maximum connections than previous routers then consider it as
			//best router
			if(count > max_count)
			{		max_count=count;
					best=i;
					min=val;				
			}
			//if source router has same number of connected routers than previous routers then
			//compare cost of both routers
			else if(count==max_count)
			{
				//if cost of new router is less than previous best router then consider 
				//new router as best router
				if(val < min)
				{
					max_count=count;
					best=i;
					min=val;
				}
			}
			
			
		}
		
		//Display best router which has connected to maximum number of routers and minimum cost.
		System.out.println("The beast router which has shortest distance to all other routers is "+(best+1));
		return;
		
	}
}
